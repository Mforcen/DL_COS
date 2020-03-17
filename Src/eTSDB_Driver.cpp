#include <eTSDB_Driver.hpp>

namespace eTSDB
{
    Driver::Driver(uint32_t offsetAddress, uint32_t size, SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin):
    	SPIFlash(hspi, gpio, pin), _alloc(_alloc_buf, sizeof(_alloc_buf))
    {
        _offset = offsetAddress;
        _size = size;
		Page::setAllocator(&_alloc);

		_error = 0;
		_stateIdx = 0;
		_opBuf = nullptr;

		_page = nullptr;
		_row = nullptr;
    }

    RetValue Driver::createHeader(uint8_t* name, uint8_t period, uint8_t colLen, Format* formats, uint8_t** colNames)
    {
		RetValue prep_ret = prepareCom();
		if(prep_ret != Ok) return prep_ret;

		_states[0] = State::Find;
		_states[1] = State::StoreIndexInPage;
		_states[2] = State::WritePageTableIndexInRoot;
		_states[3] = State::WritePageHeader;
		_states[4] = State::Full;

		HeaderPage* hp = new HeaderPage(); // this will be allocated using the mempool

		_page = hp;

		hp->_period = period;

		if(colLen > 16) colLen = 16;

		int i;
		for(i = 0; i < 16 && name[i] != 0; ++i)
		{
			hp->_name[i] = name[i];
		}

		for(int i = 0; i < colLen; ++i)
		{
			hp->_formats[i] = formats[i];
			for(int j = 0; j < 16 && colNames[i][j] != 0; ++j) hp->_colNames[i][j] = colNames[i][j];
		}

		hp->_header_spacing = hp->getSize();

		return findEmptyPage();
    }

	RetValue Driver::findHeader(uint8_t* name)
	{
		RetValue prep_ret = prepareCom();
		if(prep_ret != Ok) return prep_ret;

		_states[0] = State::FindString; // hay que tener en cuenta que este estado itera sobre si mismo
		_states[1] = State::ReadPageHeader;

		for(int i = 0; i < 16; ++i)
		{
			if(name[i] == 0)
			{
				_opLen = i;
				break;
			}
		}

		_opBuf = _alloc.Allocate(16);
		_opAddr = 0;

		HeaderPage* hp = new HeaderPage();

		for(int i = 0; i < 16; ++i) hp->_name[i] = 0;
		for(int i = 0; i < _opLen; ++i)
		{
			static_cast<uint8_t*>(_opBuf)[i] = name[i];
			hp->_name[i] = name[i];
		}

		_page = hp;

		readPage(256, 0);

		return Ok;
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
		_states[1] = State::StoreIndexInPage;
		_states[2] = State::WritePageTableIndexInRoot;
		_states[3] = State::DataPageFindEmptyInHeader;
		_states[4] = State::DataPageWriteInHeader;
		_states[5] = State::WritePageHeader;
		_states[6] = State::Full;

		DataPage* dp = new DataPage();
		_page = dp;

		dp->_header = &header;
		dp->_block_date = date;
		dp->_table_idx = header.getTableIdx();
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
    	dp->_table_idx = header.getTableIdx();

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
		_row = static_cast<Row*>(_alloc.Allocate(sizeof(Row)));
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
		_opBuf = _alloc.Allocate(page._rowWidth);

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
					_alloc.Deallocate(_opBuf);
					_opBuf = nullptr;
				}
				step();
			}
		}
		else if(_states[_stateIdx] == State::FindString)
		{
            int i, valid = 0;
            uint8_t* opBuf = static_cast<uint8_t*>(_opBuf);
            for(int i = 0; i < 256; ++i)
			{
				if(pop() == opBuf[valid])
				{
					valid++;
					if(valid == _opLen)
					{
						step();
						break;
					}
				}
				else
				{
					valid = 0;
				}
			}
			//poner algo para leer más
		}
		else if(_states[_stateIdx] == State::Read)
		{
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
				_alloc.Deallocate(static_cast<void*>(_row));
				_row = nullptr;
			}
    	}

    	for(int i = 0; i < 16; ++i) _states[i] = State::Nop;
    	_stateIdx = 0;
    }

    void Driver::step()
    {
        bool exit;
        do
		{
			++_stateIdx;
			exit = true;
			if(_states[_stateIdx] == State::StoreIndexInPage)
			{
				_page->_page_idx = _opAddr/2+1; // _opAddr es la dirección donde estaba el valor, que en rootpage cada 2 bytes es una página pero la página 0 se salta
				if(_page->getType() == HeaderType) _page->_table_idx = _opIdx+1;
				exit = false;
			}
			else if(_states[_stateIdx] == State::WritePageTableIndexInRoot)
			{
				uint8_t buf[2] = {_page->getTableIdx() & 0xff, (_page->getTableIdx() >> 8) & 0xff};
				_states[_stateIdx] = State::Write;
				_opLen = 0;
				writePage(buf, 2, (_page->getPageIdx()-1)*2);
			}
			else if(_states[_stateIdx] == State::WritePageHeader)
			{
				_opLen = _page->getSize();
				_opAddr = _page->getPageIdx()*PageWidth;
				_opBuf = _alloc.Allocate(_opLen);
				_page->serialize(static_cast<uint8_t*>(_opBuf));

				int writingLen = _opLen;
				if(writingLen > 255) writingLen = 255;
				_opLen -= writingLen;
				_opAddr+= writingLen;

				_states[_stateIdx] = State::Write;

				writePage(static_cast<uint8_t*>(_opBuf), writingLen, _opAddr-writingLen);
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
		} while(!exit);
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
