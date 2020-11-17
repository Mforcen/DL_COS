#ifndef DIGITAL_DRIVER_H
#define DIGITAL_DRIVER_H

#include "Module.h"
#include "stm32f1xx_hal.h"

namespace FwLogger
{
	enum class PinMode : uint8_t
	{
		Output,
		Input,
		InputIT
	};

	struct PinStatus
	{
		GPIO_TypeDef* port;
		uint16_t pin;
		PinMode mode;
		int counter;
	};

	class Digital_Driver : public Module
	{
		public:
			Digital_Driver();
			~Digital_Driver();

			int digitalRead(int pin);
			void digitalWrite(int pin, int value);
			int pulseRead(int pin);

			void ISR(int pin);
			bool loop();
		protected:

		private:
			void changeMode(int pin, PinMode newMode);
			PinStatus _pins[6];
	};
}

#endif // DIGITAL_DRIVER_H
