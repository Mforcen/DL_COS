#include "SPIFlash.h"

#define CMD_WRITE_ENABLE 		0x06
#define CMD_WRITE_DISABLE		0x04
#define CMD_READ_STATUS_REG		0x0F
#define CMD_WRITE_STATUS_REG	0x01
#define CMD_PAGE_READ			0x13
#define CMD_READ_DATA			0x03
#define CMD_WRITE_DATA			0x02
#define CMD_PROGRAM_EXEC		0x10
#define CMD_SECTOR_ERASE		0xD8

#include "printf.h"


extern uint16_t getUS();


namespace FwLogger
{
	SPIFlash::SPIFlash(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin, const char* moduleName, const char bin_id, uint8_t major, uint8_t minor) :
					   SPI_Device(hspi), Module(moduleName, bin_id, major, minor)
	{
		_gpio = gpio;
		_pin = pin;
		_actions.clear();
		_delay_time = 0;

		tx_buffer = &_tx_buffer[3];
		rx_buffer = &_rx_buffer[4];
	}

	void SPIFlash::_delay(int ms)
	{
		_curr_time = HAL_GetTick();
		_delay_time = ms;
		_actions.push_front(Delay);
	}

	int SPIFlash::_welCheck()
	{
		if(rx_buffer[2] & 0x02) return 0;
		return -1;
	}

	int SPIFlash::_busyCheck() // call after status is checked
	{
		if(rx_buffer[2] & 0x01) return 0;
		return -1;
	}

	void SPIFlash::_readStatus(uint8_t statreg)
	{
		_tx_buffer[0] = CMD_READ_STATUS_REG;
		_tx_buffer[1] = statreg;
		_tx_buffer[2] = 0;
		_rx_buffer[0] = 0;
		_rx_buffer[1] = 0;
		_rx_buffer[2] = 0;
		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
		startTxRx(_tx_buffer, _rx_buffer, 3);
	}

	void SPIFlash::_writeEnable()
	{
		_tx_buffer[0] = CMD_WRITE_ENABLE;
		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
	}

	void SPIFlash::_programDataLoad()
	{
		_tx_buffer[0] = CMD_WRITE_DATA;
		_tx_buffer[1] = 0;
		_tx_buffer[2] = 0;

		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
		startWrite(_tx_buffer, 2052);
	}

	void SPIFlash::_programExecute(uint16_t page)
	{
		_tx_buffer[0] = CMD_PROGRAM_EXEC;
		_tx_buffer[1] = 0;
		_tx_buffer[2] = (page >> 8) & 0xff;
		_tx_buffer[3] = page & 0xff;

		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
		startWrite(_tx_buffer, 4);
	}

	void SPIFlash::_pageRead(uint16_t page)
	{
		_tx_buffer[0] = CMD_PAGE_READ;
		_tx_buffer[1] = 0;
		_tx_buffer[2] = (page >> 8) & 0xff;
		_tx_buffer[3] = page & 0xff;

		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
		startWrite(_tx_buffer, 4);
	}

	void SPIFlash::_read()
	{
		_tx_buffer[0] = CMD_READ_DATA;
		_tx_buffer[1] = 0;
		_tx_buffer[2] = 0;
		_tx_buffer[3] = 0;

		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
		startTxRx(_tx_buffer, _rx_buffer, 2052);
	}

	void SPIFlash::_exec(Action a)
	{
		switch(a)
		{
			case BusyCheck:
			case WELCheck:
				_readStatus(3);
				break;
			case WriteEnable:
				_writeEnable();
				break;
			case ProgramDataLoad:
				_programDataLoad();
				break;
			case ProgramExecute:
				_programExecute(_page_idx);
				break;
			case PageDataRead:
				_pageRead(_page_idx);
				break;
			case Read:
				_read();
				break;
			case Erase:
				break;
			default:
				break;
		}
	}

	void SPIFlash::_isr(Action a)
	{

		switch(a)
		{
			case BusyCheck:
				if(_busyCheck() == 0)
					_step();
				else
					_exec(a);
				break;
			case WELCheck:
				if(_welCheck() == 0)
					_step();
				else
					_actions.push_front(WriteEnable);
					_exec(WriteEnable);
				break;
			case ProgramDataLoad:
				_step();
				break;
			case ProgramExecute:
				_step();
				break;
			case PageDataRead:
				_step();
				break;
			case Read:
				rx_size = 2052;
				rx_idx = 4;
				_actions.clear();
				break;
			case Erase:
				_actions.clear();
				break;
			default:
				break;
		}
	}

	void SPIFlash::_step()
	{
		_actions.delete_front();
		Action a;
		if(_actions.pop_front(&a)==0)
			_exec(a);
	}

	int SPIFlash::writePage(uint8_t* data, uint16_t len, uint16_t page)
	{
		if(len == 0 || len > 2048) return EIO;
		if(try_lock() != 0) return EBUSY;
		if(_actions.size() > 0) return EBUSY;

		_page_idx = page;

		for(int i = 0; i < len; ++i) tx_buffer[i] = data[i];

		_actions.push_back(BusyCheck);
		_actions.push_back(WriteEnable);
		_actions.push_back(WELCheck);
		_actions.push_back(ProgramDataLoad);
		_actions.push_back(ProgramExecute);

		_exec(BusyCheck);

		return 0;
	}

	int SPIFlash::readPage(uint16_t page)
	{
		if(try_lock() != 0) return EBUSY;
		if(_actions.size() > 0) return EBUSY;

		_page_idx = page;

		_actions.push_back(BusyCheck);
		_actions.push_back(PageDataRead);
		_actions.push_back(Read);

		_exec(BusyCheck);

		return 0; // más cuatro para mandar la shitty shit de la cabecera
	}

	int SPIFlash::eraseSector(uint16_t page)
	{
		if(try_lock() != 0) return EBUSY;
		if(_actions.size() > 0) return EBUSY;

		_page_idx = page;

		_actions.push_back(BusyCheck);
		_actions.push_back(WriteEnable);
		_actions.push_back(WELCheck);
		_actions.push_back(Erase);

		_exec(BusyCheck);

		return 0;
	}

	int SPIFlash::jedecId(uint8_t* data)
	{
		if(try_lock() != 0) return EBUSY;
		if(_actions.size() > 0) return EBUSY;

		_tx_buffer[0] = 0x9f;
		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(_hspi, _tx_buffer, 1, 1000);
		HAL_SPI_Receive(_hspi, _rx_buffer, 4, 1000);
		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET);
		return 0;
	}

	void SPIFlash::ISR()
	{
		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET);

		_last_us = getUS();

		_isr_launched = true;
	}

	void SPIFlash::flashModuleISR()
	{
		unlock();
	}

	bool SPIFlash::loop()
	{
		Action a;
		if(_actions.peek_front(&a) == 0)
		{
			switch(a)
			{
			case Delay:
				if(HAL_GetTick()-_curr_time > _delay_time)
					_step();
				break;
			default:
				if(_isr_launched && (getUS() - _last_us > 100))
				{
					_isr_launched = false;
					_isr(a);
				}
				break;
			}
			return true;
		}

		return false;
	}

	int SPIFlash::available()
	{
		if(_actions.size() > 0) return 0;
		return (rx_size-rx_idx);
	}

	int SPIFlash::peek()
	{
		if(!available()) return -1;
		return _rx_buffer[rx_idx];
	}

	int SPIFlash::pop()
	{
		if(!available()) return -1;
		uint8_t c = _rx_buffer[rx_idx++];
		return c;
	}

	int SPIFlash::bin_eval(uint8_t* buf, uint8_t* outbuf)
	{
		return 0;
	}
}
