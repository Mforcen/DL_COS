#ifndef SDI12_DRIVER_H
#define SDI12_DRIVER_H

#include "mem_structs.hpp"
#include "stm32f1xx_hal.h"

uint16_t getUS();

class SDI12_Driver
{
	public:
		SDI12_Driver(GPIO_TypeDef* gpio, uint32_t pin);
		int available();
		int pop_front(uint8_t* ph);
		int write(uint8_t* cmd);

		void timer_isr();
		void pin_isr();
		void setFudge(uint8_t fudge);


		enum SDI12Status
		{
			Disabled,
			Breaking,
			MarkingTx,
			MarkingRx,
			Receiving,
			Transmitting
		};

		SDI12Status getStatus();

	protected:

	private:
		GPIO_TypeDef* _gpio;
		uint32_t _pin;

		circular_buffer rx_buffer;
		fixed_string tx_buffer;

		SDI12Status _status;
		void setStatus(SDI12Status status);

		int _error;
		int _counter;
		int _last_rx_counter;

		uint8_t parity_even(uint8_t byte);

		uint8_t rx_char;
		uint32_t last_rx;
		const static uint8_t rxFudge = 75;
};

#endif // SDI12_DRIVER_H
