#ifndef _NUTS_TUPLE_
#define _NUTS_TUPLE_

namespace nuts
{
	template <int Idx, typename Head, typename... Tail>
	struct tuple_impl : public tuple_impl<Idx + 1, Tail...>
	{
		Head val {Head()};
		tuple_impl() = default;
		tuple_impl(const Head& head, const Tail&... tail)
		    : tuple_impl<Idx + 1, Tail...>(tail...), val(head) {}
	};

	template <int Idx, typename Head>
	struct tuple_impl<Idx, Head>
	{
		Head val {Head()};
		tuple_impl() = default;
		explicit tuple_impl(const Head& head) : val(head) {}
	};

	template <typename... Elements>
	struct tuple : public tuple_impl<0, Elements...>
	{
		using Base = tuple_impl<0, Elements...>;
		using Base::Base;
	};

	template <int Idx, typename Tuple>
	struct tuple_element;

	template <int Idx, typename Head, typename... Tail>
	struct tuple_element<Idx, tuple<Head, Tail...>> : tuple_element<Idx - 1, tuple<Tail...>>
	{
	};

	template <typename Head, typename... Tail>
	struct tuple_element<0, tuple<Head, Tail...>>
	{
		using type = Head;
	};

	template <int Idx, typename Head, typename... Tail>
	Head& get_helper(tuple_impl<Idx, Head, Tail...>& t)
	{
		return t.val;
	}

	template <int Idx, typename... Elements>
	typename tuple_element<Idx, tuple<Elements...>>::type& get(tuple<Elements...>& t)
	{
		return get_helper<Idx>(t);
	}

	template <class T>
	struct unwrap_refwrapper
	{
		using type = T;
	};

	template <class T>
	struct unwrap_refwrapper<std::reference_wrapper<T>>
	{
		using type = T&;
	};

	template <class T>
	using unwrap_decay_t = typename unwrap_refwrapper<typename std::decay<T>::type>::type;

	template <class... Types>
	tuple<unwrap_decay_t<Types>...>
	make_tuple(Types&&... args)
	{
		return nuts::tuple<unwrap_decay_t<Types>...>(std::forward<Types>(args)...);
	}
}

#endif