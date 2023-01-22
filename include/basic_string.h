#ifndef _NUTS_BASIC_STRING_
#define _NUTS_BASIC_STRING_

#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>

#include "algorithm.h"
#include "functional.h"
#include "iterator.h"
#include "type.h"

#ifndef STD_EXPAN
#define STD_EXPAN 5U
#endif

namespace nuts
{
	template <class T = char>
	class basic_string
	{
	public:
		using value_type = T;

	protected:
		T* data_ptr = nullptr;
		size_t v_size = 0;
		size_t v_capacity = 0;

	public:
		basic_string() = default;                                   // Void constructor
		basic_string(const basic_string<T>& src);                   // Copy constructor
		basic_string(basic_string<T>&& src) { move(src); }          // Move constructor
		basic_string(const basic_string<T>& obj, T* start, T* stop);// Copy part of string range in [start, stop]
		basic_string(const T* obj);                                 // Init by a cstring with '\0'
		basic_string(const std::initializer_list<T>& ilist);        // Init by a {ilist}
		basic_string(u64 init_size);
		~basic_string() { destroy(); }

		T* data() const { return data_ptr; }
		T* c_str() const { return data_ptr; }
		size_t size() const { return v_size; }            // Return the number of elements
		size_t capacity() const { return v_capacity; }    // Return the current capacity
		void clear();                                     // Clear all values, but don't destroy
		bool empty() const { return v_size == 0; }        // Check whether the string is empty
		bool exist() const { return data_ptr != nullptr; }// Check whether the vector is existed
		void destroy();                                   // Clear the contents and release memory, contain exist()
		basic_string<T>& shrink_to_fit();                 // Reduce memory usage by freeing unused memory
		basic_string<T>& resize(size_t N);                // Reduce or expand capacity
		basic_string<T>& push_back(const T& obj);         // Add an element to the end
		basic_string<T>& pop_back();                      // Remove the last element
		basic_string<T>& move(basic_string<T>& src);

		T& front() { return data_ptr[0]; }
		T& back() { return data_ptr[size() - 1]; }
		const T& front() const { return data_ptr[0]; }
		const T& back() const { return data_ptr[size() - 1]; }

		T& operator[](size_t N);
		const T& operator[](size_t N) const;
		basic_string<T>& operator=(const basic_string<T>& obj);// Deep copy operator
		basic_string<T>& operator=(basic_string<T>&& src) { return move(src); }
		basic_string<T>& operator=(const T* obj);                         // Covered by a cstring
		basic_string<T>& operator=(const std::initializer_list<T>& ilist);// Covered by a {ilist}

		basic_string<T>& operator+=(const basic_string<T>& obj);// Connect two strings
		basic_string<T>& operator+=(const T* obj);              // Connect two strings

		basic_string<T> operator()(const T* obj) const;

		size_t find(const T& c) const;                // search a char
		size_t find(const T* obj) const;              // search a substr
		size_t find(const basic_string<T>& obj) const;// search a substr

		void _POP_EXC_0()
		{
			while (size() >= 2 &&
			       data_ptr[size() - 2] == '\0') pop_back();
		}

		friend std::ostream&
		operator<<(std::ostream& output, const basic_string<T>& obj)
		{
			output << obj.data();
			return output;
		}

		friend std::istream&
		operator>>(std::istream& input, basic_string<T>& obj)
		{
			input.get();
			char tmp;
			while ((tmp = getchar()) != '\n' && tmp != '\r')
				obj.push_back(tmp);
			obj.push_back('\0');
			return input;
		}

		class iterator : public random_access_iterator
		{
		public:
			using value_type = T;

		protected:
			T* _ptr = nullptr;

		public:
			iterator() = default;
			iterator(T* obj)
			{
				_ptr = obj;
			}
			iterator(const iterator& obj)
			{
				_ptr = obj._ptr;
			}

			T* get() const { return const_cast<T*>(_ptr); }

			T& operator*() { return *_ptr; }
			const T& operator*() const { return *_ptr; }

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

			inline bool operator==(T* obj) const { return _ptr == obj; }
			inline bool operator!=(T* obj) const { return _ptr != obj; }

			inline bool operator==(const iterator& obj) const { return _ptr == obj._ptr; }
			inline bool operator!=(const iterator& obj) const { return _ptr != obj._ptr; }

			inline bool operator<(const iterator& obj) const { return _ptr < obj._ptr; }
			inline bool operator<=(const iterator& obj) const { return _ptr <= obj._ptr; }

			inline bool operator>(const iterator& obj) const { return _ptr > obj._ptr; }
			inline bool operator>=(const iterator& obj) const { return _ptr >= obj._ptr; }

			iterator& operator++()
			{
				_ptr++;
				return *this;
			}

			iterator operator++(int)
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

			iterator operator--(int)
			{
				iterator res = *this;
				--(*this);
				return res;
			}

			iterator operator+(i64 bias) { return iterator(_ptr + bias); }

			void operator+=(i64 bias) { _ptr += bias; }

			iterator operator-(i64 bias) { return iterator(_ptr - bias); }

			void operator-=(i64 bias) { _ptr -= bias; }

			T& operator[](u64 _n) { return *((*this) + _n); }
			const T& operator[](u64 _n) const { return *((*this) + _n); }

			friend i64 operator-(const iterator& a,
			                     const iterator& b) { return a.get() - b.get(); }
		};

		iterator begin()
		        const { return iterator(data_ptr); }

		iterator end()
		        const { return iterator(&data_ptr[size() - 1]); }
	};

	using string = basic_string<char>;// Default in char_8bit type

	template <class T>
	bool operator==(const basic_string<T>& a, const basic_string<T>& b)
	{
		u64 _n = a.size() < b.size() ? a.size() : b.size();
		return __memcmpeq(a.data(), b.data(), _n) == 0;
	}

	template <class T>
	bool operator==(const basic_string<T>& a, const T* b)
	{
		u64 _n = a.size() < strlen(b) ? a.size() : strlen(b);
		return __memcmpeq(a.data(), b, _n) == 0;
	}

	template <class T>
	bool operator!=(const basic_string<T>& a, const basic_string<T>& b)
	{
		return !(a == b);
	}

	template <class T>
	bool operator!=(const basic_string<T>& a, const T* b)
	{
		return !(a == b);
	}

	template <class T>
	bool operator<(const basic_string<T>& a, const basic_string<T>& b)
	{
		u64 _n = a.size() < b.size() ? a.size() : b.size();
		return memcmp(a.data(), b.data(), _n) < 0;
	}

	template <class T>
	bool operator<(const basic_string<T>& a, const T* b)
	{
		u64 _n = a.size() < strlen(b) ? a.size() : strlen(b);
		return memcmp(a.data(), b, _n) < 0;
	}

	template <class T>
	bool operator>(const basic_string<T>& a, const basic_string<T>& b)
	{
		u64 _n = a.size() < b.size() ? a.size() : b.size();
		return memcmp(a.data(), b.data(), _n) > 0;
	}

	template <class T>
	bool operator>(const basic_string<T>& a, const T* b)
	{
		u64 _n = a.size() < strlen(b) ? a.size() : strlen(b);
		return memcmp(a.data(), b, _n) > 0;
	}

	template <class T>
	bool operator<=(const basic_string<T>& a, const basic_string<T>& b)
	{
		u64 _n = a.size() < b.size() ? a.size() : b.size();
		return memcmp(a.data(), b.data(), _n) <= 0;
	}

	template <class T>
	bool operator<=(const basic_string<T>& a, const T* b)
	{
		u64 _n = a.size() < strlen(b) ? a.size() : strlen(b);
		return memcmp(a.data(), b, _n) <= 0;
	}

	template <class T>
	bool operator>=(const basic_string<T>& a, const basic_string<T>& b)
	{
		u64 _n = a.size() < b.size() ? a.size() : b.size();
		return memcmp(a.data(), b.data(), _n) >= 0;
	}

	template <class T>
	bool operator>=(const basic_string<T>& a, const T* b)
	{
		u64 _n = a.size() < strlen(b) ? a.size() : strlen(b);
		return memcmp(a.data(), b, _n) >= 0;
	}

	template <class T>
	basic_string<T>::basic_string(const basic_string<T>& src)
	{
		data_ptr = new T[src.size() + STD_EXPAN];
		v_size = src.size();
		v_capacity = v_size + STD_EXPAN;
		strncpy(data_ptr, src.data_ptr, v_size);
		_POP_EXC_0();
	}

	template <class T>
	basic_string<T>::basic_string(const T* obj)
	{
		v_size = strlen(obj);
		v_capacity = v_size + STD_EXPAN;
		data_ptr = new T[v_size + STD_EXPAN];
		memcpy(data_ptr, obj, v_size + 1);
		v_size++;
		_POP_EXC_0();
	}

	template <class T>
	basic_string<T>::basic_string(const std::initializer_list<T>& ilist)
	{
		v_size = ilist.size();
		v_capacity = v_size + STD_EXPAN;
		data_ptr = new T[v_size + STD_EXPAN];
		for (const T& x: ilist) push_back(x);
		_POP_EXC_0();
	}

	template <class T>
	basic_string<T>& basic_string<T>::move(basic_string<T>& src)
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
	basic_string<T>::basic_string(u64 init_size)
	{
		data_ptr = new T[init_size];
		v_size = v_capacity = init_size;
	}

	template <class T>
	void basic_string<T>::clear()
	{
		if (exist()) v_size = 0;
	}

	template <class T>
	void basic_string<T>::destroy()
	{
		if (exist())
		{
			delete[] data_ptr;
			data_ptr = nullptr;
			v_size = 0;
			v_capacity = 0;
		}
	}

	template <class T>
	basic_string<T>& basic_string<T>::shrink_to_fit()
	{
		if (v_capacity > v_size)
		{
			T* tmp = new T[v_size];
			strncpy(tmp, data_ptr, v_size);
			delete[] data_ptr;
			data_ptr = tmp;
			v_capacity = v_size;
			_POP_EXC_0();
			return *this;
		}
		_POP_EXC_0();
		return *this;
	}

	template <class T>
	basic_string<T>& basic_string<T>::resize(size_t N)
	{
		if (N != v_size)
		{
			T* tmp = new T[N];
			memcpy(tmp, data_ptr, N < size() ? N : size());
			delete[] data_ptr;
			data_ptr = tmp;
			v_capacity = N;
			if (N < v_size) v_size = N;
		}
		return *this;
	}

	template <class T>
	basic_string<T>& basic_string<T>::push_back(const T& obj)
	{
		if (v_capacity == v_size)
		{
			if (v_size != 0)
				resize(v_size * 2);
			else
				resize(STD_EXPAN);
		}
		data_ptr[v_size] = obj;
		++v_size;
		return *this;
	}

	template <class T>
	basic_string<T>& basic_string<T>::pop_back()
	{
		if (exist())
		{
			data_ptr[v_size - 1] = '\0';
			--v_size;
			return *this;
		}
		return *this;
	}

	template <class T>
	T& basic_string<T>::operator[](const size_t N)
	{
		assert(N < v_capacity);
		return data_ptr[N];
	}

	template <class T>
	const T& basic_string<T>::operator[](const size_t N) const
	{
		assert(N < v_capacity);
		return data_ptr[N];
	}

	template <class T>
	basic_string<T>& basic_string<T>::operator=(const basic_string<T>& obj)
	{
		destroy();
		data_ptr = new T[obj.size()];
		v_size = obj.size();
		v_capacity = v_size;
		strncpy(data_ptr, obj.data(), obj.size());
		return *this;
	}

	template <class T>
	basic_string<T>& basic_string<T>::operator=(const T* obj)
	{
		destroy();
		v_capacity = strlen(obj);
		v_size = v_capacity;
		data_ptr = new T[v_size + 1];
		strncpy(data_ptr, obj, v_size + 1);
		return *this;
	}

	template <class T>
	basic_string<T>& basic_string<T>::operator=(const std::initializer_list<T>& ilist)
	{
		destroy();
		v_size = ilist.size();
		v_capacity = v_size + STD_EXPAN;
		data_ptr = new T[v_size + STD_EXPAN];
		for (const T& x: ilist) push_back(x);
		return *this;
	}

	template <class T>
	basic_string<T> operator+(const basic_string<T>& a, const basic_string<T>& b)
	{
		basic_string<T> result = a;
		result._POP_EXC_0();
		result += b;
		return result;
	}

	template <class T>
	basic_string<T> operator+(const basic_string<T>& a, const T* b)
	{
		basic_string<T> result = a;
		result._POP_EXC_0();
		result += b;
		return result;
	}

	template <class T>
	basic_string<T> operator+(const T* a, const basic_string<T>& b)
	{
		basic_string<T> result = a;
		result._POP_EXC_0();
		result += b;
		return result;
	}

	template <class T>
	basic_string<T>&
	basic_string<T>::operator+=(const basic_string<T>& obj)
	{
		_POP_EXC_0();
		u64 _n = size() + obj.size() + STD_EXPAN;
		resize(_n);
		v_size = _n;
		strcat(data_ptr, obj.data());
		_POP_EXC_0();
		return *this;
	}

	template <class T>
	basic_string<T>& basic_string<T>::operator+=(const T* obj)
	{
		_POP_EXC_0();
		u64 _n = size() + strlen(obj) + STD_EXPAN;
		resize(_n);
		v_size = _n;
		strcat(data_ptr, obj);
		_POP_EXC_0();
		return *this;
	}

	template <class T>
	basic_string<T>
	basic_string<T>::operator()(const T* obj) const
	{
		return basic_string<T>(obj);
	}

	template <class T>
	size_t basic_string<T>::find(const T& c) const
	{
		return (strchr(data_ptr, c) - data_ptr) / sizeof(T);
	}

	template <class T>
	size_t basic_string<T>::find(const T* target) const
	{
		return (strstr(data_ptr, target) - data_ptr) / sizeof(T);
	}

	template <class T>
	size_t basic_string<T>::find(const basic_string<T>& target) const
	{
		return (strstr(data_ptr, target.data()) - data_ptr) / sizeof(T);
	}

	template <class T = char>
	class small_string
	{
	public:
		using value_type = T;
		using self_type = small_string<T>;

		small_string() = default;
		~small_string() = default;
		small_string(const T* src);
		small_string(const self_type& src);

		T& operator[](u64 pos) { return impl[pos]; }
		const T& operator[](u64 pos) const { return impl[pos]; }

		self_type& operator=(const T* src);
		self_type& operator=(const self_type& src);

		friend std::ostream&
		operator<<(std::ostream& output, const small_string<T>& obj)
		{
			for (int i = 0; i < 24; i++)
				output << obj[i];
			return output;
		}

	protected:
		value_type impl[24] = {'\0'};
	};

	template <typename T>
	small_string<T>::small_string(const value_type* src)
	{
		auto k = strlen(src) + 1;
		strncpy(impl, src, k <= 24 ? k : 24);
	}

	template <typename T>
	small_string<T>::small_string(const self_type& src)
	{
		strncpy(impl, src.impl, 24);
	}

	template <typename T>
	small_string<T>& small_string<T>::operator=(const value_type* src)
	{
		auto k = strlen(src) + 1;
		strncpy(impl, src, k <= 24 ? k : 24);
		return *this;
	}

	template <typename T>
	small_string<T>& small_string<T>::operator=(const self_type& src)
	{
		strncpy(impl, src.impl, 24);
		return *this;
	}

	template <>
	struct hash<string>
	{
		u64 operator()(const string& x) const
		{
			u64 i = 0;
			for_each(x, [&i](const auto& c) { i += (u64) c & 0xbf5847; });

			auto hasher = [](u64 x) -> u64 {
				x = (x ^ (x >> 30)) * u64(0xbf58476d1ce4e5b9);
				x = (x ^ (x >> 27)) * u64(0x94d049bb133112eb);
				x = x ^ (x >> 31);
				return x;
			};

			return hasher(i);
		}
	};
}

#endif
