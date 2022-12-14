#ifndef _NUTS_TYPE_
#define _NUTS_TYPE_

// Primitive data types

namespace nuts
{
	using i8 = signed char;
	using i16 = signed short;
	using i32 = signed int;
	using i64 = signed long long;

	using u8 = unsigned char;
	using u16 = unsigned short;
	using u32 = unsigned int;
	using u64 = unsigned long long;

	using f32 = float;
	using f64 = double;

	using nullptr_t = decltype(nullptr);
}

#endif
