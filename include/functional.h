#ifndef _NUTS_FUNC_
#define _NUTS_FUNC_ 1

#include "type.h"
#include "utility.h"

namespace nuts
{
	template <typename FuncType>
	class function
	{
	public:
		using func_pointer = FuncType*;

	protected:
		func_pointer fn = nullptr;

	public:
		function() = default;
		function(const func_pointer R) : fn(static_cast<func_pointer>(R)) {}
		function(const function<FuncType>& OBJ) : fn(OBJ.fn) {}
		~function() { fn = nullptr; }

		template <typename... _ArgTypes>
		auto operator()(_ArgTypes... args)
		{
			return fn(args...);
		}
	};

	template <typename FuncType>
	auto make_function(const FuncType& F)
	{
		return nuts::function(F);
	}

	template <typename T>
	struct less
	{
		bool operator()(const T& a, const T& b)
		        const { return a < b; }
	};

	template <typename T>
	struct greater
	{
		bool operator()(const T& a, const T& b)
		        const { return a > b; }
	};

	template <typename T>
	struct equal
	{
		bool operator()(const T& a, const T& b)
		        const { return a == b; }
	};

	/* HASHER */

	template <typename x>// Abstract hash function
	struct hash;

	// Full specialization of hash function
	template <>
	struct hash<u32>
	{
		u64 operator()(u32 x) const
		{
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = (x >> 16) ^ x;
			return x;
		}
	};

	template <>
	struct hash<u64>
	{
		u64 operator()(u64 x) const
		{
			x = (x ^ (x >> 30)) * static_cast<u64>(0xbf58476d1ce4e5b9);
			x = (x ^ (x >> 27)) * static_cast<u64>(0x94d049bb133111eb);
			x = x ^ (x >> 31);
			return x;
		}
	};

	template <>
	struct hash<i32>
	{
		u64 operator()(i32 x) const
		{
			// x = x & 0x7fffffff;
			// // x = ((x >> 15) ^ x) * 0x45d9f3b;
			// x = ((x >> 15) ^ x) * 0x45d3f3b;
			// x ^= (x << 6);
			// return x;

			// x = x & 0x7fffffff;
			// x = ((x >> 16) ^ x) * 0x45d9f3b;
			// x = ((x >> 16) ^ x) * 0x45d9f3b;
			// x = (x >> 16) ^ x;
			// return x;

			return static_cast<u64>(x);
		}
	};

	template <>
	struct hash<i64>
	{
		u64 operator()(i64 x) const
		{
			x = x & 0x7fffffffffffffff;
			x = (x ^ (x >> 30)) * static_cast<u64>(0xbf58476d1ce4e5b9);
			x = (x ^ (x >> 27)) * static_cast<u64>(0x94d049bb133111eb);
			x = x ^ (x >> 31);
			return x;
		}
	};
}// namespace nuts

#endif
