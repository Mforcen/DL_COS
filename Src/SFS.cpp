#include "SFS.h"

namespace FwLogger
{

	SFS::SFS(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin, const char* moduleName,
					 char bin_id, uint8_t major, uint8_t minor) : SPIFlash(hspi, gpio, pin, "SFS", 'S', 0, 1)
	{
		_ops.push_back(Initing);
	}

	int SFS::init()
	{
		Op* op = _ops.at_front();
		if(op == nullptr) return -1;
		else if(*op != Initing) return -2;
		_tailBlock = 0;
		_tailPage = 0;
		readPage(0);

		return 0;
	}

	int SFS::append(const uint8_t* buf, int len)
	{
		if(_ops.size() > 0) return -1;
		if(len > 2000) return -1;

		int idx = 0;

		if(_tailPage == 0)
		{
			tx_buffer[idx++] = 0xff; // keep as good block
			tx_buffer[idx++] = 0x00; // used block
		}

		tx_buffer[idx++] = len & 0xff;
		tx_buffer[idx++] = (len >> 8) & 0xff;
		tx_buffer[idx++] = (len >> 16) & 0xff;
		tx_buffer[idx++] = (len >> 24) & 0xff;

		memcpy(tx_buffer+idx, buf, len);

		_ops.push_back(Append);

		_buf_len = idx+len;

		if(_tailPage >= 63) // prepare for the next one
		{
			_ops.push_back(FindBlock);
		}

		_exec();
		return _tailBlock*64+_tailPage;
	}

	int SFS::startRead(int page)
	{
		if(_ops.size() > 0) return -1;

		_read_closed = false;
		_read_sent = false;
		_read_idx = 0;
		_read_len = 0;

		_markBlock = page;
		_ops.push_back(Read);
		_exec();
		return 0;
	}

	int SFS::mark_completed(int block)
	{
		if(_ops.size() > 0) return -1;
		_markBlock = block;
		_ops.push_back(Closing);

		readPage(_markBlock*64);
		return 0;
	}

	int SFS::mark_sended(int page)
	{
		if(_ops.size() > 0) return -1;
		_markBlock = page;
		_ops.push_back(MarkSended);
		readPage(_markBlock);

		return 0;
	}

	bool SFS::loop()
	{
		return _ops.size() > 0 || SPIFlash::loop();
	}

	void SFS::flashModuleISR()
	{
		Op* op;
		op = _ops.at_front();
		if(op == nullptr) return; // will raise an error
		switch(*op)
		{
		case FindBlock:
			_findBlockIt();
			break;

		case Append:
			_incPos();
		case Writing:
			_step();
			break;

		case Initing:
			if(rx_buffer[0] == 0xff) // good block
			{
				if(rx_buffer[1] != 0xff) // used block
				{
					_tailBlock+=1;
					readPage(_tailBlock*64);
				}
				else
				{
					_step();
				}
			}
			else
			{
				_tailBlock += 1;
				readPage(_tailBlock*64);
			}
			break;

		case MarkSended:
			{
				int buf_idx = 0;
				if((_markBlock&0x3f) == 0) // skip two byte header
				{
					buf_idx = 2;
				}
				int msgsize = rx_buffer[buf_idx++];
				msgsize |= (rx_buffer[buf_idx++] << 8);
				msgsize |= (rx_buffer[buf_idx++] << 16);
				msgsize |= (rx_buffer[buf_idx++] << 24);

				buf_idx += msgsize;
				memcpy(rx_buffer, tx_buffer, buf_idx);
				tx_buffer[buf_idx++] = 0;

				*op = Writing;
				writePage(tx_buffer, buf_idx, _markBlock);
			}
			break;

		case Closing:
			{
				int buf_idx = 0;
				if((_markBlock&0x3f) == 0) // skip two byte header
				{
					buf_idx = 2;
				}
				int msgsize = rx_buffer[buf_idx++];
				msgsize |= (rx_buffer[buf_idx++] << 8);
				msgsize |= (rx_buffer[buf_idx++] << 16);
				msgsize |= (rx_buffer[buf_idx++] << 24);

				buf_idx += msgsize;
				memcpy(rx_buffer, tx_buffer, buf_idx);
				tx_buffer[buf_idx++] = 0; // sended
				tx_buffer[buf_idx++] = 0; // closed

				*op = Writing;
				writePage(tx_buffer, buf_idx, _markBlock*644);
			}
			break;

		case Read:
			{
				int buf_idx = 0;
				if((_markBlock & 0x3f) == 0) buf_idx = 2;
				unsigned int buf_len = rx_buffer[buf_idx++];
				buf_len |= (rx_buffer[buf_idx++] << 8);
				buf_len |= (rx_buffer[buf_idx++] << 16);
				buf_len |= (rx_buffer[buf_idx++] << 24);
				if(buf_len == 0xffffffff)
				{
					_read_len = -1; // this is error
					_step();
					return;
				}
				_read_len = buf_len + buf_idx;
				_read_idx = buf_idx;
				_read_sent = rx_buffer[buf_idx++] == 0 ? true : false;
				if((_markBlock & 0x3ff) == 0) _read_closed = rx_buffer[buf_idx++] == 0 ? true : false;
				_step();
			}

			break;

		default:
			break;
		}
	}

	int SFS::available()
	{
		return _read_len - _read_idx;
	}

	int SFS::pop()
	{
		if(!available()) return -1;
		return rx_buffer[_read_idx++];
	}

	void SFS::_findBlockIt()
	{
		if(rx_buffer[0] == 0xff) // error block
		{
			if(rx_buffer[1] == 0xff) // free block
			{
				_step(); // found
			}
			else // block used
			{
				_tailBlock++;
				readPage(_tailBlock*64);
			}
		}
	}

	void SFS::_exec()
	{
		Op* op = _ops.at_front();
		if(op == nullptr) return;
		switch(*op)
		{
		case Append:
		case Writing:
			writePage(tx_buffer, _buf_len, _tailBlock*64+_tailPage);
			break;

		case FindBlock:
			readPage(_tailBlock * 64);
			break;

		case Read:
			readPage(_markBlock);
			break;

		default:
			break;
		}
	}

	void SFS::_step()
	{
		_ops.delete_front();
		_exec();
	}

	void SFS::_incPos() // increments pointer positions
	{
		if(++_tailPage>=64)
		{
			_tailPage = 0;
			_tailBlock++;
		}
	}
}
