#include <eTSDB_Driver.hpp>
namespace FwLogger
{
	namespace eTSDB
	{
		Driver::Driver(uint32_t offsetAddress, uint32_t size, SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin, Allocator<128>* alloc):
			SPIFlash(hspi, gpio, pin), Module("eTSDB 0.2")
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
			}
			else
			{
				_states[1] = State::LoadPage;
				_states[2] = State::Full;
			}

			findNewObject();

			return Ok;
		}

		RetValue Driver::openHeader(const uint8_t* name, PageAccessMode am, HeaderInitializer hi)
		{
			const uint8_t* colnames[16];
			for(int i = 0; i < 16; ++i) colnames[i] = &hi.colNames[i][0];

			return openHeader(name, am, hi.period, hi.colLen, hi.formats, colnames);
		}

		RetValue Driver::deleteHeader(HeaderPage& header)
		{
			if(_error != 0) return Error;

			return Ok;
		}

		/**
		  * Create, find and delete data pages
		  */
		RetValue Driver::createDataPage(HeaderPage& header, Date& date)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			_states[0] = State::Find;
			_states[1] = State::WritePageObjectIndexInRoot;
			_states[2] = State::DataPageFindEmptyInHeader;
			_states[3] = State::DataPageWriteInHeader;
			_states[4] = State::WritePageHeader;
			_states[5] = State::Full;

			DataPage* dp = new DataPage();
			_page = dp;

			dp->_header = &header;
			dp->_block_date = date;
			dp->_object_idx = header.getObjectIdx();
			dp->_period = header.getPeriod();

			_opLen = dp->getSize();

			for(int i = 0; i < 16; ++i) dp->_formats[i] = header._formats[i];

			dp->_rowWidth = header.getColumnStride();

			//_opAddr = dp->getPageIdx()*0x1000;

			return findEmptyPage();
		}

		RetValue Driver::findDataPage(HeaderPage& header, uint16_t headerIdx)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			_states[0] = State::Read;
			_states[1] = State::HeaderPageReadDataIndex;
			_states[2] = State::DataPageReadHeader;
			_states[3] = State::DataPageFindEmptyBody;
			_states[4] = State::Full;

			DataPage* dp = new DataPage();
			_page = dp;

			dp->_header = &header;
			dp->_period = header.getPeriod();
			dp->_rowWidth = header.getColumnStride();
			dp->_object_idx = header.getObjectIdx();

			readPage(2, header.getPageIdx()*0x1000+header.getSize()+headerIdx*2);

			return Ok;
		}

		RetValue Driver::deleteDataPage(DataPage& dp)
		{
			if(_error != 0) return Error;

			return Ok;
		}

		/**
		  * Read and write values
		  */
		RetValue Driver::readValue(DataPage& page, uint16_t meas_idx)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			_opAddr = page.getPageIdx()*PageWidth + DataPage::_header_span + page._rowWidth*meas_idx;
			_row = static_cast<Row*>(_alloc->Allocate(sizeof(Row)));
			_row->clear();


			for(int i = 0; i < 16; ++i)  _row->vals[i].format = page._formats[i];

			_states[0] = State::ReadValue;
			_states[1] = State::Full;

			readPage(page._rowWidth, _opAddr);
			return Ok;
		}

		RetValue Driver::appendValue(DataPage& page, Row& row)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			if(page._rowWidth != row.getBufSize()+4) return Error; // Format check

			uint16_t writeAddr = page._header_span + page._rowIdx*page._rowWidth;
			if(0x1000-writeAddr < page._rowWidth) return FullPage; // Space check

			uint64_t block_secs = page.getBlockDate().seconds()+getSecondsFromPeriod(page._period)*page._rowIdx;

			int32_t dtime = row.rowDate.seconds()-block_secs;
			if(dtime < 0) dtime = -dtime;
			if(dtime > getSecondsFromPeriod(page._period)*.1) return OutOfTime; // Time check

			_states[0] = State::Write;

			_opAddr = page.getPageIdx()*0x1000 + writeAddr;
			++page._rowIdx; // porque es lo que va a pasar
			_opLen = 0; // no tiene que mandarse na más
			_opBuf = _alloc->Allocate(page._rowWidth);

			uint8_t* opBuf = static_cast<uint8_t*>(_opBuf);

			opBuf[0] = DataPage::_starter_magic[0];
			opBuf[1] = DataPage::_starter_magic[1];

			int bufIdx = 2;
			int valIdx;
			for(valIdx = 0; valIdx < 16 && row.vals[valIdx].format != Format::Invalid; ++valIdx)
			{
				for(int innerIdx=0; innerIdx< getFormatWidth(row.vals[valIdx].format); ++innerIdx)
					opBuf[bufIdx++] = row.vals[valIdx].data.bytes[innerIdx];
			}

			opBuf[bufIdx++] = DataPage::_ender_magic[0];
			opBuf[bufIdx++] = DataPage::_ender_magic[1];

			writePage(opBuf, page._rowWidth, _opAddr);

			return Ok;
		}

		/**
		  * File functions
		  */

		RetValue Driver::openFile(const uint8_t* name, PageAccessMode mode) // debería añadir un comprobador de colisiones, cambiar por open
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			FilePage* fp = new FilePage();

			_page = fp;
			fp->_page_state = 0xff;
			fp->_page_mode = mode;
			fp->_file_size = 0xffff;

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
				file._databuf = static_cast<uint8_t*>(_alloc->Allocate(128));
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

		RetValue Driver::readNextBlockFile(FilePage& file)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			if(file._read_status == 2) return FullBuffer;
			file._read_status = 1;

			if(file._page_mode == PageAccessMode::PageEmpty) return NotFound;

			_states[0] = State::Read;
			_page = &file;


			if(file._file_size <= file._read_bytes) return FileEnded;

			_opLen = 128;
			if(file._file_size - file._read_bytes < _opLen) _opLen = file._file_size - file._read_bytes;
			_opAddr = file.getPageIdx()*PageWidth + file.getSize() + file._read_bytes;

			file._read_bytes += _opLen;

			if(file._databuf == nullptr)
				file._databuf = static_cast<uint8_t*>(_alloc->Allocate(_opLen));

			readPage(_opLen, _opAddr, file._databuf);

			return Ok;
		}

		RetValue Driver::closeFile(FilePage& file)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

			file._page_state = 0x0f;

			_states[1] = State::CloseFile;
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

			uint16_t fileHdrSize = file.getSize();

			uint32_t opAddr = file.getPageIdx()*PageWidth + fileHdrSize + file._file_size;
			file._file_size += file._data_idx;
			uint16_t writingLen = file._data_idx;
			file._data_idx = 0;

			writePage(&file._databuf[file._data_status*64], writingLen, opAddr);

			return Ok;
		}

		RetValue Driver::deleteFile(FilePage& file)
		{
			RetValue prep_ret = prepareCom();
			if(prep_ret != Ok) return prep_ret;

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
					if( ((_opAddr+256)/0x1000) != (_opAddr/0x1000))
					{
						_opAddr += 256;
						readPage(256, _opAddr);
					}
					else
					{
						_opAddr = 0;
						step();
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
				if(magic != 0xdead)
				{
					_error = 1;
					_states[_stateIdx] = State::Nop;
					return;
				}
				for(int i = 0; i < 16; ++i)
				{
					for(int j = 0; j < getFormatWidth(_row->vals[i].format); ++j)
					{
						_row->vals[i].data.bytes[j] = pop() & 0xff;
					}
				}
				_states[_stateIdx] = State::Full;
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
			if(_states[_stateIdx] != State::Full && _states[_stateIdx] != State::FreePage) return nullptr;
			_states[_stateIdx] = State::FreePage;
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
					delete _page;
					_page = nullptr;
				}
				if(_row != nullptr)
				{
					_alloc->Deallocate(static_cast<void*>(_row));
					_row = nullptr;
				}
			}

			for(int i = 0; i < 16; ++i) _states[i] = State::Nop;
			_stateIdx = 0;
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
				_opBuf = _alloc->Allocate(_opLen);
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
				}
				else if(_states[_stateIdx-1] == State::FindNewObject) //page does not exist
				{
					_states[_stateIdx+1] = State::WritePageObjectIndexInRoot;
					_states[_stateIdx+2] = State::WritePageHeader;
					_states[_stateIdx+3] = State::Full;
				}
				else
				{
					_states[_stateIdx+1] = State::Error;
					_error = Invalid;
				}
				step();
			}

			else if(_states[_stateIdx] == State::DataPageFindEmptyInHeader)
			{
				DataPage* dp = static_cast<DataPage*>(_page);
				_opFind = 0xffff;
				_opAddr = dp->_header->getPageIdx()*PageWidth+dp->_header->_header_spacing;

				_states[_stateIdx] = State::Find;

				readPage(256, _opAddr);
			}

			else if(_states[_stateIdx] == State::DataPageWriteInHeader)
			{
				uint8_t buf[2] = {_page->getPageIdx()&0xff, (_page->getPageIdx() >> 8) & 0xff};
				_states[_stateIdx] = State::Write;
				_opLen = 0;
				writePage(buf, 2, _opAddr);
			}

			else if(_states[_stateIdx] == State::HeaderPageReadDataIndex)
			{
				uint16_t pageIdx = (pop()& 0xff) | ((pop() << 8) | 0xff00);
				_page->_page_idx = pageIdx;

				_states[_stateIdx] = State::Read;
				readPage(DataPage::_header_span, _page->_page_idx*PageWidth);

			}

			else if(_states[_stateIdx] == State::DataPageReadHeader)
			{
				pop(); pop(); pop();
				pop(); pop();
				uint8_t dbuf[5];
				for(int i = 0; i < 5; ++i) dbuf[i] = pop();

				DataPage* dp = static_cast<DataPage*>(_page);
				dp->_block_date.deserialize(dbuf);

				_opFind = 0xffff;
				_states[_stateIdx] = State::Find;
				readPage(256, dp->_page_idx*PageWidth+DataPage::_header_span);
			}

			else if(_states[_stateIdx] == State::DataPageFindEmptyBody)
			{
				if(_opAddr == 0)
				{
					static_cast<DataPage*>(_page)->_rowIdx = 0xffff;
				}
				else
				{
					DataPage* dp = static_cast<DataPage*>(_page);
					uint32_t innerAddr = _opAddr-dp->getPageIdx()*PageWidth-DataPage::_header_span;
					if(innerAddr % dp->_rowWidth != 0)
					{
						//unformatted block
						dp->_rowIdx = 0xffff;
					}
					else
					{
						dp->_rowIdx = innerAddr/dp->_rowWidth;
					}
				}
				step(); // estará llena y listo
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
					hp->_header_spacing = hp->getSize();
					_states[_stateIdx] = State::Full;
				}
				else if(_page->getType() == DataType)
				{

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
						fp->_databuf = reinterpret_cast<uint8_t*>(_alloc->Allocate(128));
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
			else if(_states[_stateIdx] == State::Full || _states[_stateIdx] == State::FreePage || _states[_stateIdx] == State::Error || _states[_stateIdx] == State::Wait)
			{
				unlock();
			}
		}

		RetValue Driver::prepareCom()
		{
			if(_error != 0) return Error;
			if(_states[_stateIdx] == State::Full) return FullBuffer;
			if(try_lock() == -1 ) return Busy;

			reset();

			return Ok;
		}
	}
}
