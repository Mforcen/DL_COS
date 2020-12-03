#ifndef MEM_STRUCTS_HPP
#define MEM_STRUCTS_HPP

#include <cstdint>
#include <cstring>
#include <utility>
#include <new>

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
		if(idx >= sizeval) return -1;
		buf[idx++]=val;
		return 0;
	}

	void pop_back()
	{
		buf[--idx] = 0;
	}

	uint8_t& back()
	{
		return buf[idx-1];
	}

	void clear()
	{
		idx = 0;
		//for(std::size_t i = 0; i < sizeval; ++i) buf[i] = 0;
	}

	size_t size()
	{
		return idx;
	}

	uint8_t& operator[](size_t idx)
	{
		return buf[idx];
	}
};
template <std::size_t sizeval, typename T = uint8_t>
struct circular_buffer
{
public:
	circular_buffer()
	{
		write_ptr = 0;
		read_ptr = 0;
		full = 0;
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
		buffer[read_ptr].~T();
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

	int8_t delete_back()
	{
		if((read_ptr == write_ptr) && !full)
			return -1;
		int new_write = write_ptr-1;
		if(new_write < 0) new_write = sizeval-1;
		write_ptr = new_write;
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

	T* at_back()
	{
		if(read_ptr == write_ptr && !full) //vacío
		{
            return nullptr;
		}
		int read_back = write_ptr-1;
		if(read_back < 0) read_back = sizeval -1;
		return &buffer[read_back];
	}

	T* at(unsigned int pos)
	{
		if(pos >= size()) return nullptr;
		return &buffer[(read_ptr+pos) % sizeval];
	}

	void clear()
	{
		T t;
		while(pop_front(&t)!=-1)
		{
			t.~T();
		}
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
	return 0;
}

template <typename T>
int prio_queue<T>::del(int idx)
{
	arr[idx].prio = 0xff;
	fix_down(idx);
	return 0;
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

template <std::size_t pageSize>
class Allocator // 128bytes per object
{
	public:
		Allocator(uint8_t* mem, uint8_t* idx, void** ownership, std::size_t size)
		{
			_mem = mem;
			_idx = idx;
			_ownership = ownership;
			for(std::size_t i = 0; i < size; ++i) _mem[i] = 0;
			for(std::size_t i = 0; i < size/pageSize; ++i) _idx[i] = 0;

			_numBlocks = size/pageSize;
		}

		void* __attribute__((optimize("O0"))) Allocate(std::size_t reqSize, void* owner)
		{
			//FwLogger::Log::Verbose("Allocating %d bytes for 0x%x\n", reqSize, owner);
			uint8_t reqBlocks = int_ceil(reqSize, pageSize);

			uint8_t freeBlocks = 0;
			std::size_t i = 0;

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
					return &_mem[(i-reqBlocks)*pageSize];
				}
			}
			//FwLogger::Log::Error("Allocation failed\n");
			return nullptr;
		}

		void __attribute__((optimize("O0"))) Deallocate(void* ptr)
		{
			//printf("Deallocating 0x%016x\n", ptr);
			uintptr_t dealloc_ptr = reinterpret_cast<uintptr_t>(ptr);
			uintptr_t mem_ptr = reinterpret_cast<uintptr_t>(_mem);
			uint8_t numBlock = (dealloc_ptr-mem_ptr)/pageSize;

			//if(_idx[numBlock] == 0) FwLogger::Log::Warning("Error, double deallocation for block %d\n", numBlock);
			//FwLogger::Log::Verbose("Deallocating block %d\n", numBlock);
			_idx[numBlock] = 0;
			_ownership[numBlock] = nullptr;
		}

	private:
		uint8_t* _mem;
		uint8_t* _idx;
		void** _ownership;
		std::size_t _numBlocks;

		int int_ceil(int D, int d)
		{
			return (D / d) + (D % d > 0 ? 1 : 0);
		}
};

extern Allocator<128>* _vectAllocator;

template<typename T>
class vector
{
	public:
		vector()
		{
			_data = nullptr;
			_size = 0;
			_capacity = 0;
		}

		~vector()
		{
			clear();
		}

		vector& operator= (const vector& other)
		{
			clear();

			_size = other._size;
			_capacity = other._capacity;
			if(other._data != nullptr)
			{
				_data = reinterpret_cast<T*>(_vectAllocator->Allocate(_capacity*sizeof(T), this));

				for(std::size_t i = 0; i < _size; ++i)
					_data[i] = other._data[i];
			}

			return *this;
		}

		vector& operator= (vector&& other)
		{
			clear();

			_size = other._size;
			_capacity = other._capacity;
			_data = other._data;

			other._size = 0;
			other._capacity = 0;
			other._data = nullptr;

			return *this;
		}

		void push_back(const T& val)
		{
			if(_size <= _capacity)
			{
				reserve(_size+1);
			}
			_data[_size++] = val;
		}

		void clear()
		{
			if(_data != nullptr)
			{
				for(std::size_t i = 0; i < _size; ++i)
					_data[i].~T();
				_vectAllocator->Deallocate(_data);
			}
			_size = 0;
			_capacity = 0;
			_data = nullptr;
		}

		void pop_back()
		{
			_data[--_size].~T();
		}

		void resize(std::size_t count)
		{
			if(count > _capacity)
				reserve(count);

			if(_size < count)
			{
				for(;_size < count; ++_size)
					new(&_data[_size]) T();
			}

			else if(_size > count)
			{
				for(std::size_t i = count; i < _size; ++i)
					_data[i].~T();
			}
			_size = count;

		}

		void reserve(std::size_t count)
		{
			std::size_t mem_req = int_ceil(count * sizeof(T), 128)*128;
			if(_capacity*sizeof(T) < mem_req)
			{
				T* old_data = _data;
				_data = reinterpret_cast<T*>(_vectAllocator->Allocate(mem_req, this));

				if(old_data != nullptr)
				{
					memcpy(_data, old_data, _size*sizeof(T));
					_vectAllocator->Deallocate(old_data);
					_capacity = mem_req / sizeof(T);
				}
			}
		}

		T& at(std::size_t pos)
		{
			return _data[pos];
		}

		T& operator[](std::size_t pos)
		{
			return _data[pos];
		}

		T& front()
		{
			return _data[0];
		}

		T& back()
		{
			return _data[_size-1];
		}

		T* data()
		{
			return _data;
		}

		std::size_t size()
		{
			return _size;
		}

	private:
		T* _data;
		std::size_t _size;
		std::size_t _capacity;

		static int int_ceil(int D, int d)
		{
			return (D / d) + (D % d > 0 ? 1 : 0);
		}
};

template <typename T>
class list
{
	size_t m_size;

public:
	class iterator;

private:
	class node
	{
		public:
		T data;
		node* next;
		node()
		{
			next = nullptr;
		}
		friend list;
		friend iterator;
	};

public:
	class iterator
	{
		node* p;
	public:
		iterator(node* node) : p(node) {}
		void operator++(){ p = p->next; }
		void operator++(int){ p = p->next; }
		bool operator==(const iterator& o) { return o.p == p; }
		bool operator!=(const iterator& o) { return o.p != p; }
		T& operator*() const { return p->data;}

		iterator operator+(int i)
		{
			iterator iter = *this;
			while(i-- > 0 && iter.p)
			{
				++iter;
			}
			return iter;
		}

		friend list;
	};

	node *head, *tail;

	node* _createNewNode()
	{
		node* nn = reinterpret_cast<node*>(_vectAllocator->Allocate(sizeof(node), this));
		new(&nn->data) T();
		return nn;
	}

	void _deleteNode(node* node)
	{
		node->next = nullptr;
		node->data.~T();
		_vectAllocator->Deallocate(node);
	}

public:
	list()
	{
		m_size = 0;
		head = nullptr;
		tail = nullptr;
	}
	bool empty() { return head == nullptr; }
	size_t size() { return m_size; }

	void pop_back()
	{
		if(!tail) return;
		if(m_size == 1)
		{
			_deleteNode(tail);
			tail = nullptr;
			head = nullptr;
			return;
		}
		node* last_node = head;
		while(last_node->next != tail)
		{
			last_node = last_node->next;
		}
		_deleteNode(tail);
		tail = last_node;
		m_size--;
	}

	void push_front(T value)
	{
		node* old_head = head;
		head = _createNewNode();
		head->next = old_head;
		if(tail == nullptr)
			tail = head;
		head->data = value;
		m_size++;
	}

	void push_back(T value)
	{
		node* old_tail = tail;
		tail = _createNewNode();
		tail->data = value;
		if(old_tail != nullptr)
			old_tail->next = tail;
		if(head == nullptr)
			head = tail;
		m_size++;
	}

	iterator insert(iterator it, T value)
	{
		node* new_node = _createNewNode();
		new_node->data = value;
		new_node->next = it.p->next;
		it.p->next = new_node;
		it.p = new_node;
		m_size++;
		return it;
	}

	iterator erase(iterator it)
	{
		if(head == nullptr) return iterator(nullptr);
		node* last_node = head;
		while(last_node->next != it.p) last_node = last_node->next;
		last_node->next = it.p->next;
		_deleteNode(it.p);
		it.p = last_node->next;
		m_size--;
		return it;
	}

	void clear()
	{
		node* now_node = head;
		while(now_node != nullptr)
		{
			node* next_node = now_node->next;
			_deleteNode(now_node);
			now_node = next_node;
		}
	}

	iterator begin() { return iterator(head); }
	iterator end() { return iterator(nullptr); }

	T& operator[](size_t idx)
	{
		iterator it = begin();
		for(size_t i = 0; i < idx; ++i) it++;
		return *it;
	}
};

#endif // MEM_STRUCTS_HPP
