#ifndef _NUTS_ITER_
#define _NUTS_ITER_ 1

#include "type.h"

namespace nuts
{
	struct forward_iterator_tag
	{
		// Allow ++it
	};

	struct bidirectional_iterator_tag
	    : public forward_iterator_tag
	{
		// Allow ++/--it
	};

	struct random_access_iterator_tag
	    : public bidirectional_iterator_tag
	{
		// Allow it+/-_n
	};

	struct forward_iterator
	{
		using Category = forward_iterator_tag;
	};

	struct bidirectional_iterator
	    : public forward_iterator
	{
		using Category = bidirectional_iterator_tag;
	};

	struct random_access_iterator
	    : public bidirectional_iterator
	{
		using Category = random_access_iterator_tag;
	};

	template <typename T>
	struct is_forward_iterator
	{
		static const bool value = true;
	};

	template <typename T>
	struct is_bidirectional_iterator
	{
		static const bool value = false;
	};

	template <typename T>
	struct is_random_access_iterator
	{
		static const bool value = false;
	};

	template <>
	struct is_bidirectional_iterator<bidirectional_iterator_tag>
	{
		static const bool value = true;
	};

	template <>
	struct is_bidirectional_iterator<random_access_iterator_tag>
	{
		static const bool value = true;
	};

	template <>
	struct is_random_access_iterator<random_access_iterator_tag>
	{
		static const bool value = true;
	};

	template <typename Itr>
	auto iterator_category(const Itr& x)
	{
		using itr_type = typename Itr::Category;
		return itr_type();
	}

	// template <typename Itr>
	// concept OutputItr = requires(Itr it)
	// {
	// 	typename Itr::value_type;
	// };
	
	// template <typename Itr>
	// concept InputItr = requires(Itr it)
	// {
	// 	typename Itr::value_type;
	// };

	template <typename Itr>
	concept Forward_Itr = requires(Itr it)
	{
		typename Itr::value_type;
		*it;
		++it;
	};

	template <typename Itr>
	concept Bidirectional_Itr =
	        Forward_Itr<Itr> && requires(Itr it)
	{
		--it;
	};

	template <typename Itr>
	concept Random_Itr =
	        Bidirectional_Itr<Itr> && requires(Itr it, u64 n)
	{
		it[n];
	};

	template <typename P>
	concept NonVoidPointer = requires(P x, P y, i64 n)
	{
		requires requires { !Forward_Itr<P>; };
		++x;
		--x;
		x + n;
		x - n;
		x - y;
		*x;
	};
}

#endif