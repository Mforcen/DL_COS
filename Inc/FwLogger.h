#ifndef FWLOGGER_H
#define FWLOGGER_H

#include <cstdint>
#include <cstring>

struct fixed_string
{
	fixed_string()
	{
		reset();
	}
	uint8_t idx;
	uint8_t buf[128];
	int8_t push(uint8_t val)
	{
		buf[idx++]=val;
	}
	void reset()
	{
		idx = 0;
		for(int i = 0; i < 128; ++i) buf[i] = 0;
	}
};

struct circular_buffer
{
public:
	circular_buffer()
	{
		reset();
	}

	int8_t push(uint8_t val)
	{

		uint8_t new_write = (write_ptr+1)%128;
		if(full)
			return -1;
		buffer[write_ptr] = val;
		write_ptr = new_write;
		full = write_ptr == read_ptr;
		return 0;
	}
	int8_t pop(uint8_t* ph)
	{
		if(read_ptr == write_ptr && !full)
			return -1;
		*ph = buffer[read_ptr];
		read_ptr = (read_ptr+1)%128;
		full = 0;
		return 0;
	}

	void reset()
	{
		for(int i = 0; i < 128; ++i) buffer[i] = 0;
		write_ptr = 0;
		read_ptr = 0;
		full = 0;
	}

private:
	uint8_t buffer[128];
	uint8_t write_ptr;
	uint8_t read_ptr;
	uint8_t full;
};

extern circular_buffer tx_buffer;

struct LoggerTask
{

};

class FwLogger
{
	public:
		//static FwLogger* getInstance();
		FwLogger();

		void init();
		void eval(uint8_t* input_str);

	protected:

	private:
};

#endif // FWLOGGER_H
