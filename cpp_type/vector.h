#ifndef _NUTS_VECTOR_
#define _NUTS_VECTOR_

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

#define STD_CAPACITY 5
#define EXPAN_COEF 2

namespace nuts
{
	template <class T>
	class vector
	{
	protected:
		T *data_ptr = nullptr;
		size_t v_size = 0;
		size_t v_capacity = 0;

	public:
		vector() = default;                                               // Void constructor
		explicit vector(size_t userInputSize);                           // Init by size
		explicit vector(size_t userInputSize, const T &userInputData); // Init by size and value
		vector(const vector<T> &obj);                                   // Copy constructor
		vector(std::initializer_list<T> ilist);                           // Init by a {ilist}
		~vector() { this->destroy(); }

		size_t size() const { return this->v_size; }             // Return the number of elements
		size_t capacity() const { return this->v_capacity; }     // Return the current capacity
		bool empty() const { return this->v_size == 0; }         // Check whether the vector is empty
		bool exist() const { return this->data_ptr != nullptr; } // Check whether the vector is existed
		void clear();                                             // Clear all values, but don't destroy
		void destroy();                                             // Clear the contents and release memory, contain exist()
		vector<T> &shrink_to_fit();                                 // Reduce memory usage by freeing unused memory
		vector<T> &resize(size_t N);                             // Reduce or expand capacity

		vector<T> &push_back(const T &obj); // Add an element to the end
		vector<T> &pop_back();                // Remove the last element
		vector<T> &move(vector<T> &after);    // Deprive other's ownership

		T &operator[](size_t N); // Access specified element
		const T &operator[](size_t N) const;
		vector<T> &operator=(const vector<T> &obj);              // Deep copy operator
		vector<T> &operator=(std::initializer_list<T> ilist); // Covered by a {inlist}
		bool operator==(const vector<T> &obj) const;          // Compare two vectors in same type

#if _DEBUG
		friend std::ostream &operator<<(std::ostream &output, const vector<T> &obj)
		{
			for (size_t i = 0; i < obj.size(); i++)
			{
				std::cout << obj.data_ptr[i] << ' ';
			}
			return output;
		}
#endif

		class iterator
		{
		protected:
			T *ptr_onNode = nullptr;

		public:
			iterator() = default;
			explicit iterator(T *obj)
			{
				this->ptr_onNode = obj;
			}
			iterator(const iterator &obj)
			{
				this->ptr_onNode = obj.ptr_onNode;
			}

			T &operator*() { return *ptr_onNode; }

			iterator &operator=(T *obj)
			{
				this->ptr_onNode = obj;
				return *this;
			}

			iterator &operator=(const iterator &obj)
			{
				this->ptr_onNode = obj.ptr_onNode;
				return *this;
			}

			bool operator==(T *obj) const { return this->ptr_onNode == obj; }
			bool operator!=(T *obj) const { return this->ptr_onNode != obj; }

			bool operator==(const iterator &obj) const { return this->ptr_onNode == obj.ptr_onNode; }
			bool operator!=(const iterator &obj) const { return this->ptr_onNode != obj.ptr_onNode; }

			bool operator<(const iterator &obj) const { return this->ptr_onNode < obj.ptr_onNode; }
			bool operator<=(const iterator &obj) const { return this->ptr_onNode <= obj.ptr_onNode; }

			bool operator>(const iterator &obj) const { return this->ptr_onNode > obj.ptr_onNode; }
			bool operator>=(const iterator &obj) const { return this->ptr_onNode >= obj.ptr_onNode; }

			iterator &operator++()
			{
				this->ptr_onNode++;
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
				this->ptr_onNode--;
				return *this;
			}

			iterator operator--(int) // postposition
			{
				iterator res = *this;
				--(*this);
				return res;
			}

			iterator operator+(const int &bias)
			{
				return iterator(this->ptr_onNode + bias);
			}

			void operator+=(const int &bias)
			{
				ptr_onNode += bias;
			}

			iterator operator-(const int &bias)
			{
				return iterator(this->ptr_onNode - bias);
			}

			void operator-=(const int &bias)
			{
				ptr_onNode -= bias;
			}

			i64 operator-(const iterator &b)
			{
				return this->ptr_onNode - b.ptr_onNode;
			}

			T *operator->() { return ptr_onNode; }

			template <class>
			friend
			class vector;
		};

		iterator begin() // Return iterator to the first element
		{
			return iterator(this->data_ptr);
		}

		iterator end() // Return iterator to the last element
		{
			return iterator(&this->data_ptr[this->size() - 1]);
		}

		T &front() { return this->data_ptr[0]; }
		T &back() { return this->data_ptr[this->size() - 1]; }

		const T &front() const { return this->data_ptr[0]; }
		const T &back() const { return this->data_ptr[this->size() - 1]; }

		vector(const iterator &st, const iterator &ed);
	};

	template <class T>
	vector<T>::vector(size_t userInputSize)
	{
		this->data_ptr = new T[userInputSize + STD_CAPACITY];
		this->v_size = userInputSize;
		this->v_capacity = this->v_size + STD_CAPACITY;
	}

	template <class T>
	vector<T>::vector(size_t userInputSize, const T &userInputData)
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
		for (size_t i = 0; i < this->v_size; i++)
			this->data_ptr[i] = obj.data_ptr[i];
	}

	template <class T>
	vector<T>::vector(const iterator &st, const iterator &ed)
	{
		for (auto it = st; it != ed + 1; it++)
			this->push_back(*it);
	}

	template <class T>
	vector<T>::vector(const std::initializer_list<T> ilist)
	{
		this->v_size = ilist.size();
		this->v_capacity = this->v_size + STD_CAPACITY;
		this->data_ptr = new T[v_size + STD_CAPACITY];
		T *q = this->data_ptr;
		for (auto p = ilist.begin(); p != ilist.end(); ++p, q++)
			*q = *p;
	}

	template <class T>
	void vector<T>::clear()
	{
		if (this->exist())
		{
			T INVALID;
			std::fill_n(data_ptr, this->v_size, INVALID);
			this->v_size = 0;
			// for (size_t i = 0; i < this->v_size; i++)
			// 	this->data_ptr[i] = INVALID;
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
			for (size_t i = 0; i < this->v_size; i++)
				tmp[i] = this->data_ptr[i];
			delete[] this->data_ptr;
			this->data_ptr = tmp;
			this->v_capacity = this->v_size;
		}
		return *this;
	}

	template <class T>
	vector<T> &vector<T>::resize(size_t N)
	{
		if (N != this->v_size)
		{
			T *tmp = new T[N];
			for (size_t i = 0; i < this->v_size && i < N; i++)
				tmp[i] = this->data_ptr[i];
			delete[] this->data_ptr;
			this->data_ptr = tmp;
			this->v_capacity = N;
			if (N < this->v_size)
				this->v_size = N;
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
		if (this->exist())
		{
			T tmp;
			this->data_ptr[this->v_size - 1] = tmp;
			this->v_size--;
			return *this;
		}
		else
			return *this;
	}

	template <class T>
	vector<T> &vector<T>::move(vector<T> &after)
	{
		this->destroy(); // Self destroy

		this->data_ptr = after.data_ptr; // Pass ownership
		this->v_size = after.v_size;
		this->v_capacity = after.v_capacity;

		after.data_ptr = nullptr; // Set invalid
		after.v_size = 0;
		after.v_capacity = 0;
		return *this;
	}

	template <class T>
	T &vector<T>::operator[](const size_t N)
	{
		// assert(N < this->v_size);
		// return this->data_ptr[N];
		assert(N < this->v_capacity);
		if (N >= v_size)
			this->v_size++;
		return this->data_ptr[N];
	}

	template <class T>
	const T &vector<T>::operator[](const size_t N) const
	{
		assert(N < this->v_size);
		// return this->data_ptr[N];
		return this->data_ptr[N];
	}

	template <class T>
	vector<T> &vector<T>::operator=(const vector<T> &obj)
	{
		this->destroy();
		this->data_ptr = new T[obj.size()];
		this->v_size = obj.size();
		this->v_capacity = this->v_size;
		for (size_t i = 0; i < this->v_size; i++)
			this->data_ptr[i] = obj.data_ptr[i];
		return *this;
	}

	template <class T>
	vector<T> &vector<T>::operator=(const std::initializer_list<T> ilist)
	{
		this->destroy();
		this->v_size = ilist.size();
		this->v_capacity = this->v_size + STD_CAPACITY;
		this->data_ptr = new T[v_size + STD_CAPACITY];
		T *p = this->data_ptr;
		for (auto q = ilist.begin(); q != ilist.end(); ++q)
		{
			*p = *q;
			p++;
		}
		return *this;
	}

	template <class T>
	bool vector<T>::operator==(const vector<T> &obj) const
	{
		size_t count = 0;
		for (size_t i = 0; i < this->v_size && i < obj.v_size; i++)
			if (this->data_ptr[i] == obj.data_ptr[i])
				count++;
		return count == this->v_size;
	}
}

#endif
