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

SPIFlash::SPIFlash(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin) : SPI_Device(hspi)
{
	_gpio = gpio;
	_pin = pin;
	_action = None;
	_max_delay_time = 0;
	_last_write_ms = HAL_GetTick();
}

int SPIFlash::writeEnable() // solo se llama después de un try_lock
{
	HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
	if(HAL_GetTick()-_last_write_ms < _max_delay_time)
	{
		_action = WaitBusy;
		readStatus(0);
		return -1;
	}

	uint8_t buf = CMD_WRITE_ENABLE;
	_action = WriteEnable;
	startWrite(&buf, 1);
	return 0;
}

int SPIFlash::writeDisable()
{
	return 0;
}

int SPIFlash::writePage(uint8_t* data, uint8_t len, uint32_t addr)
{
	if(_action != None) return EBUSY;
	if(try_lock() != 0) return EBUSY;

	if(writeEnable() == 0) _max_delay_time = 3;
	tx_buffer[0] = CMD_WRITE_DATA;
	tx_buffer[1] = (addr >> 16) & 0xff;
	tx_buffer[2] = (addr >> 8) & 0xff;
	tx_buffer[3] = addr & 0xff;
	for(int i = 4; i < len+4; ++i) tx_buffer[i] = data[i];
	tx_size = len+4;

	return 0;
}

int SPIFlash::readPage(uint8_t len, uint32_t addr)
{
	if(try_lock() != 0) return EBUSY;
	if(_action != None) return EBUSY;

	HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);

	tx_buffer[0] = CMD_READ_DATA;
	tx_buffer[1] = (addr >> 16) & 0xff;
	tx_buffer[2] = (addr >> 8) & 0xff;
	tx_buffer[3] = addr & 0xff;
	for(int i = 4; i < len+4; ++i) rx_buffer[i] = 0;

	rx_size = len+1;
	rx_idx = 0;

	_action = Read;
	startTxRx(tx_buffer, rx_buffer, len+4); // más cuatro para mandar la shitty shit de la cabecera

	return 0;
}

int SPIFlash::eraseSector(uint32_t addr)
{
	if(try_lock() != 0) return EBUSY;
	if(_action != None) return EBUSY;

	if(writeEnable() == 0) _max_delay_time = 400;

	tx_buffer[0] = CMD_SECTOR_ERASE;
	tx_buffer[1] = (addr >> 16) & 0xff;
	tx_buffer[2] = (addr >> 8) & 0xff;
	tx_buffer[3] = addr & 0xff;

	tx_size = 4;

	return 0;
}

int SPIFlash::eraseChip()
{
	if(try_lock() != 0) return EBUSY;
	if(_action != None) return EBUSY;

	if(writeEnable() == 0) _max_delay_time = 100000;

	tx_buffer[0] = CMD_CHIP_ERASE;

	tx_size = 1;

	return 0;
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

void SPIFlash::CpltCallbackModule()
{
	HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET); // Hay que dar pulsos en ss para que se procesen las órdenes
	_last_us = getUS();// aquí irán las cosas inmediatas

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
	else if(_action == Write)
	{
		unlock();
		_action = None;
	}
	else if(_action == Read)
	{
		unlock();
		_action = None;
	}
}

void SPIFlash::pollNextAction()
{
	switch(_action)
	{
	case WENDelay:
		if(getUS()-_last_us > 5)
		{
			_action = WriteEnable;
			writeEnable();
		}
		break;

	case WriteStart: // Para dar la espera para el pulso de NSS, que se requiere después de WriteEnable
		if(getUS()-_last_us > 5)
		{
			HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
			if(tx_size == 1) _max_delay_time = 100000;
			else if(tx_size == 4) _max_delay_time = 400;
			else _max_delay_time = 3;
			_action = Write;
			startWrite(tx_buffer, tx_size);
		}
		break;

	case WaitBusy:
		{
			int dt = HAL_GetTick()-_last_write_ms;
			if(dt < _max_delay_time)
			{
				int left_time = _max_delay_time-dt;
				if(left_time > 10000) // ejecutar cada 20s
				{
					if(dt >= 20000) //20 segundos desde la última comprobación
					{
						_last_write_ms += 20000;
						readStatus(0);
					}
				}
				else if(left_time > 1000)
				{
					if(dt >= 150)
					{
						_last_write_ms += 150;
						readStatus(0);
					}
				}
				else if(left_time > 100)
				{
					if(dt >= 45)
					{
						_last_write_ms += 45;
						readStatus(0);
					}
				}
				else
				{
					if(dt >= 1)
					{
						_last_write_ms += 1;
						readStatus(0);
					}
				}
				_action = WriteEnable;
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
}

int SPIFlash::available()
{
	if(_action != None) return 0;
    return rx_size-rx_idx;
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

void SPIFlash::readStatus(int statusreg)
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
