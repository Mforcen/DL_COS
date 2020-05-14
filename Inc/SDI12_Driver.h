#ifndef SDI12_DRIVER_H
#define SDI12_DRIVER_H

#include "errno.h"
#include "mem_structs.hpp"
#include "Module.h"

#include "stm32f1xx_hal.h"

uint16_t getUS();

namespace FwLogger
{
	// TODO (forcen#1#): Create better interfaces for other parts of the program
	class SDI12_Driver : public Module
	{
		public:
			SDI12_Driver(GPIO_TypeDef* gpio, uint32_t pin);
			int write(uint8_t* cmd);

			void timer_isr();
			void pin_isr();

			int ackActive(uint8_t addr);
			int sensorId(uint8_t addr);
			int changeAddr(uint8_t srcAddr, uint8_t dstAddr);
			int queryAddr();
			int startVerification(uint8_t addr);
			uint8_t* getCmdResponse();

			int startMeasurement(uint8_t addr, uint8_t type = 0, uint8_t additional = 0, bool crc = false);
			int getData();

		protected:

		private:
			GPIO_TypeDef* _gpio;
			uint32_t _pin;

			fixed_string<80> _rx_buffer;
			fixed_string<20> _tx_buffer;

			enum TransceiverStatus
			{
				Disabled,
				Breaking,
				MarkingTx,
				MarkingRx,
				Receiving,
				Transmitting,
				MeasWaiting
			};
			TransceiverStatus _status;
			void setStatus(TransceiverStatus status);

			enum SDI12States
			{
				Nop,
				Command,
				EndCommand,
				StartMeasure,
				WaitingSR,
				GettingData,
				DataFull
			};
			SDI12States _state;

			int8_t _error;
			int _counter;
			int _last_rx_counter;
			uint8_t _measNumber;
			uint8_t _additionalMeas;
			uint8_t _measAddr;

			uint8_t parity_even(uint8_t byte);

			uint8_t _rx_char;
			uint32_t _last_rx;
			const static uint8_t _rxFudge = 75;
	};
}
#endif // SDI12_DRIVER_H
