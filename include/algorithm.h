#ifndef _NUTS_ALG_
#define _NUTS_ALG_ 1

#include "concept.h"
#include "functional.h"
#include "iterator.h"
#include "move.h"
#include "range.h"
#include "type.h"

namespace nuts
{
	template <Add... T>
	auto sum(T... s)
	{
		return (... + s);
	}

	template <Forward_Itr Itr>
	i64 distance(Itr st, Itr ed)
	{
		if (st == ed) return 0;
		i64 n = 0;
		for (ed = next(ed);
		     st != ed; ++st) {
			++n;
		}
		return n;
	}

	template <Random_Itr Itr>
	inline i64 distance(Itr st, Itr ed)
	{
		return st == ed ? 0
		                : (ed - st + 1);
	}

	template <Forward_Itr Itr>
	Itr advance(Itr itr, i64 n)
	{
		if (n >= 0)
			while (n--) ++itr;
		return itr;
	}

	template <Bidirectional_Itr Itr>
	Itr advance(Itr itr, i64 n)
	{
		if (n > 0)
			while (n--) ++itr;
		else
			while (n++) --itr;
		return itr;
	}

	template <Random_Itr Itr>
	inline Itr advance(Itr itr, i64 n)
	{
		return itr + n;
	}

	template <Forward_Itr Itr>
	auto next(Itr itr)
	{
		return advance(itr, 1);
	}

	template <Bidirectional_Itr Itr>
	auto prev(Itr itr)
	{
		return advance(itr, -1);
	}

	template <Random_Itr Itr>
	inline Itr prev(Itr itr, i64 n)
	{
		return itr - n;
	}

	template <Forward_Itr Itr, class Func>
	auto for_each(Itr st, Itr ed, Func&& fn)
	{
		for (ed = next(ed);
		     st != ed; ++st) {
			fn(*st);
		}
		return fn;
	}

	template <Container Box, class Func>
	auto for_each(const Box& box, Func&& fn)
	{
		return for_each(box.begin(),
		                box.end(), fn);
	}

	template <Less T>
	inline const T& min(const T& a, const T& b)// Return the minimum
	{
		return (a < b) ? a : b;
	}

	template <Greater T>
	inline const T& max(const T& a, const T& b)// Return the maximum
	{
		return (a > b) ? a : b;
	}

	template <Forward_Itr Itr, class Compare = less<deref_t<Itr>>>
	Itr min_in(Itr st, Itr ed, Compare cmp = Compare())
	// Give range by itr: st && ed -> O(n)
	{
		auto tmp = st;
		if (st != ed) {
			auto _end = next(ed);
			for (auto i = st; i != _end; i++)
				if (cmp(*i, *tmp))
					tmp = i;
		}
		return tmp;
	}

	template <Forward_Itr Itr, class Compare = greater<deref_t<Itr>>>
	Itr max_in(Itr st, Itr ed, Compare cmp = Compare())
	// Give range by itr: st && ed -> O(n)
	{
		auto tmp = st;
		if (st != ed) {
			auto _end = next(ed);
			for (auto i = st; i != _end; i++)
				if (cmp(*i, *tmp))
					tmp = i;
		}
		return tmp;
	}

	template <Container Box, class Compare = less<typename Box::value_type>>
	auto min_in(const Box& box, Compare cmp = Compare())
	{
		return min_in(box.begin(), box.end(), cmp);
	}

	template <Container Box, class Compare = less<typename Box::value_type>>
	auto max_in(const Box& box, Compare cmp = Compare())
	{
		return max_in(box.begin(), box.end(), cmp);
	}

	template <typename T>
	inline void swap(T& a, T& b)
	{
		using nuts::move;
		auto tmp = move(b);
		b = move(a);
		a = move(tmp);
	}

	template <Forward_Itr Itr>
	inline void itr_swap(Itr a, Itr b)
	{
		if (a == b) return;
		swap(*a, *b);
	}

	template <Bidirectional_Itr Itr>
	void reverse(Itr st, Itr ed)
	// Give range by itr: st && ed -> O(N)
	{
		while (st != ed && st != prev(ed)) {
			itr_swap(st, ed);
			++st;
			--ed;
		}
		if (st != ed)
			itr_swap(st, ed);
	}

	template <Forward_Itr Itr, class Compare = less<deref_t<Itr>>>
	bool is_sorted(Itr st, Itr ed, Compare cmp = Compare())
	// Give range by itr: st && ed -> O(N)
	{
		if (st == ed)
			return true;
		auto it = st;
		for (++it; it != ed; st = it, (void) ++it)
			if (cmp(*it, *st))
				return false;
		return true;
	}

	template <Container Box, class Compare = less<typename Box::value_type>>
	    requires Bidirectional_Itr<typename Box::iterator> bool
	is_sorted(const Box& box, Compare cmp = Compare())
	// Give range by itr: st && ed -> O(N)
	{
		return is_sorted(box.begin(), box.end(), cmp);
	}

	template <Forward_Itr Itr, class Func>
	Itr find_if(Itr st, Itr ed, Func fn)
	// Give range by itr: st && ed -> O(N)
	{
		for (ed = next(ed); st != ed; ++st) {
			if (fn(*st)) break;
		}
		return st;
	}

	template <Container Box, class Func>
	auto find_if(const Box& x, Func fn)
	// Give range by itr: st && ed -> O(N)
	{
		return find_if(x.begin(), x.end(), fn);
	}

	template <Forward_Itr Itr, class Func>
	Itr find_if_not(Itr st, Itr ed, Func fn)
	// Give range by itr: st && ed -> O(N)
	{
		auto inverse_fn = [&](const auto& x) { return !fn(x); };
		return find_if(st, ed, inverse_fn);
	}

	template <Container Box, class Func>
	auto find_if_not(const Box& x, Func fn)
	// Give range by itr: st && ed -> O(N)
	{
		return find_if_not(x.begin(), x.end(), fn);
	}

	template <Forward_Itr Itr, typename T, class Func = equal<T>>
	Itr find(Itr st, Itr ed, const T& val, Func cmp = Func())
	// Give range by itr: st && ed -> O(N)
	{
		return find_if(st, ed, [&](const T& x) { return cmp(x, val); });
	}

	template <Container Box, typename T>
	auto find(const Box& box, const T& val)
	// Give range by itr: st && ed -> O(N)
	{
		return find(box.begin(), box.end(), val);
	}

	template <Forward_Itr Itr>
	// Must be sorted at first, Return null if not found -> O(logN)
	Itr binary_search(Itr st, Itr ed,
	                  const deref_t<Itr>& val)
	{
		i64 size = distance(st, ed),
		    left = 0,
		    right = size;

		while (left < right) {
			i64 mid = left + size / 2;
			const auto& tmp = *advance(st, mid);

			if (tmp > val)
				left = mid + 1;
			else if (tmp < val)
				right = mid;
			else
				return advance(st, mid);
			size = right - left;
		}

		return Itr();
	}

	template <Container Box>
	// Must be sorted at first, Return null if not found -> O(logN)
	auto binary_search(const Box& box,
	                   const typename Box::value_type& val)
	{
		return binary_search(box.begin(), box.end(), val);
	}

	template <Forward_Itr Itr>
	Itr fill_n(Itr st, u64 n, const deref_t<Itr>& val)
	{
		auto res = st;
		while (n--) {
			*st = val;
			++st;
		}
		return res;
	}

	template <typename T>
	T* fill_n(T* st, u64 n, const T& val)
	{
		auto res = st;
		while (n--) {
			*st = val;
			++st;
		}
		return res;
	}

	template <StreamOutput T>
	void print(const T& fmt)
	{
		std::cout << fmt;
	}

	template <HasPrintMethod T>
	void print(const T& Box)
	{
		Box.print();
	}

	template <typename T>
	    requires StreamOutput<T> && HasPrintMethod<T>
	void print(const T& x)
	{
		std::cout << x;
	}

	template <typename... T>
	void print(const T&... args)
	{
		(..., [](const Display auto& x) { nuts::print(x); }(args));
	}

	template <Forward_Itr Itr>
	void print(Itr st, Itr ed)
	{
		for_each(st, ed, [](const Display auto& x) { nuts::print(x, ' '); });
	}

	template <StreamOutput T>
	void println(const T& fmt)
	{
		nuts::print(fmt, '\n');
	}

	template <HasPrintMethod T>
	void println(const T& Box)
	{
		nuts::print(Box);
	}

	template <typename T>
	    requires StreamOutput<T> && HasPrintMethod<T>
	void println(const T& x)
	{
		nuts::print(x, '\n');
	}

	template <typename... T>
	void println(const T&... args)
	{
		(..., [](const Display auto& x) { nuts::print(x); }(args));
		nuts::print('\n');
	}

	template <Forward_Itr Itr>
	void println(Itr st, Itr ed)
	{
		for_each(st, ed, [](const Display auto& x) { nuts::print(x, ' '); });
		nuts::print('\n');
	}

	template <Container Box, class Func>
	    requires Forward_Itr<typename Box::iterator>
	inline auto partition(Box& box, Func fn)
	{
		return partition(box.begin(), box.end(), fn);
	}

	template <Forward_Itr Itr, class Func>
	Itr partition(Itr st, Itr ed, Func fn)
	{
		if (st == ed)
			return st;

		while (fn(*st)) {
			if (++st == ed)
				return st;
		}

		auto nt = st;
		ed = next(ed);

		while (++nt != ed) {
			if (fn(*nt)) {
				itr_swap(st, nt);
				++st;
			}
		}

		return st;
	}

	template <Bidirectional_Itr Itr, class Compare = less<deref_t<Itr>>>
	// Stable && Average case -> O(n^2)
	void insertion_sort(Itr st, Itr ed, Compare cmp = Compare())
	{
		if (st != ed) {

			// Make a hole by removing the element you are checking,
			// and move elements up to that hole until you find the correct position of the element removed.
			// ++cur: Assume the first element is sorted.

			auto end = next(ed);
			for (auto cur = st; ++cur != end;) {
				auto val = move(*cur);
				auto it = cur, pre = prev(it);
				while (cmp(val, *pre)) {
					*it = move(*pre);
					it = pre;
					if (pre != st)
						--pre;
					else
						break;
				}
				*it = move(val);
			}

			// itr_swap(st, min_in(st, ed, cmp));
			// ed = next(ed);
			// for (auto it = st; ++it != ed; st = it)
			// 	for (auto c = it; cmp(*c, *st); --c, --st)
			// 		itr_swap(st, c);

			// auto end = next(ed);
			// for (auto front = next(st); front != end; ++front) {
			// 	auto cur = front, pre = prev(cur);
			// 	while (cur != st && cmp(*cur, *pre))
			// 		itr_swap(cur--, pre--);
			// }
		}
	}

	template <Container Box, class Compare = less<typename Box::value_type>>
	    requires Bidirectional_Itr<typename Box::iterator>
	// Stable && Average case -> O(n^2)
	void insertion_sort(Box& box, Compare cmp = Compare())
	{
		insertion_sort(box.begin(), box.end(), cmp);
	}

	template <Forward_Itr Itr, class Compare = less<deref_t<Itr>>>
	// Average case -> O(n^2)
	void selection_sort(Itr begin, Itr end, Compare cmp = Compare())
	{
		auto ed = next(end);
		for (auto i = begin; i != ed; ++i)
			itr_swap(i, min_in(i, end, cmp));
	}

	template <Container Box, class Compare = less<typename Box::value_type>>
	    requires Forward_Itr<typename Box::iterator>
	// Average case -> O(n^2)
	void selection_sort(Box& box, Compare cmp = Compare())
	{
		selection_sort(box.begin(), box.end(), cmp);
	}

	template <Bidirectional_Itr Itr, class Compare = less<deref_t<Itr>>>
	// Average case -> O(nlogn)
	void quick_sort(Itr st, Itr ed, Compare cmp = Compare())
	{
		if (st == ed) return;

		// necessary copy
		const auto pivot = *advance(st, (distance(st, ed) - 1) / 2);

		auto l_end = partition(st, ed,
		                       [&](const auto& elem) { return cmp(elem, pivot); }),
		     r_begin = partition(l_end, ed,
		                         [&](const auto& elem) { return !cmp(pivot, elem); });

		quick_sort(st, l_end, cmp);
		quick_sort(r_begin, ed, cmp);
	}

	template <Container Box, class Compare = less<typename Box::value_type>>
	    requires Bidirectional_Itr<typename Box::iterator>
	// Average case -> O(nlogn)
	void quick_sort(Box& box, Compare cmp = Compare())
	{
		quick_sort(box.begin(), box.end(), cmp);
	}

	template <Bidirectional_Itr Itr, class Compare = less<deref_t<Itr>>>
	// Average case -> O(n^(4/3))
	void shell_sort(Itr first, Itr last, Compare cmp = Compare())
	{
		auto ed = next(last);
		auto numElements = distance(first, last);
		if (numElements <= 1) return;

		static constexpr i64 OptimalIncrements[] = {
		        68491,
		        27396,
		        10958,
		        4383,
		        1750,
		        701,
		        301,
		        132,
		        57,
		        23,
		        10,
		        4,
		        1,
		        0,
		};

		i64 increment = OptimalIncrements[0],
		    incrementIndex = 0;
		while (increment >= numElements)
			increment = OptimalIncrements[++incrementIndex];

		while (increment > 0) {
			auto stripe = first;
			auto offset = increment;
			stripe = advance(stripe, offset);
			while (stripe != ed) {
				auto right = stripe,
				     left = stripe;
				left = advance(left, -(increment));
				auto compare = *right;
				auto posRight = offset;
				while (true) {
					if (!cmp(compare, *left))
						break;
					*right = move(*left);
					right = left;
					posRight -= increment;
					if (posRight < increment) break;
					left = advance(left, -(increment));
				}
				if (posRight != offset)
					*right = move(compare);
				++stripe;
				++offset;
			}
			increment = OptimalIncrements[incrementIndex++];
		}
	}

	template <Container Box, class Compare = less<typename Box::value_type>>
	    requires Bidirectional_Itr<typename Box::iterator>
	// Average case -> O(n^(4/3))
	void shell_sort(Box& box, Compare cmp = Compare())
	{
		shell_sort(box.begin(), box.end(), cmp);
	}

	template <Forward_Itr Itr, class Compare = less<deref_t<Itr>>>
	// Stable && Average case -> O(nlogn)
	void merge_sort_in_place(Itr first, Itr last,
	                         Compare cmp = Compare(), i64 size = 0)
	{
		if (size == 0 && first != last)
			size = distance(first, last);
		if (size <= 1) return;
		if (size == 2) {
			if (cmp(*last, *first))
				itr_swap(first, last);
			return;
		}
		auto firstHalf = size / 2,
		     secondHalf = size - firstHalf;
		auto mid = advance(first, firstHalf);
		merge_sort_in_place(first, mid, cmp, firstHalf);
		merge_sort_in_place(mid, last, cmp, secondHalf);
		insertion_sort(first, last, cmp);
	}

	template <Container Box, class Compare = less<typename Box::value_type>>
	    requires Forward_Itr<typename Box::iterator>
	// Stable && Average case -> O(nlogn)
	void merge_sort(Box& box, Compare cmp = Compare())
	{
		merge_sort(box.begin(), box.end(), cmp);
	}

	template <Forward_Itr Itr, class Compare = less<deref_t<Itr>>>
	// Stable && Average case -> O(nlogn)
	void merge_sort(Itr first, Itr last, Compare cmp = Compare())
	{
		merge_sort_in_place(first, last, cmp);
	}

	template <Bidirectional_Itr Itr, class Compare = less<deref_t<Itr>>>
	// Average case -> O(nlogn)
	void intro_sort_with_depth_limit(Itr st, Itr ed, Compare cmp = Compare())
	{
		auto n = distance(st, ed);
		static i64 depth = std::__lg((u64) n) * 2;

		if (n < 2)
			return;
		if (n == 2 && !cmp(*st, *ed))
			return itr_swap(st, ed);
		if (n <= 16)
			return insertion_sort(st, ed, cmp);
		if (depth == 0)
			return heap_sort(st, ed, cmp);

		// Special Partition
		static auto SP = [&](Itr l, Itr r) {
			auto p = advance(l, distance(l, r) / 2),
			     i = l;
			itr_swap(p, r);
			for (auto j = l; j != r; ++j) {
				if (!cmp(*r, *j)) {
					itr_swap(i, j);
					++i;
				}
			}
			itr_swap(i, r);

			return i;
		};

		auto mid = SP(st, ed);
		--depth;
		intro_sort_with_depth_limit(st, prev(mid), cmp);
		intro_sort_with_depth_limit(next(mid), ed, cmp);
	}

	template <Bidirectional_Itr Itr, class Compare = less<deref_t<Itr>>>
	// Average case -> O(nlogn)
	void intro_sort(Itr st, Itr ed, Compare cmp = Compare())
	{
		auto n = distance(st, ed);

		if (n < 2)
			return;
		if (n == 2 && !cmp(*st, *ed))
			return itr_swap(st, ed);
		if (n <= 16)
			return insertion_sort(st, ed, cmp);
		if (n <= 16 * sizeof(usize))
			return heap_sort(st, ed, cmp);

		// Special Partition
		static auto SP = [&](Itr l, Itr r) {
			// Select pivot element
			auto p = advance(l, distance(l, r) / 2),
			     i = l;
			itr_swap(p, r);
			for (auto j = l; j != r; ++j) {
				if (!cmp(*r, *j)) {
					itr_swap(i, j);
					++i;
				}
			}
			itr_swap(i, r);
			return i;
		};

		auto mid = SP(st, ed);
		intro_sort(st, prev(mid), cmp);
		intro_sort(next(mid), ed, cmp);
	}

	template <Container Box, class Compare =
	                                 less<typename Box::value_type>>
	    requires Bidirectional_Itr<typename Box::iterator>
	// Average case -> O(nlogn)
	void intro_sort(Box& box, Compare cmp = Compare())
	{
		intro_sort(box.begin(), box.end(), cmp);
	}

	template <u64 Width = 8, Bidirectional_Itr Itr,
	          class Compare = less<deref_t<Itr>>>
	// Average case -> O(nlogn)
	void heap_sort(Itr st, Itr ed, Compare cmp = Compare())
	{
		auto n = distance(st, ed);
		if (n < 2) return;

		static auto heapify = [&](Itr first,
		                          i64 pos,
		                          i64 stop) {
			first = advance(first, pos);
			auto parent = first, child = first;
			auto value = move(*parent);

			while (pos * Width + 1 < stop) {
				auto inc = pos * (Width - 1) + 1;
				pos += inc;
				child = advance(child, inc);

				auto c = Width;
				if (c + pos > stop)
					c = stop - pos;
				if (c > 1) {
					auto scan = child;
					++scan;
					i64 max = 0;
					for (i64 i = 1; i < c; i++, scan++) {
						if (cmp(*child, *scan)) {
							max = i;
							child = scan;
						}
					}
					pos += max;
				}
				if (cmp(*child, value)) {
					*parent = move(value);
					return;
				}
				*parent = move(*child);
				parent = child;
			}
			*child = move(value);
		};

		i64 first_leaf = (n + Width - 2) / Width;

		for (i64 i = first_leaf; i > 0; --i)
			heapify(st, i - 1, n);
		for (i64 i = n - 1; i > 0; --i) {
			itr_swap(st, ed);
			--ed;
			heapify(st, 0, i);
		}
	}

	template <u64 Width = 8, Container Box,
	          class Compare = less<typename Box::value_type>>
	    requires Bidirectional_Itr<typename Box::iterator>
	// Average case -> O(nlogn)
	void heap_sort(Box& box, Compare cmp = Compare())
	{
		heap_sort<Width>(box.begin(), box.end(), cmp);
	}

	template <Bidirectional_Itr Itr, class Compare = less<deref_t<Itr>>>
	// Default intro_sort() && Average case -> O(nlogn)
	void sort(Itr st, Itr ed, Compare cmp = Compare())
	{
		intro_sort(st, ed, cmp);
	}

	template <Container Box, class Compare = less<typename Box::value_type>>
	    requires Bidirectional_Itr<typename Box::iterator>
	// Default intro_sort() && Average case -> O(nlogn)
	void sort(Box& box, Compare cmp = Compare())
	{
		sort(box.begin(), box.end(), cmp);
	}
}

#endif
