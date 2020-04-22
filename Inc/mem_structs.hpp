#ifndef MEM_STRUCTS_HPP
#define MEM_STRUCTS_HPP

#include <cstdint>

template <std::size_t sizeval>
struct fixed_string
{
	fixed_string()
	{
		clear();
	}
	uint8_t idx;
	uint8_t buf[sizeval];
	int8_t push_back(uint8_t val)
	{
		if(idx == sizeval) return -1;
		buf[idx++]=val;
		return 0;
	}
	void clear()
	{
		idx = 0;
		for(std::size_t i = 0; i < sizeval; ++i)
		{
			buf[i] = 0;
		}
	}
};
template <std::size_t sizeval, typename T = uint8_t>
struct circular_buffer
{
public:
	circular_buffer()
	{
		clear();
	}

	int8_t push_back(T val)
	{

		if(full)
			return -1;
		uint8_t new_write = (write_ptr+1)%sizeval;
		buffer[write_ptr] = val;
		write_ptr = new_write;
		full = write_ptr == read_ptr;
		return 0;
	}

	int8_t pop_front(T* ph)
	{
		if(read_ptr == write_ptr && !full)
			return -1;
		*ph = buffer[read_ptr];
		read_ptr = (read_ptr+1)%sizeval;
		full = 0;
		return 0;
	}

	int8_t peek_front(T* ph)
	{
		if(read_ptr == write_ptr && !full)
			return -1;
		*ph = buffer[read_ptr];
		return 0;
	}

	void clear()
	{
		//for(std::size_t i = 0; i < sizeval; ++i) buffer[i] = 0;
		write_ptr = 0;
		read_ptr = 0;
		full = 0;
	}

	std::size_t size()
	{
		if(full) return sizeval;
		return (write_ptr+sizeval-read_ptr)%sizeval;
	}

private:
	T buffer[sizeval];
	uint8_t write_ptr;
	uint8_t read_ptr;
	uint8_t full;
};


template <std::size_t pageSize>
class Allocator // 128bytes per object
{
	public:
		Allocator(uint8_t* mem, uint8_t* idx, std::size_t size)
		{
			_mem = mem;
			_idx = idx;
			for(std::size_t i = 0; i < size; ++i) _mem[i] = 0;
			for(std::size_t i = 0; i < size/pageSize; ++i) _idx[i] = 0;

			_numBlocks = size/pageSize;
		}

		void* Allocate(std::size_t reqSize)
		{
			//printf("Allocating %d bytes ", reqSize);
			uint8_t reqBlocks = int_ceil(reqSize, pageSize);

			uint8_t freeBlocks = 0;
			int i = 0;

			while(i < (_numBlocks))
			{
				if(_idx[i] == 0) // ese bloque está libre
				{
					++freeBlocks;
					++i;
				}
				else
				{
					freeBlocks = 0;
					i+=_idx[i];
				}
				if(freeBlocks == reqBlocks)
				{
					_idx[i-reqBlocks] = reqBlocks;
					return &_mem[(i-reqBlocks)*128];
				}
			}
			//printf("failed \n");
			return nullptr;

		}
		void Deallocate(void* ptr)
		{
			//printf("Deallocating 0x%016x\n", ptr);
			uintptr_t dealloc_ptr = reinterpret_cast<uintptr_t>(ptr);
			uintptr_t mem_ptr = reinterpret_cast<uintptr_t>(_mem);
			uint8_t numBlock = (dealloc_ptr-mem_ptr)/pageSize;
			_idx[numBlock] = 0;
		}
	private:
		uint8_t* _mem;
		uint8_t* _idx;
		std::size_t _numBlocks;

		int int_ceil(int D, int d)
		{
			return (D / d) + (D % d > 0 ? 1 : 0);
		}
};

#endif // MEM_STRUCTS_HPP
