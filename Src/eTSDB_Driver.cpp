#include <eTSDB_Driver.hpp>
namespace FwLogger
{
	namespace eTSDB
	{
		int clampAddrToPage(int addr, int len)
		{
			int endAddr = addr+len;
			if(endAddr & 0xfffff000 != addr &0xfffff000)
				endAddr = endAddr & 0xfffff000;
			return endAddr;
		}

		Driver::Driver(uint32_t offsetAddress, uint32_t size, SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin, Allocator<128>* alloc):
			SPIFlash(hspi, gpio, pin, "eTSDB", ETSDB_BINID, 0, 4)
		{
			_offset = offsetAddress;
			_size = size;
			_alloc = alloc;
			Page::setAllocator(alloc);

			_error = 0;
			_stateIdx = 0;
			_opBuf = nullptr;

			_page = nullptr;
			_row = nullptr;
		}

		RetValue Driver::openHeader(const uint8_t* name, PageAccessMode am, uint8_t period, uint8_t colLen, const Format* formats, const uint8_t** colNames)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			HeaderPage* hp = new HeaderPage(); // this will be allocated using the mempool

			for(int i = 0; i < 16; ++i) hp->_name[i] = 0;

			_page = hp;
			_page->_page_mode = am;

			_states[0] = State::FindNewObject;


			if(am == PageAccessMode::PageWrite || am == PageAccessMode::PageWriteUpdate)
			{
				hp->_period = period;

				if(colLen > 16) colLen = 16;

				int i;
				for(i = 0; i < 16 && name[i] != 0; ++i)
				{
					hp->_name[i] = name[i];
				}
				if(i<16) hp->_name[i] = 0;

				for(i = 0; i < colLen; ++i)
				{
					hp->_formats[i] = formats[i];
					int j;
					for(j = 0; j < 16 && colNames[i][j] != 0; ++j)
					{
						char c = colNames[i][j];
						hp->_colNames[i][j] = c;
					}
					if(j<16) hp->_colNames[i][j] = 0;
				}

				hp->_header_spacing = hp->getSize();
				if(am == PageAccessMode::PageWrite)
				{
					_states[1] = State::WritePageObjectIndexInRoot;
					_states[2] = State::WritePageHeader;
					_states[3] = State::Full;
				}
				else if(am == PageAccessMode::PageWriteUpdate)
					_states[1] = State::WritePageUpdate;
			}
			else
			{
				for(int i = 0; i < 16 && name[i] != 0; ++i) hp->_name[i] = name[i];
				_states[1] = State::LoadPage;
				_states[2] = State::Full;
			}

			findNewObject();

			return Ok;
		}

		RetValue Driver::openHeader(const uint8_t* name, PageAccessMode am, HeaderInitializer& hi)
		{
			const uint8_t* colnames[16];
			for(int i = 0; i < 16; ++i) colnames[i] = &hi.colNames[i][0];

			return openHeader(name, am, hi.period, hi.colLen, hi.formats, colnames);
		}

		RetValue Driver::startGetNextHeader(HeaderPage& hp)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			_page = &hp;

			_states[0] = State::FindNonEmpty;
			_states[1] = State::LoadPage;
			_states[2] = State::Found;

			_opIdx = _page->_object_idx;
			_page->_page_idx++;
			_opAddr = (_page->_page_idx-1)*2;
			int endAddr = clampAddrToPage(_opAddr, 256);
			_opLen = endAddr-_opAddr;
			readPage(_opLen, _opAddr);

			return Ok;
		}

		RetValue Driver::checkGetNextHeader()
		{
			if(_states[_stateIdx] == State::Found)
			{
				_states[_stateIdx] = State::Nop;
				return Ok;
			}
			else if(_states[_stateIdx] == State::NotFound)
			{
				_states[_stateIdx] = State::Nop;
				return NotFound;
			}
			return Busy;
		}

		RetValue Driver::deleteHeader(HeaderPage& header) // TODO (forcen#1#): Implement DeleteHeader
		{
			if(_error != 0) return Error;

			return Ok;
		}

		/**
		  * Read and write values
		  */
		RetValue Driver::readNextValue(HeaderPage& hp)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			_page = &hp;
			// Assume that datapage is loaded because of yes
			if(hp._currDP->_rowIdx < hp._currDP->getNumEntries()) // read from here
			{
				_opAddr = hp._currDP->getPageIdx()*PageWidth + DataPage::_header_span + hp._currDP->_rowWidth*hp._currDP->_rowIdx;
				hp._currDP->_rowIdx++; // por lo menos
				_row = static_cast<Row*>(_alloc->Allocate(sizeof(Row), reinterpret_cast<uintptr_t>(this)));
				_row->clear();

				for(int i = 0; i < 16; ++i)  _row->vals[i].format = hp._formats[i];

				_states[0] = State::ReadValue;
				_states[1] = State::Full;

				readPage(hp._currDP->_rowWidth, _opAddr);
				return Ok;
			}
			else
			{
				_states[3] = State::DataPageReadVal;
				findDataPage(++hp._data_idx);
			}
			return Ok;
		}

		RetValue Driver::appendValue(HeaderPage& hp, Row& row)
		{
			//things that can go wrong:
			//Format check
			//Date check
			//Space check

			//The first two are handled here, the last one will be handled in next steps
			//Since space check should not be a problem (garbage collector will free the oldest data pages) it is not considered an error

			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;
			if(hp._currDP != nullptr)
				Log::Verbose("Append to pag %d, pos %d\n", hp._currDP->_page_idx, hp._currDP->_rowIdx);
			else
				Log::Verbose("Creating datapage to hp %d\n", hp._object_idx);

			_page = &hp;
			_row = new(_alloc->Allocate(sizeof(Row), reinterpret_cast<uintptr_t>(this)))Row(row);

			uint32_t period_seconds = getSecondsFromPeriod(hp.getPeriod());
			if(row.rowDate.timestamp() % period_seconds > period_seconds/10) return OutOfTime; // it does not fit
			row.rowDate.snapToPeriod(hp.getPeriod());

			if(!hp.checkFormat(row)) return ColError;

			if(hp._currDP == nullptr)// find matching dp or empty dp
			{
				hp._currDP = new DataPage();
				hp._currDP->_header = &hp;
				_states[0] = State::Read;
				_states[1] = State::HeaderPageReadDataIndex; // reset or die
				_states[2] = State::DataPageCheckHeaderDate; //if not, skip to 0
				_states[3] = State::DataPageAppend;
				_states[4] = State::Wait;

				hp._data_idx = 0;
				readPage(2, hp.getPageIdx()*PageWidth+hp.getSize());
			}
			else
				return flushValue();

			return Ok;
		}

		/**
		  * File functions
		  */

		RetValue Driver::openFile(const uint8_t* name, PageAccessMode mode)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			FilePage* fp = new FilePage();

			_page = fp;
			fp->_page_state = 0xff;
			fp->_page_mode = mode;
			fp->_file_size = 0xffff;

			for(int i = 0; i < 16; ++i) fp->_name[i] = 0;
			for(int i = 0; i < 16; ++i) fp->_name[i] = name[i];

			_states[0] = State::FindNewObject;
			if(mode == PageAccessMode::PageRead)
			{
				_states[1] = State::LoadPage;
				_states[2] = State::Full;
			}
			else if(mode == PageAccessMode::PageWrite)
			{
				_states[1] = State::WritePageObjectIndexInRoot;
				_states[2] = State::WritePageHeader;
				_states[3] = State::Full;
			}

			findNewObject();

			return Ok;
		}

		RetValue Driver::writeFile(FilePage& file, const uint8_t* data, uint16_t data_length)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			if((file._page_state & 0x70) == 0)
				return FullPage;

			if(file._databuf == nullptr)
			{
				file._databuf = static_cast<uint8_t*>(_alloc->Allocate(128, 3));
				file._data_status = 0;
				file._data_idx = 0;
			}

			if((file._file_size + file.getSize() + data_length) > PageWidth) return FullPage;

			for(int i = 0; i < data_length; ++i)
			{
				file._databuf[file._data_status * 64 + file._data_idx++] = data[i];
				if(file._data_idx >= 64)
				{
					flushFile(file);
					file._data_idx = 0;
					file._data_status = (file._data_status + 1) % 2;
				}
			}

			return Ok;
		}

		RetValue Driver::startGetNextFile(FilePage& file)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			_page = &file;

			_states[0] = State::FindNonEmpty;
			_states[1] = State::LoadPage;
			_states[2] = State::Found;

			_opIdx = _page->_object_idx;
			_page->_page_idx++;
			_opAddr = (_page->_page_idx-1)*2;
			int endAddr = clampAddrToPage(_opAddr, 256);
			_opLen = endAddr-_opAddr;
			readPage(_opLen, _opAddr);

			return Ok;
		}

		RetValue Driver::checkGetNextFile()
		{
			if(_states[_stateIdx] == State::Found)
			{
				_states[_stateIdx] = State::Nop;
				return Ok;
			}
			else if(_states[_stateIdx] == State::NotFound)
			{
				_states[_stateIdx] = State::Nop;
				return NotFound;
			}
			return Busy;
		}

		RetValue Driver::readNextBlockFile(FilePage& file)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			if(file._page_mode == PageAccessMode::PageEmpty) return NotFound;
			if(file._file_size <= file._read_bytes) return FileEnded;
			if(file._read_status == 2) return FullBuffer;
			file._read_status = 1;

			_states[0] = State::Read;
			_states[1] = State::Wait;
			_page = &file;

			_opLen = 128;
			if(file._file_size - file._read_bytes < _opLen) _opLen = file._file_size - file._read_bytes;
			_opAddr = file.getPageIdx()*PageWidth + file.getSize() + file._read_bytes;

			file._read_bytes += _opLen;

			if(file._databuf == nullptr)
				file._databuf = static_cast<uint8_t*>(_alloc->Allocate(_opLen, reinterpret_cast<uintptr_t>(&file)));

			readPage(_opLen, _opAddr, file._databuf);

			return Ok;
		}

		RetValue Driver::closeFile(FilePage& file)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			file._page_state = 0x0f;

			_states[1] = State::CloseFile;
			_states[2] = State::Wait;
			_page = &file;
			if(flushFile(file) != Ok)
			{
				step();
			}

			return Ok;
		}

		RetValue Driver::flushFile(FilePage& file)
		{
 			if(file._data_idx == 0) return Invalid;
			_states[0] = State::Write;
			//_states[1] = State::Wait;

			uint16_t fileHdrSize = file.getSize();

			uint32_t opAddr = file.getPageIdx()*PageWidth + fileHdrSize + file._file_size;
			file._file_size += file._data_idx;
			uint16_t writingLen = file._data_idx;
			file._data_idx = 0;

			writePage(&file._databuf[file._data_status*64], writingLen, opAddr);

			return Ok;
		}

		RetValue Driver::deleteFile(FilePage& file) // TODO (forcen#1#): Implement deleteFile
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			return Ok;
		}

		/**
		  * Create, find and delete data pages
		  */
		RetValue Driver::createDataPage()
		{
			_stateIdx = 0;
			_states[0] = State::Find;
			_states[1] = State::WritePageObjectIndexInRoot;
			_states[2] = State::WritePageHeader;
			_states[3] = State::SwapDataPageHeaderPage;
			_states[4] = State::DataPageFindEmptyInHeader;
			_states[5] = State::DataPageWriteInHeader;
			_states[6] = State::DataPageAppend;
			_states[7] = State::Wait;

			HeaderPage* hp = reinterpret_cast<HeaderPage*>(_page);
			DataPage* dp = hp->_currDP;

			Log::Verbose("Creating new dp after idx %d\n", dp->_page_idx);

			dp->_header = hp;
			dp->_block_date = _row->rowDate;
			dp->_object_idx = hp->getObjectIdx();
			dp->_period = hp->getPeriod();
			dp->_rowIdx = 0; // reset the row idx, since it is a new data page

			_opLen = dp->getSize();

			for(int i = 0; i < 16; ++i) dp->_formats[i] = hp->_formats[i];

			dp->_rowWidth = hp->getColumnStride();

			_page = hp->_currDP;

			return findEmptyPage();
		}

		RetValue Driver::findDataPage(uint16_t headerIdx)
		{
			_states[0] = State::Read;
			_states[1] = State::HeaderPageReadDataIndex;
			_states[2] = State::DataPageReadHeader;

			HeaderPage* hp = reinterpret_cast<HeaderPage*>(_page);

			if(hp->_currDP == nullptr)
			{
				hp->_currDP = new DataPage();
				hp->_currDP->_header = hp;
				hp->_currDP->_period = hp->getPeriod();
				hp->_currDP->_rowWidth = hp->getColumnStride();
				hp->_currDP->_object_idx = hp->getObjectIdx();
			}

			hp->_currDP->_rowIdx = 0;

			readPage(2, hp->getPageIdx()*0x1000+hp->getSize()+headerIdx*2);

			return Ok;
		}

		RetValue Driver::deleteDataPage(DataPage& dp)
		{
			if(_error != 0) return Error;

			return Ok;
		}

		RetValue Driver::flushValue()
		{
			HeaderPage* hp = reinterpret_cast<HeaderPage*>(_page);
			DataPage* dp = hp->_currDP;

			_stateIdx = 0;
			_states[0] = State::Write;
			_states[1] = State::FreeRow;
			_states[2] = State::Wait;

			uint16_t num_entry = (_row->rowDate.timestamp()-dp->_block_date.timestamp())/getSecondsFromPeriod(dp->_period);
			if(num_entry >= dp->getNumEntries()) return createDataPage(); // if full then create another page
			if(num_entry < dp->_rowIdx) return createDataPage(); // non monotonic values, raise error
			dp->_rowIdx = num_entry;

			_opAddr = num_entry*dp->_rowWidth + dp->_header_span + dp->getPageIdx()*PageWidth;
			_opLen = 0;
			_opBuf = _alloc->Allocate(dp->_rowWidth, reinterpret_cast<uintptr_t>(this));

			uint8_t* opBuf = static_cast<uint8_t*>(_opBuf);

			opBuf[0] = DataPage::_starter_magic[0];
			opBuf[1] = DataPage::_starter_magic[1];

			int bufIdx = _row->serialize(opBuf+2)+2;

			opBuf[bufIdx++] = DataPage::_ender_magic[0];
			opBuf[bufIdx++] = DataPage::_ender_magic[1];

			writePage(opBuf, dp->_rowWidth, _opAddr); //_opAddr should be updated

			return Ok;
		}

		/**
		  * ISR
		  */

		void Driver::flashModuleISR()
		{
			if(_states[_stateIdx] == State::Find)
			{
				uint16_t readVal;
				int i;

				for(i = 0; i < 128; ++i) // arreglar esto por si es la segunda lectura
				{
					readVal = (pop() & 0xff);
					readVal |= ((pop() << 8) & 0xff00);
					if(readVal == _opFind) // Lo compara con el valor que tiene que buscar
					{
						_opAddr = 2*i+_opAddr; // ha encontrado el valor

						step();
						break;
					}
					else
					{
						if(_opIdx < readVal) _opIdx = readVal;
					}
				}
				if(i == 128) // si llega al final y no encuentra lo que busca, pues sigue buscando
				{
					if(_opLen < 256 || _opFindEnd) //ending block
					{
						_opAddr = 0xffffffff;
						step();
					}
					_opAddr += 256; // next iteration starting addr
					int _endOpaddr = (_opAddr & 0xfffff000)+0x1000;
					if((_opAddr + 256) < _endOpaddr) _endOpaddr = _opAddr + 256;
					if(_opAddr & 0xfffff000 == _endOpaddr & 0xfffff000) //they are in the same block
					{
						_opAddr += 256;
						_opLen= 256;
						readPage(_opLen, _opAddr);
					}
					else
					{
						_opLen = _endOpaddr-_opAddr;
						readPage(_opLen, _opAddr);
						_opFindEnd = true;
					}
				}
			}

			else if(_states[_stateIdx] == State::Write)
			{
				if(_opLen > 0)
				{
					int writingLen = _opLen;
					if(writingLen > 255) writingLen = 255;
					_opLen -= writingLen; // le quita lo que va a escribir

					_opAddr += writingLen;
					writePage(static_cast<uint8_t*>(_opBuf)+_opAddr, writingLen, _opAddr-writingLen);
				}
				else
				{
					if(_opBuf != nullptr)
					{
						_alloc->Deallocate(_opBuf);
						_opBuf = nullptr;
					}
					step();
				}
			}

			else if(_states[_stateIdx] == State::Read)
			{
				if(_page->getType() == FileType)
				{
                    FilePage* fp = reinterpret_cast<FilePage*>(_page);
                    if(fp->_read_status = 1)
						fp->_read_status = 2;
				}
				step(); // esto se puede mejorar
			}

			else if(_states [_stateIdx] == State::ReadValue)
			{
				uint16_t magic =  ((pop() << 8) & 0xff00) | (pop() & 0xff);
				HeaderPage* hp = reinterpret_cast<HeaderPage*>(_page);
				if(magic != 0xdead)
				{
					if(hp->_currDP->_rowIdx < hp->_currDP->getNumEntries())
					{
						_opAddr = hp->_currDP->getPageIdx()*PageWidth + DataPage::_header_span + hp->_currDP->_rowWidth*hp->_currDP->_rowIdx;
						hp->_currDP->_rowIdx++;
						readPage(hp->_currDP->_rowWidth, _opAddr);
					}
					else
					{
						_states[3] = State::DataPageReadVal;
						findDataPage(++hp->_data_idx);
					}
				}
				else
				{
					for(int i = 0; i < 16; ++i)
					{
						for(int j = 0; j < getFormatWidth(_row->vals[i].format); ++j)
						{
							_row->vals[i].data.bytes[j] = pop() & 0xff;
						}
					}
					_row->rowDate = hp->_currDP->_block_date + getSecondsFromPeriod(hp->_period)*hp->_currDP->_rowIdx;
					_row->rowDate.exists = 0;
					_states[_stateIdx] = State::Full;
				}
			}

			else if(_states[_stateIdx] == State::FindNewObject) // recibir 128 bytes de la cabecera
			{
				for(int i = 0; i < 32 && _opAddr < 0x800000; ++i)
				{
					_opAddr += PageWidth; //incrementar la dirección, ya que esta apuntará a la cabecera
					uint16_t object_id = (pop()&0xff) | ((pop()<<8) & 0xff00);
					if(_opFind < object_id) // solo considerar las cabeceras
					{
						_page->_object_idx = _opFind;
						_opFind = object_id;
						if(object_id == 0xffff)
						{
							if(_page->getAccessMode() == PageAccessMode::PageRead)
							{
								_states[_stateIdx] = State::Error;
								_error = NotFound;
								return;
							}
							else
							{
								_page->_object_idx = _page->_object_idx + 1; // se guarda el último índice en la página y se avanza la opción a la escritura
								step();
								return;
							}
						}
						else
						{
							_states[_stateIdx] = State::FindObjectName;
							readPage(64, _opAddr); //leer cabecera
							return;
						}
					}
				}
				if(_opAddr < 0x800000) // comprobar si no se sale del tamaño de la flash
					readPage(64, _opAddr/PageWidth*2); // se vuelve a leer en iteraciones posteriores de _opAddr
				else
				{
					_states[_stateIdx] = State::Error;
					_error = NotFound;
				}
			}
			else if(_states[_stateIdx] == State::FindObjectName)
			{
				uint8_t pageType = pop();
				if(pageType != _page->getType()) // no es la página que se buscaba, por lo que se busca otra
				{
					_states[_stateIdx] = State::FindNewObject;
					readPage(64, _opAddr/PageWidth*2); // volver a leer índices TODO hacerlo en una función que no lea más allá del final de la flash
					return;
				}

				uint16_t object_id = (pop() & 0xff) | ((pop()<<8) & 0xff00);

				int i;
				for(i = 0; i < 16; ++i)
				{
					char c = pop();
					if(c != _page->_name[i])
					{
						_states[_stateIdx] = State::FindNewObject;
						readPage(128, _opAddr/PageWidth*2);
						return;
					}
					if(c == 0)
						break;
				}
				_page->_object_idx = _opFind;
				_page->_page_idx = _opAddr/PageWidth;
				step(); // recover page from stack
			}

			else if(_states[_stateIdx] == State::FindNonEmpty)
			{
				uint16_t readVal;
				int i;

				for(i = 0; i < _opLen/2; ++i) // arreglar esto por si es la segunda lectura
				{
					readVal = (pop() & 0xff);
					readVal |= ((pop() << 8) & 0xff00);
					if(readVal != _opFind && readVal != 0xffff) // Lo compara con el valor que tiene que buscar
					{
						_opFind = readVal; // avoid consecutive idx to be read
						_opAddr = 2*i+_opAddr; // ha encontrado el valor
						_page->_page_idx = (_opAddr/2)+1;
						_states[_stateIdx] = State::FindObjectType;
						readPage(64, _page->_page_idx*PageWidth); //from root address to page address
						break;
					}
				}
				if(i == 128) // si llega al final y no encuentra lo que busca, pues sigue buscando
				{
					if(_opLen < 256 || _opFindEnd) //ending block
					{
						_opAddr = 0xffffffff;
						_states[_stateIdx] = State::NotFound;
					}
					_opAddr += 256; // next iteration starting addr
					int _endOpaddr = (_opAddr & 0xfffff000)+0x1000;
					if((_opAddr + 256) < _endOpaddr) _endOpaddr = _opAddr + 256;
					if(_opAddr & 0xfffff000 == _endOpaddr & 0xfffff000) //they are in the same block
					{
						_opLen= 256;
						readPage(_opLen, _opAddr);
					}
					else
					{
						_opLen = _endOpaddr-_opAddr;
						readPage(_opLen, _opAddr);
						_opFindEnd = true;
					}
				}
			}

			else if(_states[_stateIdx] == State::FindObjectType)
			{
				uint8_t pageType = pop();
				if(pageType == _page->getType())
				{
					_page->_object_idx = (pop() & 0xff) | ((pop() << 8) & 0xff00);
					for(int i = 0; i < 16; ++i) _page->_name[i] = 0;
					for(int i = 0; i < 16; ++i)
					{
						char c = pop();
						if(c!=0) _page->_name[i] = c;
						else break;
					}

					_states[_stateIdx] = State::Read;
					step();
				}
				else
				{
					_states[_stateIdx] = State::FindNonEmpty;

					_opAddr += 2;
					int endAddr = _opAddr+256;
					if(endAddr & 0xfffff000 != _opAddr & 0xfffff000)
						endAddr = endAddr & 0xfffff000;

					_opLen = endAddr-_opAddr;

					readPage(_opLen, _opAddr);
				}
			}
		}

		void Driver::step()
		{
			++_stateIdx;
			if(_states[_stateIdx] == State::WritePageObjectIndexInRoot)
			{
				if(_page->getType() == DataType)
					_page->_page_idx = _opAddr/2+1; // it comes from find
				else
					_page->_page_idx = _opAddr/PageWidth; // it comes from findNewObject

				uint8_t buf[2] = {_page->getObjectIdx() & 0xff, (_page->getObjectIdx() >> 8) & 0xff};
				_states[_stateIdx] = State::Write;
				_opLen = 0;
				writePage(buf, 2, (_page->getPageIdx()-1)*2);
			}

			else if(_states[_stateIdx] == State::WritePageHeader)
			{
				_opLen = _page->getSize();
				_opAddr = _page->getPageIdx()*PageWidth;
				_opBuf = _alloc->Allocate(_opLen, reinterpret_cast<uintptr_t>(this));
				_page->serialize(static_cast<uint8_t*>(_opBuf));

				if(_page->getType() == FileType)
					static_cast<FilePage*>(_page)->_file_size = 0;

				int writingLen = _opLen;
				if(writingLen > 255) writingLen = 255;
				_opLen -= writingLen;
				_opAddr+= writingLen;

				_states[_stateIdx] = State::Write;

				writePage(static_cast<uint8_t*>(_opBuf), writingLen, _opAddr-writingLen);
			}

			else if(_states[_stateIdx] == State::WritePageUpdate)
			{
				if(_states[_stateIdx-1] == State::FindObjectName) //page exists
				{
					_states[_stateIdx+1] = State::LoadPage;
					_states[_stateIdx+2] = State::Full;
					step();
					//readPage(128, _page->getPageIdx()*PageWidth);
				}
				else if(_states[_stateIdx-1] == State::FindNewObject) //page does not exist
				{
					_states[_stateIdx+1] = State::WritePageObjectIndexInRoot;
					_states[_stateIdx+2] = State::WritePageHeader;
					_states[_stateIdx+3] = State::Full;
					step();
				}
				else
				{
					_states[_stateIdx] = State::Error;
					_error = Invalid;
				}
			}

			else if(_states[_stateIdx] == State::DataPageFindEmptyInHeader)
			{
				HeaderPage* hp = static_cast<HeaderPage*>(_page);
				_opFind = 0xffff;
				_opAddr = hp->getPageIdx()*PageWidth + hp->_header_spacing;

				_states[_stateIdx] = State::Find;

				readPage(256, _opAddr);
			}

			else if(_states[_stateIdx] == State::DataPageWriteInHeader)
			{
				DataPage* dp = reinterpret_cast<HeaderPage*>(_page)->_currDP;
				uint8_t buf[2] = {dp->getPageIdx()&0xff, (dp->getPageIdx() >> 8) & 0xff};
				_states[_stateIdx] = State::Write;
				_opLen = 0;
				writePage(buf, 2, _opAddr);
			}

			else if(_states[_stateIdx] == State::HeaderPageReadDataIndex)
			{
				uint16_t dataPageIdx = pop() | ((pop() << 8) & 0xff00);
				HeaderPage* hp = reinterpret_cast<HeaderPage*>(_page);
				if(dataPageIdx >= 2048) // there is no datapage and another one should be created
				{
					if(hp->getAccessMode() == PageAccessMode::PageRead || hp->getAccessMode() == PageAccessMode::PageEmpty)
					{
						_states[_stateIdx] = State::NotFound;
					}
					else
						createDataPage(); // create and read
				}
				else // load datapage and check date
				{
					hp->_currDP->_page_idx = dataPageIdx;
					_states[_stateIdx] = State::Read;
					readPage(11, dataPageIdx*PageWidth);
				}
			}

			else if(_states[_stateIdx] == State::DataPageReadHeader)
			{
				pop(); pop(); pop();
				pop(); pop();
				uint8_t dbuf[5];
				for(int i = 0; i < 5; ++i) dbuf[i] = pop();

				DataPage* dp = static_cast<HeaderPage*>(_page)->_currDP;
				dp->_block_date.deserialize(dbuf);
				step();
			}

			else if(_states[_stateIdx] == State::DataPageCheckHeaderDate)
			{
				HeaderPage* hp = reinterpret_cast<HeaderPage*>(_page);
				uint8_t buf[10];
				for(int i = 0; i < 10; ++i) buf[i] = pop();

				hp->_currDP->deserialize(buf);
				if(hp->_currDP->_block_date.exists) //finished block or malformed date
				{
					_stateIdx = 0;
					_states[0] = State::Read;
					_states[1] = State::HeaderPageReadDataIndex;
					_states[2] = State::DataPageCheckHeaderDate;
					_states[3] = State::Wait;

					hp->_data_idx++;
					readPage(2, hp->getPageIdx()*PageWidth+hp->getSize()+2*hp->_data_idx); // read next data page
				}
				else //good data page, should check if date is within this block
				{
					uint32_t _max_block_time = getSecondsFromPeriod(hp->getPeriod())*hp->_currDP->getNumEntries();
					uint64_t rowDateSecs = _row->rowDate.timestamp();
					uint64_t blockDate = hp->_currDP->_block_date.timestamp();
					uint64_t maxBlockDate = blockDate+_max_block_time;
					if(blockDate <= rowDateSecs && rowDateSecs <= maxBlockDate) // bingo
					{
						step();
					}
					else
					{
						_stateIdx = 0;
						_states[0] = State::Read;
						_states[1] = State::HeaderPageReadDataIndex;
						_states[2] = State::DataPageCheckHeaderDate;
						_states[3] = State::Wait;

						hp->_data_idx++;
						readPage(2, hp->getPageIdx()*PageWidth+hp->getSize()+2*hp->_data_idx); // read next data page
					}
				}
			}

			else if(_states[_stateIdx] == State::DataPageAppend)
			{
				flushValue();
			}

			else if(_states[_stateIdx] == State::DataPageReadVal)
			{
				HeaderPage* hp = reinterpret_cast<HeaderPage*>(_page);
				_opAddr = hp->_currDP->getPageIdx()*PageWidth + DataPage::_header_span + hp->_currDP->_rowWidth*hp->_currDP->_rowIdx;
				_states[_stateIdx] = State::ReadValue;
				_states[_stateIdx+1] = State::Full;

				if(_row == nullptr)
				{
					_row = static_cast<Row*>(_alloc->Allocate(sizeof(Row), reinterpret_cast<uintptr_t>(this)));
					_row->clear();
				}

				readPage(hp->_currDP->_rowWidth, _opAddr);
			}

			else if(_states[_stateIdx] == State::SwapDataPageHeaderPage)
			{
				if(_page->getType() == PageType::DataType)
				{
					_page = reinterpret_cast<DataPage*>(_page)->_header;
				}
				else if(_page->getType() == PageType::HeaderType)
				{
					_page = reinterpret_cast<HeaderPage*>(_page)->_currDP;
				}
				step();
			}

			else if(_states[_stateIdx] == State::LoadPage) //it comes from FindNewObject
			{
				if(_page->getType() == HeaderType)
				{
					HeaderPage* hp = static_cast<HeaderPage*>(_page);

					hp->_period = pop();

					int i;
					for(i = 0; i < 16; ++i)
					{
						uint8_t c = pop();
						if(c != 0 && c != 0xff)
						{
							hp->_formats[i] = static_cast<Format>(c);
							c = pop();
							int j;
							for(j = 0; j < 16; ++j)
							{
								if(c != 0) hp->_colNames[i][j] = c;
								else
								{
									 //c = pop();//hay que sacar a src_idx del 0
									break;
								}
								c = pop();
							}
						}
						else break; // después de esto ya se corta la vida
					}
					for(i = 0; i < 16; ++i) if(hp->_formats[i] > Format::Float)
					{
						Log::Error("Error on table header %s in page %d, format val is %d\n", hp->getName(), hp->getPageIdx(), hp->_formats[i]);
						hp->_formats[i] = Format::Invalid;
					}
					hp->_header_spacing = hp->getSize();

					uint16_t firstDP = pop() | ((pop() << 8) & 0xff00);
					if(firstDP != 0xffff)
					{
						hp->_currDP = new DataPage();
						hp->_currDP->_page_idx = firstDP;
						hp->_currDP->_object_idx = hp->_object_idx;
						hp->_currDP->_header = hp;
						hp->_currDP->_rowWidth = hp->getColumnStride();
						hp->_currDP->_period = hp->_period;
						hp->_data_idx = 0;
						for(int i = 0; i < 16; ++i) hp->_currDP->_formats[i] = hp->_formats[i];
						_states[_stateIdx+3] = _states[_stateIdx+1]; // full o found o lo que sea
						_states[_stateIdx] = State::Read;
						_states[_stateIdx+1] = State::LoadPage;
						_states[_stateIdx+2] = State::SwapDataPageHeaderPage;

						_page = hp->_currDP;

						readPage(5, _page->_page_idx*PageWidth+5);
					}
					else
					{
						step();
					}
				}
				else if(_page->getType() == DataType)
				{
					DataPage* dp = reinterpret_cast<DataPage*>(_page);

					uint8_t datebuf[5];
					for(int i = 0; i < 5;++i) datebuf[i] = pop();
					dp->_block_date.deserialize(datebuf);
					dp->_rowIdx = 0;
					step();
				}
				else if(_page->getType() == FileType)
				{
					FilePage* fp = static_cast<FilePage*>(_page);
					//fp->_object_idx = (pop() & 0xff) | ((pop() << 8) & 0xff00);
					//fp->_page_idx = _opAddr/PageWidth;
					/*for(int i = 0; i < 16; ++i)
					{
						if(fp->_name[i] == 0)
						{
							pop();
							break;
						}
						if(fp->_name[i] != pop())
						{
							_opAddr += PageWidth;
							readPage(24, _opAddr);
							return;
						}
					}*/
					fp->_page_state = pop();
					fp->_file_size = (pop()&0xff) | ((pop() << 8) & 0xff00);
					fp->_next_page = (pop()&0xff) | ((pop() << 8) & 0xff00);


					_states[_stateIdx] = State::Read;

					uint8_t read_len = 128;

					fp->_read_status = 1;

					if(read_len > fp->_file_size) read_len = fp->_file_size;
					fp->_read_bytes = read_len;
					if(fp->_databuf == nullptr)
					{
						fp->_databuf = reinterpret_cast<uint8_t*>(_alloc->Allocate(128, reinterpret_cast<uintptr_t>(this)));
					}
					uint32_t addr = fp->getPageIdx()*PageWidth+fp->getSize();
					readPage(read_len, addr, fp->_databuf);
				}
				else
				{
					_states[_stateIdx] = State::Error;
					_error = Invalid;
				}
			}
			else if(_states[_stateIdx] == State::CloseFile)
			{
				FilePage* fp = static_cast<FilePage*>(_page);
				_page = nullptr;
				uint8_t buf[64] = {0};

				uint8_t header_length = fp->serialize(buf);

				_states[_stateIdx] = State::Write;
				_states[_stateIdx+1] = State::Wait;
				writePage(buf, header_length, fp->getPageIdx()*PageWidth);
			}
			else if(_states[_stateIdx] == State::ReadFile)
			{
				FilePage* fp = reinterpret_cast<FilePage*>(_page);
				_states[_stateIdx] = State::Read;
				uint32_t addr = fp->getPageIdx()*PageWidth + fp->getSize();

				uint8_t read_len = 128;
				if(read_len > fp->_file_size) read_len = fp->_file_size;
				fp->_read_bytes = read_len;
				readPage(read_len, addr, fp->_databuf);
 			}
 			else if(_states[_stateIdx] == State::FreeRow)
			{
				if(_row != nullptr) _alloc->Deallocate(_row);
				_row = nullptr;
				step();
			}
			else if(_states[_stateIdx] == State::Full || _states[_stateIdx] == State::FreePage || _states[_stateIdx] == State::Error || _states[_stateIdx] == State::Wait || _states[_stateIdx] == State::Nop)
			{
				unlock();
			}
		}

		Driver::State Driver::getState()
		{
			return _states[_stateIdx];
		}


		Page* Driver::getPage()
		{
			if(_states[_stateIdx] != State::Full && _states[_stateIdx] != State::FreePage) return nullptr;
			_states[_stateIdx] = State::FreePage;
			return _page;
		}

		Row* Driver::getDataRow()
		{
			if(_states[_stateIdx] != State::Full && _states[_stateIdx] != State::FreeRow && _states[_stateIdx] != State::NotFound) return nullptr;
			if(_states[_stateIdx] == State::NotFound)
			{
				if(_row == nullptr) _row = reinterpret_cast<Row*>(_alloc->Allocate(sizeof(Row), reinterpret_cast<uintptr_t>(this)));
				_row->clear();
			}
			_states[_stateIdx] = State::FreeRow;
			return _row;
		}

		RetValue Driver::findEmptyPage()
		{
			_opAddr = 0;
			_opFind = 0xffff;
			_states[0] = State::Find;
			_stateIdx = 0;
			readPage(256, _opAddr);
			return Ok;
		}

		RetValue Driver::findNewObject()
		{
			_opAddr = 0;
			_opFind = 0;
			readPage(64, 0);
			return Ok;
		}

		int Driver::getError()
		{
			return _error;
		}

		void Driver::reset()
		{
			if(_states[_stateIdx] == State::FreePage)
			{
				if(_page != nullptr)
				{
					if(_page->getType() == PageType::DataType)
					{
						delete reinterpret_cast<DataPage*>(_page);
					}
					else if(_page->getType() == PageType::HeaderType)
					{
						delete reinterpret_cast<HeaderPage*>(_page);
					}
					else if(_page->getType() == PageType::FileType)
					{
						delete reinterpret_cast<FilePage*>(_page);
					}
					else
					{
						delete _page;
					}
					_page = nullptr;
				}
			}
			if(_states[_stateIdx] == State::FreeRow)
			{
				if(_row != nullptr)
				{
					_alloc->Deallocate(static_cast<void*>(_row));
					_row = nullptr;
				}
			}

			for(int i = 0; i < 16; ++i) _states[i] = State::Nop;
			_stateIdx = 0;
		}


		RetValue Driver::prepareCom()
		{
			if(_error != 0) return Error;
			if(_states[_stateIdx] == State::Full) return FullBuffer;
			if(_states[_stateIdx] != State::Nop && _states[_stateIdx] != State::Wait && _states[_stateIdx] != State::FreePage && _states[_stateIdx] != State::FreeRow) return Busy;
			if(try_lock() == -1 ) return Busy;

			reset();

			return Ok;
		}
	}
}
