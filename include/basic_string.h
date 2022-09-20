#ifndef _NUTS_BASIC_STRING_
#define _NUTS_BASIC_STRING_ 1

#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>

#include "algorithm.h"
#include "functional.h"
#include "iterator.h"
#include "type.h"

#define STD_EXPAN 5

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

		T* get() const { return data_ptr; }
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
		bool operator==(const basic_string<T>& obj) const;                // Compare two strings
		bool operator==(const T* obj) const;                              // Compare two strings in cstring
		bool operator!=(const basic_string<T>& obj) const;
		bool operator!=(const T* obj) const;
		bool operator<(const basic_string<T>& obj) const;
		bool operator<(const T* obj) const;
		bool operator>(const basic_string<T>& obj) const;
		bool operator>(const T* obj) const;

		basic_string<T> operator+(const basic_string<T>& obj) const;// Connect two strings
		basic_string<T> operator+(const T* obj) const;              // Connect two strings
		basic_string<T>& operator+=(const basic_string<T>& obj);    // Connect two strings
		basic_string<T>& operator+=(const T* obj);                  // Connect two strings

		basic_string<T> operator()(const T* obj) const;

		size_t find(const T& c) const;  // search a char
		size_t find(const T* obj) const;// search a substr

		void _POP_EXC_0()
		{
			while (size() >= 2 && data_ptr[size() - 2] == '\0') pop_back();
		}

		template <class Type>
		friend basic_string<Type> operator+(const Type* array, const basic_string<Type>& str);// Connect two strings

		friend std::ostream&
		operator<<(std::ostream& output, const basic_string<T>& obj)
		{
			for (int i = 0; i < obj.size(); i++)
				output << obj.data_ptr[i];
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
	basic_string<T>::basic_string(const basic_string<T>& src)
	{
		data_ptr = new T[src.size() + STD_EXPAN];
		v_size = src.size();
		v_capacity = v_size + STD_EXPAN;
		strncpy(data_ptr, src.data_ptr, v_size);
		_POP_EXC_0();
		// for (size_t i = 0; i < v_size; i++)
		// 	data_ptr[i] = src.data_ptr[i];
	}

	template <class T>
	basic_string<T>::basic_string(const T* obj)
	{
		v_size = strlen(obj);
		v_capacity = v_size + STD_EXPAN;
		data_ptr = new T[v_size + STD_EXPAN];
		strncpy(data_ptr, obj, v_size + 1);
		v_size++;
		_POP_EXC_0();

		// for (size_t i = 0; i <= v_size; i++)
		// 	data_ptr[i] = obj[i];
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
			for (size_t i = 0; i < v_size && i < N; i++)
				tmp[i] = data_ptr[i];
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

	// template <class T>
	// basic_string<T> &basic_string<T>::deprive(basic_string<T> &after)
	// {
	//     this->destroy();
	//     this->data_ptr = after.data_ptr; // Pass ownership
	//     this->v_size = after.v_size;
	//     this->v_capacity = after.v_capacity;

	//     after.data_ptr = nullptr; // Set invalid
	//     after.v_size = 0;
	//     after.v_capacity = 0;
	//     return *this;
	// }

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
		for (size_t i = 0; i < v_size; i++)
			data_ptr[i] = obj.data_ptr[i];
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

		// for (size_t i = 0; obj[i] != '\0'; i++)
		// 	this->v_size++;
		// this->data_ptr = new T[this->v_size + STD_EXPAN];
		// this->v_capacity = this->v_size + STD_EXPAN;
		// strcpy(data_ptr, obj);
		// return *this;
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
	bool basic_string<T>::operator==(const basic_string<T>& obj) const
	{
		unsigned count = 0;
		for (size_t i = 0; i < v_size && i < obj.v_size; i++)
		{
			if (data_ptr[i] == obj.data_ptr[i])
				count++;
		}
		return count == v_size && count == obj.v_size;
	}

	template <class T>
	bool basic_string<T>::operator==(const T* obj) const
	{
		size_t obj_len = 0;
		for (size_t i = 0; obj[i] != '\0'; i++)
			obj_len++;
		unsigned count = 0;
		for (size_t i = 0; i < v_size && i < obj_len; i++)
		{
			if (data_ptr[i] == obj[i])
				count++;
		}
		return count == v_size && count == obj_len;
	}

	template <class T>
	bool basic_string<T>::operator!=(const basic_string<T>& obj) const
	{
		bool flag = this->operator==(obj);
		return !flag;
	}

	template <class T>
	bool basic_string<T>::operator!=(const T* obj) const
	{
		bool flag = this->operator==(obj);
		return !flag;
	}

	template <class T>
	bool basic_string<T>::operator<(const basic_string<T>& obj) const
	{
		for (size_t i = 0; i < size(); i++)
		{
			if ((*this)[i] != obj[i])
				return (*this)[i] < obj[i];
		}
		if (size() == obj.size())
			return false;
		else
			return size() < obj.size();
	}

	template <class T>
	bool basic_string<T>::operator<(const T* obj) const
	{
		string copy = obj;
		return this->operator<(copy);
	}

	template <class T>
	bool basic_string<T>::operator>(const basic_string<T>& obj) const
	{
		bool flag = this->operator<(obj);
		return !flag;
	}

	template <class T>
	bool basic_string<T>::operator>(const T* obj) const
	{
		string copy = obj;
		return this->operator>(copy);
	}

	template <class T>
	basic_string<T> basic_string<T>::operator+(const basic_string<T>& obj) const
	{
		_POP_EXC_0();
		basic_string<T> result = *this;
		result.resize(v_size + obj.v_size + STD_EXPAN);
		for (size_t i = 0; i < obj.size(); i++)
			result[v_size + i] = obj.data_ptr[i];
		result.v_size = v_size + obj.v_size;
		result.shrink_to_fit();
		return result;
	}

	template <class T>
	basic_string<T>
	basic_string<T>::operator+(const T* obj) const
	{
		_POP_EXC_0();
		size_t obj_len = 0;
		for (size_t i = 0; obj[i] != '\0'; i++)
			obj_len++;
		basic_string<T> result(*this);
		result.resize(v_size + obj_len + STD_EXPAN);
		for (size_t i = 0; i <= obj_len; i++)
			result[v_size + i - 1] = obj[i];
		result.v_size = v_size + obj_len;
		result.push_back('\0');
		result.shrink_to_fit();
		return result;
	}

	template <class Type>
	basic_string<Type> operator+(const Type* array, const basic_string<Type>& str)
	{
		// str._POP_EXC_0();
		size_t a_len = 0;
		for (size_t i = 0; array[i] != '\0'; i++)
			a_len++;
		basic_string<Type> result(array);
		result.resize(a_len + str.v_size + STD_EXPAN);
		for (size_t i = 0; i < str.v_size; i++)
			result[a_len + i] = str[i];
		result.v_size = str.v_size + a_len;
		result.push_back('\0');
		result.shrink_to_fit();
		return result;
	}

	template <class T>
	basic_string<T>&
	basic_string<T>::operator+=(const basic_string<T>& obj)
	{
		_POP_EXC_0();
		for (int i = 0; i < obj.size(); i++)
			push_back(obj[i]);
		_POP_EXC_0();
		return *this;
	}

	template <class T>
	basic_string<T>& basic_string<T>::operator+=(const T* obj)
	{
		_POP_EXC_0();
		size_t obj_len = 0;
		for (size_t i = 0; obj[i] != '\0'; i++)
			obj_len++;
		for (size_t i = 0; i <= obj_len; i++)
			push_back(obj[i]);
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
		for (size_t i = 0, s = size(); i < s; i++)
		{
			if (*(this)[i] == c[i])
				return i;
		}
	}

	template <class T>
	size_t basic_string<T>::find(const T* target) const
	{
		return (strstr(data_ptr, target) - data_ptr) / sizeof(T);
	}

	//  Hash Specialization
	template <>
	struct hash<nuts::string>
	{
		u64 operator()(const nuts::string& x) const
		{
			u64 i = 0;
			for_each(x.begin(), x.end(),
			         [&i](const auto& c) { i += (u64) c; });

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
