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

	int8_t push_front(T val)
	{
		if(full)
			return -1;
		uint8_t new_write = read_ptr-1;
		if(new_write > sizeval-1) new_write = sizeval-1;
		read_ptr = new_write;
		buffer[read_ptr] = val;
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

	int8_t delete_front()
	{
		if(read_ptr == write_ptr && !full)
			return -1;
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

	T* at_front()
	{
		if(read_ptr == write_ptr && !full) //vacío
		{
            return nullptr;
		}
		return &buffer[read_ptr];
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

template <typename T>
class prio_queue
{
	public:
		prio_queue();
		int insert(uint8_t prio, T val);
		int del(int node_idx);
		T getMin();
		T extractMin();

	private:
		int parent(int id);
		int left(int id);
		int right(int id);
		void fix_up(int idx);
		void fix_down(int idx);

		struct pair
		{
			uint8_t prio;
			T val;
		};
		void swap(pair* a, pair *b)
		{
			pair aux;
			aux = *b;
			*b = *a;
			*a = aux;
		}

		pair arr[15];
		int heap_size;
		int cap;
};

template <typename T>
prio_queue<T>::prio_queue()
{
	cap = 15;
	heap_size = 0;
	for(int i = 0; i < 15; ++i)
		arr[i].prio = 0;
}

template <typename T>
int prio_queue<T>::insert(unsigned char prio, T val)
{
	if(heap_size == cap) return -1;
	int i = heap_size;
	heap_size++;

	arr[i].prio = prio;
	arr[i].val = val;
	fix_up(i);

}

template <typename T>
int prio_queue<T>::del(int idx)
{
	arr[idx].prio = 0xff;
	fix_down(idx);
}

template <typename T>
void prio_queue<T>::fix_up(int idx)
{
	while(idx != 0 && arr[parent(idx)].prio > arr[idx].prio)
	{
		int par = parent(idx);
		swap(&arr[par], &arr[idx]);
		idx = par;
	}
}

template <typename T>
T prio_queue<T>::extractMin()
{
	if(heap_size <= 0) return 0xff;
	if(heap_size == 1)
	{
		heap_size--;
		return arr[0];
	}

	pair root = arr[0];
	arr[0] = arr[heap_size-1];
	heap_size--;
	fix_down(0);

	return 0;
}

template <typename T>
void prio_queue<T>::fix_down(int idx)
{
	int l = left(idx);
	int r = right(idx);
	int smallest = idx;

	if(l < heap_size && arr[l].prio < arr[idx].prio)
		smallest = l;
	if(r < heap_size && arr[r].prio < arr[smallest].prio)
		smallest = r;
	if(smallest != idx)
	{
		swap(&arr[idx], &arr[smallest]);
		fix_down(smallest);
	}
}

template <typename T>
int prio_queue<T>::parent(int id)
{
	return (id-1)/2;
}

template <typename T>
int prio_queue<T>::left(int id)
{
	return (2*id + 1);
}

template <typename T>
int prio_queue<T>::right(int id)
{
	return (2*id + 2);
}

class Allocator // 128bytes per object
{
	public:
		Allocator(uint8_t* mem, uint8_t* idx, uintptr_t* ownership, std::size_t size)
		{
			_mem = mem;
			_idx = idx;
			_ownership = ownership;
			for(std::size_t i = 0; i < size; ++i) _mem[i] = 0;
			for(std::size_t i = 0; i < size/pageSize; ++i) _idx[i] = 0;

			_numBlocks = size/pageSize;
		}

		void* Allocate(std::size_t reqSize, uintptr_t owner)
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
					_ownership[i-reqBlocks] = owner;
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
			_ownership[numBlock] = 0;
		}
	private:
		uint8_t* _mem;
		uint8_t* _idx;
		uintptr_t* _ownership;
		std::size_t _numBlocks;
		std::size_t pageSize = 128;

		int int_ceil(int D, int d)
		{
			return (D / d) + (D % d > 0 ? 1 : 0);
		}
};

#endif // MEM_STRUCTS_HPP
