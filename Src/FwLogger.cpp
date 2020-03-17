#include "FwLogger.h"

FwLogger::FwLogger():flash(&hspi1, FLASH_CS), etsdb(0, 4096*1024, &hspi1, FLASH_CS), sdi12(SDI12_1),
radio(&hspi1, LORA_DIO0, LORA_DIO1, LORA_DIO2, LORA_DIO3, LORA_RXEN, LORA_TXEN, LORA_CS, LORA_RST)
{
	TinyScript_Init(ts_inter_arena, 4096);
	errno = 0;
	//ctor
}

void FwLogger::init()
{
	radio.init(868000000);
}

void FwLogger::loop()
{
	etsdb.poll(); // esto sería interesante pasarlo a estático
	flash.poll();
	radio.poll();
}

void FwLogger::eval(uint8_t* input_str)
{
    //uint8_t* pch = (uint8_t*) strtok((char*) input_str, " "); //se empesará por usar tinyscript
    TinyScript_Run((const char*)input_str, 1, 1);
}


int FwLogger::open(char* path, int oflag)
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
	errno = EUNSPEC;
	return -1;
}

int FwLogger::read(int fd, void* buf, size_t count)
{
	errno = EUNSPEC;
	return -1;
}

int FwLogger::write(int fd, void* buf, size_t count)
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
		return count;
	}
	errno = EUNSPEC;
	return -1;
}

int16_t FwLogger::get_adc_val(int channel)
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

void FwLogger::enablePower(int enable)
{
	if(enable)
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
}
