#ifndef _NUTS_BASIC_STRING_
#define _NUTS_BASIC_STRING_

#ifndef _DEBUG
#define _DEBUG 1
#endif

#ifndef _WARNING
#define _WARNING 1
#endif

#include <iostream>
#include <cstring>
#include <cstddef>
#include <cassert>

#define STD_EXPAN 5

namespace nuts
{
	template <class T = char>
	class basic_string
	{
	protected:
		T *data_ptr = nullptr;
		size_t v_size = 0;
		size_t v_capacity = 0;

	public:
		basic_string() = default;									 // Void constructor
		basic_string(const basic_string<T> &obj);					 // Copy constructor, copy another string
		basic_string(const basic_string<T> &obj, T *start, T *stop); // Copy part of string range in [start, stop]
		basic_string(const T *obj);									 // Init by a cstring with '\0'
		basic_string(std::initializer_list<T> ilist);				 // Init by a {ilist}
		~basic_string() { this->destroy(); }

		size_t size() const { return this->v_size; }		 // Return the number of elements
		size_t capacity() const { return this->v_capacity; } // Return the current capacity
		void clear();										 // Clear all values, but don't destroy
		bool empty() { return this->v_size == 0; }			 // Check whether the string is empty
		bool exist() { return this->data_ptr != nullptr; }	 // Check whether the vector is existed
		void destroy();										 // Clear the contents and release memory, contain exist()
		basic_string<T> &shrink_to_fit();					 // Reduce memory usage by freeing unused memory
		basic_string<T> &resize(size_t N);					 // Reduce or expand capacity
		basic_string<T> &push_back(const T &obj);			 // Add an element to the end
		basic_string<T> &pop_back();						 // Remove the last element

		T &front() { return this->data_ptr[0]; }
		T &back() { return this->data_ptr[this->size() - 1]; }
		const T &front() const { return this->data_ptr[0]; }
		const T &back() const { return this->data_ptr[this->size() - 1]; }

		T &operator[](size_t N); // Access specified element in index
		const T &operator[](size_t N) const;
		basic_string<T> &operator=(const basic_string<T> &obj);		// Deep copy operator
		basic_string<T> &operator=(const T *obj);					// Covered by a cstring
		basic_string<T> &operator=(std::initializer_list<T> ilist); // Covered by a {ilist}
		bool operator==(const basic_string<T> &obj) const;			// Compare two strings
		bool operator==(const T *obj) const;						// Compare two strings in cstring
		bool operator!=(const basic_string<T> &obj) const;
		bool operator!=(const T *obj) const;
		bool operator<(const basic_string<T> &obj) const;
		bool operator<(const T *obj) const;
		bool operator>(const basic_string<T> &obj) const;
		bool operator>(const T *obj) const;

		basic_string<T> operator+(const basic_string<T> &obj);	 // Connect two strings
		basic_string<T> operator+(const T *obj);				 // Connect two strings
		basic_string<T> &operator+=(const basic_string<T> &obj); // Connect two strings
		basic_string<T> &operator+=(const T *obj);				 // Connect two strings

		basic_string<T> operator()(const T *obj);

		size_t find(const T &c);   // search a char
		size_t find(const T *obj); // search a substr

		void _POP_EXC_0()
		{
			while (this->size() >= 2 && this->data_ptr[this->size() - 2] == '\0')
				this->pop_back();
		}

		template <class Type>
		friend basic_string<Type> operator+(const Type *array, basic_string<Type> &str); // Connect two strings

		friend std::ostream &
		operator<<(std::ostream &output, const basic_string<T> &obj)
		{
			for (int i = 0; i < obj.size(); i++)
				output << obj.data_ptr[i];
			return output;
		}

		friend std::istream &
		operator>>(std::istream &input, basic_string<T> &obj)
		{
			input.get();
			char tmp;
			while ((tmp = getchar()) != '\n' && tmp != '\r')
				obj.push_back(tmp);
			obj.push_back('\0');
			return input;
		}

		class iterator
		{
		protected:
			T *ptr_onNode = nullptr;

		public:
			iterator() = default;
			iterator(T *obj)
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

			const iterator operator++(int)
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

			const iterator operator--(int)
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

			int operator-(const iterator &b)
			{
				return this->ptr_onNode - b.ptr_onNode;
			}

			T *operator->() { return ptr_onNode; }

			template <class>
			friend class basic_string;
		};

		iterator begin() // Return iterator to the first element
		{
			return iterator(this->data_ptr);
		}

		iterator end() // Return iterator to the last element
		{
			return iterator(&this->data_ptr[this->size() - 1]);
		}
	};

	// using string = basic_string<char>; // Default in char_8bit type
	typedef basic_string<char> string;

	template <class T>
	basic_string<T>::basic_string(const basic_string<T> &obj)
	{
		this->data_ptr = new T[obj.size() + STD_EXPAN];
		this->v_size = obj.size();
		this->v_capacity = this->v_size + STD_EXPAN;
		for (size_t i = 0; i < this->v_size; i++)
			this->data_ptr[i] = obj.data_ptr[i];
		_POP_EXC_0();
	}

	//    template <class T>
	//    basic_string<T>::basic_string(const basic_string<T> &obj, T *start, T *stop)
	//    {
	//        size_t N = (stop - start) / sizeof(T);
	//        this->data_ptr = new T[N + STD_EXPAN];
	//        this->v_size = N;
	//        this->v_capacity = this->v_size + STD_EXPAN;
	//        for (size_t i = 0; i < this->v_size; i++)
	//        {
	//            this->data_ptr[i] = start[i];
	//        }
	//        _POP_EXC_0();
	//    }

	template <class T>
	basic_string<T>::basic_string(const T *obj)
	{
		this->v_size = strlen(obj);
		this->data_ptr = new T[this->v_size + STD_EXPAN];
		this->v_capacity = this->v_size + STD_EXPAN;
		for (size_t i = 0; i <= this->v_size; i++)
			this->data_ptr[i] = obj[i];
		this->v_size++;
		_POP_EXC_0();

		// this->destroy();
		// this->v_capacity = strlen(obj);
		// this->v_size = this->v_capacity;
		// data_ptr = new T[this->v_size + 1];
		// strncpy(data_ptr, obj, this->v_size + 1);
		// data_ptr[v_size] = '\0';
	}

	template <class T>
	basic_string<T>::basic_string(const std::initializer_list<T> ilist)
	{
		this->v_size = ilist.size();
		this->v_capacity = this->v_size + STD_EXPAN;
		this->data_ptr = new T[v_size + STD_EXPAN];
		T *q = this->data_ptr;
		for (auto p = ilist.begin(); p != ilist.end(); ++p)
		{
			*q = *p;
			q++;
		}
		_POP_EXC_0();
	}

	template <class T>
	void basic_string<T>::clear()
	{
		if (this->exist())
		{
			T VOID;
			for (int i = 0; i < this->v_size; i++)
				this->data_ptr[i] = VOID;
			this->v_size = 0;
		}
	}

	template <class T>
	void basic_string<T>::destroy()
	{
		if (this->exist())
		{
			delete[] this->data_ptr;
			this->v_size = 0;
			this->v_capacity = 0;
			this->data_ptr = nullptr;
		}
	}

	template <class T>
	basic_string<T> &basic_string<T>::shrink_to_fit()
	{
		if (this->v_capacity > this->v_size)
		{
			T *tmp = new T[this->v_size];
			for (size_t i = 0; i < this->v_size; i++)
				tmp[i] = this->data_ptr[i];
			delete[] this->data_ptr;
			this->data_ptr = tmp;
			this->v_capacity = this->v_size;
			_POP_EXC_0();
			return *this;
		}
		_POP_EXC_0();
		return *this;
	}

	template <class T>
	basic_string<T> &basic_string<T>::resize(size_t N)
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
	basic_string<T> &basic_string<T>::push_back(const T &obj)
	{
		if (this->v_capacity == this->v_size)
		{
			if (this->v_size != 0)
				this->resize(this->v_size * 2);
			else
				this->resize(STD_EXPAN);
		}
		this->data_ptr[this->v_size] = obj;
		this->v_size++;
		return *this;
	}

	template <class T>
	basic_string<T> &basic_string<T>::pop_back()
	{
		if (this->exist())
		{
			T VOID = '\0';
			this->data_ptr[this->v_size - 1] = VOID;
			this->v_size--;
			return *this;
		}
		else
		{
#if _WARNING
			std::cout << "[Error] Empty list" << '\n';
#endif
			return *this;
		}
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
	T &basic_string<T>::operator[](const size_t N)
	{
		assert(N < this->v_capacity);
		return this->data_ptr[N];
	}

	template <class T>
	const T &basic_string<T>::operator[](const size_t N) const
	{
		assert(N < this->v_capacity);
		return this->data_ptr[N];
	}

	template <class T>
	basic_string<T> &basic_string<T>::operator=(const basic_string<T> &obj)
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
	basic_string<T> &basic_string<T>::operator=(const T *obj)
	{
		this->destroy();
		this->v_capacity = strlen(obj);
		this->v_size = this->v_capacity;
		data_ptr = new T[this->v_size + 1];
		strncpy(data_ptr, obj, this->v_size + 1);
		return *this;

		// for (size_t i = 0; obj[i] != '\0'; i++)
		// 	this->v_size++;
		// this->data_ptr = new T[this->v_size + STD_EXPAN];
		// this->v_capacity = this->v_size + STD_EXPAN;
		// strcpy(data_ptr, obj);
		// return *this;
	}

	template <class T>
	basic_string<T> &basic_string<T>::operator=(std::initializer_list<T> ilist)
	{
		this->destroy();
		this->v_size = ilist.size();
		this->v_capacity = this->v_size + STD_EXPAN;
		this->data_ptr = new T[v_size + STD_EXPAN];
		T *p = this->data_ptr;
		for (auto q = ilist.begin(); q != ilist.end(); ++q)
		{
			*p = *q;
			p++;
		}
		return *this;
	}

	template <class T>
	bool basic_string<T>::operator==(const basic_string<T> &obj) const
	{
		unsigned count = 0;
		for (size_t i = 0; i < this->v_size && i < obj.v_size; i++)
		{
			if (this->data_ptr[i] == obj.data_ptr[i])
				count++;
		}
		return count == this->v_size && count == obj.v_size;
	}

	template <class T>
	bool basic_string<T>::operator==(const T *obj) const
	{
		size_t obj_len = 0;
		for (size_t i = 0; obj[i] != '\0'; i++)
			obj_len++;
		unsigned count = 0;
		for (size_t i = 0; i < this->v_size && i < obj_len; i++)
		{
			if (this->data_ptr[i] == obj[i])
				count++;
		}
		return count == this->v_size && count == obj_len;
	}

	template <class T>
	bool basic_string<T>::operator!=(const basic_string<T> &obj) const
	{
		bool flag = this->operator==(obj);
		return !flag;
	}

	template <class T>
	bool basic_string<T>::operator!=(const T *obj) const
	{
		bool flag = this->operator==(obj);
		return !flag;
	}

	template <class T>
	bool basic_string<T>::operator<(const basic_string<T> &obj) const
	{
		for (size_t i = 0; i < size(); i++)
		{
			if ((*this)[i] != obj[i])
				return (*this)[i] < obj[i];
		}
		if (size() == obj.size())
			return true;
		else
			return size() < obj.size();
	}

	template <class T>
	bool basic_string<T>::operator<(const T *obj) const
	{
		string copy = obj;
		return this->operator<(copy);
	}

	template <class T>
	bool basic_string<T>::operator>(const basic_string<T> &obj) const
	{
		bool flag = this->operator<(obj);
		return !flag;
	}

	template <class T>
	bool basic_string<T>::operator>(const T *obj) const
	{
		string copy = obj;
		return this->operator>(copy);
	}

	template <class T>
	basic_string<T> basic_string<T>::operator+(const basic_string<T> &obj)
	{
		this->_POP_EXC_0();
		basic_string<T> result = *this;
		result.resize(this->v_size + obj.v_size + STD_EXPAN);
		for (size_t i = 0; i < obj.size(); i++)
			result[this->v_size + i] = obj.data_ptr[i];
		result.v_size = this->v_size + obj.v_size;
		result.shrink_to_fit();
		return result;
	}

	template <class T>
	basic_string<T> basic_string<T>::operator+(const T *obj)
	{
		this->_POP_EXC_0();
		size_t obj_len = 0;
		for (size_t i = 0; obj[i] != '\0'; i++)
			obj_len++;
		basic_string<T> result(*this);
		result.resize(this->v_size + obj_len + STD_EXPAN);
		for (size_t i = 0; i <= obj_len; i++)
			result[this->v_size + i - 1] = obj[i];
		result.v_size = this->v_size + obj_len;
		result.push_back('\0');
		result.shrink_to_fit();
		return result;
	}

	template <class Type>
	basic_string<Type> operator+(const Type *array, basic_string<Type> &str)
	{
		str._POP_EXC_0();
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
	basic_string<T> &basic_string<T>::operator+=(const basic_string<T> &obj)
	{
		this->_POP_EXC_0();
		for (int i = 0; i < obj.size(); i++)
			this->push_back(obj[i]);
		this->_POP_EXC_0();
		return *this;
	}

	template <class T>
	basic_string<T> &basic_string<T>::operator+=(const T *obj)
	{
		this->_POP_EXC_0();
		size_t obj_len = 0;
		for (size_t i = 0; obj[i] != '\0'; i++)
			obj_len++;
		for (size_t i = 0; i <= obj_len; i++)
			this->push_back(obj[i]);
		this->_POP_EXC_0();
		return *this;
	}

	template <class T>
	basic_string<T> basic_string<T>::operator()(const T *obj)
	{
		return string(obj);
	}

	template <class T>
	size_t basic_string<T>::find(const T &c)
	{
		for (size_t i = 0, s = this->size(); i < s; i++)
		{
			if (*(this)[i] == c[i])
				return i;
		}
	}

	template <class T>
	size_t basic_string<T>::find(const T *target)
	{
		return (strstr(data_ptr, target) - data_ptr) / sizeof(T);
	}

	// template <class T>
	// basic_string<T>::basic_string(const basic_string<T> &obj, T *start, T *stop)
	// {
	// }
}

#endif
