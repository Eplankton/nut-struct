#ifndef _NUTS_ARRAY_
#define _NUTS_ARRAY_

#ifndef _DEBUG
#define _DEBUG 1
#endif

#ifndef _WARNING
#define _WARNING 1
#endif

#include <iostream>
#include <cstddef>
#include <cassert>

namespace nuts
{
	template <class T>
	class array
	{
	private:
		T *data_ptr = nullptr;
		size_t len = 0;

	public:
		array() = default;
		explicit array(size_t length);
		explicit array(size_t length, const T &val);
		array(const array &obj);
		array(const T *obj, size_t length);
		array(std::initializer_list<T> ilist);
		~array() { this->destroy(); }

		size_t size() const { return this->len; }
		bool exist() const { return this->data_ptr != nullptr; }
		array<T> &fill(const T &val, size_t _l);
		void destroy();
		void clear();
		void print();

		T &front() { return this->data_ptr[0]; }
		T &back() { return this->data_ptr[this->size() - 1]; }

		const T &front() const { return this->data_ptr[0]; }
		const T &back() const { return this->data_ptr[this->size() - 1]; }

		T &operator[](size_t N);
		const T &operator[](size_t N) const;
		array<T> &operator=(const array &obj);
		T *data() const { return this->data_ptr; }

		class iterator
		{
		protected:
			T *_ptr = nullptr;

		public:
			iterator() = default;
			iterator(T *obj)
			{
				this->_ptr = obj;
			}
			iterator(const iterator &obj)
			{
				this->_ptr = obj._ptr;
			}

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

			iterator &operator++()
			{
				this->_ptr++;
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
				this->_ptr--;
				return *this;
			}

			const iterator operator--(int)
			{
				iterator res = *this;
				--(*this);
				return res;
			}

			iterator operator+(int bias) const
			{
				return iterator(this->_ptr + bias);
			}

			void operator+=(int bias)
			{
				_ptr += bias;
			}

			iterator operator-(int bias) const
			{
				return iterator(this->_ptr - bias);
			}

			void operator-=(int bias)
			{
				_ptr -= bias;
			}

			int operator-(const iterator &b) const
			{
				return this->_ptr - b._ptr;
			}

			T *operator->() { return _ptr; }
			const T *operator->() const { return _ptr; }
		};

		iterator begin() // Return iterator to the first element
		{
			return iterator(this->data_ptr);
		}

		iterator end() // Return iterator to the last element
		{
			return iterator(&this->data_ptr[this->size() - 1]);
		}

		const iterator begin() const
		{
			return iterator(this->data_ptr);
		}

		const iterator end() const
		{
			return iterator(&this->data_ptr[this->size() - 1]);
		}
	};

	template <class T>
	array<T>::array(size_t length)
	{
		this->data_ptr = new T[length];
		this->len = length;
	}

	template <class T>
	array<T>::array(size_t length, const T &val)
	{
		this->data_ptr = new T[length];
		this->len = length;
		std::fill_n(this->data_ptr, length, val);
	}

	template <class T>
	array<T>::array(const array &obj)
	{
		this->data_ptr = new T[obj.len];
		this->len = obj.len;
		for (int i = 0; i < len; i++)
			this->data_ptr[i] = obj[i];
	}

	template <class T>
	array<T>::array(const T *obj, size_t length)
	{
		this->data_ptr = new T[length];
		this->len = length;
		for (int i = 0; i < length; i++)
			this->data_ptr[i] = obj[i];
	}

	template <class T>
	array<T>::array(const std::initializer_list<T> ilist)
	{
		this->data_ptr = new T[ilist.size()];
		this->len = ilist.size();
		T *q = this->data_ptr;
		for (auto p = ilist.begin(); p != ilist.end(); ++p)
		{
			*q = *p;
			q++;
		}
	}

	template <class T>
	T &array<T>::operator[](size_t N)
	{
		assert(N < this->len);
		return this->data_ptr[N];
	}

	template <class T>
	const T &array<T>::operator[](size_t N) const
	{
		assert(N < this->len);
		return this->data_ptr[N];
	}

	template <class T>
	array<T> &array<T>::fill(const T &val, size_t _l)
	{
		std::fill_n(this->data_ptr, _l, val);
	}

	template <class T>
	void array<T>::destroy()
	{
		if (this->exist())
		{
			delete[] this->data_ptr;
			this->data_ptr = nullptr;
			this->len = 0;
		}
	}

	template <class T>
	void array<T>::clear()
	{
		if (this->exist())
		{
			T VOID;
			for (int i = 0; i < this->len; i++)
				this->data_ptr[i] = VOID;
			this->len = 0;
		}
	}

	template <class T>
	array<T> &array<T>::operator=(const array &obj)
	{
		if (this->exist())
			this->destroy();
		this->data_ptr = new T[obj.len];
		this->len = obj.len;
		for (int i = 0; i < len; i++)
			this->data_ptr[i] = obj[i];
		return *this;
	}

	template <class T>
	void array<T>::print()
	{
		auto print = [this](const auto &x)
		{
			std::cout << x;
			if (x != this->back())
				std::cout << ", ";
		};
		printf("\narray@%#llx = [", (u64)this->data_ptr);
		for_each(begin(), end(), print);
		std::cout << "]\n";
	}

}

#endif
