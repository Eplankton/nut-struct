#ifndef _NUTS_CONCEPT_
#define _NUTS_CONCEPT_ 1

// Only for version after C++20

#include <iostream>

namespace nuts
{
	template <typename T>
	concept Iterable = requires(T x)
	{
		typename T::value_type;
		typename T::iterator;
		x.begin();
		x.end();
		x.front();
		x.back();
	};

	template <typename T>
	concept Container = Iterable<T> &&
	        requires(T x)
	{
		x.size();
		x.clear();
		x.empty();
	};

	template <typename T>
	concept Display = requires(T x)
	{
		requires requires
		{
			std::cout << x;
		} || requires
		{
			x.print();
		};
	};

	template <typename T>
	concept Add = requires(T a, T b)
	{
		a + b;
	};

	template <typename T>
	concept Minus = requires(T a, T b)
	{
		a - b;
	};

	template <typename T>
	concept Multi = requires(T a, T b)
	{
		a* b;
	};

	template <typename T>
	concept Div = requires(T a, T b)
	{
		a / b;
	};

	template <typename T>
	concept Arithmetic = Add<T> &&
	        Minus<T> && Multi<T> && Div<T>;
}

#endif