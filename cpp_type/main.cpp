#include "bits.h"
#include "timer.h"

using namespace nuts;

int main()
{
	// set<i32> A = {7, 8, 0, 4, 2, 6, 9, 3, 5, 1};
	set<i32> A;
	set<i32> B = {0, 2, 4, 6, 8, 10};
	for (i32 i = 0; i < 200; ++i)
		A.insert(i);
	// A.print();

	map<i32, bool> m;
	for_each(A.begin(), A.end(),
			 [&m, &B](const auto &x)
			 { m[x] = !B.contains(x); });
	// m.print_as_tree();

	unordered_map<i32, bool> hmap;
	auto cptr = [&hmap](const auto &x) { hmap.insert(x); };
	for_each(m.begin(), m.end(), cptr);
	// hmap.erase(4);
	// hmap[555] = 0;
	hmap.print();

	auto get_key_set = [](const unordered_map<i32, bool> &H)
		-> set<i32>
	{
		set<i32> res;
		for_each(H.begin(), H.end(), [&res](const auto &x)
				 { res.insert(x.first); });
		return res;
	};
	// get_key_set(hmap).print();

	return 0;
}
