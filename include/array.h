#ifndef _NUTS_ARRAY_
#define _NUTS_ARRAY_ 1

#include <cassert>
#include <initializer_list>
#include <iostream>

#include "algorithm.h"
#include "iterator.h"
#include "range.h"
#include "type.h"

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

		void fill(const T& _val);
		pointer data() const { return const_cast<pointer>(data_ptr); }
		constexpr u64 size() const { return N; }
		constexpr bool empty() const { return size() == 0; }

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
			iterator(const_pointer obj) { _ptr = const_cast<pointer>(obj); }
			iterator(pointer obj) { _ptr = obj; }
			iterator(const iterator& obj) { _ptr = obj._ptr; }

			pointer get() const { return const_cast<pointer>(_ptr); }
			pointer operator->() const { return const_cast<pointer>(_ptr); }
			T& operator*() { return *_ptr; }
			const T& operator*() const { return *_ptr; }

			iterator& operator=(pointer obj)
			{
				_ptr = obj;
				return *this;
			}

			iterator& operator=(const iterator& obj)
			{
				_ptr = obj._ptr;
				return *this;
			}

			bool operator==(pointer obj) const { return _ptr == obj; }
			bool operator!=(pointer obj) const { return _ptr != obj; }

			bool operator==(const iterator& obj) const { return _ptr == obj._ptr; }
			bool operator!=(const iterator& obj) const { return _ptr != obj._ptr; }

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

			iterator operator+(i64 bias)
			        const { return iterator(_ptr + bias); }

			void operator+=(i64 bias) { _ptr += bias; }

			iterator operator-(i64 bias)
			        const { return iterator(_ptr - bias); }

			void operator-=(i64 bias) { _ptr -= bias; }

			friend i64 operator-(const iterator& a,
			                     const iterator& b) { return a.get() - b.get(); }

			T& operator[](u64 _n) { return *((*this) + _n); }
			const T& operator[](u64 _n) const { return *((*this) + _n); }
		};

		iterator begin() const { return {data()}; }
		iterator end() const { return {&data_ptr[size() - 1]}; }
	};

	template <typename T, u64 N>
	array<T, N>::array(const T& val)
	{
		nuts::fill_n(data_ptr, N, val);
	}

	template <typename T, u64 N>
	array<T, N>::array(const std::initializer_list<T>& ilist)
	{
		auto st = ilist.begin();
		for (u64 i: range(0, size())) data_ptr[i] = *(st++);
	}

	template <typename T, u64 N>
	T& array<T, N>::at(u64 _n)
	{
		assert(_n < size() && "Index_Bound");
		return data_ptr[_n];
	}

	template <typename T, u64 N>
	const T& array<T, N>::at(u64 _n) const
	{
		assert(_n < size() && "Index_Bound");
		return data_ptr[_n];
	}

	template <typename T, u64 N>
	void array<T, N>::fill(const T& _val)
	{
		nuts::fill_n(data_ptr, N, _val);
	}

	template <typename T, u64 N>
	void array<T, N>::print() const
	{
		auto print = [this](const auto& x) {
			std::cout << (T) x;
			if (&x != &this->back())
				printf(", ");
		};

		printf("\narray @%#llx = [", (u64) data());
		if (!empty()) for_each(begin(), end(), print);
		printf("]\n");
	}
}

#endif