#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest/doctest.h"
#include "../nanobench/src/include/nanobench.h"
#include "../include/bits.h"

#define TEST_NAME "Test"
TEST_CASE(TEST_NAME)
{
	// Create a single benchmark instance that is used in multiple benchmark
	// runs, with different settings for complexityN.
	ankerl::nanobench::Bench bench;
	ankerl::nanobench::Rng rng;

	// Running the benchmark multiple times, with different number of elements
	for (auto n: {
	             10U,
	             100U,
	             1000U,
	             10000U,
	             100000U,
	             1000000U,
	             10000000U,
	     }) {

		nuts::vector<uint64_t> v;
		v.reserve(n);

		while (v.size() < n) {
			v.emplace_back(rng());
		}

		bench.complexityN(v.size()).run(TEST_NAME, [&] {
			nuts::sort(v.begin(), v.end());
			// nuts::find(v, rng());
		});
	}

	// calculate BigO complexity best fit and print the results
	std::cout << bench.complexityBigO() << std::endl;
}

// #include <random>
// int main()
// {
// 	using namespace nuts;
// 	i64 n = 1e6;
// 	vector<i32> v;
// 	v.reserve((u64)n);
// 	std::random_device rng;
// 	while (n--)
// 		v.emplace_back(rng());

// 	sort(v);
// 	return 0;
// }