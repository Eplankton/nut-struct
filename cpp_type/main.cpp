#include "bits.h"
#include "timer.h"

using namespace nuts;

int main()
{
	// set<i32> A = {7, 8, 0, 4, 2, 6, 9, 3, 5, 1};
	set<i32> A;
	set<i32> B = {1, 4, 7, 10, 13, 16, 19, 22, 25, 28};
	for (i32 i = 0; i < 1000; ++i)
		A.insert(i);
	// A.print_as_tree();
	B.print();

	map<i32, bool> m;
	for_each(A.begin(), A.end(),
	         [&m, &B](const auto& x) { m[x] = !B.contains(x); });
	// m.print();

	unordered_map<i32, bool> hmap;
	auto cptr = [&hmap](const auto& x) { hmap.insert(x); };
	for_each(m.begin(), m.end(), cptr);

	// hmap.erase(4);
	// hmap[555] = 0;
	// hmap.print_as_table();

	// auto get_key_set = [](const unordered_map<i32, bool> &H)
	// 	-> set<i32>
	// {
	// 	set<i32> res;
	// 	for_each(H.begin(), H.end(), [&res](const auto &x)
	// 			 { res.insert(x.first); });
	// 	return res;
	// };
	// get_key_set(hmap).print();

	queue<string> s = {"human", "dog", "cat", "cow",
	                   "sheep", "monkey", "turtle",
	                   "elephant"};
	s.print();

	return 0;
}
