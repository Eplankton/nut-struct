#ifndef _NUTS_VECTOR_
#define _NUTS_VECTOR_ 1

#ifndef _DEBUG
#define _DEBUG 1
#endif

#ifndef _WARNING
#define _WARNING 1
#endif

#include <iostream>
#include <cstddef>
#include <cassert>

#include "type.h"
#include "algorithm.h"

#define STD_CAPACITY 5
#define EXPAN_COEF 2

namespace nuts
{
	template <class T>
	class vector
	{
	protected:
		T *data_ptr = nullptr;
		u64 v_size = 0;
		u64 v_capacity = 0;

	public:
		vector() = default;											// Void constructor
		explicit vector(u64 userInputSize);							// Init by size
		explicit vector(u64 userInputSize, const T &userInputData); // Init by size and value
		vector(const vector<T> &obj);								// Copy constructor
		vector(const std::initializer_list<T> &ilist);				// Init by a {ilist}
		~vector() { this->destroy(); }

		u64 size() const { return this->v_size; }				 // Return the number of elements
		u64 capacity() const { return this->v_capacity; }		 // Return the current capacity
		bool empty() const { return this->v_size == 0; }		 // Check whether the vector is empty
		bool exist() const { return this->data_ptr != nullptr; } // Check whether the vector is existed
		void clear();											 // Clear all values, but don't destroy
		void destroy();											 // Clear the contents and release memory, contain exist()
		vector<T> &shrink_to_fit();								 // Reduce memory usage by freeing unused memory
		vector<T> &resize(u64 N);								 // Reduce or expand capacity
		void print() const;
		void sort();
		T *data() const { return this->data_ptr; }

		vector<T> &push_back(const T &obj); // Add an element to the end
		vector<T> &pop_back();				// Remove the last element
		vector<T> &move(vector<T> &after);	// Deprive other's ownership

		T &operator[](u64 N); // Access specified element
		const T &operator[](u64 N) const;

		vector<T> &operator=(const vector<T> &obj);					 // Deep copy operator
		vector<T> &operator=(const std::initializer_list<T> &ilist); // Covered by a {inlist}
		bool operator==(const vector<T> &obj) const;				 // Compare two vectors in same type

		class iterator
		{
		protected:
			T *_ptr = nullptr;

		public:
			iterator() = default;

			explicit iterator(T *obj)
			{
				this->_ptr = obj;
			}

			explicit iterator(std::nullptr_t obj)
			{
				this->_ptr = obj;
			}

			iterator(const iterator &obj)
			{
				this->_ptr = obj._ptr;
			}
			
			T *get() const { return _ptr; }

			T *operator->() { return _ptr; }
			const T *operator->() const { return _ptr; }

			T &operator*() { return *_ptr; }
			const T &operator*() const { return *_ptr; }

			iterator &operator=(T *obj)
			{
				this->_ptr = obj;
				return *this;
			}

			iterator &operator=(const iterator &obj)
			{
				this->_ptr = obj._ptr;
				return *this;
			}

			bool operator==(T *obj) const { return this->_ptr == obj; }
			bool operator!=(T *obj) const { return this->_ptr != obj; }

			bool operator==(const iterator &obj) const { return this->_ptr == obj._ptr; }
			bool operator!=(const iterator &obj) const { return this->_ptr != obj._ptr; }

			bool operator<(const iterator &obj) const { return this->_ptr < obj._ptr; }
			bool operator<=(const iterator &obj) const { return this->_ptr <= obj._ptr; }

			bool operator>(const iterator &obj) const { return this->_ptr > obj._ptr; }
			bool operator>=(const iterator &obj) const { return this->_ptr >= obj._ptr; }

			iterator &operator++()
			{
				this->_ptr++;
				return *this;
			}

			iterator operator++(int) // postposition
			{
				iterator res = *this;
				++(*this);
				return res;
			}

			iterator &operator--()
			{
				this->_ptr--;
				return *this;
			}

			iterator operator--(int) // postposition
			{
				iterator res = *this;
				--(*this);
				return res;
			}

			iterator operator+(i64 bias) const
			{
				return iterator(this->_ptr + bias);
			}

			void operator+=(i64 bias)
			{
				_ptr += bias;
			}

			iterator operator-(i64 bias) const
			{
				return iterator(this->_ptr - bias);
			}

			void operator-=(i64 bias)
			{
				_ptr -= bias;
			}

			friend i64 operator-(const iterator &a,
								 const iterator &b)
			{
				return a.get() - b.get();
			}
		};

		iterator begin() // Return iterator to the first element
		{
			return iterator(this->data_ptr);
		}

		iterator end() // Return iterator to the last element
		{
			return iterator(&this->data_ptr[this->size() - 1]);
		}

		iterator begin() const
		{
			return iterator(this->data_ptr);
		}

		iterator end() const
		{
			return iterator(&this->data_ptr[this->size() - 1]);
		}

		T &front() { return this->data_ptr[0]; }
		T &back() { return this->data_ptr[this->size() - 1]; }

		const T &front() const { return this->data_ptr[0]; }
		const T &back() const { return this->data_ptr[this->size() - 1]; }

		template <typename Itr>
		void assign(Itr st, Itr ed);

		template <typename Itr>
		vector(Itr st, Itr ed);
	};

	template <class T>
	vector<T>::vector(u64 userInputSize)
	{
		this->data_ptr = new T[userInputSize + STD_CAPACITY];
		this->v_size = userInputSize;
		this->v_capacity = this->v_size + STD_CAPACITY;
	}

	template <class T>
	vector<T>::vector(u64 userInputSize, const T &userInputData)
	{
		this->data_ptr = new T[userInputSize + STD_CAPACITY];
		std::fill_n(data_ptr, userInputSize, userInputData);
		this->v_size = userInputSize;
		this->v_capacity = this->v_size + STD_CAPACITY;
	}

	template <class T>
	vector<T>::vector(const vector<T> &obj)
	{
		this->data_ptr = new T[obj.size() + STD_CAPACITY];
		this->v_size = obj.size();
		this->v_capacity = this->v_size + STD_CAPACITY;
		for (u64 i = 0; i < this->v_size; i++)
			this->data_ptr[i] = obj.data_ptr[i];
	}

	template <class T>
	vector<T>::vector(const std::initializer_list<T> &ilist)
	{
		this->v_size = ilist.size();
		this->v_capacity = this->v_size + STD_CAPACITY;
		this->data_ptr = new T[v_size + STD_CAPACITY];
		T *q = this->data_ptr;
		for (auto p = ilist.begin(); p != ilist.end(); ++p, ++q)
		{
			*q = *p;
		}
	}

	template <class T>
	template <typename Itr>
	vector<T>::vector(Itr st, Itr ed)
	{
		this->assign(st, ed);
	}

	template <class T>
	void vector<T>::clear()
	{
		if (this->exist())
		{
			T INVALID;
			std::fill_n(data_ptr, this->v_size, INVALID);
			this->v_size = 0;
		}
	}

	template <class T>
	void vector<T>::destroy()
	{
		if (this->exist())
		{
			delete[] this->data_ptr;
			this->data_ptr = nullptr;
			this->v_size = 0;
			this->v_capacity = 0;
		}
	}

	template <class T>
	vector<T> &vector<T>::shrink_to_fit()
	{
		if (this->v_capacity > this->v_size)
		{
			T *tmp = new T[this->v_size];
			for (u64 i = 0; i < this->v_size; i++)
				tmp[i] = this->data_ptr[i];
			delete[] this->data_ptr;
			this->data_ptr = tmp;
			this->v_capacity = this->v_size;
		}
		return *this;
	}

	template <class T>
	vector<T> &vector<T>::resize(u64 N)
	{
		if (N > this->v_size)
		{
			T *tmp = new T[N];
			for (u64 i = 0; i < this->v_size; i++)
				tmp[i] = this->data_ptr[i];
			delete[] this->data_ptr;
			this->data_ptr = tmp;
			this->v_capacity = N;
			return *this;
		}
		if (N < this->v_size)
		{
			this->v_size = N;
			return *this;
		}
		return *this;
	}

	template <class T>
	vector<T> &vector<T>::push_back(const T &obj)
	{
		if (this->v_capacity - this->v_size == 0)
		{
			if (this->v_size != 0)
				this->resize(this->v_size * EXPAN_COEF);
			else
				this->resize(STD_CAPACITY);
		}
		this->data_ptr[this->v_size] = obj;
		this->v_size++;
		return *this;
	}

	template <class T>
	vector<T> &vector<T>::pop_back()
	{
		if (!this->empty())
		{
			this->v_size--;
			return *this;
		}
		else
			return *this;
	}

	template <class T>
	vector<T> &vector<T>::move(vector<T> &after)
	{
		this->destroy();				 // Self destroy
		this->data_ptr = after.data_ptr; // Pass ownership
		this->v_size = after.v_size;
		this->v_capacity = after.v_capacity;
		after.data_ptr = nullptr; // Set invalid
		after.v_size = 0;
		after.v_capacity = 0;
		return *this;
	}

	template <class T>
	T &vector<T>::operator[](const u64 N)
	{
		assert(N < this->v_capacity);
		if (N >= v_size)
			this->v_size++;
		return this->data_ptr[N];
	}

	template <class T>
	const T &vector<T>::operator[](const u64 N) const
	{
		assert(N < this->v_size);
		return this->data_ptr[N];
	}

	template <class T>
	vector<T> &vector<T>::operator=(const vector<T> &obj)
	{
		this->destroy();
		this->data_ptr = new T[obj.size()];
		this->v_size = obj.size();
		this->v_capacity = this->v_size;
		for (u64 i = 0; i < this->v_size; i++)
			this->data_ptr[i] = obj.data_ptr[i];
		return *this;
	}

	template <class T>
	vector<T> &
	vector<T>::operator=(const std::initializer_list<T> &ilist)
	{
		this->destroy();
		this->v_size = ilist.size();
		this->v_capacity = this->v_size + STD_CAPACITY;
		this->data_ptr = new T[v_size + STD_CAPACITY];
		auto st = ilist.begin();
		for (u64 i = 0; i < size(); ++i)
			data_ptr[i] = *(st++);
		return *this;
	}

	template <class T>
	bool vector<T>::operator==(const vector<T> &obj) const
	{
		u64 count = 0;
		for (u64 i = 0; i < this->v_size &&
						i < obj.v_size;
			 i++)
			if (this->data_ptr[i] == obj.data_ptr[i])
				count++;
		return count == this->v_size;
	}

	template <class T>
	void vector<T>::print() const
	{
		auto print = [this](const auto &x)
		{
			std::cout << x;
			if (&x != &this->back())
				printf(", ");
		};

		printf("\nvector @%#llx = [",
			   (u64)(this->data_ptr));
		for_each(begin(), end(), print);
		printf("]\n");
	}

	template <class T>
	void vector<T>::sort()
	{
		nuts::quick_sort(*this);
	}

	template <class T>
	template <typename Itr>
	void vector<T>::assign(Itr st, Itr ed)
	{
		for_each(st, ed,
				 [=](const auto &x)
				 { this->push_back(x); });
	}
}

#endif
