#include "SDI12_Driver.h"

SDI12_Driver::SDI12_Driver(GPIO_TypeDef* gpio, uint32_t pin)
{
	_gpio = gpio;
	_pin = pin;
}

SDI12_Driver::~SDI12_Driver()
{
	//dtor
}

int SDI12_Driver::available()
{
	return rx_buffer.size();
}

int SDI12_Driver::pop_front(uint8_t* ph)
{
	return rx_buffer.pop_front(ph);
}

int SDI12_Driver::write(uint8_t* cmd)
{
	if(_status != Disabled) return -1;
	if(tx_buffer.idx != 0) return -1;//de momento solo utilizar una única movida

	int i;
	for(i = 0; cmd[i]; ++i) tx_buffer.push_back(cmd[i]);
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

	case Holding:

		break;

	case Receiving:

		break;

	case Transmitting: //TODO revisar porque esto no está bien, hay que cumplir los 10 bits
		{
			int bit_idx, byte_idx;
			bit_idx = _counter % 10; //obtener qué hay que poner (start, 7 bits, paridad, stop)
			byte_idx = _counter / 10;

			if(bit_idx == 0)
				HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET);
			else if(bit_idx == 8)
				HAL_GPIO_WritePin(_gpio, _pin, parity_even(tx_buffer.buf[byte_idx]) ? GPIO_PIN_RESET : GPIO_PIN_SET);
			else if(bit_idx == 9)
				HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(_gpio, _pin, (tx_buffer.buf[byte_idx] >> bit_idx) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);

			_counter++;
			byte_idx = _counter / 10;
			if(byte_idx == tx_buffer.idx)
			{
				tx_buffer.clear();
				setStatus(MarkingRx);
			}
		}
		break;

	default:
		break;
	}
}

void SDI12_Driver::pin_isr() //Esto se queda en Listening forever
{
    switch(_status){
	case Receiving:
		{
			uint16_t dt_rx = getUS()-last_rx;
			last_rx = getUS();
			uint32_t bits = (1200*(dt_rx+rxFudge))/1000000;
			GPIO_PinState pinValue = HAL_GPIO_ReadPin(_gpio, _pin);
			while(bits-->0)
			{
				if(_counter == 0) // Viene desde marking, entonces, solo puede haber uno independientemente de bits
				{
					if(pinValue == GPIO_PIN_RESET)
					{
						setStatus(Disabled);
					}
					rx_char = 0;
					bits = 0;
				}
				else if(_counter == 9)
				{
					rx_buffer.push_back(rx_char & 0x7f);
					if(pinValue == GPIO_PIN_RESET)//antes era low, por lo que se asume que esto es un bit de start
					{
						_counter = 0; //de forma que luego vale 1
						rx_char = 0;
					}
				}
				else
				{
					if(pinValue == GPIO_PIN_SET)//Antes era todo low, por lo que eran 1's
						rx_char |= (0x01 << (_counter - 1));

				}
				_counter = (_counter + 1) % 10;
			}
			last_rx = getUS();

		}
		break;

	default:
		break;
    }
}

SDI12_Driver::SDI12Status SDI12_Driver::getStatus()
{
	return _status;
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

void SDI12_Driver::setStatus(SDI12_Driver::SDI12Status status)
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
			HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET);
			HAL_GPIO_Init(_gpio, &gpio_init);
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		}
		break;

	case Holding:
		HAL_GPIO_WritePin(_gpio, _pin, GPIO_PIN_SET);
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
