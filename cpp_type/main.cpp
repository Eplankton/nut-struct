#include <iostream>
#include <cmath>
#include "list.h"
#include "vector.h"
#include "basic_string.h"
#include "array.h"
#include "stack.h"
#include "queue.h"
#include "algorithm.h"
#include "memory.h"
#include "functional.h"
#include "hash_table.h"
#include "type.h"
#include "binary_tree.h"

using namespace nuts;

int main()
{
	string in = "pi";
	// std::cout << "\nPlease enter a name: ";
	// std::cin >> in;

	list<string> strList_A;
	strList_A
		.push_back("This is the world of Nutstruct!")
		.push_back("I'm learning to use Vim now!");

	strList_A.insert(strList_A.begin(), "Hello, world, my friends!");

	strList_A.push_front("My name is " + in + ":)")
		.print();

	array<string> c = {"Hello world! " + strList_A.front() + "\nI'm using Vim now and I feel great!!!\n"};

	queue<string> d; // Test for string and reverse() algorithm
	d.push(c[0]);
	d.push("f*ck you c++\n");
	reverse(d.begin(), d.end());
	reverse(d.back().begin(), d.back().end());
	std::cout << d.front() << d.back() << std::endl;

	list<i32> intList = {7, 8, 0, 4, 2, 6, 9, 3, 5, 1}; // Test for sort() algorithm
	std::cout << intList << std::endl;

	auto cmp = [](const auto &a,const auto &b) { return a < b; };
	insertion_sort(intList.begin(), intList.end(), cmp);
	binary_insertion_sort(intList);
	selection_sort(intList.begin(), intList.end());
	shell_sort(intList);
	quick_sort(intList);
	merge_sort(intList);
	heap_sort(intList);
	std::cout << intList << std::endl;

	hash_table<i32, i32> m; // Test for hash_table based on vector<list<pair<key, value>>>
	m.insert(3, 6);
	m.insert(1001, 7);
	m.insert(make_pair(6, 9));
	std::cout << m[3] << m[1001] << m[6] << std::endl;

	auto print = [](const auto &x) { std::cout << x; };
	AVL<int> bst = {7, 8, 0, 4, 2, 6, 9, 3, 5, 1};
	// for_each(bst.begin(), bst.end(), print);
	bst.in_trav(print);

	return 0;
}
