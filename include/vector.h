#ifndef _NUTS_VECTOR_
#define _NUTS_VECTOR_

#include <cassert>
#include <cstddef>
#include <iostream>

#include "algorithm.h"
#include "iterator.h"
#include "type.h"

#ifndef STD_EXPAN
#define STD_EXPAN 5U
#endif

#ifndef EXPAN_COEF
#define EXPAN_COEF 2U
#endif

namespace nuts
{
	template <class T>
	class vector
	{
	public:
		using value_type = T;
		using pointer = T*;

		class iterator
		    : public random_access_iterator
		{
		public:
			using value_type = T;

		protected:
			pointer _ptr = nullptr;

		public:
			iterator() = default;
			iterator(pointer obj) noexcept { _ptr = obj; }
			iterator(std::nullptr_t obj) noexcept : _ptr(obj) {}
			iterator(const iterator& obj) noexcept : _ptr(obj._ptr) {}

			inline pointer get() const noexcept { return _ptr; }

			inline value_type& operator*() { return *_ptr; }
			inline const value_type& operator*() const { return *_ptr; }

			inline pointer operator->() const noexcept { return _ptr; }

			inline iterator& operator=(value_type* obj) noexcept
			{
				_ptr = obj;
				return *this;
			}

			inline iterator& operator=(const iterator& obj) noexcept
			{
				_ptr = obj._ptr;
				return *this;
			}

			inline bool operator==(T* obj) const noexcept { return _ptr == obj; }
			inline bool operator!=(T* obj) const noexcept { return _ptr != obj; }

			inline bool operator==(const iterator& obj) const noexcept { return _ptr == obj._ptr; }
			inline bool operator!=(const iterator& obj) const noexcept { return _ptr != obj._ptr; }

			inline bool operator<(const iterator& obj) const noexcept { return _ptr < obj._ptr; }
			inline bool operator<=(const iterator& obj) const noexcept { return _ptr <= obj._ptr; }

			inline bool operator>(const iterator& obj) const noexcept { return _ptr > obj._ptr; }
			inline bool operator>=(const iterator& obj) const noexcept { return _ptr >= obj._ptr; }

			inline iterator& operator++()
			{
				_ptr++;
				return *this;
			}

			inline iterator operator++(int)// postposition
			{
				iterator res = *this;
				++(*this);
				return res;
			}

			inline iterator& operator--()
			{
				_ptr--;
				return *this;
			}

			inline iterator operator--(int)// postposition
			{
				iterator res = *this;
				--(*this);
				return res;
			}

			inline iterator operator+(i64 bias)
			        const noexcept { return iterator(_ptr + bias); }

			inline void operator+=(i64 bias) noexcept { _ptr += bias; }

			inline iterator operator-(i64 bias)
			        const noexcept { return iterator(_ptr - bias); }

			inline void operator-=(i64 bias) noexcept { _ptr -= bias; }

			inline value_type& operator[](u64 _n) noexcept { return *((*this) + _n); }
			inline const T& operator[](u64 _n) const noexcept { return *((*this) + _n); }

			friend i64
			operator-(const iterator& a, const iterator& b) { return a.get() - b.get(); }
		};

		vector() = default;                                        // Void constructor
		explicit vector(u64 userInputSize);                        // Init by size
		explicit vector(u64 userInputSize, const T& userInputData);// Init by size and value
		vector(const vector<T>& obj);                              // Copy constructor
		vector(vector<T>&& src) noexcept { move(src); }            // Move constructor
		vector(const std::initializer_list<T>& ilist);   // Init by a {ilist}
		~vector() noexcept { destroy(); }

		inline T* data() const noexcept { return const_cast<T*>(data_ptr); }
		inline u64 size() const noexcept { return v_size; }               // Return the number of elements
		inline u64 capacity() const noexcept { return v_capacity; }       // Return the current capacity
		inline bool empty() const noexcept { return v_size == 0; }        // Check whether the vector is empty
		inline bool exist() const noexcept { return data_ptr != nullptr; }// Check whether the vector is existed

		vector<T>& shrink_to_fit();// Reduce memory usage by freeing unused memory
		vector<T>& resize(u64 N);  // Reduce or expand size
		vector<T>& reserve(u64 N) { return (N <= v_capacity) ? *this : resize(N); }
		vector<T>& expand();

		inline void clear();// Clear all values, but don't destroy
		void destroy();     // Clear the contents and release memory, contain exist()
		void print() const;

		void push_back(const T& obj);// Add an element to the end
		void push_back(T&& src);
		void emplace_back();// Add an element to the end
		void emplace_back(const T& val);
		void emplace_back(T&& val);

		inline void pop_back();                    // Remove the last element
		vector<T>& move(vector<T>& after) noexcept;// Deprive other's ownership

		inline T& operator[](u64 N) noexcept;// Access specified element
		inline const T& operator[](u64 N) const noexcept;

		inline T& at(u64 N) noexcept;
		inline const T& at(u64 N) const noexcept;

		vector<T>& operator=(const vector<T>& obj);// Deep copy operator
		inline vector<T>& operator=(vector<T>&& src) { return move(src); }

		inline iterator begin() const { return {const_cast<T*>(data())}; }
		inline iterator end() const
		{
			return size() == 0 ? begin()
			                   : iterator {const_cast<T*>(&data_ptr[size() - 1])};
		}

		inline T& front() { return *begin(); }
		inline T& back() { return *end(); }

		inline const T& front() const { return *begin(); }
		inline const T& back() const { return *end(); }

		template <Forward_Itr Itr>
		void assign(Itr st, Itr ed);

		template <Forward_Itr Itr>
		vector(Itr st, Itr ed);

	protected:
		pointer data_ptr = nullptr;
		u64 v_size = 0, v_capacity = 0;
	};

	// Deduction Guide
	template <class T>
	vector(const std::initializer_list<T>&) -> vector<T>;

	template <Forward_Itr Itr>
	vector(Itr st, Itr ed) -> vector<typename Itr::value_type>;

	template <class T>
	vector<T>::vector(u64 userInputSize)
	{
		data_ptr = new T[userInputSize + STD_EXPAN];
		v_size = userInputSize;
		v_capacity = v_size + STD_EXPAN;
	}

	template <class T>
	vector<T>::vector(u64 userInputSize, const T& userInputData)
	{
		data_ptr = new T[userInputSize + STD_EXPAN];
		nuts::fill_n(data_ptr, userInputSize, userInputData);
		v_size = userInputSize;
		v_capacity = v_size + STD_EXPAN;
	}

	template <class T>
	vector<T>::vector(const vector<T>& obj)
	{
		data_ptr = new T[obj.size() + STD_EXPAN];
		v_size = obj.size();
		v_capacity = v_size + STD_EXPAN;
		memcpy(data_ptr, obj.data(), sizeof(T) * obj.size());
	}

	template <class T>
	vector<T>::vector(const std::initializer_list<T>& ilist)
	{
		reserve(ilist.size() + STD_EXPAN);
		for (const T& x: ilist) push_back(x);
	}

	template <class T>
	template <Forward_Itr Itr>
	vector<T>::vector(Itr st, Itr ed)
	{
		assign(st, ed);
	}

	template <class T>
	inline void vector<T>::clear()
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
				tmp[i] = nuts::move(data_ptr[i]);
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
				tmp[i] = nuts::move(data_ptr[i]);
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
	vector<T>& vector<T>::expand()
	{
		if (v_size != 0)
			reserve(v_size * EXPAN_COEF);
		else
			reserve(STD_EXPAN);
		return *this;
	}

	template <class T>
	void vector<T>::emplace_back()
	{
		if (v_capacity == v_size)
			expand();
		++v_size;
	}

	template <class T>
	void vector<T>::emplace_back(T&& val)
	{
		if (v_capacity == v_size)
			expand();
		auto ret = *new (data_ptr + v_size++) T(val);
	}

	template <class T>
	void vector<T>::emplace_back(const T& val)
	{
		T tmp = val;
		emplace_back(nuts::move(tmp));
	}

	template <class T>
	void vector<T>::push_back(const T& src)
	{
		emplace_back(src);
	}

	template <class T>
	void vector<T>::push_back(T&& src)
	{
		emplace_back(src);
	}

	template <class T>
	inline void vector<T>::pop_back()
	{
		if (!empty())
			--v_size;
	}

	template <class T>
	vector<T>& vector<T>::move(vector<T>& src) noexcept
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
	inline T& vector<T>::operator[](const u64 N) noexcept
	{
		// assert(N < v_capacity);
		return data_ptr[N];
	}

	template <class T>
	inline const T& vector<T>::operator[](const u64 N) const noexcept
	{
		assert(N < v_size);
		return data_ptr[N];
	}

	template <class T>
	inline T& vector<T>::at(const u64 N) noexcept
	{
		assert(N < v_capacity);
		return data_ptr[N];
	}

	template <class T>
	inline const T& vector<T>::at(const u64 N) const noexcept
	{
		// assert(N < v_size);
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
		auto print = [&](const auto& x) {
			nuts::print(x);
			if (&x != &back()) printf(", ");
		};

		printf("vector @%#llx = [", (u64) data());
		auto st = begin(), ed = end();
		for_each(st, ed, print);
		printf("]\n");
	}

	template <class T>
	template <Forward_Itr Itr>
	void vector<T>::assign(Itr st, Itr ed)
	{
		for_each(st, ed, [&](const auto& x) { push_back(x); });
	}
}

#endif
