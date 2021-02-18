#include "SDI12_Driver.h"

namespace FwLogger
{
	SDI12_Driver::SDI12_Driver(GPIO_TypeDef* gpio, uint32_t pin) : Module("SDI12Soft", SDI12_BINID, 1, 0)
	{
		_gpio = gpio;
		_pin = pin;

		HAL_GPIO_WritePin(SDI12_DIR, GPIO_PIN_RESET); // B to A

		GPIO_InitTypeDef gpio_dir;
		gpio_dir.Mode = GPIO_MODE_OUTPUT_PP;
		gpio_dir.Pin = SDI12_Dir_Pin;
		gpio_dir.Speed = GPIO_SPEED_FREQ_LOW;
		gpio_dir.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(SDI12_Dir_GPIO_Port, &gpio_dir);

		m_debugStatus = false;
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
			if(_counter++ >= 16)
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
			_counter++;
			if(_counter == 1)
			{
				/*GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				//gpio_init.Mode = GPIO_MODE_INPUT;
				gpio_init.Mode = GPIO_MODE_IT_RISING;
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_Init(_gpio, &gpio_init);

				HAL_GPIO_WritePin(SDI12_DIR, GPIO_PIN_SET); // 5V a 3V3*/
				GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_IT_RISING; // busco bit de start
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_Init(_gpio, &gpio_init);

				HAL_GPIO_WritePin(SDI12_DIR, GPIO_PIN_SET); // 5V a 3V3
			}
			else if(_counter == 9)
			{
				/*GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_IT_RISING_FALLING;
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_Init(_gpio, &gpio_init);

				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);*/

				/*if(_pin == GPIO_PIN_15)
				{
					HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
					HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

				}
				else*/ /*if(_pin == GPIO_PIN_14)
				{
					HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
					HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				}*/
				_rx_buffer.clear();
				setStatus(Receiving);
				_counter = 0;
			}
			//if(_counter++ >= 10) // aquí incremento el contador
			//{
				/*GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_INPUT;
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_Init(_gpio, &gpio_init);

				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);*/

			//	setStatus(Receiving);
			//	_counter = 0;
			//}
			break;

		case Receiving:
			if(_state == Command || _state == GettingData)
			{
				_last_rx_counter++;
				if(_last_rx_counter < 40 && _last_rx_counter > 30)
				{
					//Log::Info("[SDI12]: RXTO\n");
					if(_counter == 0) //0 bits received, sensor not responding
					{
						setStatus(RetryWaiting);
					}
					else
					{
						_last_rx_counter = 40;
						push_bits(1, GPIO_PIN_RESET); //put stop bit
					}
				}
				else if(_last_rx_counter > 50) // incomplete msg, push bits didn't end the msg
					setStatus(RetryWaiting);
				//_state = Nop;
			}
			else if(_state == WaitingSR) // add error
			{
				_last_rx_counter--;
				if(_last_rx_counter <= 0)
				{
					startGetData(_measAddr, 0);
				}
			}
			else
			{
				_last_rx_counter++;
				if(_last_rx_counter > 30 && _last_rx_counter < 40)
				{
					if(_counter == 0) //0 bits received, sensor not responding
					{
						setStatus(RetryWaiting);
					}
					else
					{
						_last_rx_counter = 40;
						push_bits(1, GPIO_PIN_RESET); //put stop bit
					}
				}
				else if(_last_rx_counter > 50) // incomplete msg, push bits didn't end the msg
					setStatus(RetryWaiting);

			}
			break;

		case Transmitting:
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
					//_tx_buffer.clear();
					setStatus(MarkingRx);
				}
			}
			break;
		case RetryWaiting:
			if(_retry_counter >= 5)
			{
				if(_state == GettingData)
				{
					for(std::size_t i = 0; i < _measSize; ++i)
						_measDst[i] = std::numeric_limits<float>::quiet_NaN();

					setState(DataFull);
					Log::Warning("[SDI12] Sensor not responding data\n");
				}
				else if(_state == Command)
				{
					Log::Warning("[SDI12] Sensor not responding command\n");
				}
				else if(_state == StartMeasure)
				{
					for(std::size_t i = 0; i < _measSize; ++i)
						_measDst[i] = std::numeric_limits<float>::quiet_NaN();

					setState(DataFull);
					Log::Warning("[SDI12] Sensor not measuring\n");
				}

				_error = 1;
				setStatus(Disabled);
			}
			_counter++;
			if(_counter >= 10)
			{
				setStatus(Transmitting);
			}
			break;

		default:
			break;
		}
	 }

	void SDI12_Driver::pin_isr() //Esto se queda en Listening hasta timeout TODO reescribir para reducir tiempo de ejecucion
	{
		if(_status == Receiving){
			if(_state == WaitingSR) return;
			_last_rx_counter = 0;

			uint16_t usNow = getUS();
			uint16_t dt_rx = static_cast<uint16_t>(usNow-_last_rx);
			_last_rx = usNow;

			uint32_t bits = (1200*(dt_rx+_rxFudge))/1000000;

			GPIO_PinState pinValue = HAL_GPIO_ReadPin(_gpio, _pin);

			//Log::Verbose("R:%d,%d,%d\n", dt_rx, pinValue, _counter);

			push_bits(bits, pinValue);
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
		if(m_debugStatus) Log::Verbose("Status: %s\tC:%d\tRW:%d\n", getTransceiverStatus(), _counter, _retry_counter);

		_counter = 0;
		switch(status)
		{
		case Disabled:
			{
				//reset();
				HAL_GPIO_WritePin(SDI12_DIR, GPIO_PIN_RESET); // B to A

				GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;

				HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
				HAL_GPIO_Init(_gpio, &gpio_init);

				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			}
			break;

		case Breaking:
			{
				HAL_GPIO_WritePin(SDI12_DIR, GPIO_PIN_RESET); // 3V3 a 5V

				GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET);
				HAL_GPIO_Init(_gpio, &gpio_init);
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			}
			break;

		case MarkingTx:
			HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
			break;

		case MarkingRx:
			{
				HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
				/*GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_IT_RISING; // busco bit de start
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_Init(_gpio, &gpio_init);

				HAL_GPIO_WritePin(SDI12_DIR, GPIO_PIN_SET); // 5V a 3V3*/
			}
			break;

		case Receiving:
			{
				/*GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_IT_RISING; // busco bit de start
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_Init(_gpio, &gpio_init);

				HAL_GPIO_WritePin(SDI12_DIR, GPIO_PIN_SET); // 5V a 3V3*/

				/*if(_pin == GPIO_PIN_15)
				{
					HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
					HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

				}
				else*/ if(_pin == GPIO_PIN_6)
				{
				}
				_last_rx = getUS();
				HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
			}
			break;

		case RetryWaiting:
			{
				_retry_counter++;
				_last_rx_counter = 0;

				HAL_GPIO_WritePin(SDI12_DIR, GPIO_PIN_RESET); // 3V3 a 5V

				GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET);
				HAL_GPIO_Init(_gpio, &gpio_init);
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);  //modo output
			}
			break;

		default:
			break;
		}
	}

	void SDI12_Driver::setState(SDI12States state)
	{
		_state = state;
		Log::Verbose("[SDI12]->%s\n", getSDI12State());
	}

	int SDI12_Driver::ackActive(uint8_t addr)
	{
		if(_state != Nop) return EBUSY;

		setState(Command);

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

		setState(Command);

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

		setState(Command);

		_tx_buffer.clear();

		if(srcAddr>9) _tx_buffer.push_back(srcAddr);
		else _tx_buffer.push_back(srcAddr+'0');

		_tx_buffer.push_back('A');

		if(dstAddr>9) _tx_buffer.push_back(dstAddr);
		else _tx_buffer.push_back(dstAddr+'0');

		_tx_buffer.push_back('!');

		setStatus(Breaking);
		return 0;
	}

	int SDI12_Driver::queryAddr()
	{
		if(_state != Nop) return EBUSY;

		setState(Command);

		_tx_buffer.clear();

		_tx_buffer.push_back('?');
		_tx_buffer.push_back('!');

		setStatus(Breaking);
		return 0;
	}

	int SDI12_Driver::startVerification(uint8_t addr)
	{
		if(_state != Nop) return EBUSY;

		setState(Command);

		_tx_buffer.clear();

		if(addr>9) _tx_buffer.push_back(addr);
		else _tx_buffer.push_back(addr+'0');
		_tx_buffer.push_back('V');
		_tx_buffer.push_back('!');

		setStatus(Breaking);
		return 0;
	}

	int SDI12_Driver::setFudge(uint8_t newFudge)
	{
		_rxFudge = newFudge;
		return _rxFudge;
	}

	uint8_t* SDI12_Driver::getCmdResponse()
	{
		if(_state != EndCommand) return nullptr;
		if(_rx_buffer.idx == 0) return nullptr;
		setState(Nop);
		return _rx_buffer.buf;
	}

	void SDI12_Driver::lowPower()
	{
		setStatus(TransceiverStatus::Disabled);
		setState(SDI12States::Nop);
	}

	int SDI12_Driver::singleMeasure(uint8_t addr, float* dst, uint8_t count, int additional)
	{
		if(_state == Nop) // start measure
		{
			reset();
			Log::Info("SDI12 %d meas on %d\n", count, addr);
			startMeasurement(addr, additional);
			_measDst = dst;
			_measSize = count;
			_measAddr = addr;
			return 1;
		}
		else if(_state == DataFull)
		{
			Log::Info("SDI12 collected\n");
			if(_error == 1)
			{
				_error = 0;
				for(uint8_t i = 0; i < count; ++i) dst[i] = std::numeric_limits<float>::quiet_NaN();
			}
			setState(Nop);
			return 0;
		}
		else
		{
			return 2; // still measuring things
		}
	}

	int SDI12_Driver::continuousMeasure(uint8_t addr, float* dst, uint8_t count, int additional)
	{
		Log::Error("Called unimplemented function on SDI12\n");
		return 0;
	}

	int SDI12_Driver::startMeasurement(uint8_t addr, uint8_t additional, uint8_t type, bool crc)
	{
		if(_state != Nop) return EBUSY;

		_retry_counter = 0;

		setState(StartMeasure);

		_tx_buffer.clear();

		_tx_buffer.push_back(addr < 9 ? addr+'0' : addr);
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
			setState(Nop);
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

	int SDI12_Driver::startGetData(uint8_t addr, uint8_t additional)
	{
		setState(GettingData);

		_tx_buffer.clear();
		_tx_buffer.push_back(addr<9 ? addr+'0' : addr);
		_tx_buffer.push_back('D');
		_tx_buffer.push_back(additional + '0');
		_tx_buffer.push_back('!');

		setStatus(Breaking);
		return 0;
	}

	int SDI12_Driver::parseData(float* dst, int count, uint8_t* buf)
	{
		int val_idx = -1;
		if(count <= 0) return 0;

		uint8_t sign, decimal = 0;
		int integer_part = 0;

		if(buf[0]-'0' != _measAddr) return -1;

		int buf_idx = 1;

		do
		{
			char c = buf[buf_idx];
			if(c == '+' || c == '-')
			{
				if(val_idx >= 0)
				{
					float decs = 1;
					for(uint8_t i = 0; i < (decimal-1); ++i) decs *= 0.1;
					dst[val_idx] += integer_part * decs;
					if(sign) dst[val_idx] = -dst[val_idx];
				}

				val_idx += 1;
				if(val_idx >= count)
					return count; // exceeds number of values

				if(c == '-') sign = 1;
				else sign = 0;
				integer_part = 0;
				decimal = 0;
			}
			else
			{
				if(c == '.')
				{
					dst[val_idx] = integer_part;
					integer_part = 0;
					decimal = 1;
				}
				else
				{
					if(decimal == 0)
					{
						/*dst[val_idx] *= 10;
						dst[val_idx] += (c - '0');*/
						integer_part *= 10;
						integer_part += (c -'0');
					}
					else
					{
						decimal++;
						integer_part *= 10;
						integer_part += (c -'0');
					}
				}
			}
		} while(buf[++buf_idx] != '\r');

		if(val_idx >= 0)
		{
			float decs = 1;
			for(uint8_t i = 0; i < (decimal-1); ++i) decs *= 0.1;
			dst[val_idx] += integer_part * decs;
			if(sign) dst[val_idx] = -dst[val_idx];
		}

		return val_idx+1;
	}

	void SDI12_Driver::push_bits(int16_t bits, GPIO_PinState pinValue)
	{
		if(reinterpret_cast<intptr_t>(this) == 0x20) // TODO (forcen#1#12/03/20): probar a quitar
		{
			return;
		}
		while(bits-->0)
		{
			if(_counter == 0) // Viene desde marking, entonces, solo puede haber uno independientemente de bits
			{
				if(pinValue == GPIO_PIN_RESET)
				{
					Log::Verbose("Error on start\n");
					for(int i = 0; i < _measSize; ++i)
					{
						_measDst[i] = std::numeric_limits<float>::quiet_NaN();
					}
					setState(StartMeasure);
					setStatus(Transmitting); // debería ser un start bit (nivel alto) pero es bajo, ergo es un error
				}

				GPIO_InitTypeDef gpio_init;
				gpio_init.Pin = _pin;
				gpio_init.Mode = GPIO_MODE_IT_RISING_FALLING; // busco bit de start
				gpio_init.Pull = GPIO_NOPULL;
				gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_Init(_gpio, &gpio_init);

				_rx_char = 0;
				_counter++;
				return;
			}
			uint8_t innerCounter = _counter % 11; //inner counter puede ser 0 (start bit), 1-7 (data bit), 8 (parity bit) y 9 (stop bit)
			if(innerCounter == 1)
			{
				_rx_char = 0;
			}
			else if(innerCounter == 9)
			{
				(void)0; // TODO (forcen#1#12/03/20): Comprobar paridad
			}
			else if(innerCounter == 10)
			{
				//Log::Verbose("C: %c\n",_rx_char);

				_rx_buffer.push_back(_rx_char & 0x7f);
				if(_rx_buffer.idx > 2)
				{
					if(_rx_buffer.buf[_rx_buffer.idx-1] == '\n' && _rx_buffer.buf[_rx_buffer.idx-2] == '\r')
					{
						if(_state == Command)
						{
							setState(EndCommand);
						}
						else if(_state == StartMeasure)
						{
							_measNumber = _rx_buffer.buf[4]-'0'; //guarda el número de medidas
							_measNumber = _measNumber < _measSize ? _measNumber : _measSize; // clamp _measNumber to _measSize
							_measIdx = 0;
							_measCommand = 1;

							_last_rx_counter = (_rx_buffer.buf[1]-'0')*100 + (_rx_buffer.buf[2]-'0')*10 + (_rx_buffer.buf[3]-'0');
							_last_rx_counter *= 1200;
							_rx_buffer.clear(); //

							setState(WaitingSR);
						}
						else if(_state == GettingData)
						{
							Log::Info("SDI12 got %d data from %c\n", _measIdx, _measAddr+'0');//TODO Parsedata can raise errors, log that
							_measIdx += parseData(&(_measDst[_measIdx]), _measSize-_measIdx, _rx_buffer.buf);
							if(_measIdx < _measNumber)
							{
								startGetData(_measAddr, _measCommand++);
							}
							else
							{
								setStatus(Disabled);
								setState(DataFull);
							}
						}
					}
				}
				if(pinValue == GPIO_PIN_SET)// antes era low, por lo que se asume que esto es un bit de start
				{
					_counter = 0; //de forma que luego vale 1
					_rx_char = 0;
				}
			}
			else
			{
				if(pinValue == GPIO_PIN_SET)//Antes era todo low, por lo que eran 1's lógicos (invertido)
					_rx_char |= (0x01 << (innerCounter - 2));
			}
			_counter++;
		}
	}

	bool SDI12_Driver::loop()
	{
		return !(_state == Nop);
	}

	void SDI12_Driver::reset()
	{
		HAL_GPIO_WritePin(SDI12_DIR, GPIO_PIN_RESET); // B to A

		GPIO_InitTypeDef gpio_dir;
		gpio_dir.Mode = GPIO_MODE_OUTPUT_PP;
		gpio_dir.Pin = SDI12_Dir_Pin;
		gpio_dir.Speed = GPIO_SPEED_FREQ_LOW;
		gpio_dir.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(SDI12_Dir_GPIO_Port, &gpio_dir);

		HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET); // line standby

		GPIO_InitTypeDef gpio_init;
		gpio_init.Pin = _pin;
		gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
		gpio_init.Pull = GPIO_NOPULL;
		gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(_gpio, &gpio_init);

		_state = Nop;
		_status = Disabled;

		_error = 0;
		_counter = 0;
		_last_rx_counter = 0;
		_measNumber = 0;
		_additionalMeas = 0;
		_measAddr = 0;
		_measDst = nullptr;
		_retry_counter = 0;
	}
}
