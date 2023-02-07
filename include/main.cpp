#include "bits.h"
#include "timer.h"
#include <random>
#include <bits/stdc++.h>

using namespace nuts;

template <Iterable Box, class Compare = less<typename Box::value_type>>
void xxsort_test(Box& x, Compare cmp = Compare {})
{
	println("::::::::::::::::::::::::::::::::::::::::::\n");
	println("#[Test]: N = ", x.size());

	using xxsort_func_type = function<void(Box&, less<typename Box::value_type>)>;

	queue<xxsort_func_type> fst {
	        insertion_sort,
	        selection_sort,
	        quick_sort,
	        shell_sort,
	        merge_sort,
	        intro_sort,
	        heap_sort,
	};

	static hash_map<i32, string> index_map {
	        {0, "<insertion_sort>"},
	        {1, "<selection_sort>"},
	        {2, "<quick_sort>"},
	        {3, "<shell_sort>"},
	        {4, "<merge_sort>"},
	        {5, "<intro_sort>"},
	        {6, "<heap_sort>"},
	};

	array<Box, 7> m(x);
	vector<pair<double, string>> rank;

	for (u64 i: range(0, 7)) {
		print('\n', index_map[i]);
		auto& fn = fst.front();

		Timer clock;
		fn(m[i], cmp);
		auto r = clock.print();

		assert("Sort failed!" && is_sorted(m[i]));

		rank.push_back({r, index_map[i]});
		// print(m[i]);
		fst.pop();
	}

	println("\n#[Ranking]:");
	insertion_sort(rank);
	for_each(rank, [&](const auto& x) { println(x._0(), "(ms) ", x._1()); });
	println("\n::::::::::::::::::::::::::::::::::::::::::");
}

template <Iterable Box_Type,
          class Compare = less<typename Box_Type::value_type>>
void visual()
{
	queue<function<void(Box_Type&, less<i32>)>> fst {
	        insertion_sort,
	        selection_sort,
	        quick_sort,
	        shell_sort,
	        merge_sort,
	        intro_sort,
	        heap_sort,
	};

	auto cmp = Compare {};

	std::random_device rd;
	Box_Type v;

	u64 base, len;
	std::cin >> base >> len;
	for (const auto& fn: range(fst)) {
		for (i32 n: range(base, base + len)) {
			for (u64 i: range(0, n))
				v.push_back(rd() % INT32_MAX);
			Timer c;
			fn(v, cmp);
			f64 cst = 1 / c.elapsed();
			println(n, '\n', cst);
			v.clear();
		}
	}
}

template <class Fn_A, class Fn_B>
void test_fn(Fn_A&& f1, Fn_B&& f2)
{
	Timer c1;
	f1();
	c1.print();

	Timer c2;
	f2();
	c2.print();
}

template <bool T>
requires T void deduction_guide_test()
{
	array a {1, 2, 3, 4, 5};
	vector v {1, 2, 3, 4, 5};
	list l {1, 2, 3, 4, 5};
	deque d {1, 2, 3, 4, 5};
	set s {1, 2, 3, 4, 5};
	unordered_set hs {1, 2, 3, 4, 5};

	static const array
	        as {"human", "dog", "cat", "cow",
	            "sheep", "monkey", "turtle", "elephant"};

	map m {
	        pair {as[0], 0},
	        {as[1], 1},
	        {as[2], 2},
	};

	unordered_map hm {
	        pair {as[0], 0},
	        {as[1], 1},
	        {as[2], 2},
	};

	println(a, v, l, d, s, hs, m, hm);
}

int main()
{
	u64 n = pow(2, 16);
	std::random_device rd;
	// vector<i32> v;

	// for (int i = 0; i < n; ++i)
	// 	v.push_back(rd() % (u64) n);

	// xxsort_test(v);
	// visual<vector<i32>>();
	// deduction_guide_test<true>();

	std::vector<i32> a;
	nuts::vector<i32> b;

	for (i32 i = 0; i < n; ++i) {
		auto tmp = rd() % (i32) n;
		a.push_back(tmp);
		b.push_back(tmp);
	}

	test_fn([&] { std::sort(a.begin(), a.end()); },
	        [&] { nuts::sort(b.begin(), b.end()); });

	// struct Foo
	//     : public tuple<string, i32>
	// {
	// 	using tuple::tuple;
	// };

	return 0;
}