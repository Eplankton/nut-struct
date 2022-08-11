#ifndef _NUTS_ALG_
#define _NUTS_ALG_ 1

#include "concept.h"
#include "functional.h"
#include "iterator.h"
#include "range.h"
#include "type.h"

#include <cassert>

namespace nuts
{
	template <typename Itr, class Func>
	Func for_each(Itr st, Itr ed, Func fn)
	{
		ed += 1;
		for (; st != ed; ++st) fn(*st);
		return fn;
	}

	template <typename T, class Func>
	Func for_each(const T& x, Func fn)
	{
		for (auto& i: range(x)) fn(i);
		return fn;
	}

	template <Bidirectional_Itr Itr>
	i64 distance(Itr st, Itr ed)
	{
		if (st == ed) return 0;
		i64 n = 0;
		ed += 1;
		while (st != ed)
		{
			++st;
			++n;
		}
		return n;
	}

	template <Random_Itr Itr>
	i64 distance(Itr st, Itr ed)
	{
		return (st == ed) ? 0 : ed - st + 1;
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
	Itr advance(Itr it, i64 n)
	{
		return it + n;
	}

	template <typename T>
	const T& min(const T& a, const T& b)// Return the minimum
	{
		return (a < b) ? a : b;
	}

	template <typename T>
	const T& max(const T& a, const T& b)// Return the maximum
	{
		return (a > b) ? a : b;
	}

	template <typename Itr>
	Itr min_in(Itr st, Itr ed)// Give range by itr: st && ed -> O(N)
	{
		Itr tmp = st;
		ed += 1;
		for (auto i = st; i != ed; i++)
			if (*i < *tmp) tmp = i;
		return tmp;
	}

	template <typename Itr>
	Itr max_in(Itr st, Itr ed)// Give range by itr: st && ed -> O(N)
	{
		Itr tmp = st;
		ed += 1;
		for (auto i = st; i != ed; i++)
			if (*i > *tmp) tmp = i;
		return tmp;
	}

	template <typename T>
	void swap(T& a, T& b)// Swap value
	{
		auto tmp = std::move(b);
		b = std::move(a);
		a = std::move(tmp);
	}

	template <typename Itr>
	void itr_swap(Itr a_itr, Itr b_itr)// Swap two val by itr or ptr
	{
		swap(*a_itr, *b_itr);
	}

	template <Bidirectional_Itr Itr>
	void reverse(Itr st, Itr ed)// Give range by itr: st && ed -> O(N)
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

	template <typename Itr, class Comp = nuts::less<typename Itr::value_type>>
	bool is_sorted(Itr st, Itr ed, Comp cmp = Comp())
	{
		ed += 1;
		for (auto it = st + 1; it != ed; it++)
		{
			if (cmp(*it, *(it - 1)))
				return false;
		}
		return true;
	}

	template <typename Container, class Comp = nuts::less<typename Container::value_type>>
	bool is_sorted(const Container& x, Comp cmp())
	{
		auto ed = x.end() + 1;
		for (auto it = x.begin() + 1; it != ed; it++)
		{
			if (cmp(*it, *(it - 1)))
				return false;
		}	
		return true;
	}

	template <typename Itr, class Func>
	Itr find_if(Itr st, Itr ed, Func fn)// Basic linear version -> O(N)
	{
		ed += 1;
		while (st != ed)
		{
			if (fn(*st))
				return st;
			++st;
		}
		return Itr();
	}

	template <typename Itr, typename T>
	Itr find(Itr st, Itr ed, const T& val)// Basic linear version -> O(N)
	{
		auto cpt = [&val](const T& x) { return x == val; };
		return find_if(st, ed, cpt);
	}

	template <typename Itr, typename T>
	Itr lower_bound(Itr st, Itr ed, const T& val)
	// Return itr:none if not found && must be random_access -> O(logN)
	{
		Itr it, cpy = st;
		u64 count = distance(st, ed), step = 0;
		while (count > 0)
		{
			it = st;
			step = count / 2;
			it += step;
			if (*it < val)
			{
				st = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		if (st >= cpy && st <= ed)
			return st;
		else
			return Itr();
	}

	template <typename Itr, typename T>
	bool binary_search(Itr st, Itr ed, const T& val)
	// Return false if not found -> O(logN)
	{
		st = lower_bound(st, ed, val);
		return st != nullptr && val == *st;
	}

	template <typename Seq, typename T>
	bool binary_search(Seq& arr, const T& val)
	// For seq version -> O(logN)
	{
		return binary_search(arr.begin(), arr.end(), val);
	}

	template <typename Itr>
	// General itr version -> O(N^2)
	void insertion_sort(Itr st, Itr ed)
	{
		for (Itr i = st + 1; i != ed + 1; i++)
		{
			for (Itr j = i; j != st; j--)
			{
				Itr tmp = j - 1;
				if (tmp != nullptr && (*j) < (*tmp))
					itr_swap(j, tmp);
				else
					break;
			}
		}
	}

	template <typename Itr, typename T, class Compare = nuts::less<T>>
	// General itr version with cmp() -> O(N^2)
	void insertion_sort(Itr st, Itr ed,
	                    Compare cmp = Compare())
	{
		for (Itr i = st + 1; i != ed + 1; i++)
		{
			for (Itr j = i; j != st; j--)
			{
				Itr tmp = j - 1;
				if (tmp != nullptr && cmp((*j), (*tmp)))
					itr_swap(j, tmp);
				else
					break;
			}
		}
	}

	template <typename T, typename Seq, typename Compare = nuts::less<T>>
	// General seq version with Compare():cmp -> O(N^2)
	void insertion_sort(Seq& arr, Compare cmp = Compare())
	{
		auto st = arr.begin(), ed = arr.end() + 1;
		for (auto i = st + 1; i != ed; ++i)
		{
			for (auto j = i; j != st; --j)
			{
				auto tmp = j - 1;
				if (tmp != nullptr && cmp((*j), (*tmp)))
					itr_swap(j, tmp);
				else
					break;
			}
		}
	}

	template <typename T, typename Itr, typename Compare = nuts::less<T>>
	// General itr version with Compare():cmp -> O(N^2)
	void insertion_sort(Itr st, Itr ed, Compare cmp = Compare())
	{
		for (Itr i = st + 1; i != ed + 1; i++)
		{
			for (Itr j = i; j != st; j--)
			{
				Itr tmp = j - 1;
				if (tmp != nullptr && cmp((*j), (*tmp)))
					itr_swap(j, tmp);
				else
					break;
			}
		}
	}

	template <typename Seq>
	// General seq version
	void binary_insertion_sort(Seq& a)
	{
		int left, right, middle;
		for (int i = 1; i < a.size(); i++)
		{
			auto key = a[i];
			left = 0, right = i - 1;
			while (left <= right)
			{
				middle = (left + right) / 2;
				if (a[middle] > key)
					right = middle - 1;
				else
					left = middle + 1;
			}
			for (int j = i - 1; j >= left; j--)
				a[j + 1] = a[j];
			a[left] = key;
		}
	}

	template <typename Itr>
	void selection_sort(Itr st, Itr ed)
	// General itr version -> O(N^2)
	{
		for (auto i = st; i != ed + 1; i++)
			itr_swap(i, min_in(i, ed));
	}

	template <typename Seq>
	void selection_sort(Seq& arr)
	// General itr version -> O(N^2)
	{
		selection_sort(arr.begin(), arr.end());
	}

	template <typename Seq>
	void shell_sort(Seq& arr)
	// General seq version -> O(nlog(N)^2)
	{
		i64 n = arr.size();
		for (i64 step = n / 2; step > 0; step /= 2)
		{
			for (i64 i = 0; i < step; i++)
			{
				for (i64 j = i + step; j < n; j += step)
				{
					if (arr[j] < arr[j - step])
					{
						auto tmp = arr[j];
						i64 k = j - step;
						while (k >= 0 && arr[k] > tmp)
						{
							arr[k + step] = arr[k];
							k -= step;
						}
						arr[k + step] = tmp;
					}
				}
			}
		}
	}

	template <typename Seq>
	void quick_sort_helper(Seq& arr, int l, int r)
	{
		if (l >= r)
			return;
		auto mid = arr[(l + r) >> 1];
		int i = l - 1, j = r + 1;
		while (i < j)
		{
			do
				i++;
			while (arr[i] < mid);
			do
				j--;
			while (arr[j] > mid);
			if (i < j)
			{
				auto tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
		quick_sort_helper(arr, l, j);
		quick_sort_helper(arr, j + 1, r);
	}

	template <typename Seq>
	void quick_sort(Seq& arr)
	// General seq version -> O(nlogN)
	{
		quick_sort_helper(arr, 0, arr.size() - 1);
	}

	template <typename Seq>
	void merge_sort_helper(Seq& data, Seq& copy, int start, int end)
	{
		if (start == end)
			return;
		int mid = (start + end) / 2;
		merge_sort_helper(copy, data, start, mid);
		merge_sort_helper(copy, data, mid + 1, end);
		int i = mid, j = end, index = end;
		while (i >= start && j >= mid + 1)
		{
			if (data[i] > data[j])
				copy[index--] = data[i--];
			else
				copy[index--] = data[j--];
		}
		for (; i >= start;)
			copy[index--] = data[i--];
		for (; j >= mid + 1; j--)
			copy[index--] = data[j--];
	}

	template <typename Seq>
	void merge_sort(Seq& v)// General seq version
	{
		Seq copy;
		int len = v.size();
		for (int i = 0; i < len; i++)
			copy.push_back(v[i]);
		merge_sort_helper(v, copy, 0, len - 1);
		v = copy;
	}

	template <typename Seq>
	void max_heapify(Seq& arr, int start, int end)
	{
		int dad = start, son = dad * 2 + 1;
		while (son <= end)
		{
			if (son + 1 <= end && arr[son] < arr[son + 1])
				son++;
			if (arr[dad] > arr[son])
				return;
			else
			{
				swap(arr[dad], arr[son]);
				dad = son;
				son = dad * 2 + 1;
			}
		}
	}

	template <typename Seq>
	void heap_sort(Seq& arr)// General seq version
	{
		int len = arr.size();
		for (int i = len / 2 - 1; i >= 0; i--)
		{
			max_heapify(arr, i, len - 1);
		}
		for (int i = len - 1; i > 0; i--)
		{
			swap(arr[0], arr[i]);
			max_heapify(arr, 0, i - 1);
		}
	}
}

#endif
