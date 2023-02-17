#ifndef _NUTS_RANGE_
#define _NUTS_RANGE_

#include "type.h"
#include "iterator.h"

namespace nuts
{
	template <typename C>
	class Range
	{
	public:
		using itr_type = typename C::iterator;

	public:
		Range() = default;
		~Range() = default;
		explicit Range(const C& x)
		{
			if (x.begin() == x.end())
			{
				ed = st = x.begin();
			}
			else
			{
				st = x.begin();
				ed = next(x.end());
			}
		}

		inline itr_type& begin() { return st; }
		inline itr_type& end() { return ed; }

		inline const itr_type& begin() const { return st; }
		inline const itr_type& end() const { return ed; }

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

			inline bool operator==(const iterator& obj) const { return curr == obj.curr; }
			inline bool operator!=(const iterator& obj)
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

		inline iterator begin() const { return {lb, s}; }
		inline iterator end() const { return {ub, s}; }

	protected:
		i64 lb, ub, s;
	};

	inline Range<i64> range(i64 lb, i64 ub, i64 s = 1)
	{
		return {lb, ub, s};
	}
}

#endif
