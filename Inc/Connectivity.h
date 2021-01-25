#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#define SOCKET_TABLE_SIZE 16
#define SOCKET_DESCRIPTOR_OFFSET 0x100

#include "mem_structs.hpp"
#include "KernelMQ.h"

#include "stm32hal_libs.h"
#include "LoRa.h"
#include "LinkLayer.h"
#include "Log.h"
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
			params.ptr = nullptr;
			state = 0;
			self_managed = true;
			data = nullptr;
			recv_bytes = 0;
			size = 0;
		}

		~Socket()
		{
			type = None;
			params.ptr = nullptr;
			state = 0;

			data = nullptr;
			recv_bytes = 0;
			size = 0;
		}

		SocketType getType() { return type; }

		SocketType type;
		union
		{
			void* ptr;
			int val;
		} params;

		int state;
		bool self_managed; // free on write

		//TODO eliminar
		uint8_t* data;
		int recv_bytes, size;
	};

	struct PortTask
	{
		enum class Op : uint8_t
		{
			None,
			Init,
			Write,
			PowerOff
		};
		Op op;

		Socket* sock;
		void* data;
		size_t len;

		PortTask()
		{
			sock = nullptr;
			data = nullptr;
			len = 0;
			op = Op::None;
		}

		PortTask(Op cop, Socket* csock, void* cdata, size_t clen)
		{
			op = cop;
			sock = csock;
			data = cdata;
			len = clen;
		}
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
		static bool isPort(void* ptr);
		static bool isSocket(void* ptr);

		static bool loop();
		void static setAllocator(Allocator<128>* alloc) { _alloc = alloc; }

	private:
		static Port* _ports[6];
		static size_t _currPorts;

		friend Port;

		static Allocator<128>* _alloc;
		static int _getEmptySock();
		static Socket _sockTable[SOCKET_TABLE_SIZE];
		static uint8_t _in_use[SOCKET_TABLE_SIZE];

		static Socket* _createSelfManagedSocket();
		static int _getSocketDescriptor(Socket* sock);

		static void addPort(Port* port);
	};

	// TODO (forcen#1#): Add WiFi support
	// TODO (forcen#1#12/03/20): Add read capabilities

	class Port
	{
	public:
		enum class Status : uint8_t
		{
			None,
			Init,
			Ready,
			Busy,
			Error,
			Off
		};

		Port() { PortManager::addPort(this); }
		virtual int open(Socket* sock, const char* scheme, const char* path) = 0;
		int write(Socket* sock, const void* buf, size_t count);
		virtual bool loop() { return false; }
		virtual void reset() {}
		virtual void powerOff() {}

		Status getPortStatus() { return m_portStatus; }

	protected:
		static Allocator<128>* getAllocator() { return PortManager::_alloc; }
		static Socket* _getSocketTable() { return PortManager::_sockTable; }
		static Socket* _createSelfManagedSocket() { return PortManager::_createSelfManagedSocket(); }
		static int _getSocketDescriptor(Socket* sock) { return PortManager::_getSocketDescriptor(sock); }

		circular_buffer<16, PortTask> m_tasks;

		Socket* m_sockNow;

		Status m_portStatus;
	};

	class PortCDC : public Port
	{
	public:
		PortCDC(const PortCDC& c) = delete;
		int open(Socket* sock, const char* scheme, const char* path);
		int write(Socket* sock, const void* buf, size_t count);
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
		int open(Socket* sock, const char* scheme, const char* path);
		int write(Socket* sock, const void* buf, size_t count);
		void push_rx(uint8_t buf);
		void setAddrPtr(uint16_t* addrPtr);
		bool loop();

		static PortUART& get();

		void reset();
		void powerOff();

		ParserStatus m_pStatus;
		int m_pIndex, m_pSize, m_pLastRecv;
		uint16_t* m_llAddr;
		uint8_t UART_txing;

		circular_buffer<64> m_rxbuf;
		fixed_string<8> m_rxHeader;

		uint8_t m_txbuf[2048];
		uint16_t m_txRead, m_txWrite;
		bool m_txFull;
		int _txPush(const uint8_t* c, uint16_t len);
		uint16_t _txGetTransferSize();
		uint16_t _txGetSize();
		void _txStart(); // TODO improve error handling
	private:
		PortUART();
	};

	// TODO (forcen#1#): Allow the driver to use IP Stack instead HTTP stack
	// TODO (forcen#1#12/03/20): Add configurable timeout things
	// TODO (forcen#1#12/03/20): Add before SAPBR query in order to get if modem is registered in to network
	// TODO (forcen#1#12/03/20): Fix LED blinking when low power mode is active
	// TODO (forcen#1#12/03/20): Add read function capabilities from socket (read from HTTP as GET from url)
	// TODO (forcen#1#12/03/20): Make HTTP write to read status code

	class PortGSM : public Port
	{
	public:
		int open(Socket* sock, const char* scheme, const char* path);
		void push_char(uint8_t c);
		bool loop();
		void eval(uint8_t* buf, int len);

		void TxCplt();

		static PortGSM& get();

		circular_buffer<128> m_rxbuf;
		uint8_t m_txbuf[1024];

		void reset();
		void powerOff();

		void setAPN(const char* apn);

		void setBaudRate(int baudrate);

		enum class GSMStatus : uint8_t
		{
			Init,
			Ready,
			Sending,
			NoSim,
			Error,
			Off
		};

		GSMStatus getStatus()
		{
			return m_status;
		}

		const char* getStatusStr()
		{
			switch(m_status)
			{
			case GSMStatus::Init:
				return "Init";
				break;
			case GSMStatus::Ready:
				return "Ready";
				break;
			case GSMStatus::Sending:
				return "Sending";
				break;
			case GSMStatus::NoSim:
				return "NoSim";
				break;
			case GSMStatus::Error:
				return "Error";
				break;
			case GSMStatus::Off:
				return "Off";
				break;
			}
			return "";
		}

	private:
		PortGSM();

		enum class GSMOp : uint8_t
		{
			Nop,

			InitModem,

			TestCommand,
			ReadCommand,
			WriteCommand,
			ExecCommand,
			RecvResponse,

			GetSignal,

			SMSSend,

			BearerOpen,
			BearerClose,

			HTTPData,
			HTTPAction,

			IPShut,

			DisableEcho,
			DisableModem,

			Delay,
			StartSend,
			RawSend,
			EndTask,
		};

		struct GSMWriteParams
		{
			const char* command;
			const char* paramValues;
			char* nonConstValues;
		};

		struct GSMState
		{
			GSMState()
			{
				op = GSMOp::Nop;
				params.ptr = nullptr;
				counter = 0;
				state = 0;
			}
			~GSMState()
			{
				if(op == GSMOp::WriteCommand)
				{
					GSMWriteParams* wp = reinterpret_cast<GSMWriteParams*>(params.ptr);
					if(wp)
					{
						if(wp->nonConstValues) getAllocator()->Deallocate(wp->nonConstValues);
						getAllocator()->Deallocate(wp);
					}
				}
			};
			GSMOp op;
			union {
				uint32_t delay;
				void* ptr;
				const char* command;
			} params;
			uint8_t state;
			int counter;
		};

		uint8_t* m_currMsg;
		uint32_t m_currSize;
		void _push_currMsg(uint8_t c);
		void _clear_currMsg();

		void _startTask();
		void _appendWrite();

		void step();
		void execute();

		void initHTTP();

		void setStatus(GSMStatus s, const char* func = nullptr, int line = 0);

		void _gsm_tx(uint8_t* data, uint16_t len);

		GSMStatus m_status;
		int8_t m_signal;
		int m_lastSignal;
		bool m_sapbr;
		int16_t m_pin;
		uint8_t m_ipAddr[4];
		char m_apn[32];
		int8_t m_initRetry;
		uint8_t m_logbuf[512];

		uint32_t delayStart;
		circular_buffer<16, GSMState> _statusList;
	};

	class PortRadio : public Port
	{
	public:
		PortRadio(LoRa* lora);
		int open(Socket* sock, const char* scheme, const char* path);
		int write(Socket* sock, const void* buf, size_t count);
		bool loop();

	private:
		LoRa* m_lora;
		uint16_t* m_llAddr;
	};
}

#endif // CONNECTIVITY_H
