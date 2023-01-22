#ifndef _NUTS_UTILI_
#define _NUTS_UTILI_

#include "algorithm.h"
#include "move.h"
#include <iostream>

namespace nuts
{
	template <class T1, class T2>
	struct pair
	{
		T1 first;
		T2 second;

		pair() = default;
		pair(const pair<T1, T2>& obj) = default;
		pair(const T1& _first, const T2& _last)
		    : first(_first), second(_last) {}

		pair(pair<T1, T2>&& src) { move(src); }

		inline T1& get_first() { return first; }
		inline T2& get_second() { return second; }
		inline const T1& get_first() const { return first; }
		inline const T2& get_second() const { return second; }

		inline T1& _0() { return first; }
		inline T2& _1() { return second; }
		inline const T1& _0() const { return first; }
		inline const T2& _1() const { return second; }

		pair<T1, T2>& move(pair<T1, T2>& src)
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

		inline pair<T1, T2>& operator=(const pair<T1, T2>& obj)
		{
			first = obj.first, second = obj.second;
			return *this;
		}

		inline pair<T1, T2>& operator=(pair<T1, T2>&& src)
		{
			return this->move(src);
		}

		friend std::ostream&
		operator<<(std::ostream& output, const pair<T1, T2>& obj)
		{
			nuts::print('(', obj.first, ": ", obj.second, ')');
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
}

#endif
