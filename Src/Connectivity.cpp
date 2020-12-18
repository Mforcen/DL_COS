#include "Connectivity.h"

/** Hardware communication handlers */

extern "C"
{
	uint8_t UART1_rx_char, UART3_rx_char;
	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
		if(huart == &huart1)//port serial recv
		{
			FwLogger::PortUART::get().push_rx(UART1_rx_char);
			HAL_UART_Receive_IT(huart, &UART1_rx_char, 1);
		}
		else if(huart == &huart3)
		{
			FwLogger::PortGSM::get().push_char(UART3_rx_char);
			HAL_UART_Receive_IT(huart, &UART3_rx_char, 1);
		}
	}

	void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
	{
		if(huart == &huart1)
		{
			FwLogger::PortUART::get().UART_txing = 0;
		}
		else if(huart == &huart3)
		{
			FwLogger::PortGSM::get().TxCplt();

			/*uint8_t ph;
			if(FwLogger::PortGSM::get().m_txbuf.pop_front(&ph) == 0)
				HAL_UART_Transmit_IT(huart, &ph, 1);
			else
				FwLogger::PortGSM::get().UART3_txing = 0;*/
		}
	}

	void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart)
	{
		if(huart == &huart3)
		{
			if(HAL_UART_GetError(huart) == HAL_UART_ERROR_PE)
			{
				FwLogger::Log::Error("huart3 pe\n");
			}
			else if(HAL_UART_GetError(huart) == HAL_UART_ERROR_NE)
			{
				FwLogger::Log::Error("huart3 ne\n");
			}
			else if(HAL_UART_GetError(huart) == HAL_UART_ERROR_FE)
			{
				FwLogger::Log::Error("huart3 fe\n");
			}
			else if(HAL_UART_GetError(huart) == HAL_UART_ERROR_ORE)
			{
				FwLogger::Log::Error("huart3 ore\n");
				__HAL_UART_CLEAR_OREFLAG(huart);
			}
			else if(HAL_UART_GetError(huart) == HAL_UART_ERROR_DMA)
			{
				FwLogger::Log::Error("huart3 dma\n");
			}

			FwLogger::PortGSM::get().m_rxbuf.clear();
			FwLogger::PortGSM::get().reset();
		}
	}
}

namespace FwLogger
{
	/**
	  *
	  * PortManager
	  *
	  */

	Allocator<128>* PortManager::_alloc = nullptr;
	Socket PortManager::_sockTable[SOCKET_TABLE_SIZE];
	uint8_t PortManager::_in_use[SOCKET_TABLE_SIZE] = {0};
	size_t PortManager::_currPorts = 0;
	Port* PortManager::_ports[6] = { nullptr };

	int PortManager::open(const char* uri)
	{
		char local_uri[256];
		strcpy(local_uri, uri);

		char* scheme = local_uri;
		char* path = strpbrk(local_uri, ":");
		*(path++) = 0;


		int sd = _getEmptySock();
		if(sd == -1) return -1;

		if(strcmp(scheme, "raw") == 0)
		{
			if(strcmp(path, "uart") == 0)
			{
				_sockTable[sd].type = SOCK_UART;
			}
			else if(strcmp(path, "cdc") == 0)
			{
				_sockTable[sd].type = SOCK_CDC;
			}
			else if(strcmp(path, "lora") == 0)
			{
				_sockTable[sd].type = SOCK_RADIO;
			}
			else if(strcmp(path, "gsm") == 0)
			{
				_sockTable[sd].type = SOCK_GSM;
			}
		}
		else if(strcmp(scheme, "sms") == 0)
		{
			PortGSM::get().open(&_sockTable[sd], scheme, path);
		}
		else if(strcmp(scheme, "http") == 0)
		{
			PortGSM::get().open(&_sockTable[sd], scheme, path);
		}
		else if(strcmp(scheme, "https") == 0)
		{
			PortGSM::get().open(&_sockTable[sd], scheme, path);
		}
		else
			return -1;

		_in_use[sd] = 1;
		_sockTable[sd].self_managed = false;

		return sd + SOCKET_DESCRIPTOR_OFFSET;
	}

	int PortManager::write(int sock_d, const void* buf, size_t count)
	{
		if(sock_d < SOCKET_DESCRIPTOR_OFFSET) return -2;

		sock_d -= SOCKET_DESCRIPTOR_OFFSET;

		if(sock_d < 0 || sock_d >= SOCKET_TABLE_SIZE) return -2;
		if(!_in_use[sock_d]) return -1;

		Socket& sock  = _sockTable[sock_d];

		switch(sock.type)
		{
		case SOCK_UART:
			return PortUART::get().write(nullptr, buf, count);
		case SOCK_CDC:
			return PortCDC::get().write(nullptr, buf, count);
		case SOCK_GSM:
			return PortGSM::get().write(nullptr, buf, count);
		case SOCK_RADIO:
			return 0;//PortRadio::get().write(nullptr, buf, count);
		case SOCK_GSM_SMS:
		case SOCK_GSM_HTTP:
		case SOCK_GSM_HTTPS:
			return PortGSM::get().write(&sock, buf, count);
		default:
			return -2;
		}

		return -1;
	}

	int PortManager::close(int sd) // revisar
	{
		sd -= SOCKET_DESCRIPTOR_OFFSET;
		if(!_in_use[sd]) return -1;

		_sockTable[sd].type = SocketType::None;

		if(_sockTable[sd].type == SOCK_GSM_HTTP || _sockTable[sd].type == SOCK_GSM_HTTPS)
			if(_sockTable[sd].params.ptr != nullptr) _alloc->Deallocate(_sockTable[sd].params.ptr);

		_sockTable[sd].~Socket();
		_in_use[sd] = 0;

		return 0;
	}

	Socket* PortManager::getSocket(int sd)
	{
		if(sd < SOCKET_DESCRIPTOR_OFFSET) return nullptr;
		return &_sockTable[sd-SOCKET_DESCRIPTOR_OFFSET];
	}

	bool PortManager::isPort(void* ptr)
	{
		for(unsigned int i = 0; i < _currPorts; ++i)
			if(&_ports[i] == ptr) return true;
		return false;
	}

	bool PortManager::isSocket(void* ptr)
	{
		for(int i = 0; i < SOCKET_TABLE_SIZE; ++i)
		{
			if(_in_use[i]){
				if(&_sockTable[i] == ptr) return true;
			}
		}
		return false;
	}

	bool PortManager::loop()
	{
		bool retval = false;
		for(size_t i = 0; i < _currPorts; ++i)
			retval |= _ports[i]->loop();
		return retval;
	}

	int PortManager::_getEmptySock()
	{
		for(std::size_t i = 0; i < 16; ++i)
		{
			if(_in_use[i] == 0) return i;
		}

		return -1;
	}

	Socket* PortManager::_createSelfManagedSocket()
	{
		int sd = _getEmptySock();
		if(sd == -1) return nullptr;

		_in_use[sd] = 1;
		_sockTable[sd].self_managed = true;
		return &_sockTable[sd];
	}

	int PortManager::_getSocketDescriptor(Socket* sock)
	{
		int sd = (sock-_sockTable);
		if(sd < 0 || sd >= 16) return -1;
		return sd + SOCKET_DESCRIPTOR_OFFSET;
	}

	void PortManager::addPort(Port* port)
	{
		_ports[_currPorts++] = port;
	}

	/**
	  *
	  * Port implementation
	  *
	  */

	int Port::write(Socket* sock, const void* buf, size_t count)
	{
		if(m_portStatus == Status::Error || m_portStatus == Status::None) return -2; // error

		PortTask p;
		p.op = PortTask::Op::Write;
		p.sock = sock;
		p.data = getAllocator()->Allocate(count, this);
		if(p.data == nullptr) return -2;
		memcpy(p.data, buf, count);
		p.len = count;

		if(m_tasks.push_back(p) == -1)
		{
			getAllocator()->Deallocate(p.data);
			return -2;
		}

		return count;
	}

	/**
	  *
	  * PortCDC driver
	  *
	  */

	PortCDC::PortCDC()
	{
		m_pStatus = ParserStatus::Start;
		m_pIndex = 0;
		m_pSize = 0;
		m_pLastRecv = HAL_GetTick();
	}

	int PortCDC::open(Socket* sock, const char* scheme, const char* path)
	{
		return 0;
	}

	int PortCDC::write(Socket* sock, const void* buf, size_t count)
	{
		/*if(sock == nullptr)
		{
			uint8_t* ui8_buf = (uint8_t*) buf;
			for(unsigned int i = 0; i < count; ++i)
			{
				if(m_txbuf.push_back(ui8_buf[i]) != 0)
				{
					//errno = EBADF;
					return -1;
				}
			}

			uint8_t ph;
			if(!UART_txing) // esto se podría ver como un flush
			{
				if(m_txbuf.pop_front(&ph) == 0)
				{
					HAL_UART_Transmit_IT(&huart1, &ph, 1);
					UART_txing = 1;
				}
			}
		}
		else
		{

		}*/
		return 0;
	}

	void PortCDC::push_rx(uint8_t* buf, size_t count)
	{
		/*m_pLastRecv = HAL_GetTick();
		for(size_t i = 0; i < count; ++i)
		{
			char c = buf[i];
			if(m_pStatus == ParserStatus::Start)
			{
				m_rxbuf.clear();

				if(std::isalnum(c))
				{
					//write_type(sockid, &c, 1);
					if(c == '\b' || c == 127) return;
					m_pStatus = ParserStatus::AsciiCommand;
					m_rxbuf.push_back(c);

					Socket sockNow;
					sockNow.type = SOCK_CDC;
					sockNow.data = reinterpret_cast<uintptr_t>(_alloc->Allocate(128, reinterpret_cast<uintptr_t>(this)));
					sockNow.recv_bytes = 1;
					reinterpret_cast<uint8_t*>(sockNow.data)[0] = c;
				}
				else
				{
					m_rxbuf.push_back(c);
					m_pStatus = ParserStatus::LLHeader;
				}
			}
			else if(m_pStatus == ParserStatus::AsciiCommand)
			{
				//write_type(sockid, &c, 1); //echo back
				if(c == '\b' || c == 127)
				{

					if(m_pSize > 0)
						--m_pSize;

					m_rxbuf.delete_back();
				}
				else if(c == '\n')
				{
					Task tsk;
					tsk.op = Operation::Eval;
					tsk.fd = 0; // mismo
					tsk.buf = reinterpret_cast<void*>(_alloc->Allocate(128, reinterpret_cast<uintptr_t>(this)));
					for(size_t i = 0; i < m_rxbuf.size(); ++i)
					{
						reinterpret_cast<uint8_t*>(tsk.buf)[i] = *m_rxbuf.at(i);
					}
					msgQueue.push_front(tsk);
					m_rxbuf.clear();
				}
				else
					m_rxbuf.push_back(c);
			}
			else if(m_pStatus == ParserStatus::LLHeader)
			{
				m_rxbuf.push_back(c);
				if(m_rxbuf.size() == 8)
				{
					LLHeader header;
					if(!header.deserialize(m_rxbuf.at(0)))
					{
						m_pStatus = ParserStatus::Start; //malformed packet
					}
					else
					{
						if(header.dstAddr != *m_llAddr) // message for others
						{
							m_pStatus = ParserStatus::Start;
							continue;
						}
						if(m_sockNow != nullptr) // active socket
						{
							if(m_sockNow->getType() == SOCK_CDC_LL)
							{
								LLConn* conn = reinterpret_cast<LLConn*>(m_sockNow->params);
								if(conn->remoteAddr != header.srcAddr) // misma dirección
								{
									m_sockNow = nullptr;
								}
							}
						}

						if(m_sockNow == nullptr)
						{
							for(Socket& sock : m_sockets) // checking if there is a socket for this conn
							{
								if(reinterpret_cast<LLConn*>(sock.params)->remoteAddr == *m_llAddr)
								{
									m_sockNow = &sock;
									break;
								}
							}
						}

						if(m_sockNow == nullptr) // no socket for this
						{
							Socket sockNow;
							sockNow.type = SOCK_CDC_LL;

							LLConn* conn = reinterpret_cast<LLConn*>(_alloc->Allocate(sizeof(LLConn), reinterpret_cast<uintptr_t>(this)));
							sockNow.params = reinterpret_cast<uintptr_t>(conn);

							conn->type = header.type;
							conn->version = header.version;
							conn->remoteAddr = header.srcAddr;
							conn->id = header.id;

							sockNow.data = reinterpret_cast<uintptr_t>(_alloc->Allocate(header.len, reinterpret_cast<uintptr_t>(this)));

							m_sockets.push_back(sockNow);

							m_sockNow = &(*m_sockets.end());
							m_sockNow->recv_bytes = header.seq*118; // configure offset
						}

						LLConn* conn = reinterpret_cast<LLConn*>(m_sockNow->params);
						conn->seq = header.seq;

						m_pStatus = ParserStatus::LLPayload;
					}
				}
				//lluart.receive(&c, 1); // this will exit the status automatically
			}

			else if(m_pStatus == ParserStatus::LLPayload)
			{
				uint8_t* data = reinterpret_cast<uint8_t*>(m_sockNow->data);
				data[m_sockNow->recv_bytes++] = c;
				if(m_sockNow->recv_bytes == m_sockNow->size)
				{
					Task tsk;
					tsk.op = Operation::BinEval;
					tsk.fd = reinterpret_cast<uintptr_t>(m_sockNow);
					tsk.buf = reinterpret_cast<void*>(m_sockNow->data);
					m_sockNow->data = reinterpret_cast<uintptr_t>(nullptr);
					msgQueue.push_back(tsk);
				}
			}
		}*/
	}

	void PortCDC::setAddrPtr(uint16_t* addrPtr)
	{
		m_llAddr = addrPtr;
	}

	bool PortCDC::loop()
	{
		if(HAL_GetTick()-m_pLastRecv > 5000)
		{
			m_pStatus = ParserStatus::Start;
			m_pLastRecv = HAL_GetTick();
			m_rxbuf.clear();
		}
		else
			return true;
		return false;
	}

	PortCDC& PortCDC::get()
	{
		static PortCDC S;
		return S;
	}

	/**
	  *
	  *  PortUART Driver
	  *
	  */

	PortUART::PortUART()
	{
		m_pIndex = 0;
		m_pStatus = ParserStatus::Start;
		m_pSize = 0;
		m_pLastRecv = HAL_GetTick();

		HAL_UART_Receive_IT(&huart1, &UART1_rx_char, 1);

		m_txFull = false;
		m_txRead = 0;
		m_txWrite = 0;

		m_portStatus = Status::Ready;
	}

	int PortUART::open(Socket* sock, const char* scheme, const char* path)
	{
		return 0;
	}

	int PortUART::write(Socket* sock, const void* buf, size_t count)
	{
		if(sock == nullptr)
		{
			_txPush(reinterpret_cast<const uint8_t*>(buf), count);
		}
		else
		{

		}
		return count;
	}

	void PortUART::push_rx(uint8_t c) //TODO reimplement to avoid ISR overrun
	{
		m_pLastRecv = HAL_GetTick();
		m_rxbuf.push_back(c);
	}

	void PortUART::setAddrPtr(uint16_t* addrPtr)
	{
		m_llAddr = addrPtr;
	}

	bool PortUART::loop()
	{
		uint8_t c = 0;
		while(m_rxbuf.pop_front(&c) != -1)
		{
			if(m_pStatus == ParserStatus::Start)
			{
				if(std::isalnum(c))
				{
					_txPush(reinterpret_cast<uint8_t*>(&c), 1);
					if(c == '\b' || c == 127) continue;
					m_pStatus = ParserStatus::AsciiCommand;

					m_sockNow = nullptr;

					for(size_t i = 0; i < SOCKET_TABLE_SIZE; ++i) // find SOCK_UART
					{
						if(_getSocketTable()[i].type == SOCK_UART)
						{
							m_sockNow = &_getSocketTable()[i];
							break;
						}
					}

					if(m_sockNow == nullptr) m_sockNow = _createSelfManagedSocket();
					if(m_sockNow == nullptr) continue;

					m_sockNow->type = SOCK_UART;
					m_sockNow->data = reinterpret_cast<uint8_t*>(getAllocator()->Allocate(128, this));
					m_sockNow->recv_bytes = 1;
					reinterpret_cast<uint8_t*>(m_sockNow->data)[0] = c;

				}
				else
				{
					m_rxHeader.clear();
					m_rxHeader.push_back(c);
					m_pStatus = ParserStatus::LLHeader;
				}
			}
			else if(m_pStatus == ParserStatus::AsciiCommand)
			{
				_txPush(reinterpret_cast<uint8_t*>(&c), 1);
				if(c == '\b' || c == 127)
				{

					if(m_pSize > 0)
						--m_pSize;

					reinterpret_cast<uint8_t*>(m_sockNow->data)[--m_sockNow->recv_bytes] = 0;
				}
				else if(c == '\n')
				{
					reinterpret_cast<uint8_t*>(m_sockNow->data)[m_sockNow->recv_bytes] = 0;

					Task tsk;
					tsk.op = Operation::Eval;
					tsk.fd = _getSocketDescriptor(m_sockNow);
					tsk.buf = reinterpret_cast<void*>(m_sockNow->data);
					tsk.counter = m_sockNow->recv_bytes;
					m_sockNow->data = reinterpret_cast<uintptr_t>(nullptr);

					m_pStatus = ParserStatus::Start;
					m_sockNow = nullptr;

					msgQueue.push_front(tsk);
				}
				else
				{
					reinterpret_cast<uint8_t*>(m_sockNow->data)[m_sockNow->recv_bytes++] = c;
				}
			}
			else if(m_pStatus == ParserStatus::LLHeader)
			{
				m_rxHeader.push_back(c);
				if(m_rxHeader.size() == 8)
				{
					LLHeader header;
					if(!header.deserialize(m_rxHeader.buf))
					{
						m_pStatus = ParserStatus::Start; //malformed packet
					}
					else
					{
						if(header.dstAddr != *m_llAddr) // message for others
						{
							m_pStatus = ParserStatus::Start;
							continue;
						}
						if(m_sockNow != nullptr) // active socket
						{
							if(m_sockNow->getType() == SOCK_CDC_LL)
							{
								LLConn* conn = reinterpret_cast<LLConn*>(m_sockNow->params.ptr);
								if(conn->remoteAddr != header.srcAddr) // misma dirección
								{
									m_sockNow = nullptr;
								}
							}
						}

						if(m_sockNow == nullptr) //inactive
						{
							/*for(size_t i = 0; i < 16; ++i) // checking if there is a socket for this conn
							{
								if(_sockTable[i].type == SocketType::SOCK_CDC_LL)
								if(reinterpret_cast<LLConn*>(_sockTable[i].params)->remoteAddr == *m_llAddr)
								{
									m_sockNow = i;
									break;
								}
							}*/
						}

						if(m_sockNow == nullptr) // no socket for this
						{

							m_sockNow = _createSelfManagedSocket();
							if(m_sockNow == nullptr) continue;

							new(m_sockNow) Socket();
							m_sockNow->type = SOCK_CDC_LL;

							LLConn* conn = reinterpret_cast<LLConn*>(getAllocator()->Allocate(sizeof(LLConn), this));
							m_sockNow->params.ptr = conn;

							conn->type = header.type;
							conn->version = header.version;
							conn->remoteAddr = header.srcAddr;
							conn->id = header.id;

							m_sockNow->data = reinterpret_cast<uint8_t*>(getAllocator()->Allocate(header.len, this));
							m_sockNow->recv_bytes = header.seq*118; // configure offset
						}

						LLConn* conn = reinterpret_cast<LLConn*>(m_sockNow->params.ptr);
						conn->seq = header.seq;

						m_pStatus = ParserStatus::LLPayload;
					}
				}
			}

			else if(m_pStatus == ParserStatus::LLPayload)
			{
				uint8_t* data = reinterpret_cast<uint8_t*>(m_sockNow->data);
				data[m_sockNow->recv_bytes++] = c;

				if(m_sockNow->recv_bytes == m_sockNow->size)
				{
					Task tsk;
					tsk.op = Operation::BinEval;
					tsk.fd = reinterpret_cast<uintptr_t>(m_sockNow);
					tsk.buf = reinterpret_cast<void*>(m_sockNow->data);
					m_sockNow->data = reinterpret_cast<uintptr_t>(nullptr);
					msgQueue.push_back(tsk);
				}
			}
		}

		if(HAL_GetTick()-m_pLastRecv > 5000)
		{
			m_pStatus = ParserStatus::Start;
			m_pLastRecv = HAL_GetTick();
			m_rxbuf.clear();
			m_rxHeader.clear();
		}

		if(UART_txing) return true;
		else
		{
			_txStart();
		}
		return false;
	}

	PortUART& PortUART::get()
	{
		static PortUART S;
		return S;
	}

	void PortUART::reset()
	{
		HAL_UART_Receive_IT(&huart1, &UART1_rx_char, 1);
	}

	void PortUART::powerOff()
	{
		HAL_UART_AbortReceive_IT(&huart1);
	}

	int PortUART::_txPush(const uint8_t* buf, uint16_t len)
	{
		if(1024-_txGetSize() < len) return -1;
		if(m_txWrite+len <= 1024)
		{
			memcpy(&m_txbuf[m_txWrite], buf, len);
			m_txWrite += len;
			m_txWrite %= 1024; // if m_txWrite == 1024 -> m_txWrite = 0
		}
		else
		{
			int l1n = 1024-m_txWrite;
			memcpy(&m_txbuf[m_txWrite], buf, l1n);
			m_txWrite = len-l1n;
			memcpy(m_txbuf, &buf[l1n], m_txWrite);
		}
		return len;
	}

	uint16_t PortUART::_txGetTransferSize()
	{
		if(m_txWrite == m_txRead)
		{
			if(m_txFull) return 1024-m_txRead;
			else return 0;
		}
		else if(m_txWrite > m_txRead)
		{
			return m_txWrite-m_txRead;
		}
		else
		{
			return 1024-m_txRead;
		}
	}

	uint16_t PortUART::_txGetSize()
	{
		if(m_txFull) return 1024;
		return (m_txWrite+1024-m_txRead)%1024;
	}

	void PortUART::_txStart()
	{
		int txSize = _txGetTransferSize();
		if(txSize > 0)
		{
			UART_txing = true;
			HAL_UART_Transmit_DMA(&huart1, &m_txbuf[m_txRead], txSize);
			m_txRead += txSize;
			m_txRead %= 1024;
		}
	}

	/**
	  *
	  * PortGSM Driver
	  *
	  */

	namespace GSMCommands
	{
		const char* AT = "AT";
		const char* ATE = "ATE";
		const char* CERR = "AT+CEER";
		const char* CSQ = "AT+CSQ";
		const char* ATI = "ATI";
		const char* CMEE = "AT+CMEE";
		const char* CREG = "AT+CREG";
		const char* CPIN = "AT+CPIN";
		const char* COPS = "AT+COPS";
		const char* CBC = "AT+CBC";
		const char* CFUN = "AT+CFUN";
		const char* CSCLK = "AT+CSCLK";
		const char* CSGS = "AT+CSGS";

		/** SMS */
		const char* SMS_CMGF = "AT+CMGF";
		const char* SMS_CSCS = "AT+CSCS";
		const char* SMS_CSCA = "AT+CSCA";
		const char* SMS_CMGS = "AT+CMGS";

		/** IP */
		const char* IP_CIPSHUT = "AT+CIPSHUT";
		const char* IP_BEARER = "AT+SAPBR";

		/** HTTP */
		const char* HTTP_INIT = "AT+HTTPINIT";
		const char* HTTP_PARA = "AT+HTTPPARA";
		const char* HTTP_DATA = "AT+HTTPDATA";
		const char* HTTP_READ = "AT+HTTPREAD";
		const char* HTTP_ACTION = "AT+HTTPACTION";
		const char* HTTP_TERM = "AT+HTTPTERM";
	}

	PortGSM::PortGSM()
	{
		HAL_UART_Receive_IT(&huart3, &UART3_rx_char, 1);
		m_rxbuf.clear();

		GPIO_InitTypeDef ginit;
		ginit.Mode = GPIO_MODE_OUTPUT_PP;
		ginit.Pin = GPIO_PIN_10;
		ginit.Speed = GPIO_SPEED_FREQ_LOW;
		ginit.Pull = GPIO_NOPULL;

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_Init(GPIOB, &ginit);

		m_apn[0] = 'T';
		m_apn[1] = 'M';
		m_apn[2] = '\0';

		m_sapbr = false;
		m_pin = 0;
		m_lastSignal = HAL_GetTick();
		m_status = GSMStatus::Init;
		m_portStatus = Status::Init;
		_clear_currMsg();

		reset();
	}

	int PortGSM::open(Socket* sock, const char* scheme, const char* path)
	{
		if(strcmp(scheme, "sms") == 0)
		{
			sock->type = SOCK_GSM_SMS;
			sock->params.val = atoi(path);
		}
		else if(strcmp(scheme, "http") == 0)
		{
			sock->type = SOCK_GSM_HTTP;

			uint8_t* url = reinterpret_cast<uint8_t*>(getAllocator()->Allocate(strlen(path), this));
			strcpy(reinterpret_cast<char*>(url), path);
			sock->params.ptr = url;
		}
		else if(strcmp(scheme, "https") == 0)
		{
			sock->type = SOCK_GSM_HTTPS;
			uint8_t* url = reinterpret_cast<uint8_t*>(getAllocator()->Allocate(strlen(path), this));
			strcpy(reinterpret_cast<char*>(url), path);
			sock->params.ptr = url;
		}
		else return -1;

		return 0;
	}

	void PortGSM::push_char(uint8_t c)
	{
		if(m_rxbuf.push_back(c) != 0) setStatus(GSMStatus::Error, __func__, __LINE__);
	}

	bool PortGSM::loop()
	{
		if(m_status == GSMStatus::Error) return false;

		uint8_t c = 0;
		while(m_rxbuf.pop_front(&c) != -1)
		{
			if(c == '\r')
			{
				if(m_currSize == 0) continue;
				_push_currMsg(0);

				Task tsk;
				tsk.op = Operation::ModemEval;
				tsk.counter = m_currSize;
				tsk.buf = m_currMsg;
				m_currMsg = nullptr;

				Socket* sock = _createSelfManagedSocket();
				sock->type = SOCK_GSM;

				tsk.fd = _getSocketDescriptor(sock);

				msgQueue.push_back(tsk);
				_clear_currMsg();

			}
			else if(c == '\n')
			{

			}
			else if(c == ' ')
			{
				GSMState* st = _statusList.at_front();
				if(st != nullptr)
				{
					if(st->op == GSMOp::SMSSend)
					{
						if(st->state == 1)
						{
							if(m_currMsg[m_currSize] == '>')
							{
								st->state = 2;
								_clear_currMsg();
								_gsm_tx(reinterpret_cast<uint8_t*>(st->params.ptr), strlen(reinterpret_cast<char*>(st->params.ptr)));
								continue;
							}
						}
						else if(st->state == 2)
						{
							if(m_currMsg[m_currSize] == '>')
							{
								getAllocator()->Deallocate(st->params.ptr);
								m_txbuf[0] = '\r';
								m_txbuf[1] = '\n';
								m_txbuf[2] = '\x1a';
								_clear_currMsg();
								_gsm_tx(m_txbuf, 3);
								continue;
							}
						}
					}
				}
				_push_currMsg(c); //should be reached when >
			}
			else
				_push_currMsg(c);
		}

		GSMState* stat =_statusList.at_front();

		if(stat == nullptr) // not pending ops
		{
			if(m_tasks.size() > 0) _startTask();
			else return false; // nothing to do
			/*if(m_status == GSMStatus::Ready)
			{
				if(HAL_GetTick()-m_lastSignal > 10000)
				{
					m_lastSignal = HAL_GetTick();

					GSMState st;
					st.op = GSMOp::GetSignal;
					//_statusList.push_back(st);
					return true;
				}
			}*/
		}

		else if(stat->op == GSMOp::Delay)
		{
			if(HAL_GetTick()-delayStart >= stat->params.delay)
				step();
		}
		else if(stat->op == GSMOp::InitModem)
		{
			if(stat->state == 0)
			{
				if(HAL_GetTick()-delayStart >= 200)
				{
					delayStart = HAL_GetTick();
					stat->state = 1;
				}
			}
			else if(stat->state == 1)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
				if(HAL_GetTick()-delayStart >= 3000)
				{
					HAL_UART_Receive_IT(&huart3, &UART3_rx_char, 1);

					delayStart = HAL_GetTick();
					stat->state = 2;

					strcpy(reinterpret_cast<char*>(m_txbuf), "AT\r");
					_gsm_tx(m_txbuf, strlen("AT\r"));
				}
			}
			else if(stat->state == 2)
			{
				if(HAL_GetTick()-delayStart > 1500) // debería haber recibido un OK
				{
					if(m_initRetry++ > 5)
					{
						setStatus(GSMStatus::Error, __func__, __LINE__);
					}
					else
					{
						HAL_UART_AbortReceive_IT(&huart3);
						delayStart = HAL_GetTick();
						stat->state = 0;
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
					}
				}
			}
			else if(stat->state == 3)
			{
				if(HAL_GetTick()-delayStart >= 15000)
				{
					setStatus(GSMStatus::Ready, __func__, __LINE__);
					step();
				}
			}
		}
		else if(stat->op == GSMOp::DisableModem)
		{
			if(HAL_GetTick()-delayStart > 10000)
			{
				step();
			}
		}
		else if(stat->op == GSMOp::RecvResponse)
		{
			if(HAL_GetTick()-stat->params.delay > 5000)
				step();
		}
		else
		{
			if(HAL_GetTick()-delayStart > 10000)
				execute();
		}
		return true;
	}

	int parse_num(int* dst, char* str)
	{
		int i = 0;
        int val = 0;
        while(str[i] >= '0' && str[i] <= '9')
        {
                val *= 10;
                val += str[i++] - '0';
        }
        *dst = val;
        return i;
	}

	void ipdecode(char* str, uint8_t* ip)
	{
		int advances = 0;
        for(int i = 0; i < 4; ++i)
        {
                int num;
                advances += parse_num(&num, str+advances);
                advances += 1;
                ip[i] = num;
        }
	}

	void PortGSM::eval(uint8_t* buf, int len)
	{
		GSMState* stat = _statusList.at_front();
		if(stat == nullptr) return;

		buf[len] = 0; // suboptimal

		if(len >= 5) // reset modem if smt returns error
		{
			if(strncmp(reinterpret_cast<char*>(buf), "ERROR", 5) == 0)
			{
				Log::Error("[Modem(%s)]: error, state: %d\n", getStatusStr(), stat->state);
				reset();
			}
		}

		if(stat->op == GSMOp::InitModem)
		{
			if(stat->state == 2)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0 || strncmp(reinterpret_cast<char*>(buf), "RDY",3) == 0)
				{
					delayStart = HAL_GetTick();
					stat->state = 3;
				}
			}
			else if(stat->state == 3)
			{
				if(strncmp(reinterpret_cast<char*>(buf), "*PSUTTZ", 7) == 0)
				{
					setStatus(GSMStatus::Ready, __func__, __LINE__);
					step();
				}
			}
		}

		else if(stat->op == GSMOp::RecvResponse)//todo repensar
		{
			if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0) step();
		}

		else if(stat->op == GSMOp::GetSignal)
		{
			if(strncmp(reinterpret_cast<char*>(buf), "+CSQ", 4) == 0)
			{

			}
			else if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
			{
				step();
			}
		}

		else if(stat->op == GSMOp::SMSSend)
		{
			if(stat->state == 2)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
					step();
			}
		}

		else if(stat->op == GSMOp::BearerOpen)
		{
			if(stat->state == 0)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
				{
					stat->state = 1;
					int len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=3,1,\"APN\",\"%s\"\r", GSMCommands::IP_BEARER, m_apn);
					_gsm_tx(m_txbuf, len);
				}
			}
			else if(stat->state == 1)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
				{
					stat->state = 2;
					int len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=1,1\r", GSMCommands::IP_BEARER);
					_gsm_tx(m_txbuf, len);
				}
			}
			else if(stat->state == 2)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
				{
					stat->state = 3;
					int len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=2,1\r", GSMCommands::IP_BEARER);
					_gsm_tx(m_txbuf, len);
				}
			}
			else if(stat->state == 3)
			{
				if(strncmp(reinterpret_cast<char*>(buf), GSMCommands::IP_BEARER+2, 6) == 0)
				{
					ipdecode(reinterpret_cast<char*>(buf+11), m_ipAddr);
				}
				else if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
				{
					step();
				}
			}
		}
		else if(stat->op == GSMOp::BearerClose)
		{
			if(stat->state == 0)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "SHUT OK") == 0)
				{
					stat->state = 1;
					int len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=0,1\r", GSMCommands::IP_BEARER);
					_gsm_tx(m_txbuf, len);
				}
			}
			else if(stat->state == 1)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
					step();
			}
		}

		else if(stat->op == GSMOp::HTTPData)
		{
			if(stat->state == 0)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "DOWNLOAD") == 0)
				{
					HAL_UART_Transmit_IT(&huart3, reinterpret_cast<uint8_t*>(stat->params.ptr), stat->counter);
					stat->state = 1;
				}
			}
			else if(stat->state == 1)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
					step();
			}
		}
		else if(stat->op == GSMOp::HTTPAction)
		{
			if(stat->state == 0)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
					stat->state = 1;
			}
			else if(stat->state == 1)
				step();
		}


		else if(stat->op == GSMOp::IPShut)
		{
			if(stat->state == 0)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "SHUT OK") == 0)
					step();

				else
					execute();
			}
		}

		else if(stat->op == GSMOp::DisableEcho)
		{
			if(stat->state == 0)
			{
				if(buf[0] == 0) // skips 0 char
					buf = buf + 1;

				if(strcmp(reinterpret_cast<char*>(buf), "AT") == 0)
					stat->state = 1;

				else if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
					step();
			}
			else if(stat->state == 1)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
				{
					int len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s0&W\r", GSMCommands::ATE);
					_gsm_tx(m_txbuf, len);
					stat->state = 2;
				}
			}
			else if(stat->state == 2)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
					step();
			}
		}
		else if(stat->op == GSMOp::DisableModem)
		{
			delayStart = HAL_GetTick();
			if(stat->state == 0)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
				{
					int len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=0\r", GSMCommands::CFUN);
					_gsm_tx(m_txbuf, len);
					stat->state = 1;
				}
			}
			else if(stat->state == 1)
			{
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0 || strncmp(reinterpret_cast<char*>(buf), "+CPIN", 5) == 0)
				{
					stat->state = 2;
					//HAL_UART_AbortReceive_IT(&huart3);
					//step();

					int len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=1\r", GSMCommands::CSCLK);
					_gsm_tx(m_txbuf, len);

					//int len = sprintf(reinterpret_cast<char*>(m_txbuf), "AT+CPOWD=1\r");
					//HAL_UART_Transmit_IT(&huart3, m_txbuf, len);
				}
			}
			else if(stat->state == 2)
			{
				//if(strcmp(reinterpret_cast<char*>(buf), "NORMAL POWER DOWN") == 0)
				if(strcmp(reinterpret_cast<char*>(buf), "OK") == 0)
				{
					HAL_UART_AbortReceive_IT(&huart3);
					step();
				}
			}
		}
	}

	void PortGSM::TxCplt()
	{
		GSMState* stat = _statusList.at_front();
		if(stat == nullptr) return;

		if(stat->op == GSMOp::TestCommand || stat->op == GSMOp::ReadCommand || stat->op == GSMOp::ExecCommand)
		{
			stat->op = GSMOp::RecvResponse;
			stat->params.delay = HAL_GetTick();
		}

		else if(stat->op == GSMOp::WriteCommand)
		{
			GSMWriteParams* wp = reinterpret_cast<GSMWriteParams*>(stat->params.ptr);
			if(wp)
			{
				if(wp->nonConstValues) getAllocator()->Deallocate(wp->nonConstValues);
				getAllocator()->Deallocate(wp);
			}
			stat->op = GSMOp::RecvResponse;
		}

		else if(stat->op == GSMOp::SMSSend)
		{
			if(stat->state == 0)
				stat->state = 1; // waiting > in push rx
		}

		else if(stat->op == GSMOp::HTTPData)
		{
			if(stat->state == 1)
			{
				getAllocator()->Deallocate(stat->params.ptr);
				stat->params.ptr = nullptr;
			}
		}

		else if(stat->op == GSMOp::RawSend)
		{
			step();
		}
	}

	PortGSM& PortGSM::get()
	{
		static PortGSM S;
		return S;
	}

	void PortGSM::reset()
	{
		_statusList.clear(); //think how to safely clean memory
		m_initRetry = 0;

		GSMState st;
		st.op = GSMOp::InitModem;
		_statusList.push_back(st);

		execute();
	}

	void PortGSM::powerOff()
	{
		PortTask t;
		t.op = PortTask::Op::PowerOff;
		m_tasks.push_back(t);
	}

	void PortGSM::setAPN(const char* apn)
	{
		strcpy(m_apn, apn);
	}

	void PortGSM::_push_currMsg(uint8_t c)
	{
		if(m_currMsg == nullptr)
		{
			m_currMsg = reinterpret_cast<uint8_t*>(getAllocator()->Allocate(256, this));
			m_currSize = 0;
		}
		m_currMsg[m_currSize++] = c;
	}

	void PortGSM::_clear_currMsg()
	{
		if(m_currMsg != nullptr) getAllocator()->Deallocate(m_currMsg);
		m_currSize = 0;
	}

	void PortGSM::_startTask()
	{
		PortTask* t = m_tasks.at_front();
		if(t == nullptr) return;

		if(t->op == PortTask::Op::Init)
		{
			Log::Verbose("[Modem] Task: Init\n");

			if(m_status == GSMStatus::Init) return;
			_statusList.clear();
			reset();
		}
		else if(t->op == PortTask::Op::PowerOff)
		{
			if(m_status == GSMStatus::Off) return;
			_statusList.push_back(GSMState());
			_statusList.at_back()->op = GSMOp::DisableModem;

			_statusList.push_back(GSMState());
			_statusList.at_back()->op = GSMOp::EndTask;

			Log::Verbose("[Modem] Task: Power Off\n");

			if(m_status == GSMStatus::Ready) execute();
		}
		else if(t->op == PortTask::Op::Write)
		{
			if(m_status == GSMStatus::Off)
				reset();

			if(m_status == GSMStatus::Init)
				_appendWrite();
			else if(m_status == GSMStatus::Ready)
			{
				Log::Verbose("[Modem] Task: Write\n");

				_appendWrite();
				execute();
			}
		}
	}

	void PortGSM::_appendWrite()
	{
		PortTask* t = m_tasks.at_front();

		if(t->sock == nullptr)
		{
			_statusList.push_back(GSMState());
			_statusList.at_back()->op = GSMOp::StartSend;

			_statusList.push_back(GSMState());
			_statusList.at_back()->op = GSMOp::RawSend;
		}
		else
		{
			if(t->sock->type == SOCK_GSM_SMS)
			{
				GSMState stat;
				stat.op = GSMOp::StartSend;
				_statusList.push_back(stat);

				stat.op = GSMOp::WriteCommand;
				GSMWriteParams* write_params = reinterpret_cast<GSMWriteParams*>(getAllocator()->Allocate(sizeof(GSMWriteParams), this));
				stat.params.ptr = write_params;
				write_params->command = GSMCommands::SMS_CMGF;
				write_params->paramValues = "1";
				write_params->nonConstValues = nullptr;

				_statusList.push_back(stat);

				write_params = reinterpret_cast<GSMWriteParams*>(getAllocator()->Allocate(sizeof(GSMWriteParams), this));
				stat.params.ptr = write_params;
				write_params->command = GSMCommands::SMS_CSCS;
				write_params->paramValues = "\"GSM\"";
				write_params->nonConstValues = nullptr;

				_statusList.push_back(stat);

				stat.op = GSMOp::SMSSend;
				stat.state = 0;

				char* msg = reinterpret_cast<char*>(getAllocator()->Allocate(t->len+3, this));
				stat.params.ptr = msg;

				stat.counter = t->sock->params.val; // numero de telefono

				memcpy(msg, t->data, t->len);
				msg[t->len] ='\r';
				msg[t->len+1] = '\n';
				msg[t->len+2] = 0;

				_statusList.push_back(stat);

				stat.op = GSMOp::EndTask;
				_statusList.push_back(stat);
			}
			else if(t->sock->type == SOCK_GSM_HTTP || t->sock->type == SOCK_GSM_HTTPS)
			{
				_statusList.push_back(GSMState());
				_statusList.at_back()->op = GSMOp::StartSend;

				if(!m_sapbr)
					initHTTP();

				GSMState* stat;

				_statusList.push_back(GSMState());
				stat = _statusList.at_back();
				stat->op = GSMOp::WriteCommand;
				stat->params.ptr = getAllocator()->Allocate(sizeof(GSMWriteParams), this);
				reinterpret_cast<GSMWriteParams*>(stat->params.ptr)->command = GSMCommands::HTTP_PARA;
				reinterpret_cast<GSMWriteParams*>(stat->params.ptr)->paramValues = "\"CID\",1";
				reinterpret_cast<GSMWriteParams*>(stat->params.ptr)->nonConstValues = nullptr;

				_statusList.push_back(GSMState());
				stat = _statusList.at_back();
				stat->op = GSMOp::WriteCommand;
				stat->params.ptr = getAllocator()->Allocate(sizeof(GSMWriteParams), this);
				reinterpret_cast<GSMWriteParams*>(stat->params.ptr)->command = GSMCommands::HTTP_PARA;
				reinterpret_cast<GSMWriteParams*>(stat->params.ptr)->nonConstValues = reinterpret_cast<char*>(getAllocator()->Allocate(6+strlen(reinterpret_cast<char*>(t->sock->params.ptr)), this));
				sprintf(reinterpret_cast<GSMWriteParams*>(stat->params.ptr)->nonConstValues, "\"URL\",\"%s\"", reinterpret_cast<char*>(t->sock->params.ptr));

				if(t->sock->type == SOCK_GSM_HTTPS)
				{
					_statusList.push_back(GSMState());
					_statusList.at_back()->op = GSMOp::ExecCommand;
					_statusList.at_back()->params.command = "AT+HTTPSSL=1\r";
				}

				_statusList.push_back(GSMState());
				_statusList.at_back()->op = GSMOp::HTTPData;
				_statusList.at_back()->params.ptr = reinterpret_cast<char*>(getAllocator()->Allocate(t->len, this));
				_statusList.at_back()->counter = t->len;
				memcpy(_statusList.at_back()->params.ptr, t->data, t->len);

				_statusList.push_back(GSMState());
				_statusList.at_back()->op = GSMOp::HTTPAction;
				_statusList.at_back()->counter = 1; // post action

				_statusList.push_back(GSMState());
				_statusList.at_back()->op = GSMOp::ExecCommand;
				_statusList.at_back()->params.command = GSMCommands::HTTP_READ;


				_statusList.push_back(GSMState());
				_statusList.at_back()->op = GSMOp::ExecCommand;
				_statusList.at_back()->params.command = GSMCommands::HTTP_TERM;

				if(!m_sapbr)
				{
					_statusList.push_back(GSMState());
					_statusList.at_back()->op = GSMOp::BearerClose;
					m_sapbr = true;
				}

			}
		}
		_statusList.push_back(GSMState());
		_statusList.at_back()->op = GSMOp::EndTask;
	}

	void PortGSM::step()
	{
		_statusList.delete_front();
		execute();
	}

	void PortGSM::execute()
	{
		int len = 0;
		GSMState* stat = _statusList.at_front();
		delayStart = HAL_GetTick();

		if(stat == nullptr)
		{
			if(m_status == GSMStatus::Sending)
				setStatus(GSMStatus::Ready, __func__, __LINE__);
			return;
		}

		if(stat->op == GSMOp::InitModem)
		{
			HAL_UART_AbortReceive_IT(&huart3); // if only
			stat->state = 0;
			setStatus(GSMStatus::Init, __func__, __LINE__);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
			delayStart = HAL_GetTick();
		}

		else if(stat->op == GSMOp::TestCommand)
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=?\r",stat->params.command);
		else if(stat->op == GSMOp::ReadCommand)
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s?\r", stat->params.command);
		else if(stat->op == GSMOp::WriteCommand)
		{
			GSMWriteParams* p = reinterpret_cast<GSMWriteParams*>(stat->params.ptr);
			if(p->nonConstValues == nullptr)
				len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=%s\r", p->command, p->paramValues); // repensar
			else
			{
				len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=%s\r", p->command, p->nonConstValues);
				getAllocator()->Deallocate(p->nonConstValues);
			}
			getAllocator()->Deallocate(p);
		}
		else if(stat->op == GSMOp::ExecCommand)
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s\r", stat->params.command);

		else if(stat->op == GSMOp::GetSignal)
		{
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s\r", GSMCommands::CSQ);
		}

		else if(stat->op == GSMOp::SMSSend)
		{
			stat->state = 0;
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=\"0034%d\"\r", GSMCommands::SMS_CMGS, stat->counter);
		}

		else if(stat->op == GSMOp::BearerOpen)
		{
			m_sapbr = false;
			stat->state = 0;
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=3,1,\"ConType\",\"GPRS\"\r", GSMCommands::IP_BEARER);
		}
		else if(stat->op == GSMOp::BearerClose)
		{
			stat->state = 0;
			m_ipAddr[0] = 0;
			m_ipAddr[1] = 0;
			m_ipAddr[2] = 0;
			m_ipAddr[3] = 0;
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s\r", GSMCommands::IP_CIPSHUT);
		}

		else if(stat->op == GSMOp::HTTPData)
		{
			stat->state = 0;
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=%d,10000\r", GSMCommands::HTTP_DATA, strlen(reinterpret_cast<char*>(stat->params.ptr)));
		}
		else if(stat->op == GSMOp::HTTPAction)
		{
			stat->state = 0;
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=%d\r", GSMCommands::HTTP_ACTION, stat->counter);
		}

		else if(stat->op == GSMOp::IPShut)
		{
			stat->state = 0;
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s\r", GSMCommands::IP_CIPSHUT);
		}

		else if(stat->op == GSMOp::DisableEcho)
		{
			stat->state = 0;
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s\r", GSMCommands::AT);
		}
		else if(stat->op == GSMOp::DisableModem)
		{
			stat->state = 0;
			setStatus(GSMStatus::Off, __func__, __LINE__);
			len = sprintf(reinterpret_cast<char*>(m_txbuf), "%s=0\r", GSMCommands::CSGS);
			delayStart = HAL_GetTick();
		}

		else if(stat->op == GSMOp::Delay)
		{
			delayStart = HAL_GetTick();
		}

		else if(stat->op == GSMOp::StartSend)
		{
			setStatus(GSMStatus::Sending, __func__, __LINE__);
			step();
		}
		else if(stat->op == GSMOp::RawSend)
		{
			PortTask* t = m_tasks.at_front();
			_gsm_tx(reinterpret_cast<uint8_t*>(t->data), t->len);
		}
		else if(stat->op == GSMOp::EndTask)
		{
			getAllocator()->Deallocate(m_tasks.at_front()->data);
			m_tasks.at_front()->data = 0;
			m_tasks.delete_front();
			if(m_status == GSMStatus::Sending) setStatus(GSMStatus::Ready, __func__, __LINE__);
			_statusList.delete_front();
		}

		if(len > 0)
			_gsm_tx(m_txbuf, len);
	}

	void PortGSM::initHTTP()
	{
		GSMState st;
		st.op = GSMOp::BearerOpen;
		_statusList.push_back(st);

		st.op = GSMOp::ExecCommand;
		st.params.command = GSMCommands::HTTP_INIT;
		_statusList.push_back(st);
	}

	void PortGSM::_gsm_tx(uint8_t* data, uint16_t len)
	{
		uint8_t buf[128];
		memcpy(buf, data, len);
		buf[len] = 0;
		HAL_UART_Transmit_IT(&huart3, data, len);
		Log::Verbose("[Modem(%s)] T: %s\n", getStatusStr(), buf);
	}

	/** PortRadio Driver */

	PortRadio::PortRadio(LoRa* lora)
	{
		m_lora = lora;
	}

	int PortRadio::open(Socket* sock, const char* scheme, const char* path)
	{
		return -1;
	}

	bool PortRadio::loop()
	{
		return false;
	}
}
