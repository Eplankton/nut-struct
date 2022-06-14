#ifndef _NUTS_UTILI_
#define _NUTS_UTILI_

namespace nuts
{
	template <class T1, class T2>
	struct pair
	{
		T1 first;
		T2 second;

		pair() = default;
		pair(const T1 &_first, const T2 &_last)
			: first(_first), second(_last) {}

		pair(const pair &obj)
			: first(obj.first), second(obj.second) {}

		bool operator<(const pair<T1, T2> &after) const
		{
			if (this->first == after.first)
				return this->second < after.second;
			else
				return this->first < after.first;
		}

		bool operator>(const pair<T1, T2> &after) const
		{
			if (this->first == after.first)
				return this->second > after.second;
			else
				return this->first > after.first;
		}

		bool operator==(const pair<T1, T2> &after) const
		{
			return (this->first == after.first) &&
                   (this->second == after.second);
		}

		bool operator!=(const pair<T1, T2> &after) const
		{
			bool res = !this->operator==(after);
			return res;
		}

		pair<T1, T2> &operator=(const pair<T1, T2> &obj)
		{
			first = obj.first, second = obj.second;
			return *this;
		}
	};

	template <class T1, class T2>
	pair<T1, T2> make_pair(const T1 &_first, const T2 &_second) // Generate a pair<T1, T2>
	{
		return pair<T1, T2>(_first, _second);
	}
}

#endif
