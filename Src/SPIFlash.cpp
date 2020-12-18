#include "SPIFlash.h"

#define CMD_WRITE_ENABLE 		0x06
#define CMD_WRITE_DISABLE		0x04
#define CMD_READ_STATUS_REG1	0x05
#define CMD_READ_STATUS_REG2	0x15
#define CMD_READ_STATUS_REG3	0x25
#define CMD_WRITE_STATUS_REG	0x01
#define CMD_READ_DATA			0x03
#define CMD_WRITE_DATA			0x02
#define CMD_SECTOR_ERASE		0x20
#define CMD_CHIP_ERASE			0x60

#include "printf.h"

namespace FwLogger
{
	SPIFlash::SPIFlash(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin, const char* moduleName, const char bin_id, uint8_t major, uint8_t minor) :
					   SPI_Device(hspi), Module(moduleName, bin_id, major, minor)
	{
		_gpio = gpio;
		_pin = pin;
		_action = None;
		_max_delay_time = 0;
		_last_write_ms = HAL_GetTick();
		_action = None;
	}

	int SPIFlash::_writeEnable() // solo se llama después de un try_lock
	{
		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
		if(HAL_GetTick()-_last_write_ms < _max_delay_time)
		{
			_action = WaitBusy;
			_readStatus(0);
			return -2;
		}

		uint8_t buf = CMD_WRITE_ENABLE;
		_action = WriteEnable;
		return startWrite(&buf, 1);
	}

	int SPIFlash::_writeDisable()
	{
		return 0;
	}

	int SPIFlash::writePage(uint8_t* data, uint16_t len, uint32_t addr)
	{
		if(len == 0 || len > 256) return EIO;
		if(_action != None) return EBUSY;
		if(try_lock() != 0) return EBUSY;

		uint32_t addr_high = (addr + len) & 0xffff00;
		uint16_t len_low;

		if((addr&0xffff00) == addr_high) // empieza y acaba en el mismo bloque
		{
			len_low = len;
		}
		else
		{
            len_low = addr_high-addr;
		}

		uint16_t len_high = len - len_low;


		WritingOp low_op;
		low_op.addr = 0;
		low_op.len = len_low+4;

		write_ops.push_back(low_op);

		tx_buffer[0] = CMD_WRITE_DATA;
		tx_buffer[1] = (addr >> 16) & 0xff;
		tx_buffer[2] = (addr >> 8) & 0xff;
		tx_buffer[3] = addr & 0xff;
		int i;
		for(i = 0; i < len_low; ++i) tx_buffer[i+4] = data[i];

		if(len_high > 0)
		{
			WritingOp high_op;
			high_op.addr = i + 4;
			high_op.len = len_high + 4;
			write_ops.push_back(high_op);

			tx_buffer[i+4] = CMD_WRITE_DATA;
			tx_buffer[i+5] = (addr_high >> 16) & 0xff;
			tx_buffer[i+6] = (addr_high >> 8) & 0xff;
			tx_buffer[i+7] = 0;
			for(;i<len;++i) tx_buffer[i+8] = data[i];
		}


		int error = _writeEnable();

		if(error == 0) _max_delay_time = 3;

		return error;
	}

	int SPIFlash::readPage(uint16_t len, uint32_t addr, uint8_t* buf)
	{
		if(try_lock() != 0) return EBUSY;
		if(_action != None) return EBUSY;

		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);

		tx_buffer[0] = CMD_READ_DATA;
		tx_buffer[1] = (addr >> 16) & 0xff;
		tx_buffer[2] = (addr >> 8) & 0xff;
		tx_buffer[3] = addr & 0xff;

		rx_size = len;

		ext_rx_buf = buf;

		_action = SendReadAddr;
		return startWrite(tx_buffer, 4); // más cuatro para mandar la shitty shit de la cabecera
	}

	int SPIFlash::readPage(uint16_t len, uint32_t addr)
	{
		if(try_lock() != 0) return EBUSY;
		if(_action != None) return EBUSY;

		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);

		tx_buffer[0] = CMD_READ_DATA;
		tx_buffer[1] = (addr >> 16) & 0xff;
		tx_buffer[2] = (addr >> 8) & 0xff;
		tx_buffer[3] = addr & 0xff;
		for(int i = 4; i < len+4; ++i) rx_buffer[i] = 0;

		rx_size = len+4;
		rx_idx = 4;

		_action = Read;
		return startTxRx(tx_buffer, rx_buffer, len+4); // más cuatro para mandar la shitty shit de la cabecera
	}

	int SPIFlash::eraseSector(uint32_t addr)
	{
		if(try_lock() != 0) return EBUSY;
		if(_action != None) return EBUSY;

		tx_buffer[0] = CMD_SECTOR_ERASE;
		tx_buffer[1] = (addr >> 16) & 0xff;
		tx_buffer[2] = (addr >> 8) & 0xff;
		tx_buffer[3] = addr & 0xff;

		WritingOp op;
		op.addr = 0;
		op.len = 4;

		write_ops.push_back(op);

		int error = _writeEnable();

		if(error == 0) _max_delay_time = 400;

		return error;
	}

	int SPIFlash::eraseChip()
	{
		if(try_lock() != 0) return EBUSY;
		if(_action != None) return EBUSY;

		tx_buffer[0] = CMD_CHIP_ERASE;

		WritingOp op;
		op.addr = 0;
		op.len = 1;

		write_ops.push_back(op);

		int error = _writeEnable();
		if(error == 0) _max_delay_time = 100000;

		return error;
	}

	int SPIFlash::jedecId(uint8_t* data)
	{
		if(try_lock() != 0) return EBUSY;
		if(_action != None) return EBUSY;

		tx_buffer[0] = 0x9f;
		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(_hspi, tx_buffer, 1, 1000);
		HAL_SPI_Receive(_hspi, rx_buffer, 3, 1000);
		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET);
		return 0;
	}

	int SPIFlash::powerDown()
	{
		if(try_lock() != 0) return EBUSY;
		if(_action != None) return EBUSY;

		return 0;
	}

	int SPIFlash::wakeUp()
	{
		if(try_lock() != 0) return EBUSY;
		if(_action != None) return EBUSY;

		return 0;
	}

	void SPIFlash::ISR()
	{
		if(_action != SendReadAddr)
			HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET); // Hay que dar pulsos en ss para que se procesen las órdenes

		_last_us = getUS();// aquí irán las cosas inmediatas

		_isr_launched = true;

		/*if(_action == WaitBusy) // ha leído el status, por lo que seguirá en polling
		{
			if(_rx_status[1] & 0x01) // está busy
			{
				_last_write_ms = HAL_GetTick();
			}
			else
			{
				_max_delay_time = 0;
				_action = WENDelay; // se llamará en polling
			}
		}
		else if(_action == WriteEnable)
		{
			_action = WriteStart; // pero tendrá que esperar
		}
		else if(_action == Write)
		{
			//unlock();
			_action = None;
		}
		else if(_action == Read)
		{
			//unlock();
			_action = None;
		}
		if(_action == None)
			flashModuleISR(); // cuando se termina la transacción, en el caso de ser un módulo, se ejecuta dicha función
			*/
	}

	void SPIFlash::flashModuleISR()
	{
		unlock();
	}

	bool SPIFlash::loop()
	{
		if(_isr_launched)
		{
			_isr_launched = false;

			if(_action == WaitBusy) // ha leído el status, por lo que seguirá en polling
			{
				if(_rx_status[1] & 0x01) // está busy
				{
					_last_write_ms = HAL_GetTick();
				}
				else
				{
					_max_delay_time = 0;
					_action = WENDelay; // se llamará en polling
				}
			}
			else if(_action == WriteEnable)
			{
				_action = WriteStart; // pero tendrá que esperar
			}
			else if(_action == SendReadAddr)
			{
                startRead(ext_rx_buf, rx_size);
                _action = Read;
			}
			else if(_action == Write)
			{
				//unlock();
				_last_write_ms = HAL_GetTick();
				if(write_ops.size())
				{
					_writeEnable();
				}
				else
					_action = None;
			}
			else if(_action == Read)
			{
				//unlock();
				_action = None;
			}
			if(_action == None)
			{
				flashModuleISR(); // cuando se termina la transacción, en el caso de ser un módulo, se ejecuta dicha función
			}

			return true; //something to do
		}


		switch(_action)
		{
		case WENDelay:
			if(static_cast<uint16_t>(getUS()-_last_us) > 5)
			{
				_writeEnable();
			}
			break;

		case WriteStart: // Para dar la espera para el pulso de NSS, que se requiere después de WriteEnable
			if(static_cast<uint16_t>(getUS()-_last_us) > 5)
			{
				HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);

				WritingOp op;
				write_ops.pop_front(&op);

				if(op.len == 1) _max_delay_time = 100000;
				else if(op.len == 4) _max_delay_time = 400;
				else _max_delay_time = 3;
				_action = Write;
				int error = startWrite(&tx_buffer[op.addr], op.len);
				if(error < 0)
					printf_("error %d\n",error);
			}
			break;

		case WaitBusy:
			{
				uint32_t dt = HAL_GetTick()-_last_write_ms;
				if(dt < _max_delay_time)
				{
					int left_time = _max_delay_time-dt;
					if(left_time > 10000) // ejecutar cada 20s
					{
						if(dt >= 20000) //20 segundos desde la última comprobación
						{
							_last_write_ms += 20000;
							_readStatus(0);
						}
					}
					else if(left_time > 1000)
					{
						if(dt >= 150)
						{
							_last_write_ms += 150;
							_readStatus(0);
						}
					}
					else if(left_time > 100)
					{
						if(dt >= 45)
						{
							_last_write_ms += 45;
							_readStatus(0);
						}
					}
					else
					{
						if(dt >= 1)
						{
							_last_write_ms += 1;
							_readStatus(0);
						}
					}
					_writeEnable();
				}
				else
				{
					_action = WENDelay;
					_last_us = getUS();
				}
			}
			break;

		default:
			break;
		}
		if(_action == None) return false; // nothing to do
		return true; // something to do
	}

	int SPIFlash::available()
	{
		if(_action != None) return 0;
		return (rx_size-rx_idx);
	}

	int SPIFlash::peek()
	{
		if(!available()) return -1;
		return rx_buffer[rx_idx];
	}

	int SPIFlash::pop()
	{
		if(!available()) return -1;
		uint8_t c = rx_buffer[rx_idx++];
		return c;
	}

	int SPIFlash::bin_eval(uint8_t* buf, uint8_t* outbuf)
	{
		if(buf[0] == 'r')
		{
			int addr = (buf[1] & 0xff)  | ((buf[2] << 8) & 0xff00) | ((buf[3] << 16) & 0xff0000) | ((buf[4] << 24) & 0xff000000);
			int len = buf[5];
			readPage(len, addr);

			HAL_Delay(1);
			loop();
			outbuf[0] = 'r';

			for(int i = 0; i < len; ++i)
				outbuf[1+i] = rx_buffer[rx_idx+i];
			return len+1;
			unlock();
		}
		return 0;
	}

	void SPIFlash::_readStatus(int statusreg)
	{
		if(statusreg == 1)
			_tx_status[0] = CMD_READ_STATUS_REG2;
		else if(statusreg == 2)
			_tx_status[0] = CMD_READ_STATUS_REG3;
		else
			_tx_status[0] = CMD_READ_STATUS_REG1;

		_rx_status[1] = 0;
		startTxRx(_tx_status, _rx_status, 2);
	}
}
