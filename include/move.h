#ifndef _NUTS_MOVE_
#define _NUTS_MOVE_

namespace nuts
{
	template <typename T>
	inline constexpr T&&
	move(T& x) noexcept
	{
		return static_cast<T&&>(x);
	}

	template <typename T>
	inline constexpr T&&
	move(T&& x) noexcept
	{
		return static_cast<T&&>(x);
	}
}

#endif