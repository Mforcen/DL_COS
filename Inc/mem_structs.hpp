#ifndef MEM_STRUCTS_HPP
#define MEM_STRUCTS_HPP

#include <stdint.h>

struct fixed_string
{
	fixed_string()
	{
		clear();
	}
	uint8_t idx;
	uint8_t buf[128];
	int8_t push_back(uint8_t val)
	{
		if(idx == 128) return -1;
		buf[idx++]=val;
		return 0;
	}
	void clear()
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
		clear();
	}

	int8_t push_back(uint8_t val)
	{

		uint8_t new_write = (write_ptr+1)%128;
		if(full)
			return -1;
		buffer[write_ptr] = val;
		write_ptr = new_write;
		full = write_ptr == read_ptr;
		return 0;
	}
	int8_t pop_front(uint8_t* ph)
	{
		if(read_ptr == write_ptr && !full)
			return -1;
		*ph = buffer[read_ptr];
		read_ptr = (read_ptr+1)%128;
		full = 0;
		return 0;
	}

	void clear()
	{
		for(int i = 0; i < 128; ++i) buffer[i] = 0;
		write_ptr = 0;
		read_ptr = 0;
		full = 0;
	}

	int size()
	{
		if(full) return 128;
		return (write_ptr+128-read_ptr)%128;
	}

private:
	uint8_t buffer[128];
	uint8_t write_ptr;
	uint8_t read_ptr;
	uint8_t full;
};

#endif // MEM_STRUCTS_HPP
