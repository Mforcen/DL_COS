#include "FwLogger.h"

namespace FwLogger
{
	OS* OS::ptr = nullptr;
	OS& OS::get()
	{
		return *ptr;
	}

	OS::OS():_alloc(_alloc_buf, _alloc_idx, _alloc_ownership, 4096), flash(&hspi1, FLASH_CS), etsdb(0, 8192*1024, &hspi1, FLASH_CS, &_alloc), sdi12(SDI12_0),
	radio(&hspi1, LORA_DIO0, LORA_DIO1, LORA_RXEN, LORA_TXEN, LORA_CS, LORA_RST)//, disp(&hi2c2, 0x78, 128, 32)
	{
		for(int i = 0; i < 32; ++i) m_name[i] = 0;
		errno = 0;

		m_pStatus = ParserStatus::Start;
		m_pSize = 0;
		m_pIndex = 0;

		m_rtcFlag = false;
		m_name[0] = 'h';
		m_name[1] = 'o';
		m_name[2] = 'l';
		m_name[3] = 'a';
		m_name[4] = 0;

		Log::setLogLevel(Log::LevelError);
		//ctor
	}

	void OS::setOS(OS* os)
	{
		ptr = os;
	}

	void OS::init()
	{
		radio.init(868000000);
		/*disp.Init();
		disp.setCursor(0,0);
		disp.fill();
		disp.writeString("hola");
		disp.updateScreen();*/
		radio.receive(1);

		m_lpEnabled = false;
	}

	void OS::push_rx(uint8_t c)
	{
		m_pLastRecv = HAL_GetTick();
		Task* currTask = _ops.at_front();
		if(currTask->op == Operation::Upload)
		{
			write(currTask->fd, &c, 1);

			if(--m_pSize <= 0)
			{
				Task tsk;
				tsk.op = Operation::Close;
				tsk.fd = currTask->fd;

				_ops.push_back(tsk);
				_ops.delete_front();
			}
		}
		else
		{
			if(m_pStatus == ParserStatus::Start)
			{
				rx_buffer.idx = 0;
				if(c == 0x0b)
				{
					m_pStatus = ParserStatus::BinSize;
					m_pSize = 2;
				}
				else if(std::isalnum(c))
				{
					if(c == '\b' || c == 127) return;
					rx_buffer.push_back(c);
					m_pStatus = ParserStatus::AsciiCommand;
				}
			}
			else if(m_pStatus == ParserStatus::AsciiCommand)
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
					_ops.push_front(tsk);
				}
				else
					rx_buffer.push_back(c);
			}
			else if(m_pStatus == ParserStatus::BinSize)
			{
				rx_buffer.push_back(c);
				if(--m_pSize <= 0)
				{
					m_pSize = (rx_buffer.buf[0] & 0xff) | ((rx_buffer.buf[1] << 8) & 0xff00);
					m_pIndex = 0;
					m_pCmdBuf = reinterpret_cast<uint8_t*>(_alloc.Allocate(m_pSize, reinterpret_cast<uintptr_t>(this)));
					m_pStatus = ParserStatus::BinCommand;
				}
			}
			else if(m_pStatus == ParserStatus::BinCommand)
			{
				m_pCmdBuf[m_pIndex++] = c;
				if(m_pIndex == m_pSize)
				{
					Task tsk;
					tsk.op = Operation::BinEval;
					tsk.buf = m_pCmdBuf;
					m_pCmdBuf = nullptr;

					tsk.counter = m_pSize;
					m_pSize = 0;
					m_pIndex = 0;

					_ops.push_back(tsk);
					m_pStatus = ParserStatus::Start;
				}
			}
		}
	}

	void OS::RTC_ISR()
	{
		m_rtcFlag = true;
	}

	void OS::loop()
	{
		volatile bool work_left = false;
		for(int i = 0; i < Module::getModuleNumber(); ++i)
		{
			work_left = work_left | Module::getModule(i)->loop();
		}

		work_left = task_loop() | work_left;

		if(work_left == false)
		{
			if(m_pendingTask == true) // there was a pending task in the previous loop
			{
				//hacer cosas
				_lastTaskTime = HAL_GetTick();
			}
			else
			{
				if((HAL_GetTick()-_lastTaskTime) > 5000)
				{
					_lastTaskTime = HAL_GetTick();
					sleep();
				}
			}
		}

		m_pendingTask = work_left;

		if(m_lpEnabled && !m_pendingTask)
		{
			sleep();
		}

		if(m_pStatus != ParserStatus::Start && (HAL_GetTick()-m_pLastRecv) > 5000)
		{
			m_pStatus = ParserStatus::Start;
			m_pIndex = 0;
			m_pSize = 0;
			rx_buffer.clear();
		}

		if(m_rtcFlag) // if it has to wake up from sleep
		{
			m_rtcFlag = 0;
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

		if(vm.getSaveFlag()) //if it has to save data
		{
            eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[vm.HeaderFD-1].ptr);
            if(hp != nullptr)
			{
				eTSDB::Row* dataRow = new (_alloc.Allocate(sizeof(eTSDB::Row), reinterpret_cast<uintptr_t>(this))) eTSDB::Row();
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
				_ops.push_back(op);

				vm.ackSaveFlag();
			}
		}

		if(radio.available())
		{
			radio_eval();
		}
	}

	bool OS::task_loop()
	{
		Task* currTask = _ops.at_front();

		if(currTask == nullptr)
			return false;

		if(currTask->op == Operation::Eval)
		{
			eval();
			_ops.delete_front();
			rx_buffer.clear();
		}

		else if(currTask->op == Operation::BinEval)
		{
			bin_eval(reinterpret_cast<uint8_t*>(currTask->buf));
			_alloc.Deallocate(currTask->buf);
			_ops.delete_front();
		}

		else if(currTask->op == Operation::OpenFile)
		{
			if(etsdb.openFile(currTask->name_buf, static_cast<eTSDB::PageAccessMode>(currTask->counter)) == eTSDB::Ok)
				currTask->op = Operation::GetPage;
		}

		else if(currTask->op == Operation::OpenHeader)
		{
			eTSDB::RetValue retval;
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
			}
		}

		else if(currTask->op == Operation::ListTables) // TODO (forcen#1#): Add binary mode for these functions
		{
			eTSDB::RetValue retval = etsdb.checkGetNextHeader();
			eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(currTask->buf);
			if(retval == eTSDB::Ok)
			{
				if(currTask->counter == 0)
					printf("%s\tColumns: %d\n", hp->getName(), hp->getNumColumn());
				else
				{

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
				_ops.delete_front();
			}
		}

		else if(currTask->op == Operation::ReadTable)
		{
			eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[currTask->fd-1].ptr);
			if(currTask->counter & 1 == 0)
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
						_ops.delete_front();
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
							for(int i = 0; i < 16; ++i)
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
			}
		}

		else if(currTask->op == Operation::GetPage)
		{
			eTSDB::Page* page = etsdb.getPage();
			if(page != nullptr)
			{
				if(page->getType() == eTSDB::PageType::FileType)
				{
					eTSDB::FilePage* fp = new eTSDB::FilePage();
					_fds[currTask->fd-1].ptr = fp;
					fp->copy(reinterpret_cast<eTSDB::FilePage*>(page));
					if(fp->getAccessMode() == eTSDB::PageAccessMode::PageRead)
						_fds[currTask->fd-1].buf_idx = 0x80; // el buf está lleno, para obtener página
				}
				else if(page->getType() == eTSDB::PageType::HeaderType)
				{
					eTSDB::HeaderPage* hp = new eTSDB::HeaderPage();
					_fds[currTask->fd-1].ptr = hp;
					hp->copy(reinterpret_cast<eTSDB::HeaderPage*>(page));
				}
				_ops.delete_front();
			}
		}

		else if(currTask->op == Operation::ReadNext)
		{
			eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fds[currTask->fd-1].ptr);
			eTSDB::RetValue retval = etsdb.readNextBlockFile(*fp);
            if(retval == eTSDB::Ok)
			{
				_ops.delete_front();
			}
			else if(retval == eTSDB::FileEnded)
			{
				_ops.delete_front();
			}
		}

		else if(currTask->op == Operation::ListFiles)
		{
			eTSDB::RetValue retval = etsdb.checkGetNextFile();
			if(retval == eTSDB::Ok)
			{
				eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(currTask->buf);
				if(currTask->counter == 0)
					printf("File name: %s\tFile size: %d\n", fp->getName(), fp->getFileSize());
				else
				{
					uint8_t outbuf[128] = {0};
					outbuf[0] = 0x0b;
					outbuf[3] = 'f';
					outbuf[4] = 'l';
					outbuf[5] = fp->getPageIdx() & 0xff;
					outbuf[6] = (fp->getPageIdx() >> 8)  & 0xff;
					outbuf[7] = fp->getFileSize() & 0xff;
					outbuf[8] = (fp->getFileSize() >> 8) & 0xff;
					currTask->counter++;
					int outlen = 6; // TODO change strlen for one with boundary check
					strcpy(reinterpret_cast<char*>(outbuf+9),reinterpret_cast<char*>(fp->getName()));
					outlen += strlen(reinterpret_cast<char*>(fp->getName()))+1; // +1 keeps the 0
					outbuf[1] = outlen & 0xff;
					outbuf[2] = (outlen >> 8) & 0xff;
					write(0, outbuf, outlen+3);
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
					const uint8_t outbuf[] = {0x0b, 3, 0, 'f', 'l', 'f'};
					write(0, outbuf, sizeof(outbuf));
				}
				delete fp;
				_ops.delete_front();
			}
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
				eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fds[currTask->fd-1].ptr);
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
			_ops.delete_front();
		}

		else if(currTask->op == Operation::Close)
		{
			HAL_Delay(10);
			if(close(currTask->fd) == 0)
				_ops.delete_front();
		}
		else if(currTask->op == Operation::LoadProgram)
		{
			if(currTask->buf == nullptr)
			{
				currTask->counter = 0;
				currTask->buf = _alloc.Allocate(sizeof(ProgramInitializer), reinterpret_cast<uintptr_t>(this));
				for(int i = 0; i < sizeof(ProgramInitializer); ++i)
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
							if(file_buf[i] == 0)
							{
								pi->status= 2; // no more formats

								Task open_tsk;
								open_tsk.fd = _createFD(FDType::TS);
								open_tsk.buf = currTask->buf;
								open_tsk.op = Operation::OpenHeader;
								_ops.push_back(open_tsk);

								vm.HeaderFD = open_tsk.fd;

								pi->hi.colLen = currTask->counter;

								currTask->counter = 0;
							}
							else
							{
								pi->hi.formats[currTask->counter] = static_cast<eTSDB::Format>(file_buf[i]);
								pi->table_status++;
								pi->name_counter = 0;
							}
						}

						else if(pi->table_status == 3)
						{
							if(file_buf[i] == 0)
							{
								pi->table_status = 2;
								currTask->counter++;
							}
							else
							{
								pi->hi.colNames[currTask->counter][pi->name_counter++] = file_buf[i];

								if(pi->name_counter == 16)
								{
									pi->table_status = 2;
									currTask->counter++;

									if(currTask->counter == 16) //16 columns
									{
										pi->status = 2;

										Task open_tsk;
										open_tsk.fd = _createFD(FDType::TS);
										open_tsk.buf = currTask->buf;
										open_tsk.op = Operation::OpenHeader;
										_ops.push_back(open_tsk);

										vm.HeaderFD = open_tsk.fd;

										pi->hi.colLen = currTask->counter;

										currTask->counter = 0;
									}
								}
							}
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
						}
					}
					else if(pi->status == 3)
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
					vm.enable(true);
					printf("running\n");
					_ops.delete_front();
				}
			}
		}
		else if(currTask->op == Operation::SaveRow)
		{
			eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[currTask->fd-1].ptr);
			if(hp == nullptr) return true;
			eTSDB::RetValue retval = etsdb.appendValue(*hp, *reinterpret_cast<eTSDB::Row*>(currTask->buf));
			if(retval == eTSDB::Ok)
			{
				_alloc.Deallocate(currTask->buf);
				_ops.delete_front();
			}
		}
		else if(currTask->op != Operation::Upload)
		{
			printf_("Invalid task with opcode: %d\n", currTask->op);
			_ops.delete_front();
		}
		return true;
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
				m_pSize = remaining_file;

				_ops.push_back(tsk);
			}
			else if(strcmp(token, "list") == 0)
			{
				Task tsk;
				tsk.op = Operation::ListFiles;
				tsk.buf = new eTSDB::FilePage();
				tsk.counter = 0;

				etsdb.startGetNextFile(*reinterpret_cast<eTSDB::FilePage*>(tsk.buf));

				_ops.push_back(tsk);
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

				_ops.push_back(tsk);
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
				_ops.push_back(open_tsk);

				Task readTable_tsk;
				readTable_tsk.op = Operation::ReadTable;
				readTable_tsk.fd = open_tsk.fd;
				readTable_tsk.counter = 0;
				_ops.push_back(readTable_tsk);
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

                if(len == 0) write(0, "invalid value\n", 14);
                else
				{
					flash.readPage(len, addr);
					HAL_Delay(1);
					flash.loop();
					for(int i = 0; i < len; ++i)
					{
						tx_buffer.push_back(flash.pop());
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

				_ops.push_back(tsk);
			}
			else if(strcmp(token, "name") == 0)
			{
				printf("%s\n", vm.getPrgName());
			}
			else if(strcmp(token, "status") == 0)
			{
				if(vm.getEnabled())
					printf("VM enabled\n");
				else
					printf("VM disabled\n");
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
			else if(strcmp(token, "gd") == 0)
			{
				uint8_t* res_data = sdi12.getCmdResponse();
				if(res_data == nullptr)
				{
					printf("Not data yet\n");
				}
				else
				{
					printf("Data:\n%s\n", res_data);
				}
			}
			else if(strcmp(token, "meas") == 0)
			{
				printf("Start measure\n");
				sdi12.singleMeasure(0, sdi12_test, 2);
			}
			else if(strcmp(token, "fudge") == 0)
			{
				token = strtok(NULL, " ");
				uint8_t newFudge = std::atoi(token);
				sdi12.setFudge(newFudge);
				printf("New fudge: %d\n", newFudge);
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
						if(_alloc_idx[i] == 0)
						{

						}
						else if(_alloc_idx[i] < 100) // this is not a debug code
						{
							printf("Owner: eTSDB::Page new\tBlocks used: %d\n", _alloc_idx[i]);
						}
						else
						{
							printf("Owner: %s\tBlocks used: %d\n", reinterpret_cast<Module*>(_alloc_ownership)->getName(), _alloc_idx[i]);
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
				else if(strcmp(token, "test") == 0)
				{
					uint64_t secs = time().timestamp();
					secs += 5;

					RTC_AlarmTypeDef sAlarm;
					sAlarm.Alarm = RTC_ALARM_A;
					sAlarm.AlarmTime.Hours = (secs % 86400) / 3600;
					secs %= 3600;
					sAlarm.AlarmTime.Minutes = secs/60;
					sAlarm.AlarmTime.Seconds = secs%60;

					HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
					printf("testing clock for %d:%d:%d\n", sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds);
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
		}
		else if(strcmp(token, "power") == 0)
		{
            token = strtok(NULL, " ");
            int power_enable = std::atoi(token);
            enablePower(power_enable);
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
	}

	void OS::bin_eval(uint8_t* ebuf)
	{
		if(ebuf[0] == 'a') // ack
		{
			uint8_t outbuf[] = {0xb, 1, 0, 'a'};
			write(0, outbuf, 4);
		}
		else if(ebuf[0] == 's') // system
		{
			if(ebuf[1] == 'c') // config
			{
				if(ebuf[2] == 'g') // get
				{
					uint8_t outbuf[67] = {0}; // 64+3
					strcpy(reinterpret_cast<char*>(outbuf+3), m_name);
					int outlen = strlen(m_name); //limit to 32
					for(int i = 0; i < Module::getModuleNumber(); ++i)
					{
						Module::getModule(i)->getConfig(outbuf+outlen+3);
						outlen += Module::getModule(i)->getConfigSize();
					}
					outbuf[0] = 'c';
					outbuf[1] = outlen & 0xff;
					outbuf[2] = (outlen >> 8) & 0xff;
					write(0, outbuf, outlen+3);
				}
			}
			else if(ebuf[1] == 'n') // name
			{
				if(ebuf[2] == 'g')
				{
					uint8_t outbuf[67] = {0};
					int num_len = strlen(m_name);
					outbuf[0] = 0x0b;
					outbuf[1] = (num_len + 4) & 0xff;
					outbuf[2] = ((num_len + 4) >> 8) & 0xff;
 					outbuf[3] = 's';
					outbuf[4] = 'n';
					outbuf[5] = 'g';

					strcpy(reinterpret_cast<char*>(outbuf+6), m_name);
					write(0, outbuf, 7 + num_len);
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
				outbuf[0] = 0x0b;
				outbuf[1] = 0x06;
				outbuf[2] = 0x00;
				outbuf[3] = 's';
				outbuf[4] = 'M';
				outbuf[5] = used_blocks & 0xff;
				outbuf[6] = (used_blocks >> 8) & 0xff;
				outbuf[7] = 0x00;
				outbuf[8] = 0x10;
 				write(0, outbuf, 9);
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
				outbuf[0] = 0x0b;
				outbuf[3] = 's';
				outbuf[4] = 'f';
				int num_file = 0;
				for(int i = 0; i < 16; ++i) if(_fds[i].type != FDType::None) num_file++;
				outbuf[5] = num_file;
				int buf_idx = 6;
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
				outbuf[1] = buf_idx & 0xff;
				outbuf[2] = (buf_idx >> 8) & 0xff;
				write(0, outbuf, buf_idx+3);
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
			}
			else if(ebuf[1] == 'v') // version
			{
				uint8_t outbuf[10] = {0};
				outbuf[0] = 0x0b;
				outbuf[1] = 0x04;
				outbuf[2] = 0x00;
				outbuf[3] = 's';
				outbuf[4] = 'v';
				outbuf[5] = FWVMINOR;
				outbuf[6] = FWVMAJOR;

				write(0, outbuf, 7);
			}
			else if(ebuf[1] == 'm') // modules
			{
				uint8_t outbuf[128] = {0};
				outbuf[0] = 0x0b;
				outbuf[3] = 's';
				outbuf[4] = 'm';

				int buf_len = 0;
				for(int i = 0; i < Module::getModuleNumber(); ++i)
				{
					Module* mod = Module::getModule(i);
					buf_len += sprintf(reinterpret_cast<char*>(outbuf+5+buf_len), "%c,%s,%d,%d;", mod->bin_id,
										mod->getName(), mod->majorVersion, mod->minorVersion);
				}
				buf_len += 2;
				outbuf[1] = buf_len & 0xff;
				outbuf[2] = (buf_len >> 8) & 0xff;


				write(0, outbuf, buf_len+3);
			}
			else if(ebuf[1] == 'C') // clock
			{
				if(ebuf[2] == 'g')
				{
					uint8_t outbuf[15] = {0};
					outbuf[0] = 0x0b;
					outbuf[1] = 0x08;
					outbuf[2] = 0x00;
					outbuf[3] = 's';
					outbuf[4] = 'C';
					outbuf[5] = 'g';
					time().serialize(outbuf+6);
					write(0, outbuf, 11);
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
					HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

					sTime.Hours = new_time.hour;
					sTime.Minutes = new_time.minute;
					sTime.Seconds = new_time.second;
					HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

					uint8_t outbuf[] = {0x0b, 0x03, 0x00, 's','C','s'};
					write(0, outbuf, 6);
				}
			}
		}
		else if(ebuf[0] == 'f')
		{
			if(ebuf[1] == 'u')
			{
				char path[26] = {0};
				strcpy(path, "/file/");
				strcpy(path+6, reinterpret_cast<char*>(ebuf+4));

				int remaining_file = ebuf[2] | (ebuf[3] << 8);
				if(remaining_file == 0)
				{
					const uint8_t outbuf[] = {0x0b, 3, 0, 'f', 'u', 'e'};
					write(0, outbuf, sizeof(outbuf));
					return;
				}

				int fd = open(path, O_WRONLY);

				Task tsk;
				tsk.op = Operation::Upload;
				tsk.fd = fd;
				m_pSize = ebuf[2] | (ebuf[3] << 8);

				_ops.push_back(tsk);
			}
			else if(ebuf[1] == 'd')
			{
				char path[26] = {0};
				strcpy(path, "/file/");
				strcpy(path+6, reinterpret_cast<char*>(ebuf+2));
				Task tsk;
				tsk.op = Operation::DownloadFile;
				tsk.counter = 0;
				tsk.fd = open(path, O_RDONLY);
				_ops.push_back(tsk);
			}
			else if(ebuf[1] == 'l')
			{
				Task tsk;
				tsk.op = Operation::ListFiles;
				tsk.buf = new eTSDB::FilePage();
				tsk.counter = 1;

				etsdb.startGetNextFile(*reinterpret_cast<eTSDB::FilePage*>(tsk.buf));

				_ops.push_back(tsk);
			}
			else if(ebuf[1] == 'e')
			{
				flash.eraseChip();
				flash.unlock();
			}
		}
		else if(ebuf[0] == 't')
		{
			if(ebuf[1] == 'd')
			{
				Task open_tsk;
				open_tsk.op = Operation::OpenHeader;
				open_tsk.fd = _createFD(FDType::TS);
				open_tsk.buf = nullptr;
				for(int i = 0; i < 16; ++i) open_tsk.name_buf[i] = 0;
				for(int i = 0; i < 16 && ebuf[i+2] != 0; ++i) open_tsk.name_buf[i] = ebuf[i+2];
				_ops.push_back(open_tsk);

				Task readTable_tsk;
				readTable_tsk.op = Operation::ReadTable;
				readTable_tsk.fd = open_tsk.fd;
				readTable_tsk.counter = 2;
				_ops.push_back(readTable_tsk);
			}
			else if(ebuf[1] == 'l')
			{
				Task tsk;
				tsk.op = Operation::ListTables;
				tsk.buf = new eTSDB::HeaderPage();
				tsk.counter = 1;

				etsdb.startGetNextHeader(*reinterpret_cast<eTSDB::HeaderPage*>(tsk.buf));

				_ops.push_back(tsk);
			}
		}
		else if(ebuf[0] == 'p')
		{
			if(ebuf[1] == 'r') // run
			{
				vm.enable(true);
				uint8_t outbuf[] = {0x0b, 0x02, 0x00, 'p', 'r'};
				write(0, outbuf, 5);
			}
			else if(ebuf[1] == 's') //stop
			{
				vm.enable(false);
				uint8_t outbuf[] = {0x0b, 0x02, 0x00, 'p', 's'};
				write(0, outbuf, 5);
			}
			else if(ebuf[1] == 'l') // load
			{
				char buf[32];
				strcpy(buf, "/file/");
				strcpy(buf+6, reinterpret_cast<char*>(ebuf+2));
				int fd = open(buf, O_RDONLY);

				Task tsk;
				tsk.op = Operation::LoadProgram;
				tsk.fd = fd;
				tsk.buf = nullptr;
				tsk.counter = 0;

				_ops.push_back(tsk);

				uint8_t outbuf[] = {0x0b, 0x02, 0x00, 'p', 'l'};
				write(0, outbuf, 5);
			}
		}
		else
		{
			uint8_t outbuf[128] = {0};
			int out_len = 0;
			for(int i = 0; i < Module::getModuleNumber(); ++i)
			{
				if(Module::getModule(i)->bin_id == ebuf[0])
					out_len = Module::getModule(i)->bin_eval(ebuf+1, outbuf+4);
				if(out_len > 0)
				{
					out_len++;
					outbuf[0] = 0x0b;
					outbuf[1] = out_len & 0xff;
					outbuf[2] = (out_len >> 8) & 0xff;
					outbuf[3] = Module::getModule(i)->bin_id;
					write(0, outbuf, out_len+3);
					out_len = 0;
					break;
				}
			}
		}
	}

	void OS::radio_eval()
	{
		uint8_t buf[128];
		int buf_idx = 0;
		Log::Info("Radio received with RSSI: %d", radio.getPacketRSSI());
		if(radio.getPacketIRQFlags() & 0x20)
			Log::Info(" and wrong crc");
		else
			Log::Info(" and correct crc");
		Log::Info(" (flags 0x%x)\n", radio.getPacketIRQFlags());
		while(radio.available()>0 && buf_idx < 128) buf[buf_idx++] = radio.pop();

		buf[buf_idx] = 0;

		char outbuf[6];
		/*disp.setCursor(0,0);
		disp.writeString("RSSI:");
		disp.setCursor(0, 1);*/
		if(radio.getPacketIRQFlags() & 0x20)
			sprintf(outbuf, "%d!", radio.getPacketRSSI());
		else
			sprintf(outbuf, "%d", radio.getPacketRSSI());
		/*disp.writeString(outbuf);
		disp.setCursor(0,2);
		disp.writeString(reinterpret_cast<char*>(buf));
		disp.updateScreen();*/

		printf("%s\n", buf);
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
				if(_fd.buf == nullptr) _fd.buf = reinterpret_cast<uint8_t*>(_alloc.Allocate(128, reinterpret_cast<uintptr_t>(this)));
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
							fp->freeDataPage(); // mark as unread to make sure that data is not read twice

							Task tsk;
							tsk.fd = fd;
							tsk.op = Operation::ReadNext;
							_ops.push_front(tsk);
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
						_fd.buf = reinterpret_cast<uint8_t*>(_alloc.Allocate(128, reinterpret_cast<uintptr_t>(this)));
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
			eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(_fd.ptr);
            if(etsdb.closeFile(*fp) != eTSDB::RetValue::Ok)
			{
                return EBUSY;
			}
			if(_fd.buf != nullptr) _alloc.Deallocate(_fd.buf);
			delete fp;
            _alloc.Deallocate(_fd.ptr);
            _fd.ptr = nullptr;
		}
		else if(_fd.type == FDType::TS)
		{

			_alloc.Deallocate(_fd.ptr);
		}
		return 0;
	}

	eTSDB::Date OS::time()
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

	int16_t OS::get_adc_val(int channel)
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
			return adc_data[0];
		default:
			return -1;
		}
	}

	void OS::enablePower(int enable)
	{

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

	void OS::sleep()
	{
		Log::Verbose("Going to sleep\n");
		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	}

	void OS::saveConfig()
	{
		HAL_FLASH_Unlock();

		uint32_t flashaddr = 0x0807f800;

		FLASH_EraseInitTypeDef ferase;
		ferase.NbPages = 1;
		ferase.Banks = FLASH_BANK_1;
		ferase.PageAddress = flashaddr; // page 256
		ferase.TypeErase = FLASH_TYPEERASE_PAGES;

		uint32_t flashError;

		HAL_FLASHEx_Erase(&ferase, &flashError);

		FLASH_WaitForLastOperation(1000);

		if(flashError != 0xffffffff) {}

		for(int i = 0; i < 32; i = i+4) // store name
		{
			uint64_t flash_val = 0;
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

			HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashaddr, flash_val);
			flashaddr += 4;
			FLASH_WaitForLastOperation(1000);
		}

		for(int i = 0; i < Module::getModuleNumber(); ++i)
		{
			Module* mod = Module::getModule(i);
			uint8_t buf[64];
			mod->getConfig(buf);
			for(int j = 0; j < mod->getConfigSize(); j=j+4)
			{
				uint64_t flash_val = 0;
				for(int k = 0; k < 4; ++k)
				{
					flash_val |= buf[j+k] << 8*k;
				}
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashaddr, flash_val);
				flashaddr += 4;
				FLASH_WaitForLastOperation(1000);
			}
		}

		HAL_FLASH_Lock();
	}

	void OS::loadConfig()
	{
		uintptr_t flash_ptr = (uintptr_t) 0x0807f800;
		if(*reinterpret_cast<uint8_t*>(flash_ptr) == 0) return; // empty config
		for(int i = 0; i < 32; ++i)
		{
			m_name[i] = *reinterpret_cast<uint8_t*>(flash_ptr++);
			if(m_name[i] == 0)
				break;
		}
		//align 4
		flash_ptr = (flash_ptr+3) & ~(3);

		for(int i = 0; i < Module::getModuleNumber(); ++i)
		{
			Module* mod = Module::getModule(i);
			mod->setConfig(reinterpret_cast<uint8_t*>(flash_ptr));
			flash_ptr += mod->getConfigSize();
			flash_ptr = (flash_ptr+3) & (~3);
		}

	}
}
