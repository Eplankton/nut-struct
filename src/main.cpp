// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

// #include "../doctest/doctest/doctest.h"
// #include "../nanobench/src/include/nanobench.h"
// #include "../include/bits.h"

// TEST_CASE("Complexity")
// {
// 	ankerl::nanobench::Bench bench;
// 	ankerl::nanobench::Rng rng;

// 	for (auto n: {
// 	             10U,
// 	             100U,
// 	             1000U,
// 	             10000U,
// 	             100000U,
// 	             1000000U,
// 	     }) {

// 		nuts::vector<uint64_t> a;
// 		a.reserve(n);

// 		while (a.size() < n) {
// 			a.emplace_back(rng());
// 		}

// 		bench.minEpochIterations(10)
// 		        .complexityN(a.size())
// 		        .run("Complexity", [&] {
// 			        nuts::sort(a.begin(), a.end());
// 		        });
// 	}

// 	// calculate BigO complexity best fit and print the results
// 	std::cout << bench.complexityBigO() << std::endl;
// }

// TEST_CASE("Compare with STL")
// {
// 	ankerl::nanobench::Bench bench;
// 	ankerl::nanobench::Rng rng;

// 	nuts::u64 n = 1e5;
// 	nuts::vector<uint64_t> a;
// 	std::vector<uint64_t> b;
// 	a.reserve(n);
// 	b.reserve(n);

// 	while (n-- > 0) {
// 		a.emplace_back(rng());
// 		b.emplace_back(a.back());
// 	}

// 	bench.relative(true)
// 	        .minEpochIterations(5)
// 	        .run("nuts::sort", [&] { nuts::sort(a.begin(), a.end()); })
// 	        .run("std::sort", [&] { std::sort(b.begin(), b.end()); });
// }

#include <bits/stdc++.h>
#include "../include/bits.h"

namespace nuts
{
	template <nuts::Invocable... Fn>
	void time_cmp(Fn&&... fn) noexcept
	{
		std::vector<std::future<double>> results;

		static const auto wrap_timer =
		        [&](auto&& f) {
			        Timer clk;
			        f();
			        return clk.elapsed();
		        };

		(..., [&](auto&& f) {
			results.emplace_back(std::async(wrap_timer, f));
		}(fn));

		for (auto&& x: results) {
			println("Time count: ", x.get() * 1000.0, "(ms)");
		}
	}
}

int main()
{
	uint64_t n = 1e7;
	std::random_device rng;

	nuts::vector<uint64_t> a;
	std::vector<uint64_t> b;

	a.reserve(n);
	b.reserve(n);

	while (n--) {
		a.emplace_back(rng());
		b.emplace_back(a.back());
	}

	nuts::time_cmp(
	        [&] { nuts::sort(a.begin(), a.end()); },
	        [&] { std::sort(b.begin(), b.end()); });

	assert(nuts::is_sorted(a));

	return 0;
}