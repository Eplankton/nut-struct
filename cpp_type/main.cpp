#include "bits.h"
#include "timer.h"
using namespace nuts;

int main()
{
	auto print = [](const auto &x)
	{ std::cout << x; };

	set<i32> A = {7, 8, 0, 4, 2, 6, 9, 3, 5, 1};
	set<i32> B = {0, 2, 4, 6, 8, 10};
	for (i32 i = 0; i < 100; i++)
	{
		A.insert(i);
	}
	A.printBT();

	map<i32, bool> m;
	for (auto it = A.begin(); it != set<i32>::npos; it++)
	{
		m[*it] = B.contains(*it);
	}

	unordered_set<pair<i32, bool>> h;

	return 0;
}
