#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#define SOCKET_TABLE_SIZE 16
#define SOCKET_DESCRIPTOR_OFFSET 0x100

#include "mem_structs.hpp"
#include "KernelMQ.h"

#include "stm32f1xx_hal.h"
#include "LoRa.h"
#include "LinkLayer.h"
#include "printf.h"
#include <cctype>
#include <cstdlib>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

namespace FwLogger
{
	enum SocketType : uint8_t
	{
		SOCK_UART = 0,
		SOCK_UART_LL,
		SOCK_RADIO,
		SOCK_CDC,
		SOCK_CDC_LL,
		SOCK_GSM,
		SOCK_GSM_HTTP,
		SOCK_GSM_HTTPS,
		SOCK_GSM_FTP,
		SOCK_GSM_EMAIL,
		SOCK_GSM_SMS,
		None = 0xff
	};

	enum class ParserStatus : uint8_t
	{
		Start,
		AsciiCommand,
		LLHeader,
		LLPayload
	};

	struct Socket
	{
		Socket()
		{
			type = None;
			data = nullptr;
			params.ptr = nullptr;
			recv_bytes = 0;
			state = 0;
			self_managed = true;
		}

		~Socket()
		{
			type = None;
			data = nullptr;
			params.ptr = nullptr;
			recv_bytes = 0;
			state = 0;
		}

		SocketType getType() { return type; }
		uint8_t* getData() { return data; }
		size_t getDataSize() { return size; }

		SocketType type;
		uint8_t* data;
		union
		{
			void* ptr;
			int val;
		} params;

		size_t recv_bytes, size;
		int state;
		bool self_managed; // free on write
	};

	/** Class managing comm ports */
	class Port;

	class PortManager
	{
	public:
		static int open(const char* uri);
		static int write(int sock, const void* buf, size_t count);
		static int close(int sd);
		static Socket* getSocket(int sd);

		static bool loop();
		void static setAllocator(Allocator<128>* alloc) { _alloc = alloc; }

	private:
		static Port* _ports[6];
		int _currPorts;

		friend Port;

		static Allocator<128>* _alloc;
		static int _getEmptySock();
		static Socket _sockTable[SOCKET_TABLE_SIZE];
		static uint8_t _in_use[SOCKET_TABLE_SIZE];

		static Socket* _createSelfManagedSocket();
		static int _getSocketDescriptor(Socket* sock);

		void addPort(Port* port);
	};

	class Port
	{
	public:
		virtual int open_type(Socket* sock, const char* scheme, const char* path) = 0;
		virtual int write_type(Socket* sock, const void* buf, size_t count) = 0;
		virtual bool loop() { return false; }

	protected:
		static Allocator<128>* getAllocator() { return PortManager::_alloc; }
		static Socket* _getSocketTable() { return PortManager::_sockTable; }
		static Socket* _createSelfManagedSocket() { return PortManager::_createSelfManagedSocket(); }
		static int _getSocketDescriptor(Socket* sock) { return PortManager::_getSocketDescriptor(sock); }

		Socket* m_sockNow;
	};

	class PortCDC : public Port
	{
	public:
		PortCDC(const PortCDC& c) = delete;
		int open_type(Socket* sock, const char* scheme, const char* path);
		int write_type(Socket* sock, const void* buf, size_t count);
		void push_rx(uint8_t* buf, size_t count);
		void setAddrPtr(uint16_t* addrPtr);
		bool loop();

		static PortCDC& get();

	private:
		PortCDC();
		ParserStatus m_pStatus;
		int m_pIndex, m_pSize, m_pLastRecv;
		uint16_t* m_llAddr;

		fixed_string<256> m_rxbuf;
		circular_buffer<128> m_txbuf;
	};

	class PortUART : public Port
	{
	public:
		PortUART(const PortUART& c) = delete;
		int open_type(Socket* sock, const char* scheme, const char* path);
		int write_type(Socket* sock, const void* buf, size_t count);
		void push_rx(uint8_t* buf, size_t count);
		void push_tx(const uint8_t* buf, size_t count);
		void setAddrPtr(uint16_t* addrPtr);
		bool loop();

		static PortUART& get();

		ParserStatus m_pStatus;
		int m_pIndex, m_pSize, m_pLastRecv;
		uint16_t* m_llAddr;
		uint8_t UART_txing;

		fixed_string<64> m_rxbuf;
		circular_buffer<128> m_txbuf;
	private:
		PortUART();
	};

	class PortGSM : public Port
	{
	public:
		int open_type(Socket* sock, const char* scheme, const char* path);
		int write_type(Socket* sock, const void* buf, size_t count);
		void push_rx(uint8_t* buf, size_t count);
		bool loop();
		void eval(uint8_t* buf, int len);

		void TxCplt();

		static PortGSM& get();

		fixed_string<256> m_rxbuf;
		uint8_t m_txbuf[256];

	private:
		PortGSM();

		enum class GSMOp : uint8_t
		{
			TestCommand = 0,
			ReadCommand,
			WriteCommand,
			ExecCommand,
			SMSSend,
			HTTPData,
			HTTPAction,

			RecvResponse,
			RecvSMSEntry,
			DisableEcho,
			Delay,
			Nop = 0xff
		};

		struct GSMWriteParams
		{
			const char* command;
			const char* paramValues;
			char* nonConstValues;
		};

		struct GSMStatus
		{
			GSMOp op;
			union {
				uint32_t delay;
				void* ptr;
				const char* command;
			} params;
			uint8_t state;
			int counter;
		};

		void execute();
		void step();

		void initHTTP();

		uint32_t delayStart;
		circular_buffer<16, GSMStatus> _statusList;
	};

	class PortRadio : public Port
	{
	public:
		PortRadio(LoRa* lora);
		int open_type(Socket* sock, const char* scheme, const char* path);
		int write_type(Socket* sock, const void* buf, size_t count);
		void push_rx(uint8_t* buf, size_t count);
		bool loop();

	private:
		LoRa* m_lora;
		uint16_t* m_llAddr;
	};
}

#endif // CONNECTIVITY_H
