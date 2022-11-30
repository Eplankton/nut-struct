#ifndef _NUTS_MOVE_
#define _NUTS_MOVE_ 1

namespace nuts
{
	template <typename T>
	inline T&& move(T& x)
	{
		return static_cast<T&&>(x);
	}
}

#endif