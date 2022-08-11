#ifndef _NUTS_ITER_
#define _NUTS_ITER_ 1

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

	// Get traits
	template <typename Itr>
	auto iterator_category(const Itr& x)
	{
		// Itr_type is one of these 'tags'
		using Itr_type = typename Itr::Category;
		return Itr_type();
	}

	template <typename Itr>
	concept Forward_Itr = requires(Itr it)
	{
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
}

#endif