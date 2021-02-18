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
		//flash(&hspi1, GPIOA, GPIO_PIN_15),
		//s(&hspi1, FLASH_CS),
		sdi12(SDI12_0)
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

		MX_IWDG_Init();
	}

	void OS::init()
	{

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

		m_init = false;
		init_delay = HAL_GetTick();
		HAL_IWDG_Init(&hiwdg);

		//s.init();
	}

	void OS::RTC_ISR()
	{
		//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2);
		HAL_IWDG_Refresh(&hiwdg);
		uint64_t t = time(); // this updates counter
		if(_alarmTime <= t)
		{
			m_rtcFlag = true;
			Log::Info("RTC ISR\n");
			HAL_PWR_DisableSleepOnExit();
			HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

			HAL_RTCEx_BKUPWrite(&hrtc, 5, 0);
			HAL_RTCEx_BKUPWrite(&hrtc, 6, 0);
			vm.resumeExec();
		}
		else
		{
			if(setNextAlarm() == 0)
			{
				HAL_NVIC_SystemReset();
			}
		}
	}


	void OS::RTC_SecondsIT()
	{
		uint32_t CNT = RTC->CNTH << 16 | RTC->CNTL;
		if(_alarmTime <= CNT)
		{
			m_rtcFlag = true;
			Log::Info("RTC Seconds ISR\n");
		}
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
				vm.setStackSize(300);
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

		m_loop_time = HAL_GetTick()-m_last_loop;
		m_last_loop = HAL_GetTick();
		HAL_IWDG_Refresh(&hiwdg);
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
			_alloc.Deallocate(currTask->buf);
			msgQueue.delete_front();
		}

		else if(currTask->op == Operation::ModemEval)
		{
			Log::Verbose("[Modem(%s)] R: %s\n", PortGSM::get().getStatusStr(), reinterpret_cast<char*>(currTask->buf));
			PortGSM::get().eval(reinterpret_cast<uint8_t*>(currTask->buf), currTask->counter);

			uint8_t* uib = reinterpret_cast<uint8_t*>(currTask->buf);
			if(uib[0] == '*')
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

		else if(currTask->op == Operation::Close)
		{
			HAL_Delay(10);
			if(close(currTask->fd) == 0)
				msgQueue.delete_front();
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

		/*else if(strcmp(token, "flash") == 0)
		{
			uint8_t buf[16] = {0};
			flash.jedecId(buf);
			printf("%x %x %x\n", buf[0], buf[1], buf[2]);
		}*/

		else if(strcmp(token, "uart") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "state") == 0)
			{
				printf("UART state: %d\n", HAL_UART_GetState(&huart3));
			}
		}

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
				vm.setStackSize(300);
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

		else if(strcmp(token, "program") == 0)
		{
			token = strtok(NULL, " ");
			if(strcmp(token, "run") == 0)
			{

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

					//HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

					timetok = strtok(timetok, ":");
					rtc_time.Hours = atoi(timetok);

					timetok = strtok(NULL, ":");
					rtc_time.Minutes = atoi(timetok);

					timetok = strtok(NULL, ":");
					rtc_time.Seconds = atoi(timetok);
					//HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
					setTime(rtc_date, rtc_time, false);

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
					printf("standalone enabled");
				}
				else
				{
					fw_standalone = 0;
					printf("standalone disabled");
				}
			}
			else if(strcmp(token, "uptime") == 0)
			{
				printf("Uptime in ms: %d\n", HAL_GetTick());
			}
		}
		else if(strcmp(token, "adc") == 0)
		{
			token = strtok(NULL, " ");
			int chan = std::atoi(token);
			printf("ADC: %d\n", get_adc_raw(chan));
		}
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
		errno = EUNSPEC;
		return -1;
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
		}
		else
		{ // TODO change to make proper error handling
			if(PortManager::write(fd, buf, count) < 0) // it frees sockets if self managed
			{
				Log::Error("[FW]: Error on PortManager write\n");
			}
			//s.append(reinterpret_cast<const uint8_t*>(buf), count); // just in case
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
		_fd.type = FDType::None;
		return 0;
	}

	uint64_t OS::time()
	{
		RTC_DateTypeDef rtc_date;
		HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

		RTC_TimeTypeDef rtc_time;
        HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);

		uint64_t retval;
		retval = rtc_time.Seconds + rtc_time.Minutes*60 + rtc_time.Hours*3600 + (rtc_date.Date-1)*86400;

		int year = rtc_date.Year + 2000;
		for(int i = 1970; i < year; ++i)
			retval += ((i%4 == 0 )|( i%400 == 0) ? 366 : 365)*86400;

		uint8_t days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		if(year % 4 == 0 || year % 400 == 0) days[1] = 29;

		for(uint8_t month_idx = 0; month_idx < (rtc_date.Month-1); ++month_idx) retval += days[month_idx] * 86400;
		return retval;
	}

	static inline uint32_t toSecs(RTC_TimeTypeDef& time)
	{
		return time.Hours*3600 + time.Minutes*60 + time.Seconds;
	}

	void OS::setTime(RTC_DateTypeDef& date, RTC_TimeTypeDef& time, bool check)
	{
		bool alarmEnabled = ((hrtc.Instance->CRH & 2) != 0);

		if(check)
		{
			RTC_TimeTypeDef nowTime;
			HAL_RTC_GetTime(&hrtc, &nowTime, RTC_FORMAT_BIN);
			uint32_t timeSecs = toSecs(nowTime);

			int32_t timeDelta = toSecs(time) - timeSecs;
			if(timeDelta < 0) timeDelta = -timeDelta;
			if(timeDelta > 100) //just allow time drifting error correction
				return;
		}

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

	void OS::setAlarm(uint64_t secs)
	{
		Log::Info("Set alarm at %llu s\n", secs);

		_alarmTime = secs;
		if(time() >= _alarmTime) RTC_ISR();
		else
		{
			HAL_RTCEx_BKUPWrite(&hrtc, 5, _alarmTime & 0xffffffff);
			HAL_RTCEx_BKUPWrite(&hrtc, 6, (_alarmTime >> 32) & 0xffffffff);
		}
	}

	int OS::sleepFor(int s)
	{
		printf("Sleeping for %d seconds\n",s);
		HAL_Delay(100);

		uint64_t secs = time();
		secs += s;

		setAlarm(secs);

		prepareSleep();

		return 0;
	}

	void OS::setSecondsIT(int enable)
	{
		if(enable)
		{
			HAL_RTCEx_SetSecond_IT(&hrtc);
			HAL_NVIC_EnableIRQ(RTC_IRQn);
		}
		else
		{
			HAL_RTCEx_DeactivateSecond(&hrtc);
			HAL_NVIC_DisableIRQ(RTC_IRQn);
		}
	}

	void OS::reset(const char *file, int line, const char *reason)
	{
		printf("[SYS] Reset at %s:%d, reason: %s\n", file, line, reason);
		HAL_Delay(1000);
		HAL_NVIC_SystemReset();
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
	}

	void OS::sleep()
	{
		int ret = setNextAlarm();
		if(!ret) reset(__func__, __LINE__, "Cannot set RTC Alarm at sleeping");
		if(ret == -1) return; // alarm passed

		__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
		__HAL_RTC_ALARM_ENABLE_IT(&hrtc, RTC_IT_ALRA);

		/* RTC Alarm Interrupt Configuration: EXTI configuration */
		__HAL_RTC_ALARM_EXTI_ENABLE_IT();
		__HAL_RTC_ALARM_EXTI_ENABLE_RISING_EDGE();

		PortUART::get().powerOff();

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_RESET); // apagar el power

		HAL_TIM_Base_Stop_IT(&htim6);
		HAL_ADC_Stop_DMA(&hadc1);
		HAL_NVIC_DisableIRQ(SPI1_IRQn);
		HAL_NVIC_DisableIRQ(USART1_IRQn);

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

		HAL_SuspendTick();
		HAL_PWR_EnableSleepOnExit();
		if(m_rtcFlag == false)
			HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

		SystemClock_Config(); // being in stopmode changes system clock to HSI instead of PLL
		HAL_ResumeTick();

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

		HAL_TIM_Base_Start_IT(&htim6);
		HAL_ADC_Start_DMA(&hadc1, reinterpret_cast<uint32_t*>(adc_data), 7);

		HAL_NVIC_EnableIRQ(SPI1_IRQn);
		HAL_NVIC_EnableIRQ(USART1_IRQn);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_SET);
		HAL_Delay(500);
	}

	int OS::setNextAlarm()
	{
		uint64_t t = time();
		if(_alarmTime <= t)
		{
			m_rtcFlag = true;
			Log::Info("RTC ISR\n");
			HAL_PWR_DisableSleepOnExit();
			HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
			vm.resumeExec();
			return -1;
		}

		uint32_t CNT = RTC->CNTH << 16 | RTC->CNTL;
		int32_t nextAlarm = _alarmTime - (t/86400*86400); //removing decimals
		nextAlarm = nextAlarm < CNT+5 ? nextAlarm : CNT+5;

		HAL_StatusTypeDef st;
		for(int tries = 0; tries < 5; ++tries)
		{
			st = RTC_WriteAlarmCounter(&hrtc, nextAlarm);
			if(st == HAL_OK) break;
		}
		if(st == HAL_OK) return 1;
		return 0;
	}
}
