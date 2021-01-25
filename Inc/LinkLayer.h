#ifndef LINKLAYER_H
#define LINKLAYER_H

#include "mem_structs.hpp"
#include "stm32hal_libs.h"
#include <cstdint>

namespace FwLogger
{
	/*
	LLVersion version; // 4 bits
	LLType type; // 4 bits
	uint16_t srcAddr; // 12 bits
	uint16_t dstAddr; // 12 bits
	uint16_t len; // 12 bits
	uint8_t id; // 4 bits
	uint8_t seq; // 1 byte
	uint16_t checksum;


	uint8_t* payload; // payload length will be calculated from pckLen and pckSeq (min(pckLen - 118*pckSeq, 118))
	*/

	uint16_t crc_xmodem( const unsigned char *input_str, std::size_t num_bytes );

	enum LLVersion : uint8_t
	{
		Testing = 0,
		Void = 0xff
	};

	enum LLType : uint8_t
	{
		Confirmable = 0,
		Non_confirmable,
		Acknowledgement,
		PacketError,
		Reset,
		Ping,
		PingAck
	};

	enum class LLState : uint8_t
	{
		Nop,
		ReceivingHeader,
		ReceivingPayload,
		ErrorCheck,
		SendingPacket,
		WaitingAck,

		Transmitting
	};

	struct LLConn
	{
		LLType type;
		LLVersion version;
		uint16_t remoteAddr;
		uint8_t id;
		uint8_t seq;
	};

	struct LLHeader
	{
		LLVersion version; // 4 bits
		LLType type; // 4 bits
		uint16_t srcAddr; // 12 bits
		uint16_t dstAddr; // 12 bits
		uint16_t len; // 12 bits
		uint8_t id; // 4 bits
		uint8_t seq; // 1 byte

		void serialize(uint8_t* buf)
		{
			int i = 0;
			buf[i++] = (version << 4) | type;
			buf[i++] = srcAddr & 0xff;
			buf[i++] = (srcAddr >> 8) | ((dstAddr << 4) & 0xf0);
			buf[i++] = (dstAddr >> 4) & 0xff;
			buf[i++] = len & 0xff;
			buf[i++] = ((len >> 8) & 0x0f) | ((id << 4) & 0xf0);
			buf[i] = seq;
		}

		bool deserialize(uint8_t* buf)
		{
			version = static_cast<FwLogger::LLVersion>((buf[0] >> 4) & 0x0f);
			type = static_cast<FwLogger::LLType>(buf[0] & 0x0f);
			srcAddr = buf[1] | ((buf[2] << 8) & 0xf00);
			dstAddr = (buf[2] >> 4) | ((buf[3] << 4) & 0xff0); // deserialize twists addresses because a recv is assumed
			len = buf[4] | ((buf[5] << 8) & 0xf00);
			id = (buf[5] >> 4) & 0x0f;
			seq = buf[6];
			if(hdr_chksum() != buf[7]) return false;
			return true;
		}

		uint8_t hdr_chksum()
		{
			uint8_t cs = 0;
			uint8_t buf[7];
			serialize(buf);
			for(int i = 0; i < 7; ++i) cs ^= buf[i];
			return cs;
		}

		int chunkSize()
		{
			return len-118*seq < 118 ? len-118*seq: 118;
		}
	};

	struct LLPacket
	{
		LLPacket()
		{
			//version = LLVersion::Testing;
			//type = LLType::Ping;
			//srcAddr = 0;
			//dstAddr = 0;
			len = 0;
			//id = 0;
			payload = nullptr;
			seq = 0;
		}
		LLPacket(LLConn* connection, uint8_t* buf = nullptr, uint16_t c_len = 0)
		{
			conn = connection;
			len = c_len;
			payload = buf;
			seq = 0;
		}
		LLConn* conn;
		uint16_t len; // 12 bits
		uint8_t seq;

		uint8_t* payload; // payload length will be calculated from pckLen and pckSeq (min(pckLen - 118*pckSeq, 118))
		uint16_t checksum;

		/*void serialize_hdr(uint8_t* buf)
		{
			int i = 0;
			buf[i++] = (conn->version << 4) | conn->type;
			buf[i++] = conn->localAddr & 0xff;
			buf[i++] = (conn->localAddr >> 8) | ((conn->remoteAddr << 4) & 0xf0);
			buf[i++] = (conn->remoteAddr >> 4) & 0xff;
			buf[i++] = len & 0xff;
			buf[i++] = ((len >> 8) & 0x0f) | ((conn->id << 4) & 0xf0);
			buf[i] = seq;
		}

		void deserialize_hdr(uint8_t* buf)
		{
			conn->version = static_cast<FwLogger::LLVersion>((buf[0] >> 4) & 0x0f);
			conn->type = static_cast<FwLogger::LLType>(buf[0] & 0x0f);
			conn->remoteAddr = buf[1] | ((buf[2] << 8) & 0xf00);
			conn->localAddr = (buf[2] >> 4) | ((buf[3] << 4) & 0xff0); // deserialize twists addresses because a recv is assumed
			len = buf[4] | ((buf[5] << 8) & 0xf00);
			conn->id = (buf[5] >> 4) & 0x0f;
			seq = buf[6];
		}*/

		int chunkSize()
		{
			return len-118*seq < 118 ? len-118*seq: 118;
		}

		uint8_t hdr_chksum()
		{
			//TODO cambiar
			/*uint8_t cs = 0;
			uint8_t buf[7];
			serialize_hdr(buf);
			for(int i = 0; i < 7; ++i) cs ^= buf[i];
			return cs;*/
			return 0;
		}

		int serialize(uint8_t* buf, int max_len, int8_t new_seq = -1)
		{
			//TODO cambiar
			/*if(new_seq != -1)
				seq = new_seq;
			if(max_len < 8) return -1;

			serialize_hdr(buf);
			buf[7] = hdr_chksum(); // inefficient, but simple

			if(len == 0 || payload == nullptr) return 8;
			int packet_length = chunkSize()+10;
			if(max_len < packet_length) return -1;
			int i;
			for(i = 0; i < chunkSize(); ++i)
				buf[8+i] = payload[i+seq*118];
			uint16_t cs = crc_xmodem(payload+seq*118, chunkSize());
			i += 8;
			buf[i++] = cs & 0xff;
			buf[i] = (cs >> 8) & 0xff; // calculate checksum

			return packet_length;*/
			return 0;
		}
	};

	class LinkLayer
	{
		public:
			LinkLayer(Allocator<128>* alloc, uint16_t addr, void (*write)(uint8_t*, int), void (*cb)());

			uint16_t getAddr();
			int setAddr(uint16_t addr);

			int available();
			LLPacket* pop();

			void receive(uint8_t* buf, uint8_t length); // this will be called from serial or so
			int send(uint16_t dstAddr, LLType type, const uint8_t* buf, uint16_t len); // this will queue a packet

			bool loop();
		protected:

		private:
			Allocator<128>* _alloc;

			int8_t m_txPacket;
			circular_buffer<16, int8_t> m_txIdx; // this will hold where pending sending msgs are in circular buffer
			LLPacket m_txData[16]; //this will hold confirmable data until an ack is received

			circular_buffer<16> m_rxIds;// this will hold pending get msgs
			LLPacket m_rxData[16]; // hay que cambiarlo por un hashmap

			uint16_t m_addr;

			LLPacket m_pkt;

			//State m_state;
			int m_counter;

			void processRx();

			int getFreeTx();

			int m_lastTx;
			int m_lastRx;

			void send_pkt();
	};
}
#endif // LINKLAYER_H

