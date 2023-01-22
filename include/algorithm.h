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
	template <typename... T>
	auto sum(T... s)
	{
		return (... + s);
	}

	template <Iterable Box, class Func>
	Func for_each(const Box& box, Func fn)
	{
		return for_each(box.begin(), box.end(), fn);
	}

	template <typename Itr, class Func>
	Func for_each(Itr st, Itr ed, Func fn)
	{
		if (st != ed)
		{
			for (auto _end = next(ed); st != _end; ++st)
				fn(*st);
		}
		return fn;
	}

	template <Forward_Itr Itr>
	i64 distance(Itr st, Itr ed)
	{
		if (st == ed) return 0;
		i64 n = 0;
		auto _end = advance(ed, 1);
		while (st != _end)
		{
			++st;
			++n;
		}
		return n;
	}

	template <Random_Itr Itr>
	inline i64 distance(Itr st, Itr ed)
	{
		return (st == ed) ? 0 : ed - st + 1;
	}

	template <Forward_Itr Itr>
	Itr advance(Itr it, i64 n)
	{
		if (n >= 0)
			while (n--) ++it;
		return it;
	}

	template <Bidirectional_Itr Itr>
	Itr advance(Itr it, i64 n)
	{
		if (n >= 0)
			while (n--) ++it;
		else
			while (n++) --it;
		return it;
	}

	template <Random_Itr Itr>
	inline Itr advance(Itr it, i64 n)
	{
		return it + n;
	}

	template <Forward_Itr Itr>
	auto next(Itr it)
	{
		return advance(it, 1);
	}

	template <Bidirectional_Itr Itr>
	auto prev(Itr it)
	{
		return advance(it, -1);
	}

	template <typename T>
	inline const T& min(const T& a, const T& b)// Return the minimum
	{
		return (a < b) ? a : b;
	}

	template <typename T>
	inline const T& max(const T& a, const T& b)// Return the maximum
	{
		return (a > b) ? a : b;
	}

	template <Forward_Itr Itr, class Compare = less<typename Itr::value_type>>
	Itr min_in(Itr st, Itr ed, Compare cmp = Compare {})
	// Give range by itr: st && ed -> O(n)
	{
		Itr tmp = st;
		if (st != ed)
		{
			auto _end = next(ed);
			for (auto i = st; i != _end; i++)
				if (cmp(*i, *tmp))
					tmp = i;
		}
		return tmp;
	}

	template <Forward_Itr Itr, class Compare = greater<typename Itr::value_type>>
	Itr max_in(Itr st, Itr ed, Compare cmp = Compare {})
	// Give range by itr: st && ed -> O(n)
	{
		Itr tmp = st;
		if (st != ed)
		{
			auto _end = next(ed);
			for (auto i = st; i != _end; i++)
				if (cmp(*i, *tmp))
					tmp = i;
		}
		return tmp;
	}

	template <Iterable Box, class Compare = less<typename Box::value_type>>
	auto min_in(const Box& box, Compare cmp = Compare {})
	{
		return min_in(box.begin(), box.end(), cmp);
	}

	template <Iterable Box, class Compare = less<typename Box::value_type>>
	auto max_in(const Box& box, Compare cmp = Compare {})
	{
		return max_in(box.begin(), box.end(), cmp);
	}

	template <typename T>
	void swap(T& a, T& b)// Swap value
	{
		auto tmp = nuts::move(b);
		b = nuts::move(a);
		a = nuts::move(tmp);
	}

	template <Forward_Itr Itr>
	inline void itr_swap(Itr a, Itr b)// Swap value
	{
		if (a == b) return;
		swap(*a, *b);
	}

	template <Bidirectional_Itr Itr>
	void reverse(Itr st, Itr ed)
	// Give range by itr: st && ed -> O(N)
	{
		while (st != ed && st != ed - 1)
		{
			itr_swap(st, ed);
			++st;
			--ed;
		}
		if (st != ed)
			itr_swap(st, ed);
	}

	template <Bidirectional_Itr Itr, class Compare = nuts::less<typename Itr::value_type>>
	bool is_sorted(Itr st, Itr ed, Compare cmp = Compare {})
	// Give range by itr: st && ed -> O(N)
	{
		ed += 1;
		for (auto it = st + 1; it != ed; it++)
			if (cmp(*it, *(it - 1)))
				return false;
		return true;
	}

	template <Iterable Box, class Compare = nuts::less<typename Box::value_type>>
	requires Bidirectional_Itr<typename Box::iterator>
	bool is_sorted(const Box& box, Compare cmp = Compare {})
	// Give range by itr: st && ed -> O(N)
	{
		return is_sorted(box.begin(), box.end(), cmp);
	}

	template <Forward_Itr Itr, class Func>
	Itr find_if(Itr st, Itr ed, Func fn)
	// Give range by itr: st && ed -> O(N)
	{
		ed = advance(ed, 1);
		while (st != ed)
		{
			if (fn(*st))
				break;
			++st;
		}
		return st;
	}

	template <Iterable Box, class Func>
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

	template <Iterable Box, class Func>
	auto find_if_not(const Box& x, Func fn)
	// Give range by itr: st && ed -> O(N)
	{
		return find_if_not(x.begin(), x.end(), fn);
	}

	template <Forward_Itr Itr, typename T, class Func = equal<T>>
	Itr find(Itr st, Itr ed, const T& val, Func cmp = Func {})
	// Give range by itr: st && ed -> O(N)
	{
		return find_if(st, ed, [&](const T& x) { return cmp(x, val); });
	}

	template <Iterable Box, typename T>
	auto find(const Box& box, const T& val)
	// Give range by itr: st && ed -> O(N)
	{
		return find(box.begin(), box.end(), val);
	}

	template <typename Itr, typename T>
	Itr lower_bound(Itr st, Itr ed, const T& val)
	// Return null if not found && must be sorted -> O(logN)
	{
		auto cmp = less<T> {};
		assert(!is_sorted(st, ed, cmp));
		Itr it, cpy = st;
		i64 count = distance(st, ed), step = 0;
		while (count > 0)
		{
			it = st, step = count / 2, it += step;
			if (cmp(*it, val))
			{
				st = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		if (cmp(*it, val) && !cmp(*it, val)) return st;
		else
			return Itr {};
	}

	template <Forward_Itr Itr>
	Itr fill_n(Itr st, u64 n, const typename Itr::value_type& val)
	{
		auto res = st;
		while (n--)
		{
			*st = val;
			++st;
		}
		return res;
	}

	template <typename T>
	T* fill_n(T* st, u64 n, const T& val)
	{
		auto res = st;
		while (n--)
		{
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
	void print(T&&... args)
	{
		(..., [](const Display auto& x) { print(x); }(args));
	}

	template <Forward_Itr Itr>
	void print(Itr st, Itr ed)
	{
		for_each(st, ed, [](const Display auto& x) { print(x, ' '); });
	}

	template <StreamOutput T>
	void println(const T& fmt)
	{
		print(fmt, '\n');
	}

	template <HasPrintMethod T>
	void println(const T& Box)
	{
		print(Box);
	}

	template <typename T>
	requires StreamOutput<T> && HasPrintMethod<T>
	void println(const T& x)
	{
		print(x, '\n');
	}

	template <typename... T>
	void println(T&&... args)
	{
		(..., [](const Display auto& x) { print(x); }(args));
		print('\n');
	}

	template <Forward_Itr Itr>
	void println(Itr st, Itr ed)
	{
		for_each(st, ed, [](const Display auto& x) { print(x, ' '); });
		print('\n');
	}

	template <Iterable Box, class Func>
	requires Forward_Itr<typename Box::iterator>
	auto partition(Box& box, Func fn)
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

	template <Iterable Box, class Compare = less<typename Box::value_type>>
	requires Bidirectional_Itr<typename Box::iterator>
	// Default intro_sort() && Average case -> O(nlogn)
	void sort(Box& box, Compare cmp = Compare {})
	{
		sort(box.begin(), box.end(), cmp);
	}

	template <Bidirectional_Itr Itr, class Compare = less<typename Itr::value_type>>
	// Default intro_sort() && Average case -> O(nlogn)
	void sort(Itr st, Itr ed, Compare cmp = Compare {})
	{
		intro_sort(st, ed, cmp);
	}

	template <Iterable Box, class Compare = less<typename Box::value_type>>
	requires Bidirectional_Itr<typename Box::iterator>
	// Stable && Average case -> O(n^2)
	void insertion_sort(Box& box, Compare cmp = Compare {})
	{
		insertion_sort(box.begin(), box.end(), cmp);
	}

	template <Bidirectional_Itr Itr, class Compare = less<typename Itr::value_type>>
	// Stable && Average case -> O(n^2)
	void insertion_sort(Itr st, Itr ed, Compare cmp = Compare {})
	{
		itr_swap(st, min_in(st, ed));
		ed = next(ed);
		for (auto it = st; ++it != ed; st = it)
			for (auto c = it; cmp(*c, *st); --c, --st)
				itr_swap(st, c);
	}

	template <Iterable Box, class Compare = less<typename Box::value_type>>
	requires Forward_Itr<typename Box::iterator>
	// Average case -> O(n^2)
	void selection_sort(Box& box, Compare cmp = Compare {})
	{
		selection_sort(box.begin(), box.end(), cmp);
	}

	template <Forward_Itr Itr, class Compare = less<typename Itr::value_type>>
	// Average case -> O(n^2)
	void selection_sort(Itr begin, Itr end, Compare cmp = Compare {})
	{
		auto ed = next(end);
		for (auto i = begin; i != ed; ++i)
			itr_swap(i, min_in(i, end, cmp));
	}

	template <Iterable Box, class Compare = less<typename Box::value_type>>
	requires Bidirectional_Itr<typename Box::iterator>
	// Average case -> O(nlogn)
	void quick_sort(Box& box, Compare cmp = Compare {})
	{
		quick_sort(box.begin(), box.end(), cmp);
	}

	template <Bidirectional_Itr Itr, class Compare = less<typename Itr::value_type>>
	// Average case -> O(nlogn)
	void quick_sort(Itr first, Itr last, Compare cmp = Compare {})
	{
		if (first == last) return;

		// necessary copy
		const auto pivot = *advance(first, distance(next(first), last) / 2);

		auto l_end = partition(first, last,
		                       [&](const auto&elem) { return cmp(elem, pivot); }),
		     r_begin = partition(l_end, last,
		                         [&](const auto&elem) { return !cmp(pivot, elem); });

		quick_sort(first, l_end, cmp);
		quick_sort(r_begin, last, cmp);
	}

	template <Iterable Box, class Compare = less<typename Box::value_type>>
	requires Bidirectional_Itr<typename Box::iterator>
	// Average case -> O(n^(4/3))
	void shell_sort(Box& box, Compare cmp = Compare {})
	{
		shell_sort(box.begin(), box.end(), cmp);
	}

	template <Bidirectional_Itr Itr, class Compare = less<typename Itr::value_type>>
	// Average case -> O(n^(4/3))
	void shell_sort(Itr first, Itr last, Compare cmp = Compare {})
	{
		auto ed = next(last);
		auto numElements = distance(first, last);
		if (numElements <= 1) return;

		static const i64 OptimalIncrements[] = {
		        68491, 27396, 10958, 4383, 1750, 701, 301, 132, 57, 23, 10, 4, 1, 0};

		i64 increment = OptimalIncrements[0],
		    incrementIndex = 0;
		while (increment >= numElements)
			increment = OptimalIncrements[++incrementIndex];

		while (increment > 0)
		{
			auto stripe = first;
			auto offset = increment;
			stripe = advance(stripe, offset);
			while (stripe != ed)
			{
				auto right = stripe,
				     left = stripe;
				left = advance(left, -(increment));
				auto compare = *right;
				auto posRight = offset;
				while (true)
				{
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

	template <Iterable Box, class Compare = less<typename Box::value_type>>
	requires Forward_Itr<typename Box::iterator>
	// Stable && Average case -> O(nlogn)
	void merge_sort(Box& box, Compare cmp = Compare {})
	{
		merge_sort(box.begin(), box.end(), cmp);
	}

	template <Forward_Itr Itr, class Compare = less<typename Itr::value_type>>
	// Stable && Average case -> O(nlogn)
	void merge_sort(Itr first, Itr last, Compare cmp = Compare {})
	{
		merge_sort_in_place(first, last, cmp);
	}

	template <Forward_Itr Itr, class Compare = less<typename Itr::value_type>>
	// Stable && Average case -> O(nlogn)
	void merge_sort_in_place(Itr first, Itr last, Compare cmp = Compare {}, i64 size = 0)
	{
		if (size == 0 && first != last)
			size = distance(first, last);

		if (size <= 1)
			return;

		if (size <= 16)
		{
			if (size == 2)
			{
				if (cmp(*last, *first))
					itr_swap(first, last);
				return;
			}
			insertion_sort(first, last, cmp);
			return;
		}

		auto left_half = size / 2,
		     right_half = size - left_half;
		auto mid = advance(first, left_half);

		merge_sort_in_place(first, mid - 1, cmp, left_half);
		merge_sort_in_place(mid, last, cmp, right_half);

		auto right = mid;

		while (first != mid)
		{
			if (cmp(*right, *first))
			{
				auto misplaced = move(*first);
				*first = move(*right);
				auto scan = right, next = scan;
				++next;
				auto ed = advance(last, 1);

				while (next != ed && cmp(*next, misplaced))
					*scan++ = move(*next++);
				*scan = move(misplaced);
			}
			++first;
		}
	}

	template <Iterable Box, class Compare = less<typename Box::value_type>>
	requires Bidirectional_Itr<typename Box::iterator>
	// Average case -> O(nlogn)
	void intro_sort(Box& box, Compare cmp = Compare {})
	{
		intro_sort(box.begin(), box.end(), cmp);
	}

	template <Bidirectional_Itr Itr, class Compare = less<typename Itr::value_type>>
	// Average case -> O(nlogn)
	void intro_sort(Itr first, Itr last, Compare cmp = Compare {})
	{
		auto n = distance(first, last);

		if (n <= 1) return;

		if (n <= 16)
		{
			if (n == 2)
			{
				if (cmp(*last, *first))
					itr_swap(first, last);
				return;
			}
			insertion_sort(first, last, cmp);
			return;
		}

		// necessary copy
		const auto pivot = *advance(first, distance(next(first), last) / 2);

		auto l_end = partition(first, last,
		                       [&](const auto&elem) { return cmp(elem, pivot); }),
		     r_begin = partition(l_end, last,
		                         [&](const auto&elem) { return !cmp(pivot, elem); });

		intro_sort(first, l_end, cmp);
		intro_sort(r_begin, last, cmp);
	}

	template <u64 Width = 8, Iterable Box,
	          class Compare = less<typename Box::value_type>>
	requires Bidirectional_Itr<typename Box::iterator>
	// Average case -> O(nlogn)
	void heap_sort(Box& box, Compare cmp = Compare {})
	{
		heap_sort<Width>(box.begin(), box.end(), cmp);
	}

	template <u64 Width = 8, Bidirectional_Itr Itr,
	          class Compare = less<typename Itr::value_type>>
	// Average case -> O(nlogn)
	void heap_sort(Itr first, Itr last, Compare cmp = Compare {})
	{
		if (Width < 2)
		{
			heap_sort<2>(first, last, cmp);
			return;
		}

		auto n = distance(first, last);
		if (n < 2) return;

		auto heapify = [](Itr first, u64 pos, u64 stop, Compare cmp = Compare {}) {
			first = advance(first, pos);
			auto parent = first, child = first;
			auto value = move(*parent);

			while (pos * Width + 1 < stop)
			{
				auto inc = pos * (Width - 1) + 1;
				pos += inc;
				child = advance(child, inc);

				auto c = Width;
				if (c + pos > stop)
					c = stop - pos;
				if (c > 1)
				{
					auto scan = child;
					++scan;
					u64 max = 0;
					for (u64 i = 1; i < c; i++, scan++)
					{
						if (cmp(*child, *scan))
						{
							max = i;
							child = scan;
						}
					}
					pos += max;
				}
				if (!cmp(value, *child))
				{
					*parent = move(value);
					return;
				}
				*parent = move(*child);
				parent = child;
			}
			*child = move(value);
		};

		u64 firstLeaf = (n + Width - 2) / Width;

		for (u64 i = firstLeaf; i > 0; i--)
			heapify(first, i - 1, n, cmp);
		for (auto i = n - 1; i > 0; i--)
		{
			itr_swap(first, last);
			--last;
			heapify(first, 0, i, cmp);
		}
	}

	// template <typename Seq>
	// // General seq version
	// void binary_insertion_sort(Seq& a)
	// {
	// 	int left, right, middle;
	// 	for (int i = 1; i < a.size(); i++)
	// 	{
	// 		auto key = a[i];
	// 		left = 0, right = i - 1;
	// 		while (left <= right)
	// 		{
	// 			middle = (left + right) / 2;
	// 			if (a[middle] > key)
	// 				right = middle - 1;
	// 			else
	// 				left = middle + 1;
	// 		}
	// 		for (int j = i - 1; j >= left; j--)
	// 			a[j + 1] = a[j];
	// 		a[left] = key;
	// 	}
	// }

	// template <typename Seq>
	// void max_heapify(Seq& arr, int start, int end)
	// {
	// 	int dad = start, son = dad * 2 + 1;
	// 	while (son <= end)
	// 	{
	// 		if (son + 1 <= end && arr[son] < arr[son + 1])
	// 			son++;
	// 		if (arr[dad] > arr[son])
	// 			return;
	// 		else
	// 		{
	// 			swap(arr[dad], arr[son]);
	// 			dad = son;
	// 			son = dad * 2 + 1;
	// 		}
	// 	}
	// }

	// template <typename Seq>
	// void heap_sort(Seq& arr)// General seq version
	// {
	// 	int len = arr.size();
	// 	for (int i = len / 2 - 1; i >= 0; i--)
	// 	{
	// 		max_heapify(arr, i, len - 1);
	// 	}
	// 	for (int i = len - 1; i > 0; i--)
	// 	{
	// 		swap(arr[0], arr[i]);
	// 		max_heapify(arr, 0, i - 1);
	// 	}
	// }
}

#endif
