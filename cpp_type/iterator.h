#ifndef _NUTS_ITER_
#define _NUTS_ITER_ 1

namespace nuts
{
	template <typename Itr>
	auto iterator_traits(const Itr& x)
	{
		using itr_type = typename Itr::Category;
		return itr_type();
	}

	struct bidirectional_iterator_tag
	{
		// Allow ++/--it
	};

	struct random_access_iterator_tag
	    : public bidirectional_iterator_tag
	{
		// Allow it+/-_n
	};

	struct bidirectional_iterator
	{
		using Category = bidirectional_iterator_tag;
	};

	struct random_access_iterator
	{
		using Category = random_access_iterator_tag;
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
}// namespace nuts

#endif