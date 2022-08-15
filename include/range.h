#ifndef _NUTS_RANGE_
#define _NUTS_RANGE_ 1

#include "type.h"

namespace nuts
{
	template <typename C>
	class Range
	{
	public:
		using itr_type = typename C::iterator;

	public:
		Range() = default;
		Range(const C& x) : st(x.begin()), ed(x.end() + 1) {}
		~Range() = default;

		itr_type& begin() { return st; }
		itr_type& end() { return ed; }

		const itr_type& begin() const { return st; }
		const itr_type& end() const { return ed; }

	protected:
		itr_type st, ed;
	};

	template <typename T>
	Range<T> range(const T& x)
	{
		return Range<T>(x);
	}

	template <>
	class Range<i64>
	{
	public:
		class iterator
		    : public bidirectional_iterator
		{

		public:
			iterator() = default;
			iterator(i64 x, i64 s = 1) : curr(x), step(s) {}
			~iterator() = default;

			i64 operator*() { return curr; }
			i64 operator*() const { return curr; }

			bool operator==(const iterator& obj) const { return curr == obj.curr; }
			bool operator!=(const iterator& obj)
			        const
			{
				return step > 0 ? (curr <= obj.curr &&
				                   curr != obj.curr)
				                : (curr >= obj.curr &&
				                   curr != obj.curr);
			}

			iterator& operator++()
			{
				curr += step;
				return *this;
			}

			iterator operator++(int)// postposition
			{
				iterator res = *this;
				++(*this);
				return res;
			}

			iterator& operator--()
			{
				curr -= step;
				return *this;
			}

			iterator operator--(int)// postposition
			{
				iterator res = *this;
				--(*this);
				return res;
			}

		protected:
			i64 curr, step;
		};

		Range() = default;
		Range(i64 st, i64 ed, i64 step = 1) : lb(st), ub(ed), s(step) {}
		~Range() = default;

		iterator begin() const { return {lb, s}; }
		iterator end() const { return {ub, s}; }

	protected:
		i64 lb, ub, s;
	};

	Range<i64> range(i64 lb, i64 ub, i64 s = 1)
	{
		return Range<i64>(lb, ub, s);
	}
}

#endif