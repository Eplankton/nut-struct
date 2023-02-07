#ifndef _NUTS_MEMO_
#define _NUTS_MEMO_

#include <cassert>
#include <atomic>
#include "type.h"

namespace nuts
{
	template <class T>
	struct default_delete
	{
		inline void operator()(T* _ptr) const
		{
			static_assert(sizeof(T),
			              "Can't delete an incomplete type!");
			delete _ptr;
		}
	};

	template <class T>
	struct default_delete<T[]>
	{
		inline void operator()(T* _ptr) const
		{
			static_assert(sizeof(T),
			              "Can't delete an incomplete type!");
			delete[] _ptr;
		}
	};

	template <typename T, class Dx = default_delete<T>>
	class unique_ptr
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;

	public:
		unique_ptr() noexcept = default;
		unique_ptr(pointer obj) noexcept : _ptr(obj) {}
		unique_ptr(const_pointer obj) noexcept : _ptr(const_cast<pointer>(obj)) {}
		unique_ptr(nuts::nullptr_t _p) noexcept : _ptr(_p) {}
		unique_ptr(unique_ptr<T, Dx>&& src) noexcept { move(src); }
		unique_ptr(unique_ptr<T, Dx>& src)
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
		}

		~unique_ptr() noexcept
		{
			deleter(_ptr);
			_ptr = nullptr;
		}

		T& operator*()
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		T& operator*() const
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		constexpr pointer operator->()
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		constexpr const_pointer operator->() const
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		constexpr pointer get() const
		        noexcept { return const_cast<pointer>(_ptr); }

		inline pointer release() noexcept
		{
			auto tmp = _ptr;
			_ptr = nullptr;
			return tmp;
		}

		inline unique_ptr& operator=(unique_ptr<T, Dx>&& src) noexcept
		// Only allow moveable rvalue
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
			return *this;
		}

		inline unique_ptr& operator=(T* src) noexcept
		{
			_ptr = src;
			return *this;
		}

		unique_ptr<T>& move(unique_ptr<T, Dx>& src) noexcept
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
			return *this;
		}

		inline bool operator==(unique_ptr<T, Dx>& obj)
		        const noexcept { return _ptr == obj._ptr; }

		inline bool operator==(const T* obj)
		        const noexcept { return _ptr == obj; }

		inline bool operator!=(unique_ptr<T, Dx>& obj)
		        const noexcept { return _ptr != obj._ptr; }

		inline bool operator!=(const_pointer obj)
		        const noexcept { return _ptr != obj; }

	protected:
		pointer _ptr = nullptr;
		static constexpr Dx deleter {};
	};

	template <class T, class Dx>
	class unique_ptr<T[], Dx>
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;

		unique_ptr() noexcept = default;
		unique_ptr(pointer src) noexcept : _ptr(src) {}
		unique_ptr(const_pointer src) noexcept
		    : _ptr(const_cast<pointer>(src)) {}
		unique_ptr(unique_ptr<T, Dx>& src) noexcept
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
		}

		~unique_ptr() noexcept
		{
			deleter(_ptr);
			_ptr = nullptr;
		}

		T& operator[](u64 _n)
		{
			assert(_ptr != nullptr);
			return _ptr[_n];
		}

		T& operator[](u64 _n) const
		{
			assert(_ptr != nullptr);
			return _ptr[_n];
		}

		inline T& operator*()
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		const T& operator*() const
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		constexpr const_pointer operator->() const
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		constexpr pointer get()
		        const { return const_cast<pointer>(_ptr); }

		inline pointer release() noexcept
		{
			auto tmp = _ptr;
			_ptr = nullptr;
			return tmp;
		}

		inline unique_ptr& operator=(unique_ptr<T, Dx>& src) noexcept
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
			return *this;
		}

		inline unique_ptr& operator=(T* src) noexcept
		{
			_ptr = src;
			return *this;
		}

		inline unique_ptr<T>& move(unique_ptr<T, Dx>& src) noexcept
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
			return *this;
		}

		bool operator==(unique_ptr<T, Dx>& obj)
		        const noexcept { return _ptr == obj._ptr; }

		bool operator==(const T* obj)
		        const noexcept { return _ptr == obj; }

		bool operator!=(unique_ptr<T, Dx>& obj)
		        const noexcept { return _ptr != obj._ptr; }

		bool operator!=(const_pointer obj)
		        const noexcept { return _ptr != obj; }

	protected:
		pointer _ptr = nullptr;
		static constexpr default_delete<T[]> deleter {};
	};

	template <typename T, class Dx = default_delete<T>>
	class shared_ptr
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using cnt_type = std::atomic<u64*>;

		template <class... Args>
		struct merge_type
		{
			T obj;
			cnt_type cnt;
			merge_type(const Args&... pkg) : obj(pkg...) {}
		};

	public:
		shared_ptr() = default;
		shared_ptr(pointer obj) : _ptr(obj)
		{
			_cnt = new u64(0);
			(*_cnt)++;
		}

		shared_ptr(const_pointer obj) : _ptr(const_cast<pointer>(obj))
		{
			_cnt = new u64(0);
			(*_cnt)++;
		}

		shared_ptr(const shared_ptr<T, Dx>& src)
		    : _ptr(src._ptr), _cnt(src._cnt._M_b)
		{
			if (_cnt != nullptr)
				(*_cnt)++;
		}

		~shared_ptr()
		{
			if (_cnt != nullptr)
			{
				if (*_cnt > 1)
				{
					*(_cnt) -= 1;
					_ptr = nullptr, _cnt = nullptr;
				}
				else
				{
					delete _cnt;
					deleter(_ptr);
					_ptr = nullptr, _cnt = nullptr;
				}
			}
		}

		inline T& operator*()
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		inline const T& operator*() const
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		constexpr T* operator->() const
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		inline T* get() const noexcept { return _ptr; }

		inline bool operator==(shared_ptr<T, Dx>& obj)
		        const noexcept { return _ptr == obj._ptr; }

		inline bool operator==(const T* obj)
		        const noexcept { return _ptr == obj; }

		inline bool operator!=(shared_ptr<T, Dx>& obj)
		        const noexcept { return _ptr != obj._ptr; }

		inline bool operator!=(const T* obj)
		        const noexcept { return _ptr != obj; }

		shared_ptr<T, Dx>& operator=(shared_ptr<T, Dx>& src)
		{
			if (_ptr != nullptr) *(_cnt) -= 1;
			_ptr = src._ptr, _cnt = src._cnt;
			*(_cnt) += 1;
			return *this;
		}

	protected:
		pointer _ptr = nullptr;
		cnt_type _cnt = nullptr;
		static constexpr Dx deleter {};
	};

	template <class T, class Dx>
	inline T* get_raw(const unique_ptr<T, Dx>& src)
	{
		return const_cast<T*>(src.get());
	}

	template <class T, class Dx>
	inline T* get_raw(const shared_ptr<T, Dx>& src)
	{
		return const_cast<T*>(src.get());
	}

	template <class T, class... Args, class Dx = default_delete<T>>
	unique_ptr<T, Dx> make_unique(Args&&... pac)
	{
		return {new T(pac...)};
	}

	template <class T, class... Args, class Dx = default_delete<T>>
	shared_ptr<T, Dx> make_shared(Args&&... pac)
	{
		return {new T(pac...)};
	}

	template <typename T>
	struct Box
	{
		template <typename... Args>
		inline static auto make(Args&&... pac)
		{
			return make_unique<T>(pac...);
		}
	};
}

#endif
