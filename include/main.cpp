#include "bits.h"
#include "timer.h"
#include <random>

using namespace nuts;

template <Iterable Box, class Compare = nuts::less<typename Box::value_type>>
void xxsort_test(Box& x, Compare cmp = Compare())
{
	println("\n::::::::::::::::::::::::::::::::::::::::::");
	println("#[Test]: N = ", x.size());
	array<Box, 7> m(x);
	function<decltype(insertion_sort<Box>)> ex;

	queue<decltype(ex)> fst {
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

	vector<pair<double, string>> rank;

	for (auto i: range(0, 7))
	{
		print('\n', index_map[i]);
		auto& fn = fst.front();

		Timer clock;
		fn(m[i], cmp);
		auto r = clock.print();

		rank.push_back({r, index_map[i]});
		// print(m[i]);
		fst.pop();
	}

	println("\n#[Ranking]:");
	insertion_sort(rank);
	for_each(rank, [&](const auto& x) { println(x.get_first(), "(ms) ", x.get_second()); });
	println("\n::::::::::::::::::::::::::::::::::::::::::");
}

int main()
{
	deque<string> a {"human", "dog", "cat", "cow",
	                 "sheep", "monkey", "turtle", "elephant"};

	vector<i32> v;

	std::random_device rd;
	for (int i = 0; i < 1e6; ++i)
		v.push_back(rd());

	xxsort_test(a);
	xxsort_test(v);

	return 0;
}
