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
	class Range<u64>
	{
	public:
		class iterator
		    : public bidirectional_iterator
		{

		public:
			iterator() = default;
			iterator(u64 x, u64 s = 1) : curr(x), step(s) {}
			~iterator() = default;

			u64 operator*() { return curr; }
			u64 operator*() const { return curr; }

			bool operator==(const iterator& obj) const { return this->curr == obj.curr; }
			bool operator!=(const iterator& obj) const { return this->curr <= obj.curr &&
				                                                this->curr != obj.curr; }

			iterator& operator++()
			{
				this->curr += step;
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
				this->curr -= step;
				return *this;
			}

			iterator operator--(int)// postposition
			{
				iterator res = *this;
				--(*this);
				return res;
			}

		protected:
			u64 curr, step;
		};

		Range() = default;
		Range(u64 st, u64 ed, u64 step = 1) : lb(st), ub(ed), s(step) {}
		~Range() = default;

		iterator begin() const { return {lb, s}; }
		iterator end() const { return {ub, s}; }

	protected:
		u64 lb, ub, s;
	};

	Range<u64> range(u64 lb, u64 ub, u64 s = 1)
	{
		return Range<u64>(lb, ub, s);
	}
}

#endif