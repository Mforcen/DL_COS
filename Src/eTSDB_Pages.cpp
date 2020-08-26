#include "eTSDB_Pages.hpp"

static_assert(sizeof(float) == 4, "Float type should be of 4 bytes");

namespace FwLogger
{
	namespace eTSDB
	{
		const uint8_t DataPage::_starter_magic[] = {0xde};
		const uint8_t DataPage::_ender_magic[] = {0xad};

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
			else if(period < 119) return (period-59)*60;
			else if(period < 143) return (period-118)*3600;
			return 0; // error
		}

		uint64_t divRoundClosest(const uint64_t n, const uint64_t d)
		{
			return ((n < 0) ^ (d < 0)) ? ((n - d/2)/d) : ((n + d/2)/d);
		}

		void getFormatString(Format format, char* buf)
		{
			if(format == Format::Invalid) strcpy(buf, "Invalid");
			else if(format == Format::Uint8) strcpy(buf, "Uint8");
			else if(format == Format::Int8) strcpy(buf, "Int8");
			else if(format == Format::Uint16) strcpy(buf, "Uint16");
			else if(format == Format::Int16) strcpy(buf, "Int16");
			else if(format == Format::Uint32) strcpy(buf, "Uint32");
			else if(format == Format::Int32) strcpy(buf, "Int32");
			else if(format == Format::Float) strcpy(buf, "Float");
		}

		Allocator<128>* eTSDB::Page::_alloc = nullptr;

		int Page::getSize()
		{
			int size = getTypeSize();
			if(size == -1)  return -1;
			size += 3;
			return size;
		}

		uint8_t* Page::getName()
		{
			return _name;
		}

		int Page::getTypeSize()
		{
			return 0;
		}

		int Page::serialize(uint8_t* buf)
		{
			buf[0] = getType();
			buf[1] = getObjectIdx() & 0xff;
			buf[2] = (getObjectIdx() >> 8) & 0xff;
			int i = 0;
			if(_type != DataType)
			{
				for(i = 0; i < 16 && _name[i] != 0; ++i) buf[2+i] = _name[i];
				if(i < 16) buf[i+2] = 0;
			}
			return 3+i;
		}

		int Page::deserialize(uint8_t* buf)
		{
			_type = static_cast<PageType>(buf[0]);
			_object_idx = buf[1];
			_object_idx |= ((buf[2] << 8) & 0xff00);
			if(_type != DataType)
			{
				int i;
				for(i = 0; i < 16; ++i) if(buf[i+2] != 0) _name[i] = buf[i+2];
				if(i < 16) _name[i] = 0;
			}
			return 0;
		}

		/**
		  * Data Page implementation
		  */

		DataPage::DataPage() : Page(0, DataType, 0)
		{
			_rowIdx = 0;
			_rowWidth = 0;
		}

		DataPage::DataPage(uint16_t page_idx, uint16_t object_idx, Date block_date, HeaderPage* header) : Page(page_idx, DataType, object_idx)
		{
			_block_date = block_date;
			_header = header;
			_rowIdx = 0;
			_rowWidth = 0;
		}

		int DataPage::getTypeSize()
		{
			return 7;
		}

		int DataPage::serialize(uint8_t* dst)
		{
			dst[0] = getType();
			dst[1] = getObjectIdx() & 0xff;
			dst[2] = (getObjectIdx() >> 8) & 0xff;

			dst[3] = _header->getPageIdx() & 0xff;
			dst[4] = (_header->getPageIdx() >> 8) & 0xff;

			_block_date.serialize(&dst[5]);
			return 10;
		}

		void DataPage::copy(DataPage* dp)
		{
			_object_idx = dp->_object_idx;
			_page_idx = dp->_page_idx;
			_header = dp->_header;
			_rowIdx = dp->_rowIdx;
			_rowWidth = dp->_rowWidth;
			_block_date = dp->_block_date;
			_period = dp->_period;
		}

		Date DataPage::getBlockDate()
		{
			return _block_date;
		}

		int DataPage::getNumEntries()
		{
			if(_rowWidth == 0) return 0;
			return (PageWidth-_header_span)/_rowWidth;
		}

		/**
		  * Header Page implementation
		  */

		HeaderPage::HeaderPage(HeaderPage* hp) : Page(hp->getPageIdx(), HeaderType, hp->getObjectIdx(), hp->getAccessMode())
		{
			copy(hp);
		}

		HeaderPage::HeaderPage() : Page(0, HeaderType, 0)
		{
			_data_stride = 0;
			_data_idx = 0xffff;
			_currDP = nullptr;
			_period = 0;

		}

		HeaderPage::HeaderPage(uint16_t page_idx, uint16_t object_idx) : Page(page_idx, HeaderType, object_idx)
		{
			_data_stride = 0;
			_data_idx = 0xffff;
			_currDP = nullptr;
			_period = 0;
		}

		HeaderPage::~HeaderPage()
		{
			if(_currDP != nullptr)
				delete _currDP;
		}

		uint8_t HeaderPage::getNumColumn()
		{
			_cols.size();
		}

		uint8_t* HeaderPage::getColumnName(uint8_t colIdx)
		{
			return _cols[colIdx].name;
		}

		Format HeaderPage::getColumnFormat(uint8_t colIdx)
		{
			return _cols[colIdx].format;
		}

		uint16_t HeaderPage::getColumnStride()
		{
			_data_stride = 0;
			for(int i = 0; i < _cols.size(); ++i)
				_data_stride += getFormatWidth(_cols[i].format);
			_data_stride += 2; //num mágico inicial y final
			return _data_stride;
		}

		int8_t HeaderPage::getColumnIdx(int8_t span)
		{
			if(span < 4) return -1;
			span -= 4;
			int col_idx;
			for(col_idx = 0; span > 0; ++col_idx)
			{
				int formatWidth = getFormatWidth(_cols[col_idx].format);
				span -= formatWidth;
			}
			if(getFormatWidth(_cols[col_idx].format) == 0) return -2;
			if(span < 0) return -3; //algún error ha debido haber
			return col_idx;
		}

		uint8_t HeaderPage::getPeriod()
		{
			return _period;
		}

		bool HeaderPage::checkFormat(Row& row)
		{
			if(row.vals.size() != _cols.size()) return false;
			for(int i = 0; i < _cols.size(); ++i)
			{
				if(_cols[i].format != row.vals[i].format)
					return false;
			}
			return true;
		}

		void HeaderPage::getFormat(Row& row)
		{
			row.vals.resize(_cols.size());
			for(int i = 0; i < _cols.size(); ++i)
			{
				row.vals[i].format = _cols[i].format;
			}
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

			size += 2; // El period y el num column

			for(i = 0; i < _cols.size(); ++i)
			{
				++size;
				int j;
				for(j = 0; j < 16; ++j)
				{
					if(_cols[i].name[j] != 0) ++size;
					else
					{
						++size;
						j = 16;
					}
				}
			}
			return size;
		}

		int HeaderPage::serialize(uint8_t* dst)
		{
			int dst_idx = 3;
			dst[0] = getType();
			dst[1] = getObjectIdx() & 0xff;
			dst[2] = (getObjectIdx() >> 8) & 0xff;

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
			dst[dst_idx++] = _cols.size();

			for(i = 0; i < _cols.size(); ++i)
			{
				dst[dst_idx++] = (uint8_t) _cols[i].format;
				for(int j = 0; j < 16; ++j)
				{
					if(_cols[i].name[j] != 0) dst[dst_idx++] = _cols[i].name[j];
					else
					{
						dst[dst_idx++] = 0;
						j = 16;
					}
				}
			}

			return dst_idx;
		}

		int HeaderPage::deserialize(uint8_t* src)
		{
			int src_idx;

			_type = static_cast<PageType>(src[0]);
			_object_idx = (src[2] << 8) & 0xff00 | src[1] & 0xff;

			int i;
			for(i = 0; i < 16 && src[i+3] != 0; ++i)
				_name[i] = src[i+3];

			if(i == 16)
				src_idx = i+3;
			else
				src_idx = i+4; // saltarse el 0 que indica el fin de la cadena

			_period = src[src_idx++];
			_cols.resize(src[src_idx++]);

			while(src[src_idx] != 0 && src[src_idx] != 0xff)
			{
				_cols[i].format = static_cast<Format>(src[src_idx++]);
				int j;
				for(j = 0; j < 16; ++j, ++src_idx)
				{
					if(src[src_idx] != 0) _cols[i].name[j] = src[src_idx];
					else
					{
						++src_idx;//hay que sacar a src_idx del 0
						break;
					}
				}
			}
			_header_spacing = src_idx + 1;
			return 0;
		}

		void HeaderPage::copy(HeaderPage* hp)
		{
			for(int i = 0; i < 16; ++i) _name[i] = hp->_name[i];
			_cols = hp->_cols;
			_period = hp->_period;
			_header_spacing = hp->_header_spacing;

			_object_idx = hp->getObjectIdx();
			_page_idx = hp->getPageIdx();
			_page_mode = hp->_page_mode;

			if(_currDP != nullptr)
			{
				_alloc->Deallocate(_currDP);
				_currDP = nullptr;
			}

			if(hp->_currDP != nullptr)
			{
				_currDP = new DataPage();
				_currDP->copy(hp->_currDP);
			}
		}

		void HeaderPage::move(HeaderPage* hp)
		{
			for(int i = 0; i < 16; ++i) _name[i] = hp->_name[i];
			_cols = std::move(hp->_cols);
			_period = hp->_period;
			_header_spacing = hp->_header_spacing;

			_object_idx = hp->getObjectIdx();
			_page_idx = hp->getPageIdx();
			_page_mode = hp->_page_mode;

			if(_currDP != nullptr)
			{
				_alloc->Deallocate(_currDP);
				_currDP = nullptr;
			}

			if(hp->_currDP != nullptr)
			{
				_currDP = new DataPage();
				_currDP->copy(hp->_currDP);
			}
		}

		FilePage::FilePage() : Page(0, FileType, 0)
		{
			_page_state = 0xff; // msb (1-0) -> primer página o no, (111-000) escrito o vacío, (1111-0000) escrito o eliminado
			for(int i = 0; i < 16; ++i) _name[i] = 0xff;
			_file_size = 0xffff;
			_next_page = 0xffff;
			_data_status = 0;
			_data_idx = 0;
			_read_status = 0;
			_read_bytes = 0;
			_databuf = nullptr;
		}

		FilePage::FilePage(uint16_t page_idx, uint16_t object_idx) : Page(page_idx, FileType, object_idx)
		{
			_page_state = 0xff; // msb (1-0) -> primer página o no, (111-000) escrito o vacío, (1111-0000) escrito o eliminado
			for(int i = 0; i < 16; ++i) _name[i] = 0xff;
			_file_size = 0xffff;
			_next_page = 0xffff;
			_data_status = 0;
			_data_idx = 0;
			_read_status = 0;
			_read_bytes = 0;
			_databuf = nullptr;
		}

		FilePage::~FilePage()
		{
			if(_databuf != nullptr) _alloc->Deallocate(_databuf);
			_databuf = nullptr;
		}

		int FilePage::getTypeSize()
		{
			uint8_t name_length = 0;
			for(int i = 0; i < 16; ++i)
			{
				++name_length;
				if(_name[i] == 0)
					break;
			}
			return 5 + name_length;
		}

		int FilePage::serialize(uint8_t* dst)
		{
			int dst_idx = 3;
			dst[0] = getType();
			dst[1] = getObjectIdx() & 0xff;
			dst[2] = (getObjectIdx() >> 8) & 0xff;

			int i;
			for(i = 0; i < 16; ++i)
			{
				if(_name[i] != 0)
					dst[dst_idx++] = _name[i];
				else
				{
					dst[dst_idx++] = 0;
					break;
				}
			}

			dst[dst_idx++] = _page_state;

			dst[dst_idx++] = _file_size & 0xff;
			dst[dst_idx++] = (_file_size >> 8) & 0xff;
			dst[dst_idx++] = _next_page & 0xff;
			dst[dst_idx++] = (_next_page >> 8) & 0xff;

			return dst_idx;
		}

		bool FilePage::getDataReady()
		{
			return _read_status >= 2;
		}

		void FilePage::copy(FilePage* fp)
		{
			_page_idx = fp->_page_idx;
			_object_idx = fp->_object_idx;

			_file_size = fp->_file_size;
			_page_state = fp->_page_state;
			_next_page = fp->_next_page;

			for(int i = 0; i<16; ++i)
				_name[i] = fp->_name[i];

			_page_mode = fp->_page_mode;
			_read_status = fp->_read_status;
			_data_status = fp->_data_status;
			_read_bytes = fp->_read_bytes;
			if(fp->_databuf != nullptr)
			{
				_databuf = reinterpret_cast<uint8_t*>(_alloc->Allocate(128, 2));
				for(int i = 0; i<128; ++i) _databuf[i] = fp->_databuf[i];
			}
			else
				_databuf = nullptr;
		}

		uint8_t* FilePage::getDataPage()
		{
			if(!_databuf) return nullptr;
			if(_read_status < 2)
			{
				return nullptr;
			}
			_read_status = 3;
			return _databuf;
		}

		bool FilePage::freeDataPage()
		{
			if(_databuf != nullptr)
			{
				_alloc->Deallocate(_databuf);
				_databuf = nullptr;

				return true;
			}
			return false;
		}

		uint16_t FilePage::getFileSize()
		{
			return _file_size;
		}
		uint16_t FilePage::getBytesRead()
		{
			return _read_bytes;
		}
	}
}
