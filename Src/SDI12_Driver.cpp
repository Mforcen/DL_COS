#include "SDI12_Driver.h"

namespace FwLogger
{
	SDI12_Driver::SDI12_Driver(GPIO_TypeDef* gpio, uint32_t pin) : Module("SDI12Soft 1.0b")
	{
		_gpio = gpio;
		_pin = pin;
		setStatus(Disabled);
	}

	int SDI12_Driver::write(uint8_t* cmd)
	{
		if(_status != Disabled) return -1;

		int i;
		for(i = 0; cmd[i]; ++i) _tx_buffer.push_back(cmd[i]);
		setStatus(Breaking);
		return i;
	}

	void SDI12_Driver::timer_isr()
	{
		switch(_status)
		{
		case Breaking:
			if(_counter++ >= 15)
			{
				setStatus(MarkingTx);
			}
			break;

		case MarkingTx:
			if(_counter++ >= 10)
			{
				setStatus(Transmitting);
			}
			break;

		case MarkingRx:
			if(_counter++ >= 10)
			{
				setStatus(Receiving);
				_counter = 0;
			}
			break;

		case Receiving:
			if(_state == Command)
			{
				_last_rx_counter++;
				if(_last_rx_counter == 12000) setStatus(Disabled);
				_state = Nop;
			}
			else if(_state == WaitingSR)
			{
				_last_rx_counter--;
				if(_last_rx_counter <= 0)
				{
					_state = GettingData;
					_tx_buffer.clear();

					_tx_buffer.push_back(_measAddr);
					_tx_buffer.push_back('M');
					if(_additionalMeas != 0) _tx_buffer.push_back(_additionalMeas); // añadir CRC
					_tx_buffer.push_back('!');

					_counter = 0;
					setStatus(Transmitting);
				}
			}
			break;

		case Transmitting: //revisar porque esto no está bien, hay que cumplir los 10 bits
			{
				int bit_idx, byte_idx;
				bit_idx = (_counter) % 10; //obtener qué hay que poner (start, 7 bits, paridad, stop)
				byte_idx = _counter / 10;

				if(bit_idx == 0)
					HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET);
				else if(bit_idx == 8)
					HAL_GPIO_WritePin(_gpio, _pin, parity_even(_tx_buffer.buf[byte_idx]) ? GPIO_PIN_RESET : GPIO_PIN_SET);
				else if(bit_idx == 9)
					HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(_gpio, _pin, (_tx_buffer.buf[byte_idx] >> (bit_idx-1)) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);

				_counter++;
				byte_idx = _counter / 10;
				if(byte_idx == _tx_buffer.idx)
				{
					_tx_buffer.clear();
					_counter = 0;
					setStatus(MarkingRx);
				}
			}
			break;

		default:
			break;
		}
	 }

	void SDI12_Driver::pin_isr() //Esto se queda en Listening hasta timeout
	{
		if(_status == Receiving){
			_last_rx_counter = 0;
			uint16_t dt_rx = static_cast<uint16_t>(getUS()-_last_rx);
			_last_rx = getUS();
			uint32_t bits = (1200*(dt_rx+_rxFudge))/1000000;
			GPIO_PinState pinValue = HAL_GPIO_ReadPin(_gpio, _pin);
			while(bits-->0)
			{
				if(_counter == 0) // Viene desde marking, entonces, solo puede haber uno independientemente de bits
				{
					if(pinValue == GPIO_PIN_RESET) setStatus(Disabled); // debería ser un start bit (nivel alto) pero es bajo, ergo es un error
					_rx_char = 0;
					_counter++;
					return;
				}
				uint8_t innerCounter = _counter % 10; //inner counter puede ser 0 (start bit), 1-7 (data bit), 8 (parity bit) y 9 (stop bit)
				if(innerCounter == 0)
				{
					_rx_char = 0;
				}
				else if(innerCounter == 8)
				{
					0; // no compruebo paridad porque alv, pero tampoco puedo guardarlo
				}
				else if(innerCounter == 9)
				{
					_rx_buffer.push_back(_rx_char & 0x7f);
					if(_rx_buffer.idx > 2)
					{
						if(_rx_buffer.buf[_rx_buffer.idx-1] == '\n' && _rx_buffer.buf[_rx_buffer.idx-2] == '\r')
						{
							if(_state == Command)
							{
								_state = EndCommand;
							}
							else if(_state == StartMeasure)
							{
								_measNumber = _rx_buffer.buf[4]-'0'; //guarda el número de medidas

								_rx_buffer.clear(); //

								_state = WaitingSR;
								_last_rx_counter = (_rx_buffer.buf[1]-'0')*100 + (_rx_buffer.buf[2]-'0')*10 + (_rx_buffer.buf[3]-'0');
								_last_rx_counter *= 1200;

								_state = WaitingSR;
							}
							else if(_state == WaitingSR)
							{
								_state = GettingData;
								_tx_buffer.clear();

								_tx_buffer.push_back(_measAddr);
								_tx_buffer.push_back('M');
								if(_additionalMeas != 0) _tx_buffer.push_back(_additionalMeas); // añadir CRC
								_tx_buffer.push_back('!');

								_counter = 0;
								setStatus(Transmitting);
							}
							else if(_state == GettingData)
							{
								_state = DataFull;
							}
						}
					}
					if(pinValue == GPIO_PIN_RESET)// antes era low, por lo que se asume que esto es un bit de start
					{
						_counter = 0; //de forma que luego vale 1
						_rx_char = 0;
					}
				}
				else
				{
					if(pinValue == GPIO_PIN_SET)//Antes era todo low, por lo que eran 1's lógicos (invertido)
						_rx_char |= (0x01 << (innerCounter - 1));
				}
				_counter++;
			}
			_last_rx = getUS();
		}
	}

	uint8_t SDI12_Driver::parity_even(uint8_t byte)
	{
		uint8_t parity = 0;
		while(byte)
		{
			parity = !parity;
			byte = byte & (byte-1);
		}
		return parity;
	}

	void SDI12_Driver::setStatus(SDI12_Driver::TransceiverStatus status)
	{
		_status = status;
		_counter = 0;
		switch(status)
		{
		case Disabled:
			{
				GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
				HAL_GPIO_Init(_gpio, &gpio_init);
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
			}
			break;

		case Breaking:
			GPIO_InitTypeDef gpio_init;
			gpio_init.Pin = _pin;
			gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
			gpio_init.Pull = GPIO_NOPULL;
			gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
			HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET);
			HAL_GPIO_Init(_gpio, &gpio_init);
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
			break;

		case MarkingTx:
			HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
			break;

		case Receiving:
			{
				GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_IT_RISING_FALLING;
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_Init(_gpio, &gpio_init);
				if(_pin == GPIO_PIN_15)
				{
					HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
					HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

				}
				else if(_pin == GPIO_PIN_14)
				{
					HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
					HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				}
			}
			break;

		default:
			break;
		}
	}

	int SDI12_Driver::ackActive(uint8_t addr)
	{
		if(_state != Nop) return EBUSY;

		_state = Command;

		_tx_buffer.clear();

		if(addr>9) _tx_buffer.push_back(addr);
		else _tx_buffer.push_back(addr+'0');
		_tx_buffer.push_back('!');


		setStatus(Breaking);
		return 0;
	}

	int SDI12_Driver::sensorId(uint8_t addr)
	{
		if(_state != Nop) return EBUSY;

		_state = Command;

		_tx_buffer.clear();

		if(addr>9) _tx_buffer.push_back(addr);
		else _tx_buffer.push_back(addr+'0');
		_tx_buffer.push_back('I');
		_tx_buffer.push_back('!');

		setStatus(Breaking);
		return 0;
	}

	int SDI12_Driver::changeAddr(uint8_t srcAddr, uint8_t dstAddr)
	{
		if(_state != Nop) return EBUSY;

		_state = Command;

		_tx_buffer.clear();

		if(srcAddr>9) _tx_buffer.push_back(srcAddr);
		else _tx_buffer.push_back(srcAddr+'0');

		_tx_buffer.push_back('A');

		if(dstAddr>9) _tx_buffer.push_back(dstAddr);
		else _tx_buffer.push_back(dstAddr+'0');

		_tx_buffer.push_back('!');

		setStatus(Breaking);
	}

	int SDI12_Driver::queryAddr()
	{
		if(_state != Nop) return EBUSY;

		_state = Command;

		_tx_buffer.clear();

		_tx_buffer.push_back('?');
		_tx_buffer.push_back('!');

		setStatus(Breaking);
		return 0;
	}

	int SDI12_Driver::startVerification(uint8_t addr)
	{
		if(_state != Nop) return EBUSY;

		_state = Command;

		_tx_buffer.clear();

		if(addr>9) _tx_buffer.push_back(addr);
		else _tx_buffer.push_back(addr+'0');
		_tx_buffer.push_back('V');
		_tx_buffer.push_back('!');

		setStatus(Breaking);
		return 0;
	}

	uint8_t* SDI12_Driver::getCmdResponse()
	{
		if(_state != EndCommand) return nullptr;
		if(_rx_buffer.idx == 0) return nullptr;
		_state = Nop;
		return _rx_buffer.buf;
	}

	int SDI12_Driver::startMeasurement(uint8_t addr, uint8_t additional, uint8_t type, bool crc)
	{
		if(_state != Nop) return EBUSY;

		_state = StartMeasure;

		_tx_buffer.clear();

		_tx_buffer.push_back(addr<9 ? addr+'0' : addr);
		_measAddr = _tx_buffer.buf[0];

		if(type == 0 || type == 'M')
			_tx_buffer.push_back('M');
		else if(type == 'C')
			_tx_buffer.push_back('C');
		else if(type == 'R')
			_tx_buffer.push_back('R');
		else
		{
			_tx_buffer.clear();
			_state = Nop;
			return EPERM;
		}

		if(crc) _tx_buffer.push_back('C');
		_additionalMeas = 0;
		if(additional < 10 && additional > 0)
		{
			_tx_buffer.push_back(additional+'0');
			_additionalMeas = additional;
		}

		_tx_buffer.push_back('!');

		setStatus(Breaking);
		return 0;
	}

	int SDI12_Driver::getData()
	{
		if(_state != DataFull) return -1;
		_state = Nop;
		return 0;
	}

	bool SDI12_Driver::loop()
	{
		return !(_state == Nop);
	}
}
