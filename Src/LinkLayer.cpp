#include "LinkLayer.h"

namespace FwLogger
{
	//CRC courtesy from LibCRC

	static bool             crc_tabccitt_init          = false;
	static uint16_t         crc_tabccitt[256];
	#define	CRC_POLY_CCITT		0x1021
	#define CRC_START_XMODEM    0x0000
	#define CRC_START_16        0x0000

	static uint16_t crc_ccitt_generic( const unsigned char *input_str, size_t num_bytes, uint16_t start_value );
	static void init_crcccitt_tab( void );

	uint16_t crc_xmodem( const unsigned char *input_str, size_t num_bytes )
	{
		return crc_ccitt_generic( input_str, num_bytes, CRC_START_XMODEM );
	}

	static uint16_t crc_ccitt_generic( const unsigned char *input_str, size_t num_bytes, uint16_t start_value )
	{
		uint16_t crc;
		const unsigned char *ptr;
		size_t a;

		if (!crc_tabccitt_init) init_crcccitt_tab();

		crc = start_value;
		ptr = input_str;

		if ( ptr != NULL ) for (a=0; a<num_bytes; a++)
			crc = (crc << 8) ^ crc_tabccitt[ ((crc >> 8) ^ (uint16_t) *ptr++) & 0x00FF ];

		return crc;
	}

	uint16_t update_crc_ccitt( uint16_t crc, unsigned char c )
	{
		if (!crc_tabccitt_init) init_crcccitt_tab();

		return (crc << 8) ^ crc_tabccitt[ ((crc >> 8) ^ (uint16_t) c) & 0x00FF ];
	}

	static void init_crcccitt_tab( void )
	{
		uint16_t i;
		uint16_t j;
		uint16_t crc;
		uint16_t c;

		for (i=0; i<256; i++)
		{
			crc = 0;
			c   = i << 8;

			for (j=0; j<8; j++)
			{
				if ( (crc ^ c) & 0x8000 )
					crc = ( crc << 1 ) ^ CRC_POLY_CCITT;
				else
					crc =   crc << 1;

				c = c << 1;
			}
			crc_tabccitt[i] = crc;
		}
		crc_tabccitt_init = true;
	}

	LinkLayer::LinkLayer(Allocator<128>* alloc, uint16_t addr, void (*write)(uint8_t*, int), void (*cb)())
	{
		m_addr = addr;
		//m_state = State::Nop;
		m_counter = 0;
		_alloc = alloc;

		m_txPacket = -1;

		for(int i = 0; i < 16; ++i)
		{
			//m_txData[i].version = LLVersion::Void;
			//m_rxData[i].id = i;
		}
	}

	uint16_t LinkLayer::getAddr()
	{
		return m_addr;
	}

	int LinkLayer::setAddr(uint16_t addr)
	{
		if(addr == 0) return -1;

		m_addr = addr;
		return 0;
	}

	int LinkLayer::available()
	{
		return m_rxIds.size();
	}

	LLPacket* LinkLayer::pop()
	{
		uint8_t ptr;
		if(m_rxIds.pop_front(&ptr) == -1) return nullptr;
		return &m_rxData[ptr];
	}

	void LinkLayer::receive(uint8_t* buf, uint8_t length)
	{
		m_lastRx = HAL_GetTick();
		/*for(int i = 0; i < length; ++i)
		{
			if(m_state == State::Nop)
			{
				m_counter = 0;
				m_state = State::ReceivingHeader;
			}
			if(m_state == State::ReceivingHeader)
			{
				if(m_counter == 0)
				{
					m_pkt.version = static_cast<LLVersion>((buf[i] >> 4 ) & 0x0f);
					m_pkt.type = static_cast<LLType>(buf[i] & 0x0f);
				}
				else if(m_counter == 1) m_pkt.srcAddr = buf[i];
				else if(m_counter == 2)
				{
					m_pkt.srcAddr |= (buf[i] << 8) & 0xf00;
					m_pkt.dstAddr = (buf[i] >> 4) & 0x0f;
				}
				else if(m_counter == 3) m_pkt.dstAddr |= (buf[i] << 4) & 0xff0;
				else if(m_counter == 4) m_pkt.len = buf[i];
				else if(m_counter == 5)
				{
					m_pkt.len |= (buf[i] << 8) & 0xf00;
					m_pkt.id = (buf[i] >> 4) & 0x0f;
				}
				else if(m_counter == 6)
				{
					m_pkt.seq = buf[i];
				}
				else if(m_counter == 7)
				{
					if((m_pkt.hdr_chksum() != buf[i]) || (m_pkt.dstAddr != m_addr))
					//if((m_pkt.hdr_chksum() != buf[i]))
					{
						m_state = State::Nop;
						m_counter = 0;
						return;
					}
					m_counter = m_pkt.chunkSize();
					if(m_counter)
					{
						m_state = State::ReceivingPayload;
						if(m_pkt.seq == 0)
						{
							m_pkt.payload = reinterpret_cast<uint8_t*>(_alloc->Allocate(m_counter, reinterpret_cast<uintptr_t>(this)));
							m_rxData[m_pkt.id] = m_pkt;
						}
					}
					else
					{
						processRx();
						m_state = State::Nop;
					}
					continue; // avoid ++m_counter;
				}
				++m_counter;
			}
			else if(m_state == State::ReceivingPayload)
			{
				m_pkt.payload[m_pkt.chunkSize()-m_counter] = buf[i];
				--m_counter;
				if(m_counter == 0)
				{
					m_state = State::ErrorCheck;
					m_pkt.checksum = 0;
				}
			}
			else if(m_state == State::ErrorCheck)
			{
				m_pkt.checksum |= buf[i] << ((m_counter++) * 8);
				if(m_counter == 2)
				{
					m_pkt.payload = nullptr;
					m_rxIds.push_back(m_pkt.id);

					processRx();

					m_state = State::Nop; // fin de la recepción
				}
			}
		}*/
	}

	int LinkLayer::send(uint16_t dstAddr, LLType type, const uint8_t* buf, uint16_t len)
	{
		int idx = getFreeTx();
		if(idx == -1) return -1;

		LLPacket pkt;
		//pkt.version = LLVersion::Testing;
		//pkt.type = type;
		//pkt.srcAddr = m_addr;
		//pkt.dstAddr = dstAddr;
		pkt.len = len;
		//pkt.id = idx;
		pkt.seq = 0;
		pkt.payload = reinterpret_cast<uint8_t*>(_alloc->Allocate(len, reinterpret_cast<uintptr_t>(this)));

		for(int i = 0; i < len; ++i) pkt.payload[i] = buf[i];

		m_txIdx.push_back(idx);
		m_txData[idx] = pkt;

		return 0;
	}

	void LinkLayer::processRx()
	{
		/*if(m_pkt.type == LLType::Confirmable)
		{
			uint8_t buf[8];
			LLPacket pkt(LLType::Acknowledgement, m_addr, m_pkt.srcAddr, m_pkt.id);
			pkt.seq = m_pkt.seq;
			pkt.serialize(buf, 8);
			m_write(buf, 8);
			m_cb();
		}
		else if(m_pkt.type == LLType::Acknowledgement)
		{
			if(m_pkt.id == m_txData[m_txPacket].id)
			{
				_alloc->Deallocate(m_txData[m_txPacket].payload);
				m_txData[m_txPacket].payload = nullptr;
				m_txData[m_txPacket].version = LLVersion::Void;
				m_txPacket = -1;
			}
		}
		else if(m_pkt.type == LLType::Ping)
		{
			uint8_t buf[8];
			LLPacket pkt(LLType::PingAck, m_addr, m_pkt.srcAddr, m_pkt.id);
			pkt.serialize(buf, 8);
			m_write(buf, 8);
		}
		else if(m_pkt.type == LLType::PacketError)
		{

		}*/
	}

	int LinkLayer::getFreeTx()
	{
		bool found;
		for(int i = 0; i < 16; ++i)
		{
			std::size_t j; // add m_txPacket check
			if(i == m_txPacket) continue;
			found = false;
			for(j = 0; j < m_txIdx.size(); ++j)
			{
				if(i == *m_txIdx.at(j))
				{
					found = true;
					break;
				}
			}
			if(!found) return i;
		}
		return -1;
	}

	bool LinkLayer::loop()
	{
		/*if(m_state != State::Nop)
		{
			if(HAL_GetTick()-m_lastRx > 500)
			{
				m_state = State::Nop; // timeout
			}
			return true;
		}

		if(m_txPacket == -1)
		{
			int8_t p;
			if(m_txIdx.pop_front(&p) == -1) return false;
			m_txPacket = p;

			if(m_txData[m_txPacket].version != LLVersion::Void)
				send_pkt();

			return true;
		}
		else
		{
			if(HAL_GetTick()-m_lastTx > 2000)
			{
				send_pkt();
			}
		}
		return true;*/
	}

	void LinkLayer::send_pkt()
	{
		/*LLPacket& pkt = m_txData[m_txPacket];
		if(pkt.version == LLVersion::Void)
		{
			m_txPacket = -1;
			return;
		}
		if(pkt.type == LLType::Non_confirmable || pkt.type == LLType::Ping)
		{
			uint8_t pkt_buf[128];
			int packet_len = pkt.serialize(pkt_buf, 128);
			m_write(pkt_buf, packet_len);
			pkt.version = LLVersion::Void;
		}
		else if(pkt.type == LLType::Confirmable)
		{
			uint8_t pkt_buf[128];
			int packet_len = pkt.serialize(pkt_buf, 128);
			m_write(pkt_buf, packet_len);
		}
		m_lastTx = HAL_GetTick();*/
	}
}
