// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// #include "../doctest/doctest/doctest.h"
// #include "../nanobench/src/include/nanobench.h"
// #include "../include/bits.h"

// TEST_CASE("Complexity")
// {
// 	ankerl::nanobench::Bench bench;
// 	ankerl::nanobench::Rng rng;

// 	for (auto n: {
// 	             50U,
// 	             500U,
// 	             5000U,
// 	             50000U,
// 	             500000U,
// 	             5000000U,
// 	     }) {

// 		nuts::vector<uint64_t> a;
// 		a.reserve(n);

// 		while (a.size() < n) {
// 			a.emplace_back(rng());
// 		}

// 		bench.minEpochIterations(5)
// 		        .complexityN(a.size())
// 		        .run("Complexity", [&] {
// 			        nuts::sort(a.begin(), a.end());
// 			        // nuts::find(v, rng());
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

// 	while (a.size() < n) {
// 		auto tmp = rng();
// 		a.emplace_back(tmp);
// 		b.emplace_back(tmp);
// 	}

// 	bench.relative(true)
// 	        .run("a", [&] { nuts::sort(a.begin(), a.end()); })
// 	        .run("b", [&] { std::sort(b.begin(), b.end()); })
// 	        .relative(false);
// }

#include <bits/stdc++.h>
#include "../include/bits.h"

namespace nuts
{
	template <nuts::Invocable... Fn>
	void time_cmp(Fn&&... fn_list) noexcept
	{
		std::vector<std::future<double>> result;

		static const auto wrap_timer =
		        [&](auto&& fn) {
			        Timer clk;
			        fn();
			        return clk.elapsed();
		        };

		(..., [&](auto&& fn) {
			result.emplace_back(std::async(wrap_timer, fn));
		}(fn_list));

		for (auto& x: result) {
			println("Time count: ", x.get() * 1000.0, "(ms)");
		}
	}
}

int main()
{
	uint64_t n = 5e6;
	std::random_device rng;

	nuts::vector<uint64_t> a;
	std::vector<uint64_t> b;

	a.reserve(n);
	b.reserve(n);

	while (n--) {
		auto tmp = rng();
		a.emplace_back(tmp);
		b.emplace_back(tmp);
	}

	nuts::time_cmp(
	        [&] { nuts::sort(a.begin(), a.end()); },
	        [&] { std::sort(b.begin(), b.end()); });

	assert(nuts::is_sorted(a));

	return 0;
}