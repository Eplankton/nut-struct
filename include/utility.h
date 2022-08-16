#ifndef _NUTS_UTILI_
#define _NUTS_UTILI_ 1

#include <iostream>

namespace nuts
{
	template <class T1, class T2>
	struct pair
	{
		T1 first;
		T2 second;

		pair() = default;
		pair(const T1& _first, const T2& _last)
		    : first(_first), second(_last) {}

		pair(const pair<T1, T2>& obj)
		    : first(obj.first), second(obj.second) {}

		pair(pair<T1, T2>&& src) { move(src); }

		T1& get_first() { return first; }
		T2& get_second() { return second; }
		const T1& get_first() const { return first; }
		const T2& get_second() const { return second; }

		pair<T1, T2>& move(pair<T1, T2>& src)
		{
			first = std::move(src.first);
			second = std::move(src.second);
			return *this;
		}

		bool operator<(const pair<T1, T2>& after) const
		{
			if (first == after.first)
				return second < after.second;
			else
				return first < after.first;
		}

		bool operator>(const pair<T1, T2>& after) const
		{
			if (first == after.first)
				return second > after.second;
			else
				return first > after.first;
		}

		bool operator==(const pair<T1, T2>& after) const
		{
			return (first == after.first) &&
			       (second == after.second);
		}

		bool operator!=(const pair<T1, T2>& after) const
		{
			bool res = !this->operator==(after);
			return res;
		}

		pair<T1, T2>& operator=(const pair<T1, T2>& obj)
		{
			first = obj.first, second = obj.second;
			return *this;
		}

		pair<T1, T2>& operator=(pair<T1, T2>&& src)
		{
			return move(src);
		}

		friend std::ostream&
		operator<<(std::ostream& output, const pair<T1, T2>& obj)
		{
			std::cout << '(' << obj.first << ", " << obj.second << ')';
			return output;
		}
	};

	template <class T1, class T2>
	pair<T1, T2> make_pair(const T1& _first, const T2& _second)
	// Generate a pair<T1, T2>
	{
		return pair<T1, T2>(_first, _second);
	}
}

#endif
