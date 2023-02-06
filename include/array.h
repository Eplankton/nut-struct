#ifndef _NUTS_ARRAY_
#define _NUTS_ARRAY_

#include <cassert>
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
		using iterator = pointer;

		array() = default;
		explicit array(const T& _val);
		array(const array<T, N>& src) = default;
		array(const std::initializer_list<T>& ilist);
		~array() = default;

		constexpr pointer data() const noexcept { return const_cast<pointer>(raw); }
		static constexpr u64 size() noexcept { return N; }
		static constexpr bool empty() noexcept { return size() == 0; }

		inline T& front() { return *begin(); }
		inline T& back() { return *end(); }

		inline const T& front() const { return *begin(); }
		inline const T& back() const { return *end(); }

		inline T& operator[](u64 _n) noexcept { return raw[_n]; }
		inline const T& operator[](u64 _n) const noexcept { return raw[_n]; }

		inline T& at(u64 _n);
		inline const T& at(u64 _n) const;

		void print() const;
		void fill(const T& _val);

		constexpr iterator begin() const { return data(); }
		constexpr iterator end() const
		{
			return size() == 0 ? begin()
			                   : begin() + size() - 1;
		}

	protected:
		value_type raw[N];
	};

	// Deduction Guide
	// what the fuck?
	template <typename T, Same<T>... U>
	array(T, U...) -> array<T, 1 + sizeof...(U)>;

	template <typename T, u64 N>
	array<T, N>::array(const T& val)
	{
		nuts::fill_n(raw, N, val);
	}

	template <typename T, u64 N>
	array<T, N>::array(const std::initializer_list<T>& ilist)
	{
		auto len = N < ilist.size() ? N : ilist.size();
		memcpy(raw, ilist.begin(), sizeof(T) * len);
	}

	template <typename T, u64 N>
	T& array<T, N>::at(u64 _n)
	{
		assert(_n < size() && "Index_Bound");
		return raw[_n];
	}

	template <typename T, u64 N>
	const T& array<T, N>::at(u64 _n) const
	{
		assert(_n < size() && "Index_Bound");
		return raw[_n];
	}

	template <typename T, u64 N>
	void array<T, N>::fill(const T& _val)
	{
		nuts::fill_n(raw, N, _val);
	}

	template <typename T, u64 N>
	void array<T, N>::print() const
	{
		auto print = [&](const auto& x) {
			nuts::print(x);
			if (&x != &back()) printf(", ");
		};

		printf("array @%#llx = [", (u64) data());
		if (!empty()) for_each(*this, print);
		printf("]\n");
	}
}

#endif
