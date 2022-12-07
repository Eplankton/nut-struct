#ifndef _NUTS_CONCEPT_
#define _NUTS_CONCEPT_ 1

// Since C++20.

#include <iostream>

namespace nuts
{
	// template <typename Func, typename... T>
	// concept Callable = requires(Func &&fn, T&&... args)
	// {
	// 	fn(args...);
	// };

	template <typename Box>
	concept Iterable = requires(Box x)
	{
		typename Box::value_type;
		typename Box::iterator;

		x.begin();
		x.end();
		x.front();
		x.back();
	};

	template <typename Box>
	concept Container = Iterable<Box> &&
	        requires(Box x)
	{
		x.size();
		x.clear();
		x.empty();
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
	concept Add = requires(T a, T b)
	{
		a + b;
		b + a;
	};

	template <typename T>
	concept Minus = requires(T a, T b)
	{
		a - b;
		b - a;
	};

	template <typename T>
	concept Multi = requires(T a, T b)
	{
		a* b;
		b* a;
	};

	template <typename T>
	concept Div = requires(T a, T b)
	{
		a / b;
		b / a;
	};

	template <typename T>
	concept Arithmetic = Add<T> && Minus<T> && Multi<T> && Div<T>;
}

#endif