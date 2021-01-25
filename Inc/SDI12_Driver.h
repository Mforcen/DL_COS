#ifndef SDI12_DRIVER_H
#define SDI12_DRIVER_H

#include "errno.h"
#include "mem_structs.hpp"
#include "Module.h"
#include "Log.h"

#include <limits>

#include "pin_defs.h"

#include "stm32hal_libs.h"

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
			int setFudge(uint8_t newFudge);
			uint8_t* getCmdResponse();

			void lowPower();

			int singleMeasure(uint8_t addr, float* dst, uint8_t count, int additional = 0);
			int continuousMeasure(uint8_t addr, float* dst, uint8_t count, int additional = 0);

			int startMeasurement(uint8_t addr, uint8_t type = 0, uint8_t additional = 0, bool crc = false);

			struct Status
			{
				int8_t error;
				int8_t retry_counter;
				int counter;
				int last_rx_counter;
				uint8_t measNumber;
				uint8_t additionalMeas;

				uint8_t measAddr;
				uint8_t measSize;
				uint8_t measIdx;
				uint8_t measCommand;
				float* measDst;
			};

			Status getModuleStatus()
			{
				Status st;
				st.error = _error;
				st.retry_counter = _retry_counter;
				st.counter = _counter;
				st.last_rx_counter = _last_rx_counter;
				st.measNumber = _measNumber;
				st.additionalMeas = _additionalMeas;

				st.measAddr = _measAddr;
				st.measSize = _measSize;
				st.measIdx = _measIdx;
				st.measCommand = _measCommand;
				st.measDst = _measDst;

				return st;
			}

			const char* getTransceiverStatus()
			{
				switch(_status)
				{
				case Disabled:
					return "Disabled";
				case Breaking:
					return "Breaking";
				case MarkingTx:
					return "MarkingTx";
				case MarkingRx:
					return "MarkingRx";
				case Receiving:
					return "Receiving";
				case Transmitting:
					return "Transmitting";
				case MeasWaiting:
					return "MeasWaiting";
				case RetryWaiting:
					return "RetryWaiting";
				default:
					return "Unknown";
				}
			}

			const char* getSDI12State()
			{
				switch(_state)
				{
				case Nop:
					return "Nop";
				case Command:
					return "Command";
				case EndCommand:
					return "EndCommand";
				case StartMeasure:
					return "StartMeasure";
				case WaitingSR:
					return "WaitingSR";
				case GettingData:
					return "GettingData";
				case DataFull:
					return "DataFull";
				default:
					return "Unknown";
				}
			}

			bool loop();

			void reset();
		protected:

		private:
			bool m_debugStatus;
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
				MeasWaiting,
				RetryWaiting
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
			void setState(SDI12States state);
			SDI12States _state;

			int8_t _error;
			int8_t _retry_counter;
			int _counter;
			int _last_rx_counter;
			uint8_t _measNumber;
			uint8_t _additionalMeas;

			uint8_t _measAddr;
			uint8_t _measSize;
			uint8_t _measIdx;
			uint8_t _measCommand;
			float* _measDst;

			uint8_t parity_even(uint8_t byte);
			//int startMeasurement(uint8_t addr, uint8_t type = 0, uint8_t additional = 0, bool crc = false);
			int startGetData(uint8_t addr, uint8_t additional);
			int parseData(float* dst, int count, uint8_t* buf);

			void push_bits(int16_t bits, GPIO_PinState pinValue);

			circular_buffer<16,uint16_t> _rx_times;

			uint8_t _rx_char;
			uint32_t _last_rx;
			uint8_t _rxFudge = 200;
	};
}
#endif // SDI12_DRIVER_H
