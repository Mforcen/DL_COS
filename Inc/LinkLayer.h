#ifndef LINKLAYER_H
#define LINKLAYER_H

#include "Module.h"
#include "mem_structs.hpp"
#include "Log.h"
#include "LoRa.h"


namespace FwLogger
{
	struct LLPacket
	{
		enum LLVersion : uint8_t
		{
			Testing = 0
		} version;

		enum Type : uint8_t
		{
			Confirmable = 0,
			Non_confirmable,
			Acknowledgement,
			Reset
		} type;

		uint16_t srcAddr; // 12 bytes
		uint16_t dstAddr; // 12 bytes
		uint16_t msgIdx; // 12 bytes
		uint8_t numPackets; // 1 byte
		uint8_t numPacket; // 1 byte

		void serialize(uint8_t* buf);
		void deserialize(uint8_t* buf);
	};

	class LinkLayer : public Module
	{
		public:
			LinkLayer(LoRa* lora, Allocator<128>* alloc);

			bool sendPacket(LLPacket packet);
			bool sendPacket();

			int available();

			uint8_t get();

		protected:

		private:
			LoRa* _radio;
			Allocator<128>* _alloc;
			circular_buffer<16> _txPackets; // get the remaining packets
			circular_buffer<16> _rxPackets; // get the remaining packets
			enum class State
			{
				Nop,
				SendingPacket,
				WaitingAck
			};
	};
}
#endif // LINKLAYER_H
