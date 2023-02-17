#ifndef _NUTS_BITS_
#define _NUTS_BITS_

#include <iostream>

#include "algorithm.h"
#include "concept.h"
#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "move.h"
#include "option.h"
#include "range.h"
#include "type.h"
#include "utility.h"
#include "bitset.h"

#include "array.h"
#include "basic_string.h"
#include "deque.h"
#include "list.h"
#include "matrix.h"
#include "queue.h"
#include "stack.h"
#include "vector.h"

#include "binary_tree.h"
#include "map.h"
#include "set.h"

#include "unordered_map.h"
#include "unordered_set.h"

#include "timer.h"

//	string in = "pi";
//	// std::cout << "\nPlease enter a name: ";
//	// std::cin >> in;
//
//	list<string> strList_A;
//	strList_A
//			.push_back("This is the world of Nutstruct!")
//			.push_back("I'm learning to use Vim now!");
//
//	strList_A.insert(strList_A.begin(), "Hello, world, my friends!");
//
//	strList_A.push_front("My name is " + in + ":)")
//	         .print();
//
//	array<string> c = {"Hello world! " + strList_A.front() + "I'm using Vim now and I feel great!!!"};
//
//	queue<string> d; // Test for string and reverse() algorithm
//	d.push(c[0]);
//	d.push("f*ck you c++\n");
//	reverse(d.begin(), d.end());
//	reverse(d.back().begin(), d.back().end());
//	std::cout << d.front() << d.back() << std::endl;
//
//	list<i32> intList = {7, 8, 0, 4, 2, 6, 9, 3, 5, 1}; // Test for sort() algorithm
//	std::cout << intList << std::endl;
//
//	auto cmp = [](const auto &a, const auto &b) { return a < b; };
//	quick_sort(intList);
//	std::cout << intList << std::endl;
//
//	hash_table<i32, i32> m; // Test for hash_table based on vector<list<pair<key, value>>>
//	m.insert(3, 6);
//	m.insert(1001, 7);
//	m.insert(make_pair(6, 9));
//	std::cout << m[3] << m[1001] << m[6] << std::endl;

// nuts::stack<int> a;
// std::stack<int> b;

// for (int n = 1; n <= 10000; n++)
// {
// 	Timer ca;
// 	for (auto i: range(0, n))
// 		a.push(i);
// 	double ra = ca.elapsed() * 1000.0;

// 	Timer cb;
// 	for (auto i: range(0, n))
// 		b.push(i);
// 	double rb = cb.elapsed() * 1000.0;

// 	std::cout << "[N = " << n << "] " << rb / ra << '\n';
// }

// #include "bits.h"
// #include "timer.h"

// #include <thread>

// using namespace nuts;

// int main()
// {
// 	// set<i32> A {7, 8, 0, 4, 2, 6, 9, 3, 5, 1};
// 	set<i32> A;
// 	hash_set<i32> B {1, 4, 7, 10, 13, 16, 19, 22, 25, 28};
// 	for (auto i: range(0, 10))
// 		A.insert((i32) i);
// 	// A.print();
// 	// B.print();

// 	map<i32, bool> m;
// 	for_each(A, [&m, &B](const auto& x) { m[x] = !B.contains(x); });
// 	// m.print();

// 	hash_map<i32, bool> hmap;
// 	for_each(m, [&hmap](const auto& x) { hmap.insert(x); });
// 	// hmap.print_as_table();

// 	deque<string> s {"human", "dog", "cat", "cow",
// 	                 "sheep", "monkey", "turtle", "elephant"};

// 	// reverse(s.begin(), s.end());
// 	// s.print_detail();

// 	auto foo = [&](Iterable auto& x) {
// 		auto st1 = x.begin(),
// 		     ed2 = x.end(),
// 		     ed1 = advance(st1, distance(st1, ed2) / 2),
// 		     st2 = advance(ed1, 1);

// 		vector<std::thread> p;
// 		p.push_back(std::thread {[&] { insertion_sort(st1, ed1); }});
// 		p.push_back(std::thread {[&] { insertion_sort(st2, ed2); }});
// 		for (auto& i: range(p)) i.join();
// 		return x;
// 	};

// 	vector<i32> v {5, 1, 0, 9, 7, 3, 6, 4, 2};
// 	println(foo(v), foo(s), "\n");

// 	bitset<42> a {"1101101010111011101010111011111001101111"};
// 	a.dump();

// 	return 0;
// }

#endif