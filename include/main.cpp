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

	for (auto i: range(0, 7))
	{
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

int main()
{
	deque<string> a {"human", "dog", "cat", "cow",
	                 "sheep", "monkey", "turtle", "elephant"};
	// xxsort_test(a);

	vector<i32> v;
	auto n = 1e4;
	std::random_device rd;
	for (int i = 0; i < n; ++i)
		v.push_back(rd() % (u64) n);

	xxsort_test(v);

	return 0;
}