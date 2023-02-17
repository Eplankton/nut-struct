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
	constexpr bool is_same_v = is_same<T, U>::value;

	template <typename T, typename U>
	concept Same = is_same_v<T, U>;

	template <typename Fn, typename... T>
	concept Invocable = requires(Fn&& fn, T&&... args)
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
		a + a;
	};

	template <typename T>
	concept Minus = requires(T a)
	{
		a - a;
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
	concept Less = requires(T x)
	{
		x < x;
	};

	template <typename T>
	concept Greater = requires(T x)
	{
		x > x;
	};

	template <typename T>
	concept Equal = requires(T x)
	{
		x == x;
	};

	template <typename T>
	concept Partial_Order = Less<T> && Greater<T>;

	template <typename T>
	concept Order = Partial_Order<T> && Equal<T>;

	template <typename T>
	concept Arithmetic =
	        Add<T> &&
	        Minus<T> &&
	        Multi<T> &&
	        Div<T>;

	template <typename>
	struct is_pointer
	{
		static constexpr bool value = false;
	};

	template <typename T>
	struct is_pointer<T*>
	{
		static constexpr bool value = true;
	};

	template <typename T>
	constexpr bool is_pointer_v = is_pointer<T>::value;
	
	template <typename Ptr>
	concept Pointer = is_pointer_v<Ptr>;

	template <typename Ptr>
	concept Valid_Pointer = Pointer<Ptr> &&
	                        !Same<Ptr, nullptr_t> &&
	                        !Same<Ptr, void*>;

	template <typename Itr>
	concept Forward_Itr =
	        Valid_Pointer<Itr> || requires(Itr it)
	{
		typename Itr::value_type;
		*it;
		++it;
	};

	template <typename Itr>
	concept Bidirect_Itr =
	        Forward_Itr<Itr> && requires(Itr it)
	{
		--it;
	};

	template <typename Itr>
	concept Random_Itr =
	        Bidirect_Itr<Itr> && requires(Itr x, u64 n)
	{
		x + n;
		x - n;
		x[n];
		x - x;
	};

	template <Forward_Itr Itr>
	struct deref
	{
		using type = typename Itr::value_type;
	};

	template <typename T>
	requires Pointer<T*>
	struct deref<T*>
	{
		using type = T;
	};

	template <typename Ref>
	using deref_t = typename deref<Ref>::type;
}

#endif