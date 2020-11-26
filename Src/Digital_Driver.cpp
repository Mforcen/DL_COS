#include "Digital_Driver.h"

namespace FwLogger
{
	Digital_Driver::Digital_Driver() : Module("DigitalDriver", DIGITAL_BINID, 0, 1)
	{
		_pins[0].port = GPIOC;
		_pins[1].port = GPIOC;
		_pins[2].port = GPIOC;
		_pins[3].port = GPIOC;
		_pins[4].port = GPIOB;
		_pins[5].port = GPIOC;

		_pins[0].pin = GPIO_PIN_0;
		_pins[1].pin = GPIO_PIN_1;
		_pins[2].pin = GPIO_PIN_2;
		_pins[3].pin = GPIO_PIN_3;
		_pins[4].pin = GPIO_PIN_2;
		_pins[5].pin = GPIO_PIN_8;

		_pins[0].mode = PinMode::Input;
		_pins[1].mode = PinMode::Input;
		_pins[2].mode = PinMode::Input;
		_pins[3].mode = PinMode::Input;
		_pins[4].mode = PinMode::Input;
		_pins[5].mode = PinMode::Input;
	}

	Digital_Driver::~Digital_Driver()
	{
		//dtor
	}

	int Digital_Driver::digitalRead(int pin)
	{
		if(_pins[pin].mode != PinMode::Input) changeMode(pin, PinMode::Input);
		return HAL_GPIO_ReadPin(_pins[pin].port, _pins[pin].pin);
	}

	void Digital_Driver::digitalWrite(int pin, int value)
	{
		if(_pins[pin].mode != PinMode::Output) changeMode(pin, PinMode::Output);
		HAL_GPIO_WritePin(_pins[pin].port, _pins[pin].pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	int Digital_Driver::pulseRead(int pin)
	{
		if(_pins[pin].mode != PinMode::InputIT) changeMode(pin, PinMode::InputIT);
		return _pins[pin].counter;
	}

	void Digital_Driver::ISR(int pin)
	{
		if(pin == 0) return;
		int pos = 0;
		while((pin & 1) == 0)
		{
			pin = pin >> 1;
			++pos;
		}
		_pins[pos].counter++;
	}

	bool Digital_Driver::loop()
	{
		return false;
	}

	void Digital_Driver::changeMode(int pin, PinMode newMode)
	{
		if(_pins[pin].mode == PinMode::InputIT)
		{
			if(_pins[pin].pin == GPIO_PIN_2) HAL_NVIC_DisableIRQ(EXTI2_IRQn);
			else if(_pins[pin].pin == GPIO_PIN_3) HAL_NVIC_DisableIRQ(EXTI3_IRQn);
		}

		GPIO_InitTypeDef ginit;
		ginit.Pin = _pins[pin].pin;
		ginit.Speed = GPIO_SPEED_FREQ_MEDIUM;
		ginit.Pull = GPIO_NOPULL;

		if(newMode == PinMode::Input) ginit.Mode = GPIO_MODE_INPUT;
		else if(newMode == PinMode::InputIT)
		{
			ginit.Mode = GPIO_MODE_IT_RISING;

			if(_pins[pin].pin == GPIO_PIN_2) HAL_NVIC_EnableIRQ(EXTI2_IRQn);
			else if(_pins[pin].pin == GPIO_PIN_3) HAL_NVIC_EnableIRQ(EXTI3_IRQn);
			else return;

			_pins[pin].counter = 0;
		}
		else if(newMode == PinMode::Output) ginit.Mode = GPIO_MODE_OUTPUT_PP;

		HAL_GPIO_Init(_pins[pin].port, &ginit);
		_pins[pin].mode = newMode;
	}
}

