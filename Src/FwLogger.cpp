#include "FwLogger.h"

unsigned char fw_standalone = 1;

Allocator<128>* _vectAllocator;

#include "embeddedProgram.cpp"

namespace FwLogger
{
	circular_buffer<16, Task> msgQueue;

	OS* OS::ptr = nullptr;
	OS& OS::get()
	{
		return *ptr;
	}

	OS::OS():
		_alloc(_alloc_buf, _alloc_idx, _alloc_ownership, 8192),
		flash(&hspi1, FLASH_CS),
		//etsdb(0, 8192*1024, &hspi1, FLASH_CS, &_alloc),
		sdi12(SDI12_0),
		radio(&hspi1, LORA_DIO0, LORA_DIO1, LORA_RXEN, LORA_TXEN, LORA_CS, LORA_RST)
	{
		for(int i = 0; i < 32; ++i) m_name[i] = 0;
		errno = 0;

		m_rtcFlag = false;
		m_name[0] = 'h';
		m_name[1] = 'o';
		m_name[2] = 'l';
		m_name[3] = 'a';
		m_name[4] = 0;

		Log::setLogLevel(Log::LevelVerbose);

		//loadConfig();
		//ctor

		PortManager::setAllocator(&_alloc);
		//do not call hardware settings
	}

	void OS::setOS(OS* os)
	{
		ptr = os;
	}

	void OS::hwinit()
	{
		/* Configure the system clock */
		SystemClock_Config();

		/* USER CODE BEGIN SysInit */

		/* USER CODE END SysInit */

		/* Initialize all configured peripherals */
		MX_GPIO_Init();
		MX_DMA_Init();
		MX_ADC1_Init();
		MX_DAC_Init();
		MX_I2C1_Init();
		MX_RTC_Init();
		MX_SPI1_Init();
		MX_SPI2_Init();
		MX_TIM6_Init();
		MX_TIM7_Init();
		MX_USART1_UART_Init();
		MX_USART3_UART_Init();
		//MX_USB_PCD_Init();
	}

	void OS::init()
	{
		//flash.readPage(10, 0);
		HAL_Delay(10);
		//flash.loop(); // no preguntar, xd lol

		//radio.init(868000000);
		//radio.receive(1);

		_vectAllocator = &_alloc;
		PortUART::get(); //this inits the variables
		PortGSM::get();

		m_lpEnabled = false;

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_SET);
		if(PWR->CSR & 0x02)
		{
			Log::Verbose("[OS] Wakeup from Standby\n");
			PWR->CR |= 0x0c;
		}
		else
		{
			Log::Verbose("[OS] Init\n");
		}
		//HAL_Delay(1000);

		m_init = false;
		init_delay = HAL_GetTick();
	}

	void OS::RTC_ISR()
	{
		m_rtcFlag = true;
		Log::Info("RTC ISR\n");
		HAL_PWR_DisableSleepOnExit();
		HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
		vm.resumeExec();
	}

	void OS::loop()
	{
		volatile bool work_left = false;
		for(int i = 0; i < Module::getModuleNumber(); ++i)
		{
			work_left = work_left | Module::getModule(i)->loop();
		}

		work_left = task_loop() | work_left;

		m_pendingTask = work_left;

		m_pendingTask |= PortManager::loop();
		if(!m_init)
		{
			if(HAL_GetTick()-init_delay > 15000)
			{
				vm.setStackSize(150);
				vm.setProgram(vm_prg, 0, sizeof(vm_prg));

				vm.enable(true);

				m_init = true;
			}
		}

		if(m_lpEnabled && !m_pendingTask)
		{
			sleep();
			wakeUp();
			Log::Verbose("[OS] Waking up\n");
		}

		if(m_rtcFlag) // if it has to wake up from sleep
		{
			m_rtcFlag = false;
			m_lpEnabled = false;
			vm.resumeExec(); // this will resume VM execution if WAIT_TABLE is called
		}

		else if(vm.getWaitFlag()) // else if has to go to sleep, configure alarm
		{
			RTC_TimeTypeDef sTime;
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
            int secs = sTime.Hours*3600 + sTime.Minutes*60 + sTime.Seconds;
            secs = vm.getNextAlarm(secs);

			RTC_AlarmTypeDef sAlarm;
			sAlarm.Alarm = RTC_ALARM_A;
			sAlarm.AlarmTime.Hours = secs / 3600;
			secs %= 3600;
			sAlarm.AlarmTime.Minutes = secs/60;
			sAlarm.AlarmTime.Seconds = secs%60;

			HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
		}

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, HAL_GetTick()%5000 < 10 ? GPIO_PIN_RESET : GPIO_PIN_SET);

		/*if(vm.getSaveFlag()) //if it has to save data
		{
            eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[vm.HeaderFD-FD_BUILTINS].ptr);
            if(hp != nullptr)
			{
				eTSDB::Row* dataRow = new (_alloc.Allocate(sizeof(eTSDB::Row), this)) eTSDB::Row();
				hp->getFormat(*dataRow);
				dataRow->deserialize(vm.getTableAddress());

				RTC_AlarmTypeDef sAlarm;
				HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);

				RTC_DateTypeDef sDate;
				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

				dataRow->rowDate.exists = 0;
				dataRow->rowDate.year = sDate.Year+2000;
				dataRow->rowDate.month= sDate.Month;
				dataRow->rowDate.day = sDate.Date;
				dataRow->rowDate.hour = sAlarm.AlarmTime.Hours;
				dataRow->rowDate.minute = sAlarm.AlarmTime.Minutes;
				dataRow->rowDate.second = sAlarm.AlarmTime.Seconds;

				Task op;
				op.op = Operation::SaveRow;
				op.fd = vm.HeaderFD;
				op.buf = dataRow;
				msgQueue.push_back(op);

				Log::Verbose("Saving row\n");

				vm.ackSaveFlag();
			}
		}*/
		m_loop_time = HAL_GetTick()-m_last_loop;
		m_last_loop = HAL_GetTick();
	}

	int twodecparse(uint8_t* str)
	{
		return (str[0]-'0')*10 + (str[0]-'0');
	}

	int parse_num(int* dst, uint8_t* str)
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

	bool OS::task_loop()
	{
		Task* currTask = msgQueue.at_front();

		if(currTask == nullptr)
			return false;

		if(currTask->op == Operation::Eval)
		{
			printf_out = currTask->fd;
			eval(reinterpret_cast<uint8_t*>(currTask->buf), currTask->fd);
			_alloc.Deallocate(currTask->buf);

			Socket* sock = PortManager::getSocket(currTask->fd);
			if(sock != nullptr)
			{
				if(sock->self_managed) close(currTask->fd);
			}

			printf_out = 0;
			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::BinEval)
		{
			bin_eval(reinterpret_cast<uint8_t*>(currTask->buf), currTask->counter, currTask->fd);
			_alloc.Deallocate(currTask->buf);
			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::ModemEval)
		{
			Log::Verbose("[Modem(%s)] R: %s\n", PortGSM::get().getStatusStr(), reinterpret_cast<char*>(currTask->buf));
			PortGSM::get().eval(reinterpret_cast<uint8_t*>(currTask->buf), currTask->counter);

			uint8_t* uib = reinterpret_cast<uint8_t*>(currTask->buf);
			if(uib[0] == '+')
			{
				std::size_t space_loc;
				for(space_loc = 0; space_loc < currTask->counter; ++space_loc)
				{
					if(uib[space_loc] == ' ')
					{
						uib[space_loc] = 0;
						break;
					}
				}
				if(space_loc < currTask->counter)
				{
					uib += 1;
					if(strcmp(reinterpret_cast<char*>(uib), "CCLK:") == 0)
					{
						uib += space_loc;

						eTSDB::Date date;
						date.year = twodecparse(uib+1)+2000;
						date.month = twodecparse(uib+4);
						date.day = twodecparse(uib+7);
						date.hour = twodecparse(uib+10);
						date.minute = twodecparse(uib+13);
						date.second = twodecparse(uib+16);

						uint64_t timestamp = date.timestamp();
						date.fromTimestamp(timestamp-3600);

						RTC_DateTypeDef sDate;
						sDate.Year = date.year-2000;
						sDate.Month = date.month;
						sDate.Date = date.day;

						RTC_TimeTypeDef sTime;
						sTime.Hours = date.hour;
						sTime.Minutes = date.minute;
						sTime.Seconds = date.second;

						setTime(sDate, sTime);

					}
				}
			}
			else if(uib[0] == '*')
			{
				if(strncmp(reinterpret_cast<char*>(uib), "*PSUTTZ", 7) == 0)
				{
					uib = uib + 11;
					RTC_DateTypeDef sDate;
					RTC_TimeTypeDef sTime;
					int dst;

					uib += (parse_num(&dst, uib)+1);
					sDate.Year = dst;
					uib += parse_num(&dst, uib)+1;
					sDate.Month = dst;
					uib += parse_num(&dst, uib)+1;
					sDate.Date = dst;

					uib += parse_num(&dst, uib)+1;
					sTime.Hours = dst;
					uib += parse_num(&dst, uib)+1;
					sTime.Minutes = dst;
					uib += parse_num(&dst, uib)+1;
					sTime.Seconds = dst;

					setTime(sDate, sTime);
				}
			}

			_alloc.Deallocate(currTask->buf);

			Socket* sock = PortManager::getSocket(currTask->fd);
			if(sock != nullptr)
			{
				if(sock->self_managed) close(currTask->fd);
			}

			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::OpenFile)
		{
			/*if(etsdb.openFile(currTask->name_buf, static_cast<eTSDB::PageAccessMode>(currTask->counter)) == eTSDB::Ok)
				currTask->op = Operation::GetPage;*/
			//TODO reimplement
			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::OpenHeader)
		{
			/*eTSDB::RetValue retval;
			if(currTask->buf == nullptr)
			{
				retval = etsdb.openHeader(currTask->name_buf, eTSDB::PageAccessMode::PageRead);
			}
			else
			{
				ProgramInitializer* pi = reinterpret_cast<ProgramInitializer*>(currTask->buf);
				retval = etsdb.openHeader(pi->name, eTSDB::PageAccessMode::PageWriteUpdate, pi->hi);
			}
            if(retval == eTSDB::Ok)
			{
				if(currTask->buf != nullptr)
				{
					_alloc.Deallocate(currTask->buf);
					currTask->buf = nullptr;
				}
				currTask->op = Operation::GetPage;
			}*/
			//TODO reimplement
			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::ListTables) // TODO (forcen#1#): Add binary mode for these functions
		{
			/*eTSDB::RetValue retval = etsdb.checkGetNextHeader();
			eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(currTask->buf);
			if(retval == eTSDB::Ok)
			{
				if(currTask->counter == 0)
					printf("%s\tColumns: %d\n", hp->getName(), hp->getNumColumn());
				else
				{
					close(currTask->fd);
					msgQueue.delete_front();
				}
				etsdb.startGetNextHeader(*hp);
			}
			else if(retval == eTSDB::NotFound)
			{
				delete hp;
				if(currTask->counter == 0)
				{
					printf("Done\n");
				}
				else
				{
					uint8_t outbuf[] = {0x0b, 0x03, 0x00, 't', 'l', 'f'};
					write(0, outbuf, 6);
				}
				etsdb.unlock();
				msgQueue.delete_front();
			}*/
			//TODO reimplement
			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::ReadTable)
		{
			/*eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[currTask->fd-FD_BUILTINS].ptr);
			if((currTask->counter & 1) == 0)
			{
				if(etsdb.readNextValue(*hp) == eTSDB::Ok) currTask->counter |= 1;
			}
			else
			{
				eTSDB::Row* rowPtr = etsdb.getDataRow();
				if(rowPtr != nullptr)
				{
					if(rowPtr->rowDate.exists == 0xff)
					{
						close(currTask->fd);
						currTask->fd = 0;
						if(currTask->counter & 0x02)
						{

						}
						else
						{
							printf("Done\n");
						}
						etsdb.unlock();
						msgQueue.delete_front();
					}
					else
					{
						if(currTask->counter & 0x02) //bin
						{
							uint8_t outbuf[256] = {0};
							outbuf[0] = 0x0b;
							outbuf[3] = 't';
							outbuf[4] = 'd';
							rowPtr->rowDate.serialize(outbuf+5);
							int outlen = 10;
							for(int i = 0; i < 16 && rowPtr->vals[i].format != eTSDB::Format::Invalid; ++i)
							{
								for(int j = 0; j < eTSDB::getFormatWidth(rowPtr->vals[i].format); ++j)
								{
									outbuf[outlen++] = rowPtr->vals[i].data.bytes[j];
								}
							}
							outbuf[1] = outlen & 0xff;
							outbuf[2] = (outlen >> 8) & 0xff;
							write(0, outbuf, outlen+3);
						}
						else
						{
							printf("%d/%d/%d %d:%d:%d", rowPtr->rowDate.year, rowPtr->rowDate.month, rowPtr->rowDate.day
													  , rowPtr->rowDate.hour, rowPtr->rowDate.minute, rowPtr->rowDate.second);
							for(std::size_t i = 0; i < rowPtr->vals.size(); ++i)
							{
								if(rowPtr->vals[i].format == eTSDB::Format::Int32 || rowPtr->vals[i].format == eTSDB::Format::Uint32)
								{
									printf(",%d", rowPtr->vals[i].data._int32);
								}
								else if(rowPtr->vals[i].format == eTSDB::Format::Float)
								{
									printf(",%f", rowPtr->vals[i].data._float);
								}
								else if(rowPtr->vals[i].format == eTSDB::Format::Int8 || rowPtr->vals[i].format == eTSDB::Format::Uint8)
								{
									printf(",%d", rowPtr->vals[i].data.bytes[0]);
								}
							}
							printf("\n");
						}
						HAL_Delay(1);
						etsdb.readNextValue(*hp);
					}
				}
			}*/
			//TODO reimplement
			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::GetPage)
		{
			/*eTSDB::Page* page = etsdb.getPage();
			if(page != nullptr)
			{
				if(page->getType() == eTSDB::PageType::FileType)
				{
					eTSDB::FilePage* fp = new eTSDB::FilePage();
					_fds[currTask->fd-FD_BUILTINS].ptr = fp;
					fp->copy(reinterpret_cast<eTSDB::FilePage*>(page));
					if(fp->getAccessMode() == eTSDB::PageAccessMode::PageRead)
						_fds[currTask->fd-FD_BUILTINS].buf_idx = 0x80; // el buf está lleno, para obtener página
				}
				else if(page->getType() == eTSDB::PageType::HeaderType)
				{
					eTSDB::HeaderPage* hp = new eTSDB::HeaderPage();
					_fds[currTask->fd-FD_BUILTINS].ptr = hp;
					hp->copy(reinterpret_cast<eTSDB::HeaderPage*>(page));
				}
				msgQueue.delete_front();
			}*/
			//TODO reimplement
			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::ReadNext)
		{
			/*eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fds[currTask->fd-FD_BUILTINS].ptr);
			eTSDB::RetValue retval = etsdb.readNextBlockFile(*fp);
            if(retval == eTSDB::Ok)
			{
				msgQueue.delete_front();
			}
			else if(retval == eTSDB::FileEnded)
			{
				msgQueue.delete_front();
			}*/
			//TODO reimplement
			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::ListFiles)
		{/*
			eTSDB::RetValue retval = etsdb.checkGetNextFile();
			if(retval == eTSDB::Ok)
			{
				eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(currTask->buf);
				if(currTask->counter == 0)
					printf("File name: %s\tFile size: %d\n", fp->getName(), fp->getFileSize());
				else
				{
					uint8_t outbuf[118] = {0};
					outbuf[0] = 'f';
					outbuf[1] = 'l';
					outbuf[2] = fp->getPageIdx() & 0xff;
					outbuf[3] = (fp->getPageIdx() >> 8)  & 0xff;
					outbuf[4] = fp->getFileSize() & 0xff;
					outbuf[5] = (fp->getFileSize() >> 8) & 0xff;
					currTask->counter++;
					int outlen = 6; // TODO change strlen for one with boundary check
					strcpy(reinterpret_cast<char*>(outbuf+6),reinterpret_cast<char*>(fp->getName()));
					outlen += strlen(reinterpret_cast<char*>(fp->getName()))+1; // +1 keeps the 0
					write(currTask->fd, outbuf, outlen);
					HAL_Delay(1);
				}
				etsdb.startGetNextFile(*fp);
			}
			else if(retval == eTSDB::NotFound)
			{
				eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(currTask->buf);
				if(currTask->counter == 0)
					printf("Done\n");
				else
				{
					const uint8_t outbuf[] = {'f', 'l', 'f'};
					write(currTask->fd, outbuf, sizeof(outbuf));
					close(currTask->fd);
					HAL_Delay(5);
				}
				delete fp;
				etsdb.unlock();
				msgQueue.delete_front();
			}*/
			//TODO reimplement
			msgQueue.delete_front();
		}
		else if(currTask->op == Operation::DownloadFile)
		{
			/*uint8_t obuf[150];
			obuf[0] = 0x0b;
			obuf[3] = 'f';
			obuf[4] = 'd';
			int obuflen;
			if(currTask->counter == 0)
			{
				eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fds[currTask->fd-FD_BUILTINS].ptr);
				obuf[5] = 's';
				obuflen = strlen(fp->getName()) + 6;
				strcpy(reinterpret_cast<char*>(obuf+5), reinterpret_cast<char*>(fp->getName()));
				int read_rv = read(currTask->fd, obuf+obuflen, 64);
				if(read_rv == -1)
			}
			else
			{

			}


			obuf[5] = currTask->counter & 0xff;
			obuf[6] = (currTask->counter >> 8) & 0xff;
			int retval = read(currTask->fd, obuf+7, 128);
			if(retval == -1)
			{
				obuf[5]
			}
			obuf[1] = obuflen & 0xff;
			obuf[2] = (obuflen >> 8)& 0xff;*/

			//TODO implement
			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::Close)
		{
			HAL_Delay(10);
			if(close(currTask->fd) == 0)
				msgQueue.delete_front();
		}
		else if(currTask->op == Operation::LoadProgram)
		{
			if(currTask->buf == nullptr)
			{
				currTask->counter = 0;
				currTask->buf = _alloc.Allocate(sizeof(ProgramInitializer), this);
				for(std::size_t i = 0; i < sizeof(ProgramInitializer); ++i)
					reinterpret_cast<uint8_t*>(currTask->buf)[i] = 0;
			}
			uint8_t file_buf[128];
			int bytes = read(currTask->fd, file_buf, 128); // leo hasta 128 bytes
			if(bytes > 0) // tengo datos para incorporar
			{
				ProgramInitializer* pi = reinterpret_cast<ProgramInitializer*>(currTask->buf);
				for(int i = 0; i < bytes; ++i)
				{
					if(pi->status == 0)
					{
						pi->tables = file_buf[i];
						++pi->status;
						pi->table_status = 0;
						pi->name_counter = 0;
						currTask->counter = 0;
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
							vm.tablePeriod = eTSDB::getSecondsFromPeriod(pi->hi.period);
						}

						else if(pi->table_status == 2)
						{
							pi->hi.cols.resize(file_buf[i]);
							pi->table_status++;
							currTask->counter = 0;
						}

						else if(pi->table_status == 3)
						{
							pi->hi.cols[currTask->counter].format = static_cast<eTSDB::Format>(file_buf[i]);
							pi->table_status++;
							pi->name_counter = 0;
						}

						else if(pi->table_status == 4)
						{
							pi->hi.cols[currTask->counter].name[pi->name_counter++] = file_buf[i];

							if(pi->name_counter == 16 || file_buf[i] == 0)
							{
								currTask->counter++;
								if(currTask->counter == pi->hi.cols.size()) pi->table_status = 5;
								else pi->table_status = 3;

							}
						}
						if(pi->table_status == 5)
						{
							pi->status = 2;

							Task open_tsk;
							open_tsk.fd = _createFD(FDType::TS);
							open_tsk.buf = currTask->buf;
							open_tsk.op = Operation::OpenHeader;
							msgQueue.push_back(open_tsk);

							vm.HeaderFD = open_tsk.fd;

							currTask->counter = 0;
						}
					}
					else if(pi->status == 2)
					{
						if(currTask->counter == 0)
						{
							pi->stack_size = 0;
						}
						if(currTask->counter < 4)
						{
							pi->stack_size |= (file_buf[i] << (currTask->counter*8));
							++currTask->counter;
						}
						if(currTask->counter == 4 )
						{
							pi->status = 3;
							vm.setStackSize(pi->stack_size);
							currTask->counter = 0;
							pi->table_status = 0;
						}
					}
					else if(pi->status == 3)
					{
						if(pi->table_status == 0)
						{
							if(file_buf[i] == 0)
							{
								pi->status = 4; // there is no external variables
							}
							else
							{
								vm.resizeSymbolTable(file_buf[i]);
								pi->table_status = 1;
								for(int i = 0; i < 16; ++i) pi->name[i] = 0;
								pi->name_counter = 0;
							}
						}
						else if(pi->table_status == 1)
						{
							vm.setSymbolType(currTask->counter, TypeVal(file_buf[i]));
							pi->stack_size = 0; // reusing variable
							pi->table_status = 2;
						}
						else if(pi->table_status < 6)
						{
							pi->stack_size |= file_buf[i] << ((pi->table_status-2)*8);
							++pi->table_status;
							if(pi->table_status == 6)
								vm.setSymbolAddr(currTask->counter, pi->stack_size);
						}
						else if(pi->table_status == 6)
						{
							pi->name[pi->name_counter++] = file_buf[i];
							if((file_buf[i] == 0) || (pi->name_counter == 16))
							{
								pi->table_status = 1;
								currTask->counter++;
								if(currTask->counter == vm.getSymbolTableSize())
									pi->status = 4;
							}
						}
					}
					else if(pi->status == 4)
					{
						vm.setProgram(&file_buf[i], currTask->counter, bytes-i);
						currTask->counter+= bytes-i;
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
					// _alloc.Deallocate(currTask->buf); //should be deallocated in openheader task
					close(currTask->fd); // close file, cleaning buffers (file page and file buffer)
					//vm.enable(true);
					//printf("running\n");
					msgQueue.delete_front();
				}
			}
		}
		else if(currTask->op == Operation::SaveRow)
		{
			/*eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[currTask->fd-FD_BUILTINS].ptr);
			if(hp == nullptr) return true;
			eTSDB::RetValue retval = etsdb.appendValue(*hp, std::move(*reinterpret_cast<eTSDB::Row*>(currTask->buf)));
			if(retval == eTSDB::Ok)
			{
				Log::Verbose("Row saved\n");
				reinterpret_cast<eTSDB::Row*>(currTask->buf)->~Row();
				_alloc.Deallocate(currTask->buf);
				msgQueue.delete_front();
			}*/
		}
		else if(currTask->op != Operation::Upload)
		{
			printf_("Invalid task with opcode: %d\n", currTask->op);
			msgQueue.delete_front();
		}
		return true;
	}

	void OS::eval(uint8_t* buf, int fd)
	{
		char* token =  strtok(reinterpret_cast<char*>(buf), " ");
		if(strcmp(token, "ack") == 0)
		{
			printf("echo\n");
			return;
		}
		else if(strcmp(token, "modem") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "AT") == 0)
			{
				printf("Sending AT\n");
				PortGSM::get().write(0, "AT\r", 3);
			}
			else if(strcmp(token, "comerror") == 0)
			{
				if(HAL_UART_GetError(&huart3) != HAL_OK)
				{
					printf("There is error in huart3\n");
				}
				else
				{
					printf("No error in huart3\n");
				}
			}
			else if(strcmp(token, "http") == 0)
			{
				int sockd = PortManager::open("http:labsap.upct.es:8080/api/v1/IyW4TYJVYN9hJfAydFkx/telemetry");
				PortManager::write(sockd, "{\"meas\":\"4321\"}", strlen("{\"meas\":\"4321\"}"));
			}
			else if(strcmp(token, "time") == 0)
			{
				PortGSM::get().write(nullptr, "AT+CCLK?\r", strlen("AT+CCLK?\r"));
			}
			else if(strcmp(token, "disable") == 0)
				PortGSM::get().powerOff();
			else if(strcmp(token, "enable") == 0)
				PortGSM::get().reset();
			else if(strcmp(token, "apn") == 0)
			{
				token = strtok(NULL, " ");
				PortGSM::get().setAPN(token);
			}
			else if(strcmp(token, "baudrate") == 0)
			{
				token = strtok(NULL, " ");
				PortGSM::get().setBaudRate(std::atoi(token));
			}
			else
			{
				if(token[0] == 'A')
				{
					uint8_t buf[64] = {0};
					int len = sprintf(reinterpret_cast<char*>(buf), "%s\r", token);
					PortGSM::get().write(nullptr, buf, len);
				}
			}
		}

		else if(strcmp(token, "uart") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "state") == 0)
			{
				printf("UART state: %d\n", HAL_UART_GetState(&huart3));
			}
		}

		/*else if(strcmp(token, "file") == 0)
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
					write(fd, "Error in file format\n", 21);
				}

				int fd = open(path, O_WRONLY);

				Task tsk;
				tsk.op = Operation::Upload;
				tsk.fd = fd;
				tsk.counter = remaining_file;

				msgQueue.push_back(tsk);
			}
			else if(strcmp(token, "list") == 0)
			{
				Task tsk;
				tsk.op = Operation::ListFiles;
				tsk.buf = new eTSDB::FilePage();
				tsk.counter = 0;

				etsdb.startGetNextFile(*reinterpret_cast<eTSDB::FilePage*>(tsk.buf));

				msgQueue.push_back(tsk);
			}
			else if(strcmp(token, "unlock") == 0)
			{
				etsdb.unlock();
			}
		}

		if(strcmp(token, "table") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "list") == 0)
			{
				Task tsk;
				tsk.op = Operation::ListTables;
				tsk.buf = new eTSDB::HeaderPage();
				tsk.counter = 0;

				etsdb.startGetNextHeader(*reinterpret_cast<eTSDB::HeaderPage*>(tsk.buf));

				msgQueue.push_back(tsk);
			}

			else if(strcmp(token, "get") == 0)
			{
				token = strtok(NULL, " ");

				Task open_tsk;
				open_tsk.op = Operation::OpenHeader;
				open_tsk.fd = _createFD(FDType::TS);
				open_tsk.buf = nullptr;
				for(int i = 0; i < 16; ++i) open_tsk.name_buf[i] = 0;
				for(int i = 0; i < 16 && token[i] != 0; ++i) open_tsk.name_buf[i] = token[i];
				msgQueue.push_back(open_tsk);

				Task readTable_tsk;
				readTable_tsk.op = Operation::ReadTable;
				readTable_tsk.fd = open_tsk.fd;
				readTable_tsk.counter = 0;
				msgQueue.push_back(readTable_tsk);
			}
		}*/

		else if(strcmp(token, "test") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "read") == 0)
			{
				token = strtok(NULL, " ");
				int pin = atoi(token);
				printf("%d\n",digital.digitalRead(pin));
			}
			else if(strcmp(token, "write") == 0)
			{
				token = strtok(NULL, " ");
				int pin = atoi(token);
				token = strtok(NULL, " ");
				int value = atoi(token);
				digital.digitalWrite(pin, value);
			}
			else if(strcmp(token, "pulse") == 0)
			{
				token = strtok(NULL, " ");
				int pin = atoi(token);
				printf("%d\n",digital.pulseRead(pin));
			}
			else if(strcmp(token, "vm") == 0)
			{
				vm.setStackSize(150);
				vm.setProgram(vm_prg, 0, sizeof(vm_prg));
				vm.enable(true);
			}
			else if(strcmp(token, "vm2") == 0)
			{
				const uint8_t prg[] = {1,	203,	0,	0,	0,	0,	1,	47,	62,	2,	5,	0,	0,	0,	104,	111,	108,	97,	0,	1,	204,	0,	0,	0,	10,	1,	204,	0,	0,	0,	1,	14,
										0,	1,	0,	64,	1,	10,	0,	0,	0,	1,	23,	0,	1,	0,	64,	1,	150,	0,	0,	0,	61,	127};
				vm.setStackSize(150);
				vm.setProgram(prg, 0, 54);
				vm.enable(true);
			}
			else if(strcmp(token, "sleep") == 0)
			{
				token = strtok(NULL, " ");
				if(token == NULL)
					sleepFor(10);
				else
				{
					int sleepLen = atoi(token);
					if(sleepLen == 0) sleepFor(10);
					else sleepFor(sleepLen);
				}
			}
			else if(strcmp(token, "5v") == 0)
			{
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
			}
			else if(strcmp(token, "12v") == 0)
			{
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
			}
			else if(strcmp(token, "init") == 0)
			{
				m_init = true;
				printf("inited\n");
			}
		}

		else if(strcmp(token, "flash") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "erase") == 0)
			{
				flash.eraseChip();
				write(fd, "Erasing flash\n", 14);
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

                if(len == 0) write(fd, "invalid value\n", 14);
                else
				{
					flash.readPage(len, addr);
					HAL_Delay(10);
					flash.loop();
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

                if(len == 0) write(fd, "invalid value\n", 14);
                else
				{
					flash.readPage(len, addr);
					HAL_Delay(1);
					flash.loop();
					for(int i = 0; i < len; ++i)
					{
						//tx_buffer.push_back(flash.pop());
						if(i%50 == 0) HAL_Delay(1);
					}
					printf_("\n");
					flash.unlock();
				}
			}
			else if(strcmp(token, "jedec") == 0)
			{
				uint8_t outbuf[3];
				flash.jedecId(outbuf);
				printf("%x %x %x\n", outbuf[0], outbuf[1], outbuf[2]);
			}
		}
		else if(strcmp(token, "program") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "run") == 0)
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

				msgQueue.push_back(tsk);
			}
			else if(strcmp(token, "name") == 0)
			{
				printf("%s\n", vm.getPrgName());
			}
			else if(strcmp(token, "status") == 0)
			{
				VirtualMachine::Context ctx = vm.getCurrentContext();

				printf("VMEn\tPC\tSP\tRAdr\tLVAdr\tArgAdr\n%d\t%d\t%d\t%d\t%d\t%d\n",vm.getEnabled(), ctx.programCounter, ctx.stackPointer, ctx.returnAddr,
						ctx.localVarAddr, ctx.argAddr);
			}
			else if(strcmp(token, "stop") == 0)
			{
				vm.enable(false);
				printf("VM stopped\n");
			}
			else if(strcmp(token, "start") == 0)
			{
				vm.enable();
				printf("VM started\n");
			}
		}
		else if(strcmp(token, "sdi12") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "qa") == 0)
			{
				sdi12.queryAddr();
				printf("querying addr\n");
			}
			else if(strcmp(token, "ca") == 0)
			{
				token = strtok(NULL, " ");
				char src = token[0];

				token = strtok(NULL, " ");
				char dst = token[0];

				sdi12.changeAddr(src, dst);
			}
			else if(strcmp(token, "gd") == 0)
			{
				uint8_t* res_data = sdi12.getCmdResponse();

				if(res_data == nullptr)
					printf("Not data yet\n");

				else
					printf("Data:\n%s\n", res_data);
			}
			else if(strcmp(token, "fudge") == 0)
			{
				token = strtok(NULL, " ");
				uint8_t newFudge = std::atoi(token);
				sdi12.setFudge(newFudge);
				printf("New fudge: %d\n", newFudge);
			}
			else if(strcmp(token, "status") == 0)
			{
				SDI12_Driver::Status st = sdi12.getModuleStatus();
				printf("SDI12 Status:\nTStatus: %s\nState: %s\n", sdi12.getTransceiverStatus(), sdi12.getSDI12State());
				printf("Err\tRetry\tCounter\tLRC\tMeasN\tAMeas\tMAddr\tMSize\tMIdx\tMCmd\tMDst\n%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%p\n",
						st.error, st.retry_counter, st.counter, st.last_rx_counter, st.measNumber, st.additionalMeas, st.measAddr, st.measSize, st.measIdx, st.measCommand, st.measDst);
			}
			else if(strcmp(token, "reset") == 0)
			{
				sdi12.reset();
				printf("Resetting driver\n");
			}
		}
		else if(strcmp(token, "system") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "memory") == 0)
			{
				token = strtok(NULL, " ");
				if(strcmp(token, "show") == 0)
				{
					int used_blocks = 0;
					for(int i = 0; i < 32; ++i) used_blocks += _alloc_idx[i];
					printf("Total memory: 4096 bytes\tUsedMemory: %d bytes\n", used_blocks*128);
					for(int i = 0; i < 32; ++i)
					{
						if(_alloc_ownership[i] == 0)
						{

						}
						else if(reinterpret_cast<uintptr_t>(_alloc_ownership[i]) < 100) // this is not a debug code
						{
							printf("Owner: eTSDB::Page new\tBlocks used: %d\n", _alloc_idx[i]);
						}
						else
						{
							if(Module::isModule(_alloc_ownership[i]))
								printf("Owner: %s\tBlocks used: %d\n", reinterpret_cast<Module*>(_alloc_ownership[i])->getName(), _alloc_idx[i]);
							else if(PortManager::isPort(_alloc_ownership[i]))
								printf("Owner: Port\tBlocks used: %d\n", _alloc_idx[i]);
							else if(PortManager::isSocket(_alloc_ownership[i]))
								printf("Owner: Socket\tBlocks used: %d\n", _alloc_idx[i]);
							else
								printf("Owner: Unknown\tBlocks used: %d\n", _alloc_idx[i]);
						}
						HAL_Delay(1);
					}
				}
			}
			else if(strcmp(token, "version") == 0)
			{
                int mn = Module::getModuleNumber();
                for(int i = 0; i < mn; ++i)
				{
					printf_("%s\n", Module::getModule(i)->getName());
				}
				printf_("\n");
			}
			else if(strcmp(token, "clock") == 0)
			{
				token = strtok(NULL, " ");
				if(strcmp(token, "set") == 0)
				{
					RTC_DateTypeDef rtc_date;
					RTC_TimeTypeDef rtc_time;
					char* datetok = strtok(NULL, " ");
					char* timetok = strtok(NULL, " ");

					datetok = strtok(datetok, "/");
					int year = atoi(datetok) ;
					rtc_date.Year = year - 2000;

					datetok = strtok(NULL, "/");
					rtc_date.Month = atoi(datetok);

					datetok = strtok(NULL, "/");
					rtc_date.Date = atoi(datetok);

					HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

					timetok = strtok(timetok, ":");
					rtc_time.Hours = atoi(timetok);

					timetok = strtok(NULL, ":");
					rtc_time.Minutes = atoi(timetok);

					timetok = strtok(NULL, ":");
					rtc_time.Seconds = atoi(timetok);
					HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);

				}
				else if(strcmp(token, "get") == 0)
				{
					RTC_DateTypeDef rtc_date;
					RTC_TimeTypeDef rtc_time;

					HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
					HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);

					printf("%d/%d/%d %d:%d:%d\n", rtc_date.Year+2000, rtc_date.Month, rtc_date.Date, rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
				}
			}
			else if(strcmp(token, "log") == 0)
			{
				token = strtok(NULL, " ");
				if(strcmp(token, "level") == 0)
				{
					token = strtok(NULL, " ");
					if(strcmp(token, "verbose") == 0)
					{
						Log::setLogLevel(Log::LevelVerbose);
					}
					else if(strcmp(token, "info") == 0)
					{
						Log::setLogLevel(Log::LevelInfo);
					}
					else if(strcmp(token, "warning") == 0)
					{
						Log::setLogLevel(Log::LevelWarning);
					}
					else if(strcmp(token, "error") == 0)
					{
						Log::setLogLevel(Log::LevelError);
					}
				}
				else if(strcmp(token, "file") == 0)
				{
					token = strtok(NULL, " ");

					char path[26] = {0};
					strcpy(path, "/file/");
					strcpy(path+6, token);

					int fd = open(path, O_WRONLY);

					if(fd < 0)
					{
						Log::Error("Open file error\n");
						return;
					}
					if(Log::_fd_sink > 0 && Log::_fd_sink < 16) close(Log::_fd_sink); // close last fd if needed
					Log::_fd_sink = fd; // set new fd
				}
			}
			else if(strcmp(token, "filetable") == 0) //imprimir fd
			{
				token = strtok(NULL, " ");
				if(strlen(token) == 0)
				{
					printf("Idx\tType\n");
					for(int i = 0; i < 16; ++i)
					{
						printf("%d\t",i);
						if(_fds[i].type == FDType::File)
						{
							eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fds[i].ptr);
							printf("File\tName: %s", fp->getName());
						}
						else if(_fds[i].type == FDType::TS)
						{
							eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[i].ptr);
							printf("TS\tName: %s", hp->getName());
						}
						HAL_Delay(1);
						printf("\n");
					}
				}
				else
				{
					int file_idx = std::atoi(token);
					if(_fds[file_idx].type == FDType::File)
						{
							eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fds[file_idx].ptr);
							printf("File\nName: %s\tSize: %d\nObjIdx: %d\tPagIdx: %d", fp->getName(), fp->getFileSize(), fp->getObjectIdx(), fp->getPageIdx());
						}
						else if(_fds[file_idx].type == FDType::TS)
						{
							eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[file_idx].ptr);
							printf("TS\tName: %s\tPeriod: %d\tCols: %d\n", hp->getName(), hp->getPeriod(), hp->getNumColumn());
							int i;
							char buf[8] = {0};
							for(i = 0; i < hp->getNumColumn()-1; ++i)
							{
								eTSDB::getFormatString(hp->getColumnFormat(i), buf);
								printf("%s,", buf);
							}
							eTSDB::getFormatString(hp->getColumnFormat(i), buf);
							printf("%s\n", buf);

							for(i = 0; i < hp->getNumColumn()-1; ++i)
								printf("%s,", hp->getColumnName(i));
							printf("%s\n", hp->getColumnName(i));
						}
				}
			}
			else if(strcmp(token, "name") == 0)
			{
				token = strtok(NULL, " ");
				if(strcmp(token, "set") == 0)
				{
					token = strtok(NULL, " ");
					strcpy(m_name, token);
					printf("Name changed successfully\n");
				}
				else if(strcmp(token, "get") == 0)
				{
					printf("%s\n", m_name);
				}
			}
			else if(strcmp(token, "battery") == 0)
			{
				printf("Battery: %d\n", get_bat_mv());
			}
			else if(strcmp(token, "rawbattery") == 0)
			{
				printf("Raw battery: %d\n", get_bat_raw());
			}
			else if(strcmp(token, "charge") == 0)
			{
				printf("Charging status: %s\n", getCharging() == 1 ? "charging" : "not charging");
			}
			else if(strcmp(token, "looptime") == 0)
			{
				printf("Loop time: %d\n", m_loop_time);
			}
			else if(strcmp(token, "cpu") == 0)
			{
				printf("CPU Model: %s\n", CPU_STRING_MODEL);
			}
			else if(strcmp(token, "standalone") == 0)
			{
				token = strtok(NULL, " ");
				if(strcmp(token, "enable") == 0)
				{
					fw_standalone = 1;
				}
				else
				{
					fw_standalone = 0;
				}
			}
		}
		else if(strcmp(token, "radio") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "receive") == 0) // without error handling
			{
				token = strtok(NULL, " ");
				int continuous = std::atoi(token);
				if(continuous) continuous = 1;
				printf("receiving ");
				radio.receive(continuous);
				if(continuous)
					printf("continuous");
				else
					printf("single");
				printf("\n");
			}
			else if(strcmp(token, "set") == 0)
			{
				token = strtok(NULL, " ");
				if(strcmp(token, "bandwith") == 0)
				{
					token = strtok(NULL, " ");
					int bw = std::atoi(token);
					if(radio.setBandwidth(bw) == -1)
						printf("Invalid value for bandwidth: %d\n", bw);
					else
						printf("Bandwidth set\n");
				}
				else if(strcmp(token, "spreadfactor") == 0)
				{
					token = strtok(NULL, " ");
					int sf = std::atoi(token);
					if(radio.setSpreadingFactor(sf) == -1)
						printf("Invalid value for spread factor: %d\n", sf);
					else
						printf("Spreading factor set\n");
				}
				else if(strcmp(token, "syncword") == 0)
				{
					token = strtok(NULL, " ");
					int sw = std::atoi(token);
					if(radio.setSyncWord(sw) == -1)
						printf("Invalid value for sync word: %d\n", sw);
					else
						printf("Sync word set\n");
				}
				else if(strcmp(token, "txpower") == 0)
				{
					token = strtok(NULL, " ");
					int txpower = std::atoi(token);
					token = strtok(NULL, " ");
					int tx_pin = std::atoi(token);
					if(tx_pin > 1) tx_pin = 1;
					if(radio.setTxPower(txpower, tx_pin) == -1)
						printf("Invalid value for TxPower: %d\n", txpower);
					else
						printf("TxPower with %ddB and %d pin\n", txpower, tx_pin);
				}
				else if(strcmp(token, "recvtest") == 0)
				{
					radio.receive(1);
				}
			}
			else if(strcmp(token, "send") == 0)
			{
				token = strtok(NULL, " ");
				radio.send(strlen(token), reinterpret_cast<uint8_t*>(token));
				radio.receive(0);
				printf("%s sent\n", token);
			}
			else if(strcmp(token, "version") == 0)
			{
				int chipid = radio.getChipVersion();
				printf("Chip ID: 0x%x\n", chipid);
			}

			else if(strcmp(token, "rx_stat") == 0)
				printf("Modem status: %d\n", radio.getModemStatus());

			else if(strcmp(token, "irq_flags") == 0)
				printf("IRQ Flags: %d\n", radio.getIRQFlags());

			else if(strcmp(token, "irq_masks") == 0)
				printf("IRQ Flags: %d\n", radio.getIRQFlagMasks());

			else if(strcmp(token, "op_status") == 0)
				printf("OpStatus: %d\n", radio.readStatus());

			else if(strcmp(token, "rx_bytes") == 0)
				printf("rx_bytes: %d\n", radio.getRxNbBytes());

			else if(strcmp(token, "rx_headers") == 0)
				printf("rx_hdrs: %d\n", radio.getRxHeaderCnt());

			else if(strcmp(token, "rx_pckts") == 0)
				printf("rx_pckts: %d\n", radio.getRxPacketCnt());

			else if(strcmp(token, "rssi") == 0)
				printf("RSSI: %d\n", radio.getRSSI());

			else if(strcmp(token, "fifo") == 0)
			{
				token = strtok(NULL, " ");
				int addr = std::atoi(token);
				token = strtok(NULL, " ");
				int len = std::atoi(token);

				uint8_t buf[256] = {0};

				radio.getFifo(addr, len, buf);

				printf("FIFO Content:\n");

				for(int i = 0; i < len; ++i)
					printf("%x ", buf[i]);
			}
			else if(strcmp(token, "reghop") == 0)
			{
				printf("RegHop: %x\n", radio.getRegHopChannel());
			}
		}
		else if(strcmp(token, "adc") == 0)
		{
			token = strtok(NULL, " ");
			int chan = std::atoi(token);
			printf("ADC: %d\n", get_adc_raw(chan));
		}
	}

	void OS::bin_eval(uint8_t* ebuf, int length, int fd)
	{
		if(fd == -1) return;
		if(ebuf[0] == 'a') // ack
		{
			write(fd, "a", 1);
			close(fd);
		}
		else if(ebuf[0] == 's') // system
		{
			if(ebuf[1] == 'c') // config
			{
				if(ebuf[2] == 'g') // get
				{
					uint8_t outbuf[67] = {0}; // 64+3
					outbuf[0] = 's';
					outbuf[1] = 'c';
					outbuf[2] = 'g';
					strcpy(reinterpret_cast<char*>(outbuf+3), m_name);
					int outlen = strlen(m_name); //limit to 32
					for(int i = 0; i < Module::getModuleNumber(); ++i)
					{
						Module::getModule(i)->getConfig(outbuf+outlen+3);
						outlen += Module::getModule(i)->getConfigSize();
					}
					write(fd, outbuf, outlen+3);
				}
			}
			else if(ebuf[1] == 'n') // name
			{
				if(ebuf[2] == 'g')
				{
					uint8_t outbuf[67] = {0};
					int num_len = strlen(m_name);
 					outbuf[0] = 's';
					outbuf[1] = 'n';
					outbuf[2] = 'g';

					strcpy(reinterpret_cast<char*>(outbuf+3), m_name);
					write(fd, outbuf, 3 + num_len);
				}
				else if(ebuf[2] == 's')
				{
					for(int i = 0; i < 16; ++i) m_name[i] = 0;
					strcpy(m_name, reinterpret_cast<char*>(ebuf+3)); // guardar
					//saveConfig();
				}
			}

			else if(ebuf[1] == 'M') // memory
			{
				int used_blocks = 0;
				for(int i = 0; i < 32; ++i) used_blocks += _alloc_idx[i];
				uint8_t outbuf[10] = {0};
				outbuf[0] = 's';
				outbuf[1] = 'M';
				outbuf[2] = used_blocks & 0xff;
				outbuf[3] = (used_blocks >> 8) & 0xff;
				outbuf[4] = 0x00;
				outbuf[5] = 0x10;
 				write(fd, outbuf, 6);
				/*for(int i = 0; i < 32; ++i)
				{
					if(_alloc_idx[i] == 0) // free
					{

					}
					else if(_alloc_idx[i] < 100) // this is not a debug code
					{

					}
					else
					{

					}
				}*/
			}

			else if(ebuf[1] == 'f') // filetable
			{
				uint8_t outbuf[128] = {0};
				outbuf[0] = 's';
				outbuf[1] = 'f';
				int num_file = 0;
				for(int i = 0; i < 16; ++i) if(_fds[i].type != FDType::None) num_file++;
				outbuf[2] = num_file;
				int buf_idx = 3;
				for(int i = 0; i < 16; ++i)
				{
					if(_fds[i].type == FDType::File)
					{
						eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fds[i].ptr);
						outbuf[buf_idx++] = static_cast<uint8_t>(FDType::File);
						outbuf[buf_idx++] = i;
						strcpy(reinterpret_cast<char*>(outbuf+buf_idx), reinterpret_cast<char*>(fp->getName()));
						buf_idx += strlen(reinterpret_cast<char*>(fp->getName()));
						buf_idx++;
					}
					else if(_fds[i].type == FDType::TS)
					{
						eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[i].ptr);
						outbuf[buf_idx++] = static_cast<uint8_t>(FDType::TS);
						outbuf[buf_idx++] = i;
						strcpy(reinterpret_cast<char*>(outbuf+buf_idx), reinterpret_cast<char*>(hp->getName()));
						buf_idx += strlen(reinterpret_cast<char*>(hp->getName()));
						buf_idx++;
					}
				}
				write(fd, outbuf, buf_idx);
			}

			else if(ebuf[1] == 's') // status
			{

			}

			else if(ebuf[1] == 'l') // log
			{
				if(ebuf[2] == 0) Log::setLogLevel(Log::LogLevel::LevelError);
				else if(ebuf[2] == 1) Log::setLogLevel(Log::LogLevel::LevelWarning);
				else if(ebuf[2] == 2) Log::setLogLevel(Log::LogLevel::LevelInfo);
				else if(ebuf[2] == 3) Log::setLogLevel(Log::LogLevel::LevelVerbose);
				else if(ebuf[2] == 'e')
				{
					if(Log::_fd_sink)
						close(Log::_fd_sink);
					Log::_fd_sink = fd;
					return;
				}
				else if(ebuf[2] == 'd')
				{
					if(Log::_fd_sink != 0)
						close(Log::_fd_sink);
					Log::_fd_sink = 0;
				}
			}
			else if(ebuf[1] == 'v') // version
			{
				uint8_t outbuf[10] = {0};
				outbuf[0] = 's';
				outbuf[1] = 'v';
				outbuf[2] = FWVMINOR;
				outbuf[3] = FWVMAJOR;

				write(fd, outbuf, 4);
			}
			else if(ebuf[1] == 'm') // modules
			{
				uint8_t outbuf[118] = {0};
				outbuf[0] = 's';
				outbuf[1] = 'm';

				int buf_len = 0;
				for(int i = 0; i < Module::getModuleNumber(); ++i)
				{
					Module* mod = Module::getModule(i);
					buf_len += sprintf(reinterpret_cast<char*>(outbuf+2+buf_len), "%c,%s,%d,%d;", mod->bin_id,
										mod->getName(), mod->majorVersion, mod->minorVersion);
				}

				write(fd, outbuf, buf_len+2);
			}
			else if(ebuf[1] == 'C') // clock
			{
				if(ebuf[2] == 'g')
				{
					uint8_t outbuf[15] = {0};
					outbuf[0] = 's';
					outbuf[1] = 'C';
					outbuf[2] = 'g';
					timeETSDB().serialize(outbuf+3);
					write(fd, outbuf, 8);
				}
				else if(ebuf[2] == 's')
				{
					eTSDB::Date new_time;
					new_time.deserialize(ebuf+3);
					RTC_DateTypeDef sDate;
					RTC_TimeTypeDef sTime;

					sDate.Year = new_time.year%100;
					sDate.Month = new_time.month;
					sDate.Date = new_time.day;

					sTime.Hours = new_time.hour;
					sTime.Minutes = new_time.minute;
					sTime.Seconds = new_time.second;

					setTime(sDate, sTime);

					uint8_t outbuf[] = {'s','C','s'};
					write(fd, outbuf, 3);
				}
			}
			close(fd);
		}
		else if(ebuf[0] == 'f')
		{
			if(ebuf[1] == 'u')
			{
				char path[26];
				for(int i = 0; i < 26; ++i) path[i] = 0;
				strcpy(path, "/file/");
				for(int i = 0; i < length-4; ++i) path[i+6] = ebuf[i+4];

				int remaining_file = ebuf[2] | (ebuf[3] << 8);
				if(remaining_file == 0)
				{
					const uint8_t outbuf[] = {'f', 'u', 'e'};
					write(fd, outbuf, sizeof(outbuf));
					return;
				}

				int file_fd = open(path, O_WRONLY);

				Task tsk;
				tsk.op = Operation::Upload;
				tsk.fd = fd;
				tsk.counter = ebuf[2] | (ebuf[3] << 8);
				tsk.name_buf[0] = file_fd;

				msgQueue.push_back(tsk);
			}
			else if(ebuf[1] == 'd')
			{
				char path[26];
				for(int i = 0; i < 26; ++i) path[i] = 0;
				strcpy(path, "/file/");
				strcpy(path+6, reinterpret_cast<char*>(ebuf+2));
				Task tsk;
				tsk.op = Operation::DownloadFile;
				tsk.counter = 0;
				tsk.fd = open(path, O_RDONLY);
				msgQueue.push_back(tsk);
			}
			else if(ebuf[1] == 'l')
			{
				Task tsk;
				tsk.op = Operation::ListFiles;
				tsk.buf = new eTSDB::FilePage();
				tsk.counter = 1;
				tsk.fd = fd;

				//etsdb.startGetNextFile(*reinterpret_cast<eTSDB::FilePage*>(tsk.buf));

				msgQueue.push_back(tsk);
			}
			else if(ebuf[1] == 'e')
			{
				flash.eraseChip();
				flash.unlock();
				close(fd);
			}
		}
		else if(ebuf[0] == 't')
		{
			close(fd);
			return;
			if(ebuf[1] == 'd')
			{
				Task open_tsk;
				open_tsk.op = Operation::OpenHeader;
				open_tsk.fd = _createFD(FDType::TS);
				open_tsk.buf = nullptr;
				for(int i = 0; i < 16; ++i) open_tsk.name_buf[i] = 0;
				for(int i = 0; i < 16 && ebuf[i+2] != 0; ++i) open_tsk.name_buf[i] = ebuf[i+2];
				msgQueue.push_back(open_tsk);

				Task readTable_tsk;
				readTable_tsk.op = Operation::ReadTable;
				readTable_tsk.fd = open_tsk.fd;
				readTable_tsk.counter = 2;
				msgQueue.push_back(readTable_tsk);
			}
			else if(ebuf[1] == 'l')
			{
				/*Task tsk;
				tsk.op = Operation::ListTables;
				tsk.buf = new eTSDB::HeaderPage();
				tsk.counter = 1;

				etsdb.startGetNextHeader(*reinterpret_cast<eTSDB::HeaderPage*>(tsk.buf));

				msgQueue.push_back(tsk);*/
			}
		}
		else if(ebuf[0] == 'p')
		{
			if(ebuf[1] == 'r') // run
			{
				vm.enable(true);
				uint8_t outbuf[] = {'p', 'r'};
				write(fd, outbuf, 2);
			}
			else if(ebuf[1] == 's') //stop
			{
				vm.enable(false);
				uint8_t outbuf[] = {'p', 's'};
				write(fd, outbuf, 2);
			}
			else if(ebuf[1] == 'l') // load
			{
				char buf[32] = {0};
				strcpy(buf, "/file/");
				for(int i = 0; i < length-2; ++i) buf[i+6] = ebuf[i+2];

				int fd = open(buf, O_RDONLY);

				Task tsk;
				tsk.op = Operation::LoadProgram;
				tsk.fd = fd;
				tsk.buf = nullptr;
				tsk.counter = 0;

				msgQueue.push_back(tsk);

				uint8_t outbuf[] = {'p', 'l'};
				write(fd, outbuf, 2);
			}
			close(fd);
		}
		else
		{
			uint8_t outbuf[118] = {0};
			int out_len = 0;
			for(int i = 0; i < Module::getModuleNumber(); ++i)
			{
				if(Module::getModule(i)->bin_id == ebuf[0])
					out_len = Module::getModule(i)->bin_eval(ebuf+1, length-1, outbuf+1);
				if(out_len > 0)
				{
					out_len++;
					outbuf[0] = Module::getModule(i)->bin_id;
					write(fd, outbuf, out_len);
					out_len = 0;
					break;
				}
			}
			close(fd);
		}
	}

	void OS::radio_eval()
	{

	}

	int OS::open(char* path, int oflag)
	{
		if(path[0] != '/')
			return PortManager::open(path);

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
			msgQueue.push_back(tsk_creat);
            return fd;
		}
		else if(strcmp(pch, "etsdb") == 0)
		{
			/*int fd = _createFD(FDType::TS);
			pch = strtok(NULL, "/");
			eTSDB::PageAccessMode am;
			if(oflag == O_RDONLY) am = eTSDB::PageAccessMode::PageRead;
			if(oflag == O_WRONLY)
			{
				errno = EUNSPEC;
				return -1;
			}
			if(oflag == O_UPDATE) am = eTSDB::PageAccessMode::PageWriteUpdate;
			return fd;*/
			return -1;
		}
		errno = EUNSPEC;
		return -1;
	}

	int OS::read(int fd, void* buf, size_t count)
	{
		uint8_t* _buf = static_cast<uint8_t*>(buf);
		if(fd == 0)
		{
			//std::size_t i;
			//for(i = 0; i < count && i < rx_buffer.idx; ++i) _buf[i] = rx_buffer.buf[i];
			return 0;
		}
		else
		{
			FileDescriptor& _fd = _fds[fd-FD_BUILTINS];
			if(_fd.type == FDType::File)
			{
				if(_fd.buf == nullptr) _fd.buf = reinterpret_cast<uint8_t*>(_alloc.Allocate(128, this));
				eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fd.ptr);
				if(fp->getFileSize() <= _fd.bytes_read)
				{
					errno = EIO;
					return -1;
				}
				uint8_t* ui8b = reinterpret_cast<uint8_t*>(buf);
				for(std::size_t i = 0; i < count; ++i)
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
							fp->freeDataPage(); // mark as unread to make sure that data is not read twice

							Task tsk;
							tsk.fd = fd;
							tsk.op = Operation::ReadNext;
							msgQueue.push_front(tsk);
						}
					}
					ui8b[i] = _fd.buf[_fd.buf_idx++];
				}
				uint8_t this_max_bytes_read = fp->getBytesRead()-_fd.bytes_read;
				if(count > this_max_bytes_read) count = this_max_bytes_read;
				_fd.bytes_read += count;
				return count;
			}

		}
		errno = EUNSPEC;
		return -2;
	}

	int OS::write(int fd, const void* buf, size_t count)
	{
		if(fd == 0)
		{
			PortUART::get().write(0, buf, count);

			return count;
		}
		else if(fd < SOCKET_DESCRIPTOR_OFFSET)
		{
			FileDescriptor& _fd = _fds[fd-FD_BUILTINS];
            if(_fd.type == FDType::None)
			{
				errno = EBADF;
				return -2;
			}
			if(_fd.type == FDType::File)
			{
				if(_fd.ptr == nullptr) // there is no page yet
				{
					if(_fd.buf == nullptr)
						_fd.buf = reinterpret_cast<uint8_t*>(_alloc.Allocate(128, this));
                    for(std::size_t i = 0; i < count; ++i)
						_fd.buf[_fd.buf_idx++] = reinterpret_cast<const uint8_t*>(buf)[i];
				}
				else //there is page
				{
					if(_fd.buf != nullptr) //but there is unwritten things in buffer
					{
	 					uint8_t merge_buf[128];

						for(int i = 0; i < _fd.buf_idx; ++i)
							merge_buf[i] = _fd.buf[i];

						for(std::size_t i = 0; i < count; ++i)
							merge_buf[i+_fd.buf_idx] = reinterpret_cast<const uint8_t*>(buf)[i];

						//TODO
						//etsdb.writeFile(*reinterpret_cast<eTSDB::FilePage*>(_fd.ptr), merge_buf, count+_fd.buf_idx);

						_alloc.Deallocate(_fd.buf);
						_fd.buf = nullptr;
						_fd.buf_idx = 0;
					}
					else
					{

						//etsdb.writeFile(*reinterpret_cast<eTSDB::FilePage*>(_fd.ptr), reinterpret_cast<const uint8_t*>(buf), count);
					}
					return count;
				}
			}
		}
		else
		{
			PortManager::write(fd, buf, count); // it frees sockets if self managed
			return count;
		}
		errno = EUNSPEC;
		return -2;
	}

	int OS::close(int fd) // this return 0 if there is no operation pending
	{
		if(fd == 0) return EBADF;
		if(fd >= SOCKET_DESCRIPTOR_OFFSET) return PortManager::close(fd);
		FileDescriptor& _fd = _fds[fd-FD_BUILTINS];
		if(_fd.type == FDType::None) return EBADF;
		if(_fd.type == FDType::File)
		{
			/*eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fd.ptr);
            if(etsdb.closeFile(*fp) != eTSDB::RetValue::Ok)
			{
                return EBUSY;
			}
			if(_fd.buf != nullptr) _alloc.Deallocate(_fd.buf);
			delete fp;
            _alloc.Deallocate(_fd.ptr);
            _fd.ptr = nullptr;*/
		}
		else if(_fd.type == FDType::TS)
		{
			_alloc.Deallocate(_fd.ptr);
		}
		_fd.type = FDType::None;
		return 0;
	}

	uint64_t OS::time()
	{
		return timeETSDB().timestamp();
	}

	eTSDB::Date OS::timeETSDB()
	{
		eTSDB::Date retDate;

		RTC_DateTypeDef rtc_date;
		HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
		retDate.year = rtc_date.Year+2000;
		retDate.month = rtc_date.Month;
		retDate.day = rtc_date.Date;

		RTC_TimeTypeDef rtc_time;
        HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
        retDate.hour = rtc_time.Hours;
        retDate.minute = rtc_time.Minutes;
        retDate.second = rtc_time.Seconds;
        retDate.exists = 0;
        return retDate;
	}

	static inline uint32_t toSecs(RTC_TimeTypeDef& time)
	{
		return time.Hours*3600 + time.Minutes*60 + time.Seconds;
	}

	void OS::setTime(RTC_DateTypeDef& date, RTC_TimeTypeDef& time)
	{
		bool alarmEnabled = ((hrtc.Instance->CRH & 2) != 0);

		//RTC_TimeTypeDef nowTime;
		//HAL_RTC_GetTime(&hrtc, &nowTime, RTC_FORMAT_BIN);
		//uint32_t timeSecs = toSecs(nowTime);

		//int32_t timeDelta = toSecs(time) - timeSecs;

		HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
		HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);

		HAL_RTCEx_BKUPWrite(&hrtc, 2, hrtc.DateToUpdate.Year);
		HAL_RTCEx_BKUPWrite(&hrtc, 3, hrtc.DateToUpdate.Month);
		HAL_RTCEx_BKUPWrite(&hrtc, 4, hrtc.DateToUpdate.Date);

		if(alarmEnabled)
		{
			uint32_t alarmTS = (hrtc.Instance->ALRH << 16) | hrtc.Instance->ALRL;
			uint32_t clockTS = (hrtc.Instance->CNTH << 16) | hrtc.Instance->CNTL;
			if(alarmTS < clockTS)
			{
				HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
				RTC_ISR(); // se llama al ISR
			}
		}

	}

	int16_t OS::get_adc_raw(int channel)
	{
		switch(channel)
		{
		case ADC_SE_0:
			return adc_data[0];

		case ADC_SE_1:
			return adc_data[1];

		case ADC_SE_2:
			return adc_data[2];

		case ADC_SE_3:
			return adc_data[3];

		case ADC_SE_4:
			return adc_data[4];

		case ADC_SE_5:
			return adc_data[5];

		case ADC_SE_6:
			return adc_data[6];

		default:
			return -1;
		}
	}

	int OS::get_adc_mv(int channel)
	{
		return get_adc_raw(channel)*1200/adc_data[6];
	}

	int16_t OS::get_bat_raw()
	{
		return adc_data[5];
	}

	int OS::get_bat_mv()
	{
		return adc_data[5] * 5040 / adc_data[6] / 3;
	}

	float OS::get_battery()
	{
		return get_bat_mv()/1000.0;
	}

	int OS::getCharging()
	{
		return HAL_GPIO_ReadPin(CHG_GPIO_Port, CHG_Pin) == GPIO_PIN_RESET ? 1 : 0;
	}

	void OS::enablePower(int enable)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11 | GPIO_PIN_12, enable != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	void OS::setAlarm(RTC_TimeTypeDef& time)
	{
		RTC_AlarmTypeDef sAlarm;
		sAlarm.AlarmTime = time;
		sAlarm.Alarm = RTC_ALARM_A;
		HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
	}

	void OS::setAlarm(int secs)
	{
		Log::Info("Set alarm at %d s\n", secs);
		RTC_AlarmTypeDef sAlarm;
		secs %= 86400;

		sAlarm.AlarmTime.Hours = (secs / 3600);
		secs = secs - sAlarm.AlarmTime.Hours*3600;

		sAlarm.AlarmTime.Minutes = secs / 60;

		sAlarm.AlarmTime.Seconds = secs % 60;

		sAlarm.Alarm = RTC_ALARM_A;

		HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
	}

	int OS::sleepFor(int s)
	{
		printf("Sleeping for %d seconds\n",s);
		HAL_Delay(100);
		RTC_TimeTypeDef sTime;
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

		int secs = sTime.Hours*3600+sTime.Minutes*60+sTime.Seconds;
		secs += s;

		RTC_AlarmTypeDef sAlarm;
		sAlarm.Alarm = RTC_ALARM_A;
		sAlarm.AlarmTime.Hours = secs/3600;
		secs %= 3600;
		sAlarm.AlarmTime.Minutes = secs/60;
		sAlarm.AlarmTime.Seconds = secs%60;

		if(HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
		{
			printf("Error on AlarmA\n");
			HAL_Delay(100);
		};

		prepareSleep();

		return 0;
	}

	void OS::prepareSleep()
	{
		vm.pauseExec(); //well well...
		PortGSM::get().powerOff();

		m_lpEnabled = true;
	}

	void OS::wakeUp()
	{
		m_lpEnabled = false;

		PortGSM::get().reset();
		PortUART::get().reset();
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
				return i+FD_BUILTINS;
			}
		}
		return -1; // full file descriptor table
	}

	void OS::_deleteFD(int fd)
	{
		if(fd < FD_BUILTINS) return;
		fd -= FD_BUILTINS;
        if(_fds[fd].type == FDType::File)
		{
            if(_fds[fd].ptr != nullptr) _alloc.Deallocate(_fds[fd].ptr);
            if(_fds[fd].buf != nullptr) _alloc.Deallocate(_fds[fd].buf);
		}
	}

	void OS::sleep()
	{
		//Log::Verbose("Going to sleep\n");
		PortUART::get().powerOff();

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_RESET); // apagar el power

		HAL_TIM_Base_Stop_IT(&htim6);
		HAL_ADC_Stop_DMA(&hadc1);
		HAL_NVIC_DisableIRQ(SPI1_IRQn);
		HAL_NVIC_DisableIRQ(USART1_IRQn);

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

		HAL_SuspendTick();
		HAL_PWR_EnableSleepOnExit();
		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
		//HAL_PWR_EnterSTANDBYMode();
		SystemClock_Config(); // being in stopmode changes system clock to HSI instead of PLL
		HAL_ResumeTick();

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

		HAL_TIM_Base_Start_IT(&htim6);
		HAL_ADC_Start_DMA(&hadc1, reinterpret_cast<uint32_t*>(adc_data), 7);

		HAL_NVIC_EnableIRQ(SPI1_IRQn);
		HAL_NVIC_EnableIRQ(USART1_IRQn);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_SET);
		HAL_Delay(1000);
	}

	void OS::saveConfig()
	{
		/*HAL_FLASH_Unlock();

		uint32_t flashaddr = 0x0807f800;

		FLASH_EraseInitTypeDef ferase;
		ferase.NbPages = 1;
		ferase.Banks = FLASH_BANK_1;
		ferase.PageAddress = flashaddr; // page 256
		ferase.TypeErase = FLASH_TYPEERASE_PAGES;

		uint32_t flashError;

		HAL_FLASHEx_Erase(&ferase, &flashError);

		FLASH_WaitForLastOperation(1000);

		if(flashError != 0xffffffff)
		{
			return;
		}

		for(int i = 0; i < 32; i = i+4) // store name
		{
			uint32_t flash_val = 0;
			for(int j = 0; j < 4; ++j)
			{
				if(m_name[i+j] == 0)
				{
					i = 32;
					break;
				}
				else
				{
					flash_val |= m_name[i+j] << 8*j;
				}
			}

			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashaddr, flash_val);
			flashaddr += 4;
			FLASH_WaitForLastOperation(1000);
		}

		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flashaddr, 0);

		flashaddr += 4;

		for(int i = 0; i < Module::getModuleNumber(); ++i)
		{
			Module* mod = Module::getModule(i);
			char buf[64] = {0};
			buf[0] = mod->bin_id;
			const char** params = mod->getParamList();
			for(int j = 0; j < mod->getNumParam(); j++)
			{
				int buf_idx = 2;
				strcpy(buf+1, params[j]);
				buf_idx += strlen(params[j]); // dejar el 0
				uint8_t paramSize = mod->getParamSizes()[j];
				int dst;
				if(paramSize == 1)
				{
					mod->getParam(params[j], &dst);
					buf[buf_idx++] = dst & 0xff;
				}
				else if(paramSize == 2)
				{
					mod->getParam(params[j], &dst);
					buf[buf_idx++] = dst & 0xff;
					buf[buf_idx++] = (dst >> 8) & 0xff;
				}
				else if(paramSize == 4)
				{
					mod->getParam(params[j], &dst);
					buf[buf_idx++] = dst & 0xff;
					buf[buf_idx++] = (dst >> 8) & 0xff;
					buf[buf_idx++] = (dst >> 16) & 0xff;
					buf[buf_idx++] = (dst >> 24) & 0xff;
				}
				uint32_t flash_val;
				for(int k = 0; k < buf_idx; k += 4)
				{
					flash_val = buf[k];
					flash_val |= buf[k+1] << 8;
					flash_val |= buf[k+2] << 16;
					flash_val |= buf[k+3] << 24;
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashaddr, flash_val);
					flashaddr += 4;
					FLASH_WaitForLastOperation(1000);
				}
			}
		}

		HAL_FLASH_Lock();*/
	}

	void OS::loadConfig()
	{
		/*uintptr_t flash_ptr = (uintptr_t) 0x0807f800;
		if(*reinterpret_cast<uint8_t*>(flash_ptr) == 0) return; // empty config
		for(int i = 0; i < 32; ++i)
		{
			m_name[i] = *reinterpret_cast<uint8_t*>(flash_ptr++);
			if(m_name[i] == 0)
				break;
		}
		//align 4
		flash_ptr = (flash_ptr+3) & ~(3);

		uint16_t addr = *reinterpret_cast<uint16_t*>(flash_ptr); //LLAddr

		flash_ptr += 4;

		while(*reinterpret_cast<uint8_t*>(flash_ptr) != 0)
		{
			uint8_t module_bin_id = *reinterpret_cast<uint8_t*>(flash_ptr++);

			Module* mod = Module::getModuleById(module_bin_id);
			if(mod == nullptr) return;

			char* param = reinterpret_cast<char*>(flash_ptr);
			int namelength = strlen(param);

			int paramlength = 0;
			for(int i = 0; i < mod->getNumParam(); ++i)
			{
				if(strcmp(param, mod->getParamList()[i]) == 0)
				{
					paramlength = mod->getParamSizes()[i];
					break;
				}
			}
			if(paramlength == 0) return;

			flash_ptr += namelength+1;

			int paramValue = *reinterpret_cast<int*>(flash_ptr);

			flash_ptr += paramlength;
			flash_ptr = (flash_ptr+3) & ~(3);

			mod->setParam(param, paramValue);
		}*/
	}
}
