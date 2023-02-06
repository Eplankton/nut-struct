#ifndef _NUTS_UTILI_
#define _NUTS_UTILI_

#include "algorithm.h"
#include "move.h"
#include <iostream>

namespace nuts
{
	// Pair
	template <class T1, class T2>
	struct pair
	{
		T1 first;
		T2 second;

		pair() = default;
		pair(const pair<T1, T2>& obj)
		    : first(obj.first), second(obj.second) {}
		pair(const T1& _first, const T2& _last)
		    : first(_first), second(_last) {}

		pair(pair<T1, T2>&& src) noexcept { move(src); }

		inline T1& get_first() { return first; }
		inline T2& get_second() { return second; }
		inline const T1& get_first() const { return first; }
		inline const T2& get_second() const { return second; }

		inline T1& _0() { return first; }
		inline T2& _1() { return second; }
		inline const T1& _0() const { return first; }
		inline const T2& _1() const { return second; }

		pair<T1, T2>& move(pair<T1, T2>& src) noexcept
		{
			first = nuts::move(src.first);
			second = nuts::move(src.second);
			return *this;
		}

		inline bool operator<(const pair<T1, T2>& after) const
		{
			if (first == after.first)
				return second < after.second;
			else
				return first < after.first;
		}

		inline bool operator>(const pair<T1, T2>& after) const
		{
			if (first == after.first)
				return second > after.second;
			else
				return first > after.first;
		}

		inline bool operator==(const pair<T1, T2>& after) const
		{
			return (first == after.first) &&
			       (second == after.second);
		}

		inline bool operator!=(const pair<T1, T2>& after) const
		{
			bool res = !this->operator==(after);
			return res;
		}

		inline pair<T1, T2>& operator=(const pair<T1, T2>& obj) noexcept
		{
			first = obj.first, second = obj.second;
			return *this;
		}

		inline pair<T1, T2>& operator=(pair<T1, T2>&& src) noexcept
		{
			return this->move(src);
		}

		friend std::ostream&
		operator<<(std::ostream& output, const pair<T1, T2>& obj)
		{
			nuts::print('(', obj.first, ", ", obj.second, ')');
			return output;
		}
	};

	// Deduction Guide
	template <class T1, class T2>
	pair(T1, T2) -> pair<T1, T2>;

	template <class T1, class T2>
	inline pair<T1, T2> make_pair(const T1& _first, const T2& _second)
	// Generate a pair<T1, T2>
	{
		return pair {_first, _second};
	}

	// Tuple
	template <int Idx, typename Head, typename... Tail>
	struct tuple_impl : public tuple_impl<Idx + 1, Tail...>
	{
		Head val;
		tuple_impl() = default;
		tuple_impl(const Head& head, const Tail&... tail)
		    : tuple_impl<Idx + 1, Tail...>(tail...), val(head) {}
	};

	template <int Idx, typename Head>
	struct tuple_impl<Idx, Head>
	{
		Head val;
		tuple_impl() = default;
		explicit tuple_impl(const Head& head) : val(head) {}
	};

	template <typename... Elements>
	struct tuple : public tuple_impl<0, Elements...>
	{
		using Base = tuple_impl<0, Elements...>;
		using Base::Base;

		void print() const;
	};

	// Deduction Guide
	template <typename... T>
	tuple(T...) -> tuple<T...>;

	template <int Idx, typename Tuple>
	struct tuple_element;

	template <int Idx, typename Head, typename... Tail>
	struct tuple_element<Idx, tuple<Head, Tail...>>
	    : tuple_element<Idx - 1, tuple<Tail...>>
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
	auto& get(tuple<Elements...>& t)
	{
		return get_helper<Idx>(t);
	}

	template <int Idx, typename Head, typename... Tail>
	const Head& get_helper(const tuple_impl<Idx, Head, Tail...>& t)
	{
		return t.val;
	}

	template <int Idx, typename... Elements>
	const auto& get(const tuple<Elements...>& t)
	{
		return get_helper<Idx>(t);
	}

	template <typename Tuple, u64 N>
	struct tuple_print_helper
	{
		static void print(const Tuple& t)
		{
			tuple_print_helper<Tuple, N - 1>::print(t);
			nuts::print(", ", get<N - 1>(t));
		}
	};

	template <typename Tuple>
	struct tuple_print_helper<Tuple, 1>
	{
		static void print(const Tuple& t)
		{
			nuts::print("(", get<0>(t));
		}
	};

	template <typename... T>
	void tuple<T...>::print() const
	{
		tuple_print_helper<decltype(*this),
		                   sizeof...(T)>::print(*this);
		nuts::print(")\n");
	}

	template <typename... Args>
	std::ostream& operator<<(std::ostream& os,
	                         const std::tuple<Args...>& t)
	{
		t.print();
		return os;
	}
}

#endif
