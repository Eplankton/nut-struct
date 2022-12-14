#ifndef _NUTS_MOVE_
#define _NUTS_MOVE_

namespace nuts
{
	template <typename T>
	inline T&& move(T& x)
	{
		return static_cast<T&&>(x);
	}
}

#endif