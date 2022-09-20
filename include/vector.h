#ifndef _NUTS_VECTOR_
#define _NUTS_VECTOR_ 1

#include <cassert>
#include <cstddef>
#include <iostream>

#include "algorithm.h"
#include "iterator.h"
#include "type.h"

#define STD_CAPACITY 5U
#define EXPAN_COEF 2U

namespace nuts
{
	template <class T>
	class vector
	{
	public:
		using value_type = T;
		using pointer = T*;

	public:
		class iterator
		    : public random_access_iterator
		{
		public:
			using value_type = T;

		protected:
			T* _ptr = nullptr;

		public:
			iterator() = default;
			iterator(T* obj) { _ptr = obj; }
			iterator(std::nullptr_t obj) : _ptr(obj) {}
			iterator(const iterator& obj) : _ptr(obj._ptr) {}

			pointer get() const { return _ptr; }

			T& operator*() { return *_ptr; }
			const T& operator*() const { return *_ptr; }

			pointer operator->() const { return _ptr; }

			iterator& operator=(T* obj)
			{
				_ptr = obj;
				return *this;
			}

			iterator& operator=(const iterator& obj)
			{
				_ptr = obj._ptr;
				return *this;
			}

			bool operator==(T* obj) const { return _ptr == obj; }
			bool operator!=(T* obj) const { return _ptr != obj; }

			bool operator==(const iterator& obj) const { return _ptr == obj._ptr; }
			bool operator!=(const iterator& obj) const { return _ptr != obj._ptr; }

			bool operator<(const iterator& obj) const { return _ptr < obj._ptr; }
			bool operator<=(const iterator& obj) const { return _ptr <= obj._ptr; }

			bool operator>(const iterator& obj) const { return _ptr > obj._ptr; }
			bool operator>=(const iterator& obj) const { return _ptr >= obj._ptr; }

			iterator& operator++()
			{
				_ptr++;
				return *this;
			}

			iterator operator++(int)// postposition
			{
				iterator res = *this;
				++(*this);
				return res;
			}

			iterator& operator--()
			{
				_ptr--;
				return *this;
			}

			iterator operator--(int)// postposition
			{
				iterator res = *this;
				--(*this);
				return res;
			}

			iterator operator+(i64 bias)
			        const { return iterator(_ptr + bias); }

			void operator+=(i64 bias) { _ptr += bias; }

			iterator operator-(i64 bias)
			        const { return iterator(_ptr - bias); }

			void operator-=(i64 bias) { _ptr -= bias; }

			T& operator[](u64 _n) { return *((*this) + _n); }
			const T& operator[](u64 _n) const { return *((*this) + _n); }

			friend i64
			operator-(const iterator& a, const iterator& b) { return a.get() - b.get(); }
		};

		vector() = default;                                        // Void constructor
		explicit vector(u64 userInputSize);                        // Init by size
		explicit vector(u64 userInputSize, const T& userInputData);// Init by size and value
		vector(const vector<T>& obj);                              // Copy constructor
		vector(vector<T>&& src) { move(src); }                     // Move constructor
		vector(const std::initializer_list<T>& ilist);             // Init by a {ilist}
		~vector() { destroy(); }

		T* data() const { return const_cast<T*>(data_ptr); }
		u64 size() const { return v_size; }               // Return the number of elements
		u64 capacity() const { return v_capacity; }       // Return the current capacity
		bool empty() const { return v_size == 0; }        // Check whether the vector is empty
		bool exist() const { return data_ptr != nullptr; }// Check whether the vector is existed

		vector<T>& shrink_to_fit();// Reduce memory usage by freeing unused memory
		vector<T>& resize(u64 N);  // Reduce or expand size
		vector<T>& reserve(u64 N) { return (N <= v_capacity) ? *this : resize(N); }

		void clear();  // Clear all values, but don't destroy
		void destroy();// Clear the contents and release memory, contain exist()
		void print() const;

		vector<T>& push_back(const T& obj);// Add an element to the end
		vector<T>& push_back(T&& src);
		vector<T>& emplace_back();        // Add an element to the end
		vector<T>& pop_back();            // Remove the last element
		vector<T>& move(vector<T>& after);// Deprive other's ownership

		T& operator[](u64 N);// Access specified element
		const T& operator[](u64 N) const;

		vector<T>& operator=(const vector<T>& obj);// Deep copy operator
		vector<T>& operator=(vector<T>&& src) { return move(src); }

		iterator begin() const { return {const_cast<T*>(data())}; }
		iterator end() const { return {const_cast<T*>(&data_ptr[size() - 1])}; }

		T& front() { return data_ptr[0]; }
		T& back() { return data_ptr[size() - 1]; }

		const T& front() const { return data_ptr[0]; }
		const T& back() const { return data_ptr[size() - 1]; }

		template <Forward_Itr Itr>
		void assign(Itr st, Itr ed);

		template <Forward_Itr Itr>
		vector(Itr st, Itr ed);

	protected:
		pointer data_ptr = nullptr;
		u64 v_size = 0, v_capacity = 0;
	};

	template <class T>
	vector<T>::vector(u64 userInputSize)
	{
		data_ptr = new T[userInputSize + STD_CAPACITY];
		v_size = userInputSize;
		v_capacity = v_size + STD_CAPACITY;
	}

	template <class T>
	vector<T>::vector(u64 userInputSize, const T& userInputData)
	{
		data_ptr = new T[userInputSize + STD_CAPACITY];
		nuts::fill_n(data_ptr, userInputSize, userInputData);
		v_size = userInputSize;
		v_capacity = v_size + STD_CAPACITY;
	}

	template <class T>
	vector<T>::vector(const vector<T>& obj)
	{
		data_ptr = new T[obj.size() + STD_CAPACITY];
		v_size = obj.size();
		v_capacity = v_size + STD_CAPACITY;
		for (u64 i = 0; i < v_size; i++)
			data_ptr[i] = obj.data_ptr[i];
	}

	template <class T>
	vector<T>::vector(const std::initializer_list<T>& ilist)
	{
		reserve(ilist.size() + STD_CAPACITY);
		for (const T& x: ilist) push_back(x);
	}

	template <class T>
	template <Forward_Itr Itr>
	vector<T>::vector(Itr st, Itr ed)
	{
		assign(st, ed);
	}

	template <class T>
	void vector<T>::clear()
	{
		if (!empty()) v_size = 0;
	}

	template <class T>
	void vector<T>::destroy()
	{
		if (!empty())
		{
			delete[] data_ptr;
			data_ptr = nullptr;
			v_capacity = v_size = 0;
		}
	}

	template <class T>
	vector<T>& vector<T>::shrink_to_fit()
	{
		if (v_capacity > v_size)
		{
			T* tmp = new T[v_size];
			for (u64 i = 0; i < v_size; i++)
				tmp[i] = data_ptr[i];
			delete[] data_ptr;
			data_ptr = tmp;
			v_capacity = v_size;
		}
		return *this;
	}

	template <class T>
	vector<T>& vector<T>::resize(u64 N)
	{
		if (N > v_size)
		{
			T* tmp = new T[N];
			for (u64 i = 0; i < v_size; i++)
				tmp[i] = data_ptr[i];
			delete[] data_ptr;
			data_ptr = tmp;
			v_capacity = N;
			return *this;
		}
		if (N < v_size)
		{
			v_size = N;
			return *this;
		}
		return *this;
	}

	template <class T>
	vector<T>& vector<T>::emplace_back()
	{
		if (v_capacity == v_size)
		{
			if (v_size != 0)
				reserve(v_size * EXPAN_COEF);
			else
				reserve(STD_CAPACITY);
		}
		++v_size;
		return *this;
	}

	template <class T>
	vector<T>& vector<T>::push_back(const T& obj)
	{
		emplace_back();
		back() = obj;
		return *this;
	}

	template <class T>
	vector<T>& vector<T>::push_back(T&& src)
	{
		emplace_back();
		back() = static_cast<T&&>(src);
		return *this;
	}

	template <class T>
	vector<T>& vector<T>::pop_back()
	{
		if (!empty()) --v_size;
		return *this;
	}

	template <class T>
	vector<T>& vector<T>::move(vector<T>& src)
	{
		destroy();// Self destroy

		data_ptr = src.data_ptr;// Pass ownership
		v_size = src.v_size;
		v_capacity = src.v_capacity;

		src.data_ptr = nullptr;// Set invalid
		src.v_size = 0;
		src.v_capacity = 0;
		return *this;
	}

	template <class T>
	T& vector<T>::operator[](const u64 N)
	{
		assert(N < v_capacity);
		return data_ptr[N];
	}

	template <class T>
	const T& vector<T>::operator[](const u64 N) const
	{
		assert(N < v_size);
		return data_ptr[N];
	}

	template <class T>
	vector<T>& vector<T>::operator=(const vector<T>& obj)
	{
		destroy();
		assign(obj.begin(), obj.end());
		return *this;
	}

	template <class T>
	void vector<T>::print() const
	{
		auto print = [this](const auto& x) {
			nuts::print(x);
			if (&x != &back()) printf(", ");
		};

		printf("\nvector @%#llx = [", (u64) data());
		for_each(*this, print);
		printf("]\n");
	}

	template <class T>
	template <Forward_Itr Itr>
	void vector<T>::assign(Itr st, Itr ed)
	{
		for_each(st, ed, [this](const auto& x) { push_back(x); });
	}
}

#endif
