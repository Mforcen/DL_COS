#include "eTSDB_Pages.hpp"

static_assert(sizeof(float) == 4, "Float type should be of 4 bytes");

#define BLOCK_OFFSET 0x1000

namespace eTSDB
{

	const uint8_t DataPage::_starter_magic[] = {0xde, 0xad};
	const uint8_t DataPage::_ender_magic[] = {0xbe, 0xef};

	uint8_t getFormatWidth(Format format)
	{
		switch(format)
		{
		case Format::Uint8:
		case Format::Int8:
			return 1;
		case Format::Uint16:
		case Format::Int16:
			return 2;
		case Format::Uint32:
		case Format::Int32:
		case Format::Float:
			return 4;
		default:
			return 0;
		}
	}

	uint32_t getSecondsFromPeriod(uint8_t period)
	{
        if(period < 60) return period;
        else if(period < 120) return (period-59)*60;
        else if(period < 144) return (period-119)*86400;
	}

	Allocator* eTSDB::Page::_alloc = nullptr;

	int Page::getSize()
	{
		int size = getTypeSize();
		if(size == -1)  return -1;
		size += 3;
		return size;
	}

	int Page::getTypeSize()
	{
		return 0;
	}

	int Page::serialize(uint8_t* buf)
	{
		buf[0] = getType();
		buf[1] = getTableIdx() & 0xff;
		buf[2] = (getTableIdx() >> 8) & 0xff;
	}

	/**
	  * Data Page implementation
	  */

	DataPage::DataPage() : Page(0, DataType, 0)
	{
		_rowIdx = 0;
		_rowWidth = 0;
		for(int i = 0; i < 16; ++i) _formats[i] = Format::Invalid;
	}

	DataPage::DataPage(uint16_t page_idx, uint16_t table_idx, Date block_date, HeaderPage* header) : Page(page_idx, DataType, table_idx)
	{
		_block_date = block_date;
		_header = header;
		_rowIdx = 0;
		_rowWidth = 0;
		for(int i = 0; i < 16; ++i) _formats[i] = Format::Invalid;
	}

	int DataPage::getTypeSize()
	{
		return 7;
	}

	int DataPage::serialize(uint8_t* dst)
	{
		dst[0] = getType();
		dst[1] = getTableIdx() & 0xff;
		dst[2] = (getTableIdx() >> 8) & 0xff;

		dst[3] = _header->getPageIdx() & 0xff;
		dst[4] = (_header->getPageIdx() >> 8) & 0xff;

		_block_date.serialize(&dst[5]);
		return 10;
	}

	void DataPage::copy(DataPage* dp)
	{
		_table_idx = dp->_table_idx;
		_page_idx = dp->_page_idx;
		_header = dp->_header;
		_rowIdx = dp->_rowIdx;
		_rowWidth = dp->_rowWidth;
		_block_date = dp->_block_date;
		//_numCols = dp->_numCols;
		_period = dp->_period;
		for(int i = 0; i < 16; ++i) _formats[i] = dp->_formats[i];
	}

	Date DataPage::getBlockDate()
	{
		return _block_date;
	}

	/**
	  * Header Page implementation
	  */

	HeaderPage::HeaderPage(HeaderPage* hp) : Page(hp->getPageIdx(), HeaderType, hp->getTableIdx())
	{
		copy(hp);
	}

	HeaderPage::HeaderPage() : Page(0, HeaderType, 0)
	{
		_data_stride = 0;
	}

	HeaderPage::HeaderPage(uint16_t page_idx, uint16_t table_idx) : Page(page_idx, HeaderType, table_idx)
	{
		_data_stride = 0;
	}

	uint8_t* HeaderPage::getName()
	{
		return _name;
	}

	uint8_t HeaderPage::getNumColumn()
	{
		uint8_t cols = 0;
		for(int i = 0; i < 16; ++i)
			if(_formats[i] != Format::Invalid) ++cols;
		return cols;
	}

	uint8_t* HeaderPage::getColumnName(uint8_t colIdx)
	{
		return _colNames[colIdx];
	}

	Format HeaderPage::getColumnFormat(uint8_t colIdx)
	{
		return _formats[colIdx];
	}

	uint8_t HeaderPage::getColumnStride()
	{
		_data_stride = 0;
		for(int i = 0; i < 16; ++i) _data_stride+=getFormatWidth(_formats[i]);
		_data_stride += 4; //num mágico inicial y final
		return _data_stride;
	}

	int8_t HeaderPage::getColumnIdx(int8_t span)
	{
		if(span < 4) return -1;
		span -= 4;
		int col_idx;
		for(col_idx = 0; span > 0; ++col_idx)
		{
			int formatWidth = getFormatWidth(_formats[col_idx]);
			span -= formatWidth;
		}
		if(getFormatWidth(_formats[col_idx]) == 0) return -2;
		if(span < 0) return -3; //algún error ha debido haber
		return col_idx;
	}

	uint8_t HeaderPage::getPeriod()
	{
		return _period;
	}

	int HeaderPage::getTypeSize()
	{
		int size = 0;
		int i;
		for(i = 0; i < 16; ++i)
		{
			if(_name[i]!=0) ++size;
			else
			{
				++size;
				i = 16;
			}
		}

		++size; // El period

		for(i = 0; i < 16; ++i)
		{
			if(_formats[i] != Format::Invalid)
			{
				++size;
				int j;
				for(j = 0; j < 16; ++j)
				{
					if(_colNames[i][j] != 0) ++size;
					else
					{
						++size;
						j = 16;
					}
				}
			}
			else
			{
				++size;
				i = 16;
			}
		}
		return size;
	}

	int HeaderPage::serialize(uint8_t* dst)
	{
		int dst_idx = 3;
		dst[0] = getType();
		dst[1] = getTableIdx() & 0xff;
		dst[2] = (getTableIdx() >> 8) & 0xff;

		int i;
		for(i = 0; i < 16; ++i)
		{
			if(_name[i] != 0) dst[dst_idx++] = _name[i];
			else
			{
				dst[dst_idx++] = 0;
				i = 16;
			}
		}

		dst[dst_idx++] = _period;

		for(i = 0; i < 16; ++i)
		{
			if(_formats[i] != Format::Invalid)
			{
				dst[dst_idx++] = (uint8_t) _formats[i];
				for(int j = 0; j < 16; ++j)
				{
                    if(_colNames[i][j] != 0) dst[dst_idx++] = _colNames[i][j];
					else
					{
						dst[dst_idx++] = 0;
						j = 16;
					}
				}
			}
			else
			{
				dst[dst_idx++] = 0;
			}
		}

		return dst_idx;
	}

	int HeaderPage::deserialize(uint8_t* src)
	{
		int src_idx;

		_type = static_cast<PageType>(src[0]);
		_table_idx = (src[2] << 8) & 0xff00 | src[1] & 0xff;

        int i;
        for(i = 0; i < 16 && src[i+3] != 0; ++i)
			_name[i] = src[i+3];

		if(i == 16)
			src_idx = i+3;
		else
			src_idx = i+4; // saltarse el 0 que indica el fin de la cadena

		_period = src[src_idx++];

		for(i = 0; i < 16; ++i)
		{
			if(src[src_idx] != 0 && src[src_idx] != 0xff)
			{
				_formats[i] = static_cast<Format>(src[src_idx++]);
				int j;
				for(j = 0; j < 16; ++j, ++src_idx)
				{
					if(src[src_idx] != 0) _colNames[i][j] = src[src_idx];
					else
					{
						++src_idx;//hay que sacar a src_idx del 0
						break;
					}
				}
			}
			else break; // después de esto ya se corta la vida
		}
		_header_spacing = src_idx + 1;
		return 0;
	}

	void HeaderPage::copy(HeaderPage* hp)
	{
		for(int i = 0; i < 16; ++i) _name[i] = hp->_name[i];
		for(int i = 0; i < 16; ++i)
		{
			_formats[i] = hp->_formats[i];
			for(int j = 0; j < 16; ++j) _colNames[i][j] = hp->_colNames[i][j];
		}

		_period = hp->_period;
		_header_spacing = hp->_header_spacing;

		_table_idx = hp->getTableIdx();
		_page_idx = hp->getPageIdx();
	}
}
