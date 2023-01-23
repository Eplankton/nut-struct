#ifndef _NUTS_TYPE_
#define _NUTS_TYPE_

// Primitive data types

#include <cstdint>
#include <cmath>

namespace nuts
{
	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = signed long long;

	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = unsigned long long;

	using f32 = float;
	using f64 = double;

	using usize = uintptr_t;
	using isize = intptr_t;

	using f32 = float;
	using f64 = double;

	using nullptr_t = decltype(nullptr);
}

#endif
