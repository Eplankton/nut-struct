#include "bits.h"
#include "timer.h"
#include <random>

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
			auto cst = 1 / c.elapsed();
			println(n, '\n', cst);
			v.clear();
		}
	}
}

template <Container SrcA, Container SrcB>
auto generate(const SrcA& a, const SrcB& b)
{
	vector<pair<typename SrcA::value_type,
	            typename SrcB::value_type>>
	        ret;
	for (const auto& i: range(a)) {
		for (const auto& j: range(b)) {
			ret.push_back({i, j});
		}
	}
	return ret;
}

void deduction_guide_test()
{
	array a {1, 2, 3, 4, 5};
	vector v {1, 2, 3, 4, 5};
	list l {1, 2, 3, 4, 5};
	deque d {1, 2, 3, 4, 5};
	set s {1, 2, 3, 4, 5};
	unordered_set hs {1, 2, 3, 4, 5};

	array as {
	        "human",
	        "dog",
	        "cat",
	        "cow",
	        "sheep",
	        "monkey",
	        "turtle",
	        "elephant",
	};

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
	deque dq {
	        "human",
	        "dog",
	        "cat",
	        "cow",
	        "sheep",
	        "monkey",
	        "turtle",
	        "elephant",
	};

	// xxsort_test(dq);

	vector<i32> v;
	auto n = 3e4;
	std::random_device rd;
	for (int i = 0; i < n; ++i)
		v.push_back(rd() % (u64) n);

	xxsort_test(v);
	// visual<vector<i32>>();

	deduction_guide_test();

	// deque ass {
	//         array {
	//                 vector {
	//                         list {
	//                                 tuple {
	//                                         pair {
	//                                                 dq,
	//                                                 array {1, 2, 3},
	//                                         },
	//                                         114514,
	//                                 },
	//                         },
	//                 },
	//         },
	// };

	return 0;
}