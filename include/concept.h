#ifndef _NUTS_CONCEPT_
#define _NUTS_CONCEPT_ 1

// Only for version after C++20

namespace nuts
{
	template <typename T>
	concept Container = requires(T x)
	{
		typename T::value_type;
		x.size();
		x.front();
		x.back();
		x.begin();
		x.end();
		x.clear();
		x.empty();
	};
}

#endif