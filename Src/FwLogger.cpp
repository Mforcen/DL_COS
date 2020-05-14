#include "FwLogger.h"

namespace FwLogger
{
	OS* OS::ptr = nullptr;
	OS& OS::get()
	{
		return *ptr;
	}

	OS::OS():_alloc(_alloc_buf, _alloc_idx, _alloc_ownership, 4096), flash(&hspi1, FLASH_CS), etsdb(0, 8192*1024, &hspi1, FLASH_CS, &_alloc), sdi12(SDI12_1),
	radio(&hspi1, LORA_DIO0, LORA_DIO1, LORA_DIO2, LORA_DIO3, LORA_RXEN, LORA_TXEN, LORA_CS, LORA_RST)
	{
		errno = 0;
		parser_remaining = 0;
		parser_status = 0;
		m_rtcFlag = false;
		//ctor
	}

	void OS::setOS(OS* os)
	{
		ptr = os;
	}

	void OS::init()
	{
		radio.init(868000000);
	}

	void OS::push_rx(uint8_t c)
	{
		Task* currTask = _ops.at_front();
		if(currTask == nullptr)
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
		else if(currTask->op == Operation::Upload)
		{
			write(currTask->fd, &c, 1);

			if(--parser_remaining <= 0)
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
	}

	void OS::RTC_ISR()
	{
		m_rtcFlag = true;
	}

	void OS::loop()
	{
		etsdb.poll(); // esto sería interesante pasarlo a estático
		flash.poll();
		radio.poll();
		vm.loop();

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

		Task* currTask = _ops.at_front();

		if(currTask == nullptr)
			return;

		if(currTask->op == Operation::Eval)
		{
			eval();
			_ops.delete_front();
			rx_buffer.clear();
		}

		else if(currTask->op == Operation::OpenFile)
		{
			if(etsdb.openFile(currTask->name_buf, static_cast<eTSDB::PageAccessMode>(currTask->counter)) == eTSDB::Ok)
				_ops.delete_front();
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
				_ops.delete_front();
			}
		}

		else if(currTask->op == Operation::ListTables)
		{
			eTSDB::RetValue retval = etsdb.checkGetNextHeader();
			if(retval == eTSDB::Ok)
			{
				eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(currTask->buf);
				printf("%s\tColumns: %d\n", hp->getName(), hp->getNumColumn());
				etsdb.startGetNextHeader(*hp);
			}
			else if(retval == eTSDB::NotFound)
			{
				close(currTask->fd);
				_ops.delete_front();
			}
		}

		else if(currTask->op == Operation::ReadTable)
		{
			eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[currTask->fd-1].ptr);
			if(currTask->counter == 0)
			{
				if(etsdb.readNextValue(*hp) == eTSDB::Ok) currTask->counter = 1;
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
						_ops.delete_front();
					}
					else
					{
						printf("%d/%d/%d %d:%d:%d,%d\n", rowPtr->rowDate.year, rowPtr->rowDate.month, rowPtr->rowDate.day
													   , rowPtr->rowDate.hour, rowPtr->rowDate.minute, rowPtr->rowDate.second
													   , rowPtr->vals[0].data._int32);
						HAL_Delay(5);
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
				printf("File name: %s\tFile size: %d\n", fp->getName(), fp->getFileSize());
				etsdb.startGetNextFile(*fp);
			}
			else if(retval == eTSDB::NotFound)
			{
				eTSDB::FilePage* fp = reinterpret_cast<eTSDB::FilePage*>(currTask->buf);
				delete fp;
				_ops.delete_front();
			}
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

								Task get_tsk;
								get_tsk.fd = open_tsk.fd;
								get_tsk.op = Operation::GetPage;
								_ops.push_back(get_tsk);

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

										Task get_tsk;
										get_tsk.fd = open_tsk.fd;
										get_tsk.op = Operation::GetPage;
										_ops.push_back(get_tsk);

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
							pi->static_vars = 0;
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
					_alloc.Deallocate(currTask->buf); //deallocating program initializer
					close(currTask->fd); // close file, cleaning buffers (file page and file buffer)
					vm.enable(true);
					_ops.delete_front();
				}
			}
		}
		else if(currTask->op == Operation::SaveRow)
		{
			eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[currTask->fd-1].ptr);
			if(hp == nullptr) return;
			eTSDB::RetValue retval = etsdb.appendValue(*hp, *reinterpret_cast<eTSDB::Row*>(currTask->buf));
			if(retval == eTSDB::Ok)
			{
				_alloc.Deallocate(currTask->buf);
				_ops.delete_front();
			}
		}
		/*else if(currTask->op == Operation::TestAppend)
		{
			eTSDB::HeaderPage* hp = reinterpret_cast<eTSDB::HeaderPage*>(_fds[currTask->fd-1].ptr);
			if(hp == nullptr) return;
			eTSDB::Row* row;
			if(currTask->buf == nullptr)
			{
				currTask->buf = new(_alloc.Allocate(sizeof(eTSDB::Row), 101)) eTSDB::Row();
				row = reinterpret_cast<eTSDB::Row*>(currTask->buf);
				row->rowDate.year = 2020;
				row->rowDate.month = 05;
				row->rowDate.day = 10;
				row->rowDate.hour = 1;
				row->rowDate.minute = 20;
				row->rowDate.second = 0;
				row->rowDate.exists = 0;

				row->vals[0].format = eTSDB::Format::Int32;
				row->vals[1].format = eTSDB::Format::Invalid;

				row->vals[0].data._int32 = 0;
			}
			else
			{
				row = reinterpret_cast<eTSDB::Row*>(currTask->buf);
				if(currTask->counter)
				{
					row->rowDate = row->rowDate+10;
					row->vals[0].data._int32++;
					currTask->counter = 0;
				}
			}
			eTSDB::RetValue retval = etsdb.appendValue(*hp, *row);
			if(retval == eTSDB::Ok) currTask->counter = 1;
		}*/
		else if(currTask->op != Operation::Upload)
		{
			printf_("Invalid task with opcode: %d\n", currTask->op);
			_ops.delete_front();
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
			else if(strcmp(token, "list") == 0)
			{
				Task tsk;
				tsk.op = Operation::ListFiles;
				tsk.buf = new eTSDB::FilePage();

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

				Task get_tsk;
				get_tsk.fd = open_tsk.fd;
				get_tsk.op = Operation::GetPage;
				_ops.push_back(get_tsk);

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
			else if(strcmp(token, "status") == 0)
			{
				if(vm.getEnabled())
				{
					printf_("VM enabled\n");
				}
				else
				{
					printf_("VM disabled\n");
				}
			}
			else if(strcmp(token, "stop") == 0)
			{
				vm.enable(false);
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
					printf_("Total memory: 4096 bytes\tUsedMemory: %d bytes\n", used_blocks*128);
				}
			}
			else if(strcmp(token, "version") == 0)
			{
                int mn = Module::getModuleNumber();
                for(int i = 0; i < mn; ++i)
				{
					printf_("%s\n", Module::getNames()[i]);
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

					printf_("%d/%d/%d %d:%d:%d\n", rtc_date.Year+2000, rtc_date.Month, rtc_date.Date, rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
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
		}
		else if(strcmp(token, "power") == 0)
		{
            token = strtok(NULL, " ");
            int power_enable = std::atoi(token);
            enablePower(power_enable);
		}
		/*else if(strcmp(token, "eTestSDB") == 0)
		{
			ProgramInitializer* pi = reinterpret_cast<ProgramInitializer*>(_alloc.Allocate(sizeof(ProgramInitializer), 101));
			strcpy(reinterpret_cast<char*>(pi->name), "test");
			pi->hi.period = 10;
			pi->hi.colLen = 1;
			pi->hi.formats[0] = eTSDB::Format::Int32;
			pi->hi.formats[1] = eTSDB::Format::Invalid;
			strcpy(reinterpret_cast<char*>(pi->hi.colNames[0]), "col1");

			Task open_tsk;
			open_tsk.fd = _createFD(FDType::TS);
			open_tsk.buf = pi;
			open_tsk.op = Operation::OpenHeader;
			_ops.push_back(open_tsk);

			Task get_tsk;
			get_tsk.fd = open_tsk.fd;
			get_tsk.op = Operation::GetPage;
			_ops.push_back(get_tsk);

			Task test_tsk;
			test_tsk.fd = open_tsk.fd;
			test_tsk.op = Operation::TestAppend;
			test_tsk.buf = nullptr;
			_ops.push_back(test_tsk);
		}*/
	}

	void OS::rf_eval()
	{

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
        return retDate;
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
