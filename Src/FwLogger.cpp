#include "FwLogger.h"

namespace FwLogger
{
	OS::OS():_alloc(_alloc_buf, _alloc_idx, 4096), flash(&hspi1, FLASH_CS), etsdb(0, 8192*1024, &hspi1, FLASH_CS, &_alloc), sdi12(SDI12_1),
	radio(&hspi1, LORA_DIO0, LORA_DIO1, LORA_DIO2, LORA_DIO3, LORA_RXEN, LORA_TXEN, LORA_CS, LORA_RST)
	{
		errno = 0;
		parser_remaining = 0;
		parser_status = 0;
		//ctor
	}

	void OS::init()
	{
		radio.init(868000000);
	}

	void OS::push_rx(uint8_t c)
	{
		if(_currTask.op == Operation::Upload)
		{
			write(_currTask.fd, &c, 1);

			if(--parser_remaining <= 0)
			{
				Task tsk;
				tsk.op = Operation::Close;
				tsk.fd = _currTask.fd;

				_ops.push_back(tsk);

				_currTask.op = Operation::None;
			}
		}
		else
		{
			write(0, &c, 1);
			if(c == '\b' || c == 127)
			{

                if(rx_buffer.idx > 0)
					--rx_buffer.idx;

                rx_buffer.buf[rx_buffer.idx] = 0;
			}
			else if(c == '\n')
			{
				Task tsk;
				tsk.op = Operation::Eval;
				_ops.push_back(tsk);
			}
			else
				rx_buffer.push_back(c);
		}
	}

	void OS::loop()
	{
		etsdb.poll(); // esto sería interesante pasarlo a estático
		flash.poll();
		radio.poll();
		vm.loop();

		if(_currTask.op == Operation::None)
			_ops.pop_front(&_currTask);

		else if(_currTask.op == Operation::Eval)
		{
			_currTask.op = Operation::None;
			eval();
			rx_buffer.clear();
		}

		else if(_currTask.op == Operation::OpenFile)
		{
			if(etsdb.openFile(_currTask.name_buf, static_cast<eTSDB::PageAccessMode>(_currTask.counter)) == eTSDB::Ok)
				_currTask.op = Operation::None;
		}
		else if(_currTask.op == Operation::OpenHeader)
		{
			ProgramInitializer* pi = reinterpret_cast<ProgramInitializer*>(_currTask.buf);
			auto retval = etsdb.openHeader(pi->name, eTSDB::PageAccessMode::PageWriteUpdate, pi->hi);

            if(retval == eTSDB::Ok)
			{
				_alloc.Deallocate(_currTask.buf);
				_currTask.op = Operation::None;
			}
		}
		else if(_currTask.op == Operation::GetPage)
		{
			eTSDB::Page* page = etsdb.getPage();
			if(page != nullptr)
			{
				eTSDB::FilePage* fp = new eTSDB::FilePage();
				_fds[_currTask.fd-1].ptr = fp;
				fp->copy(reinterpret_cast<eTSDB::FilePage*>(page));
				if(fp->getAccessMode() == eTSDB::PageAccessMode::PageRead)
                    _fds[_currTask.fd-1].buf_idx = 0x80; // el buf está lleno, para obtener página
				_currTask.op = Operation::None;
			}
		}
		else if(_currTask.op == Operation::ReadNext)
		{
			eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fds[_currTask.fd-1].ptr);
			eTSDB::RetValue retval = etsdb.readNextBlockFile(*fp);
            if(retval == eTSDB::Ok)
			{
				_currTask.op = Operation::None;
			}
			else if(retval == eTSDB::FileEnded)
			{
				_currTask.op = Operation::None;
			}
		}
		else if(_currTask.op == Operation::Close)
		{
			HAL_Delay(10);
			if(close(_currTask.fd) == 0)
				_currTask.op = Operation::None;
		}
		else if(_currTask.op == Operation::LoadProgram)
		{
			if(_currTask.buf == nullptr)
			{
				_currTask.counter = 0;
				_currTask.buf = _alloc.Allocate(sizeof(ProgramInitializer));
				for(int i = 0; i < sizeof(ProgramInitializer); ++i)
					reinterpret_cast<uint8_t*>(_currTask.buf)[i] = 0;
			}
			uint8_t file_buf[128];
			int bytes = read(_currTask.fd, file_buf, 128); // leo hasta 128 bytes
			if(bytes > 0) // tengo datos para incorporar
			{
				ProgramInitializer* pi = reinterpret_cast<ProgramInitializer*>(_currTask.buf);
				for(int i = 0; i < bytes; ++i)
				{
					if(pi->status == 0)
					{
						pi->tables = file_buf[i];
						++pi->status;
						pi->table_status = 0;
						pi->name_counter = 0;
						_currTask.counter = 0;
					}
					else if(pi->status == 1) // read table
					{
						if(pi->table_status == 0) // name
						{
							if(file_buf[i] == 0)
							{
								pi->table_status++;
								pi->name_counter = 0;
							}
							else
							{
								pi->name[pi->name_counter++] = file_buf[i];

								if(pi->name_counter == 16) pi->table_status++;
							}

						}

						else if(pi->table_status == 1) // period
						{
							pi->hi.period = file_buf[i];
							pi->table_status++;
						}

						else if(pi->table_status == 2)
						{
							if(file_buf[i] == 0)
							{
								pi->status= 2; // no more formats

								Task open_tsk;
								open_tsk.fd = _createFD(FDType::TS);
								open_tsk.buf = _currTask.buf;
								open_tsk.op = Operation::OpenHeader;
								_ops.push_back(open_tsk);

								Task get_tsk;
								get_tsk.fd = open_tsk.fd;
								get_tsk.op = Operation::GetPage;
								_ops.push_back(get_tsk);

								vm.HeaderFD = open_tsk.fd;

								pi->hi.colLen = _currTask.counter;

								_currTask.counter = 0;
							}
							else
							{
								pi->hi.formats[_currTask.counter] = static_cast<eTSDB::Format>(file_buf[i]);
								pi->table_status++;
								pi->name_counter = 0;
							}
						}

						else if(pi->table_status == 3)
						{
							if(file_buf[i] == 0)
							{
								pi->table_status = 2;
								_currTask.counter++;
							}
							else
							{
								pi->hi.colNames[_currTask.counter][pi->name_counter++] = file_buf[i];

								if(pi->name_counter == 16)
								{
									pi->table_status = 2;
									_currTask.counter++;

									if(_currTask.counter == 16) //16 columns
									{
										pi->status = 2;

										Task open_tsk;
										open_tsk.fd = _createFD(FDType::TS);
										open_tsk.buf = _currTask.buf;
										open_tsk.op = Operation::OpenHeader;
										_ops.push_back(open_tsk);

										Task get_tsk;
										get_tsk.fd = open_tsk.fd;
										get_tsk.op = Operation::GetPage;
										_ops.push_back(get_tsk);

										vm.HeaderFD = open_tsk.fd;

										pi->hi.colLen = _currTask.counter;

										_currTask.counter = 0;
									}
								}
							}
						}
					}
					else if(pi->status == 2)
					{
						if(_currTask.counter == 0)
						{
							pi->stack_size = 0;
							pi->static_vars = 0;
						}
						if(_currTask.counter < 4)
						{
							pi->stack_size |= (file_buf[i] << (_currTask.counter*8));
							++_currTask.counter;
						}
						else if(_currTask.counter < 8)
						{
							pi->static_vars |= (file_buf[i] << ((_currTask.counter-4)*8));
							++_currTask.counter;
						}
						if(_currTask.counter == 8)
						{
							pi->status = 3;
							vm.setStackSize(pi->stack_size);
							_currTask.counter = 0;
						}
					}
					else if(pi->status == 3)
					{
						vm.setProgram(&file_buf[i], _currTask.counter, bytes-i);
						_currTask.counter+= bytes-i;
						i = bytes;
					}
				}
			}
			else
			{
				if(errno == EAGAIN)
				{
					errno = 0; // repeat, error acknowledged
				}
				if(errno == EIO)
				{
					vm.enable(true);
					_currTask.op = Operation::None; // finalizado
				}
			}
		}
	}

	void OS::eval()
	{
		char* str = reinterpret_cast<char*>(rx_buffer.buf);

		char* token =  strtok(str, " ");

		if(strcmp(token, "file") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "upload") == 0)
			{
				token = strtok(NULL, " ");
				char path[26] = {0};
				strcpy(path, "/file/");
				strcpy(path+6, token);

				token = strtok(NULL, " ");
				int remaining_file = std::atoi(token);
				if(remaining_file == 0)
				{
					write(0, "Error in file format\n", 21);
				}

				int fd = open(path, O_WRONLY);

				Task tsk;
				tsk.op = Operation::Upload;
				tsk.fd = fd;
				parser_remaining = remaining_file;

				_ops.push_back(tsk);
			}
		}
		else if(strcmp(token, "flash") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "erase") == 0)
			{
				flash.eraseChip();
				write(0, "Erasing flash\n", 14);
			}
			else if(strcmp(token, "unlock") == 0)
			{
				flash.unlock();
			}
			else if(strcmp(token, "read") == 0)
			{
                token = strtok(NULL, " ");
                int addr = std::atoi(token);
                token = strtok(NULL, " ");
                int len = std::atoi(token);

                if(len == 0) write(0, "invalid value\n", 14);
                else
				{
					flash.readPage(len, addr);
					HAL_Delay(1);
					flash.poll();
					for(int i = 0; i < len; ++i)
					{
						printf_("%0x ", flash.pop());
						if(i%50 == 0) HAL_Delay(10);
					}
					printf_("\n");
					flash.unlock();
				}
			}
			else if(strcmp(token, "rawread")==0)
			{
				token = strtok(NULL, " ");
                int addr = std::atoi(token);
                token = strtok(NULL, " ");
                int len = std::atoi(token);

                if(len == 0) write(0, "invalid value\n", 14);
                else
				{
					flash.readPage(len, addr);
					HAL_Delay(1);
					flash.poll();
					for(int i = 0; i < len; ++i)
					{
						tx_buffer.push_back(flash.pop());
						if(i%50 == 0) HAL_Delay(1);
					}
					printf_("\n");
					flash.unlock();
				}
			}
		}
		else if(strcmp(token, "run") == 0)
		{
			token = strtok(NULL, " ");
			char buf[32];
			strcpy(buf, "/file/");
			strcpy(buf+6, token);
			int fd = open(buf, O_RDONLY);

			Task tsk;
			tsk.op = Operation::LoadProgram;
			tsk.fd = fd;
			tsk.buf = nullptr;
			tsk.counter = 0;

			_ops.push_back(tsk);

		}
		else if(strcmp(token, "power") == 0)
		{
            token = strtok(NULL, " ");
            int power_enable = std::atoi(token);
            enablePower(power_enable);
		}
	}


	int OS::open(char* path, int oflag)
	{
		if(path[0] != '/')
		{
			errno = ENOENT;
			return -1;
		}

		char* pch;
		pch = strtok(path, "/");
		if(strcmp(pch, "spi") == 0)
		{

		}
		else if(strcmp(pch, "i2c") == 0)
		{

		}
		else if(strcmp(pch, "uart") == 0)
		{

		}
		else if(strcmp(pch, "sdi12") == 0)
		{

		}
		else if(strcmp(pch, "file") == 0)
		{
			int fd = _createFD(FDType::File);
			pch = strtok(NULL, "/");
			eTSDB::PageAccessMode am;
			if(oflag == O_RDONLY) am = eTSDB::PageAccessMode::PageRead;
			if(oflag == O_WRONLY) am = eTSDB::PageAccessMode::PageWrite;
			if(oflag == O_UPDATE) am = eTSDB::PageAccessMode::PageWriteUpdate;
			//etsdb.openFile(reinterpret_cast<uint8_t*>(pch), am);

			Task tsk_creat;
			tsk_creat.op = Operation::OpenFile;
			tsk_creat.fd = fd;
			strcpy(reinterpret_cast<char*>(tsk_creat.name_buf), pch);
			tsk_creat.counter = static_cast<uint16_t>(am);
			_ops.push_back(tsk_creat);

            Task tsk_get;
            tsk_get.op = Operation::GetPage;
            tsk_get.fd = fd;

            _ops.push_back(tsk_get);
            return fd;
		}
		else if(strcmp(pch, "etsdb") == 0)
		{
			int fd = _createFD(FDType::TS);
			pch = strtok(NULL, "/");
			eTSDB::PageAccessMode am;
			if(oflag == O_RDONLY) am = eTSDB::PageAccessMode::PageRead;
			if(oflag == O_WRONLY)
			{
				errno = EUNSPEC;
				return -1;
			}
			if(oflag == O_UPDATE) am = eTSDB::PageAccessMode::PageWriteUpdate;
			//etsdb.openHeader(pch, am);

			//Task tsk;
			//tsk.fd = fd;
			//tsk.op = Operation::GetPage;

			//_ops.push_back(tsk);
			return fd;
		}
		errno = EUNSPEC;
		return -1;
	}

	int OS::read(int fd, void* buf, size_t count)
	{
		uint8_t* _buf = static_cast<uint8_t*>(buf);
		if(fd == 0)
		{
			int i;
			for(i = 0; i < count && i < rx_buffer.idx; ++i) _buf[i] = rx_buffer.buf[i];
			return i;
		}
		else
		{
			FileDescriptor& _fd = _fds[fd-1];
			if(_fd.type == FDType::File)
			{
				if(_fd.buf == nullptr) _fd.buf = reinterpret_cast<uint8_t*>(_alloc.Allocate(128));
				eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fd.ptr);
				if(fp->getFileSize() <= _fd.bytes_read)
				{
					errno = EIO;
					return -1;
				}
				uint8_t* ui8b = reinterpret_cast<uint8_t*>(buf);
				for(int i = 0; i < count; ++i)
				{
					if(_fd.buf_idx > 0x7f)
					{
						uint8_t* dp = fp->getDataPage();
						if(dp == nullptr)
						{
							errno = EAGAIN;
							_fd.bytes_read+=i;
							return i;  //return bytes written
						}
						else
						{
							_fd.buf_idx = 0;
							for(int i = 0; i < 128; ++i) _fd.buf[i] = dp[i];

							Task tsk;
							tsk.fd = fd;
							tsk.op = Operation::ReadNext;
							_ops.push_back(tsk);
						}
					}
					ui8b[i] = _fd.buf[_fd.buf_idx++];
				}
				_fd.bytes_read += count;
				return count;
			}

		}
		errno = EUNSPEC;
		return -1;
	}

	int OS::poll(int fd)
	{
        if(fd == 0)
		{
            return rx_buffer.idx > 0;
		}
		else
		{
			FileDescriptor& _fd = _fds[fd-1];
			if(_fd.type == FDType::File)
			{
                eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fd.ptr);
                return fp->getDataReady();
			}
		}

		errno = EUNSPEC;
		return -1;
	}

	int OS::write(int fd, const void* buf, size_t count)
	{
		if(fd == 0)
		{
			uint8_t* ui8_buf = (uint8_t*) buf;
			for(unsigned int i = 0; i < count; ++i)
			{
				if(tx_buffer.push_back(ui8_buf[i]) != 0)
				{
					errno = EBADF;
					return -1;
				}
			}

			uint8_t ph;
			if(!_UART_txing) // esto se podría ver como un flush
			{
				if(tx_buffer.pop_front(&ph) == 0)
				{
					HAL_UART_Transmit_IT(&huart1, &ph, 1);
					_UART_txing = 1;
				}
			}

			return count;
		}
		else
		{
			FileDescriptor& _fd = _fds[fd-1];
            if(_fd.type == FDType::None)
			{
				errno = EBADF;
				return -1;
			}
			if(_fd.type == FDType::File)
			{
				if(_fd.ptr == nullptr) // there is no page yet
				{
					if(_fd.buf == nullptr)
						_fd.buf = reinterpret_cast<uint8_t*>(_alloc.Allocate(128));
                    for(int i = 0; i < count; ++i)
						_fd.buf[_fd.buf_idx++] = reinterpret_cast<const uint8_t*>(buf)[i];
				}
				else //there is page				{
					if(_fd.buf != nullptr) //but there is unwritten things in buffer
					{
						uint8_t merge_buf[128];

						for(int i = 0; i < _fd.buf_idx; ++i)
							merge_buf[i] = _fd.buf[i];

						for(int i = 0; i < count; ++i)
							merge_buf[i+_fd.buf_idx] = reinterpret_cast<const uint8_t*>(buf)[i];

						etsdb.writeFile(*reinterpret_cast<eTSDB::FilePage*>(_fd.ptr), merge_buf, count+_fd.buf_idx);

						_alloc.Deallocate(_fd.buf);
						_fd.buf = nullptr;
						_fd.buf_idx = 0;
					}
					else
						etsdb.writeFile(*reinterpret_cast<eTSDB::FilePage*>(_fd.ptr), reinterpret_cast<const uint8_t*>(buf), count);
					return count;
				}
			}
		}
		errno = EUNSPEC;
		return -1;
	}

	int OS::close(int fd) // this return 0 if there is no operation pending
	{
		if(fd == 0) return EBADF;
		FileDescriptor& _fd = _fds[fd-1];
		if(_fd.type == FDType::None) return EBADF;
		if(_fd.type == FDType::File)
		{
            if(etsdb.closeFile(*reinterpret_cast<eTSDB::FilePage*>(_fd.ptr)) != eTSDB::RetValue::Ok)
			{
                return EBUSY;
			}
            _alloc.Deallocate(_fd.ptr);
            _fd.ptr = nullptr;
		}
		return 0;
	}

	int16_t OS::get_adc_val(int channel)
	{
		switch(channel)
		{
		case ADC_SE_0:
			return adc3_data[0];

		case ADC_SE_1:
			return adc3_data[1];

		case ADC_SE_2:
			return adc3_data[2];

		case ADC_SE_3:
			return adc3_data[3];

		case ADC_SE_4:
			return adc1_data[0];

		case ADC_SE_5:
			return adc1_data[1];

		case ADC_DE_0:
			return adc3_data[4];

		case ADC_DE_1:
			return adc3_data[5];

		case ADC_DE_2:
			return adc1_data[2];

		case ADC_DE_3:
			return adc1_data[3];

		case ADC_DE_4:
			return adc1_data[4];

		case ADC_DE_5:
			return adc1_data[5];

		default:
			return -1;
		}
	}

	void OS::enablePower(int enable)
	{
		if(enable)
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	}

	int8_t OS::_createFD(FDType type)
	{
		for(int i = 0; i < 16; ++i)
		{
			if(_fds[i].type == FDType::None)
			{
				_fds[i].type = type;
				_fds[i].buf_idx = 0;
				_fds[i].ptr = nullptr;
				return i+1;
			}
		}
		return -1; // full file descriptor table
	}

	void OS::_deleteFD(int fd)
	{
        if(_fds[fd].type == FDType::File)
		{
            if(_fds[fd].ptr != nullptr) _alloc.Deallocate(_fds[fd].ptr);
            if(_fds[fd].buf != nullptr) _alloc.Deallocate(_fds[fd].buf);
		}
	}
}
