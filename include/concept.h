#ifndef _NUTS_CONCEPT_
#define _NUTS_CONCEPT_

// Since C++20.

#include <iostream>
#include "type.h"

namespace nuts
{
	template <typename T, typename U>
	struct is_same
	{
		static constexpr bool value = false;
	};

	template <typename T>
	struct is_same<T, T>
	{
		static constexpr bool value = true;
	};

	template <typename T, typename U>
	concept Same = is_same<T, U>::value;

	template <typename Fn, typename... T>
	concept Callable = requires(Fn&& fn, T&&... args)
	{
		fn(args...);
	};

	template <typename Box>
	concept Iterable = requires(Box x)
	{
		typename Box::value_type;
		typename Box::iterator;

		x.begin();
		x.end();
	};

	template <typename Box>
	concept Container = Iterable<Box> &&
	        requires(Box x)
	{
		x.size();
		x.front();
		x.back();
	};

	template <typename T>
	concept StreamOutput = requires(T x)
	{
		std::cout << x;
	};

	template <typename T>
	concept HasPrintMethod = requires(T Box)
	{
		Box.print();
	};

	template <typename T>
	concept Display = StreamOutput<T> || HasPrintMethod<T>;

	template <typename T>
	concept Add = requires(T a)
	{
		{a + a} -> Same<T>;
	};

	template <typename T>
	concept Minus = requires(T a)
	{
		{a - a} -> Same<T>;
	};

	template <typename T>
	concept Multi = requires(T a)
	{
		a * a;
	};

	template <typename T>
	concept Div = requires(T a)
	{
		a / a;
	};

	template <typename T>
	concept PartialOrder = requires(T x)
	{
		x < x;
	};

	template <typename T>
	concept Arithmetic = Add<T> && Minus<T> && Multi<T> && Div<T>;

	template <typename Itr>
	concept Forward_Itr = requires(Itr it)
	{
		typename Itr::value_type;
		*it;
		++it;
		it++;
	};

	template <typename Itr>
	concept Bidirectional_Itr =
	        Forward_Itr<Itr> && requires(Itr it)
	{
		--it;
		it--;
	};

	template <typename Itr>
	concept Random_Itr =
	        Bidirectional_Itr<Itr> && requires(Itr x, u64 n)
	{
		x + n;
		x - n;
		x[n];
		x - x;
	};

	template <typename T>
	concept NonNullPtr = requires(T x, u64 n)
	{
		requires requires { !Forward_Itr<T>; };
		*x;
		++x;
		--x;
		x + n;
		x - n;
		x - x;
		x[n];
	};
}

#endif