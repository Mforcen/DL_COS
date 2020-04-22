#pragma once
#ifndef __HASH_TABLE__
#define __HASH_TABLE__

#include <inttypes.h>
#include <array>

template <typename T, typename HASH_t = uint32_t, int N = 31>
class HashTable
{
    private:
        struct HT_data // nodos de lista enlazada
        {
            HT_data() = default;

            HASH_t hash;
            T data;
            HT_data* next = NULL;
        };

        struct HT_field // bucket y director a primer nodo de la lista
        {
            HT_field() = default;

            HT_data* element = nullptr;
            uint16_t n = 0;

            bool empty() { return n == 0; }

            class iterator
            {
			public:
            	HT_data* _ptr;
            	iterator(HT_data* ptr)
            	{
            		_ptr = ptr;
            	}

            	iterator()
            	{
            		_ptr = nullptr;
            	}

				iterator operator++()
				{
					if(_ptr != NULL)
						_ptr = _ptr->next;
					return *this;
				}

				iterator operator++(int)
				{
					if(_ptr != NULL)
						_ptr = _ptr->next;
					return *this;
				}

				bool operator==(const iterator& other) const
				{
					return _ptr == other._ptr;
				}

				bool operator!=(const iterator& other) const
				{
					return _ptr != other._ptr;
				}

				const HT_data& operator*() const
				{
					return *_ptr;
				}

				const HT_data* operator->() const
				{
					return _ptr;
				}

				HT_data& operator*()
				{
					return *_ptr;
				}

				HT_data* operator->()
				{
					return _ptr;
				}
            };

            iterator end()
            {
            	return iterator(nullptr);
            }

            iterator begin()
            {
            	return iterator(element);
            }
        };

        std::array<HT_field, N> bucketArr;
        int _entries;

    public:
        HashTable();
        ~HashTable();
        void insert(T data, HASH_t hash);
        bool get(HASH_t hash, T* dest);
        bool query(HASH_t hash);
        void erase(HASH_t hash);
        int size();
        bool empty();

        T& operator[](HASH_t hash)
        {
        	int index = hash % N;

			HT_field* field = &bucketArr[index];

			if(field->n == 0);

			for(auto ptr = field->element; ptr != NULL; ptr = ptr->next)
			{
				if(ptr->hash == hash)
				{
					return ptr->data;
				}
			}
        }

        HashTable<T>& operator=(HashTable<T>& arg)
        {
        	_entries = arg._entries;
            for(int i = 0; i < N; ++i)
			{
				int elems = arg.bucketArr[i].n;
				if(elems > 0)
				{
					bucketArr[i].n = arg.bucketArr[i].n;
					bucketArr[i].element = new HT_data;
					HT_data* field = bucketArr[i].element;
					HT_data* other_field = arg.bucketArr[i].element;
					field->data = other_field->data;
					field->hash = other_field->hash;
					while(other_field->next != nullptr)
					{
                        other_field = other_field->next;
                        field->next = new HT_data;
                        field = field->next;
                        field->data = other_field->data;
                        field->hash = other_field->hash;
					}
				}
			}
			return *this;
        }

        class iterator {
            private:
            	const std::array<HT_field, N>* _bktArrPtr;
                typename std::array<HT_field, N>::iterator _bktIter;
                typename HT_field::iterator _dataIter;
            public:
                iterator(const std::array<HT_field, N>& bktArr, typename std::array<HT_field, N>::iterator bktIter, typename HT_field::iterator dataIter):
                	_bktArrPtr(&bktArr), _bktIter(bktIter), _dataIter(dataIter) { }

                iterator operator++() {
					_dataIter++;
                    if(_dataIter == _bktIter->end())
					{
						_bktIter++;
						while((_bktIter != _bktArrPtr->end()) && (_bktIter->n == 0))
						{
							++_bktIter;
						}
						if(_bktIter == _bktArrPtr->end()) return *this;
						_dataIter = _bktIter->begin();
					}
                    return *this;
                }

                bool operator!=(const iterator & other) const
                {
                	return (_bktIter != other._bktIter) && (_dataIter != other._dataIter);
				}

                const HT_data* operator->() const
                {
                	return &(*_dataIter);
                }

                const HT_data& operator*() const {
                    return *_dataIter;
                }

                HT_data* operator->()
                {
                	return &(*_dataIter);
                }

                HT_data& operator*(){
                    return *_dataIter;
                }
        };

        iterator begin()
        {
        	if(empty()) return end();
            typename std::array<HT_field, N>::iterator bktIt = bucketArr.begin();
            while(bktIt->empty()) ++bktIt;
            return iterator(bucketArr, bktIt, bktIt->begin());
        }

        iterator end() { return iterator(bucketArr, bucketArr.end(), typename HT_field::iterator()); }
};

template <typename T, typename HASH_t, int N>
HashTable<T, HASH_t, N>::HashTable()
{
	_entries = 0;
	for(int i=0; i<N; i++)
    {
        bucketArr[i].n = 0;
        bucketArr[i].element = nullptr;
    }
}

template <typename T, typename HASH_t, int N>
HashTable<T, HASH_t, N>::~HashTable()
{
    for(int i=0; i<N; i++)
    {
        if(bucketArr[i].n > 0)
        {
            auto ptr_prev = bucketArr[i].element;
            for(auto ptr = ptr_prev->next; ptr != NULL; ptr = ptr->next)
            {
                delete ptr_prev;
                ptr_prev = ptr;
            }
            delete ptr_prev;
        }
    }
};

template <typename T, typename HASH_t, int N>
void HashTable<T, HASH_t, N>::insert(T data, HASH_t hash)
{
	_entries++;
    int index = hash % N;
    HT_field *field = &bucketArr[index];

    if (field->n == 0)
    {
        field->element = new HT_data;
        field->element->hash = hash;
        field->element->data = data;
    }
    else
    {
        HT_data *ptr_data = field->element;
        while(ptr_data->next != NULL)
        {
            ptr_data = ptr_data->next;
        };
        ptr_data->next = new HT_data;
        ptr_data->next->hash = hash;
        ptr_data->next->data = data;
    };

    field->n++;
};

template <typename T, typename HASH_t, int N>
bool HashTable<T, HASH_t, N>::get(HASH_t hash, T* dest)
{
    int index = hash % N;

    HT_field* field = &bucketArr[index];

    if(field->n == 0)
    {
        return false;
    }

    for(auto ptr = field->element; ptr != NULL; ptr = ptr->next)
    {
        if(ptr->hash == hash)
        {
            *dest = ptr->data;
            return true;
        }
    }

    return false;
};

template <typename T, typename HASH_t, int N>
bool HashTable<T, HASH_t, N>::query(HASH_t hash)
{
    int index = hash % N;

    HT_field* field = &bucketArr[index];

    if(field->n == 0)
    {
        return false;
    }

    for(auto ptr = field->element; ptr != NULL; ptr = ptr->next)
    {
        if(ptr->hash == hash)
        {
            return true;
        }
    }

    return false;
};

template <typename T, typename HASH_t, int N>
void HashTable<T, HASH_t, N>::erase(HASH_t hash)
{
    int index = hash % N;

    HT_field* field = &bucketArr[index];

    if(field->n == 0)
    {
        return;
    }

    if(field->n == 1 && field->element->hash == hash)
    {
        delete field->element;
        field->n--;
        return;
    }

    for(auto ptr = field->element->next, ptr_prev = field->element; ptr != NULL; ptr = ptr->next)
    {
        if(ptr->hash == hash)
        {
            if(ptr->next = NULL)
            {
                ptr_prev->next = NULL;
            }
            else
            {
                ptr_prev->next = ptr->next;
            }
            delete ptr;
            field->n--;
            return;
        }
        ptr_prev = ptr;
    }
}

template <typename T, typename HASH_t, int N>
int HashTable<T, HASH_t, N>::size()
{
	return _entries;
}

template <typename T, typename HASH_t, int N>
bool HashTable<T, HASH_t, N>::empty()
{
	return _entries == 0;
}

#endif
