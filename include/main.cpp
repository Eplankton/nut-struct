#include "bits.h"
#include "timer.h"

using namespace nuts;

int main()
{
	// set<i32> A {7, 8, 0, 4, 2, 6, 9, 3, 5, 1};
	set<i32> A;
	unordered_set<i32> B {1, 4, 7, 10, 13, 16, 19, 22, 25, 28};
	for (auto i: range(0, 113))
		A.insert((i32) i);
	// A.print();
	// B.print();

	map<i32, bool> m;
	for_each(A, [&m, &B](const auto& x) { m[x] = !B.contains(x); });
	// m.print();

	unordered_map<i32, bool> hmap;
	for_each(m, [&hmap](const auto& x) { hmap.insert(x); });
	// hmap.print_as_table();

	deque<string, 2> s {"human", "dog", "cat", "cow",
	                    "sheep", "monkey", "turtle", "elephant"};

	// reverse(s.begin(), s.end());
	// s.print_detail();

	return 0;
}
