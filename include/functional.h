#ifndef _NUTS_FUNC_
#define _NUTS_FUNC_

#include "type.h"
#include "concept.h"
#include <cstring>

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
		function(const function<FuncType>& src) = default;
		~function() { fn = nullptr; }
		func_pointer get() const { return fn; }

		template <typename... _ArgTypes>
		auto operator()(_ArgTypes&&... args)
		{
			return fn(args...);
		}

		template <typename... _ArgTypes>
		auto operator()(_ArgTypes&&... args) const
		{
			return fn(args...);
		}
	};

	template <typename FuncType>
	auto make_function(const FuncType& F)
	{
		return function<FuncType>(F);
	}

	template <typename T = void>
	struct less
	{
		inline bool constexpr
		operator()(const auto& x, const auto& y) const
		    requires Less<decltype(x), decltype(y)>
		{
			return x < y;
		}
	};

	template <typename T = void>
	struct greater
	{
		inline bool constexpr
		operator()(const auto& x, const auto& y) const
		    requires Greater<decltype(x), decltype(y)>
		{
			return x > y;
		}
	};

	template <typename T = void>
	struct equal
	{
		inline bool constexpr
		operator()(const auto& x, const auto& y) const
		    requires Equal<decltype(x), decltype(y)>
		{
			return x == y;
		}
	};

	template <typename Box,
	          typename R = less<>>
	concept Sortable = Iterable<Box> &&
	requires(typename Box::value_type x, R cmp)
	{
		{cmp(x, x)} -> Same<bool>;
	};

	/* Hasher */
	template <typename x>// Abstract hash function
	struct hash;

	// Full specialization of hash function
	template <>
	struct hash<u32>
	{
		constexpr u64 operator()(u32 x) const noexcept
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
		constexpr u64 operator()(u64 x) const noexcept
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
		constexpr u64 operator()(i32 x) const noexcept
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
		constexpr u64 operator()(i64 x) const noexcept
		{
			x = x & 0x7fffffffffffffff;
			x = (x ^ (x >> 30)) * static_cast<u64>(0xbf58476d1ce4e5b9);
			x = (x ^ (x >> 27)) * static_cast<u64>(0x94d049bb133111eb);
			x = x ^ (x >> 31);
			return x;
		}
	};

	template <>
	struct hash<const char*>
	{
		u64 operator()(const char* s) const
		{
			auto len = strlen(s);
			u64 cnt = 0;

			for (u64 i = 0; i < len; i++) {
				cnt += (u64) s[i] & 0xbf5847;
			}

			constexpr auto hasher = [](u64 x) -> u64 {
				x = (x ^ (x >> 30)) * u64(0xbf58476d1ce4e5b9);
				x = (x ^ (x >> 27)) * u64(0x94d049bb133112eb);
				x = x ^ (x >> 31);
				return x;
			};

			return hasher(cnt);
		}
	};

	template <typename T>
	concept Hashable = requires {
		                   hash<T>();
	                   };
}

#endif
