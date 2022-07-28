#ifndef _NUTS_ARRAY_
#define _NUTS_ARRAY_ 1

#include "iterator.h"
#include "type.h"
#include <cassert>

namespace nuts
{
	template <typename T, u64 N>
	class array
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;

	protected:
		value_type data_ptr[N];

	public:
		array() = default;
		explicit array(const T& _val);
		array(const std::initializer_list<T>& ilist);
		~array() = default;

		array<T, N>& fill(const T& _val);
		pointer data() const { return const_cast<pointer>(data_ptr); }
		u64 size() const { return N; }
		u64 empty() const { return size() == 0; }
		T& front() { return data_ptr[0]; }
		T& back() { return data_ptr[this->size() - 1]; }
		const T& front() const { return data_ptr[0]; }
		const T& back() const { return data_ptr[this->size() - 1]; }
		T& operator[](u64 _n) { return data_ptr[_n]; }
		const T& operator[](u64 _n) const { return data_ptr[_n]; }
		T& at(u64 _n);
		const T& at(u64 _n) const;
		void print() const;

		class iterator
		    : public random_access_iterator
		{
		public:
			using value_type = T;
			using pointer = T*;
			using const_pointer = const T*;

		protected:
			pointer _ptr = nullptr;

		public:
			iterator() = default;
			iterator(const_pointer obj) { this->_ptr = const_cast<pointer>(obj); }
			iterator(pointer obj) { this->_ptr = obj; }
			iterator(const iterator& obj) { this->_ptr = obj._ptr; }

			T* get() const { return const_cast<pointer>(_ptr); }
			T& operator*() { return *_ptr; }
			const T& operator*() const { return *_ptr; }

			iterator& operator=(T* obj)
			{
				this->_ptr = obj;
				return *this;
			}

			iterator& operator=(const iterator& obj)
			{
				this->_ptr = obj._ptr;
				return *this;
			}

			bool operator==(T* obj) const { return this->_ptr == obj; }
			bool operator!=(T* obj) const { return this->_ptr != obj; }

			bool operator==(const iterator& obj) const { return this->_ptr == obj._ptr; }
			bool operator!=(const iterator& obj) const { return this->_ptr != obj._ptr; }

			iterator& operator++()
			{
				this->_ptr++;
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
				this->_ptr--;
				return *this;
			}

			iterator operator--(int)
			{
				iterator res = *this;
				--(*this);
				return res;
			}

			iterator operator+(i64 bias)
			        const { return iterator(this->_ptr + bias); }

			void operator+=(i64 bias) { _ptr += bias; }

			iterator operator-(i64 bias)
			        const { return iterator(this->_ptr - bias); }

			void operator-=(i64 bias) { _ptr -= bias; }

			friend i64 operator-(const iterator& a,
			                     const iterator& b) { return a.get() - b.get(); }

			pointer operator->() { return _ptr; }
			const_pointer operator->() const { return _ptr; }
		};

		iterator begin() { return {data()}; }
		// Return iterator to the first element

		iterator end() { return {&data_ptr[size() - 1]}; }
		// Return iterator to the last element

		iterator begin()
		        const { return {const_cast<pointer>(data())}; }

		iterator end()
		        const { return {const_cast<pointer>(&data_ptr[size() - 1])}; }
	};

	template <typename T, nuts::u64 N>
	array<T, N>::array(const T& val)
	{
		std::fill_n(data_ptr, size(), val);
	}

	template <typename T, nuts::u64 N>
	array<T, N>::array(const std::initializer_list<T>& ilist)
	{
		auto st = ilist.begin();
		for (u64 i = 0; i < size(); ++i)
			data_ptr[i] = *(st++);
	}

	template <typename T, nuts::u64 N>
	T& array<T, N>::at(u64 _n)
	{
		assert(_n < size() && "Index_Bound");
		return data_ptr[_n];
	}

	template <typename T, nuts::u64 N>
	const T& array<T, N>::at(u64 _n) const
	{
		assert(_n < size() && "Index_Bound");
		return data_ptr[_n];
	}

	template <typename T, nuts::u64 N>
	void array<T, N>::print() const
	{
		auto print = [this](const auto& x) {
			std::cout << (T) x;
			if (&x != &this->back())
				printf(", ");
		};
		printf("\narray @%#llx = [", (u64) this->data());
		if (!empty())
			for_each(begin(), end(), print);
		printf("]\n");
	}

	template <typename T, u64 N>
	array<T, N>& array<T, N>::fill(const T& _val)
	{
		return std::fill_n(data_ptr, N, _val);
	}
}

#endif
