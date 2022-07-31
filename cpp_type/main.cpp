#include "bits.h"
#include "timer.h"

using namespace nuts;

int main()
{
	// set<i32> A = {7, 8, 0, 4, 2, 6, 9, 3, 5, 1};
	set<i32> A;
	set<i32> B = {1, 4, 7, 10, 13, 16, 19, 22, 25, 28};
	for (i32 i = 0; i < 100; ++i)
		A.insert(i);
	// A.print_as_tree();
	// B.print();

	map<i32, bool> m;
	for_each(A.begin(), A.end(),
	         [&m, &B](const auto& x) { m[x] = !B.contains(x); });

	// auto detect_scheme = [&m](const auto& it) {
	// 				if (it == m.npos)
	// 	        		return make_pair(Panic, (string)"Get npos iterator!");
	// 	            else
	// 					return make_pair(Success, (string)"Mooooo!"); };

	// auto opt = make_option(m.find(555), detect_scheme);

	// auto handle_scheme = [&opt, &m] {
	// 				if (opt.elem == m.npos) m.insert(555, 0);
	// 				else m[555] = 1;
	// 					return opt; };
	// opt.handle(handle_scheme);

	unordered_map<i32, bool> hmap;
	auto cptr = [&hmap](const auto& x) { hmap.insert(x); };
	for_each(m.begin(), m.end(), cptr);
	// hmap.print_as_table();

	// deque<string> s {"human", "dog", "cat", "cow",
	//                  "sheep", "monkey", "turtle",
	//                  "elephant"};
	// s.print();

	return 0;
}
