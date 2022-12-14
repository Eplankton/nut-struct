#ifndef _NUTS_MEMO_
#define _NUTS_MEMO_

#include <cassert>
#include "type.h"

namespace nuts
{
	template <class T>
	struct default_delete
	{
		void operator()(T* _ptr) const
		{
			static_assert(sizeof(T),
			              "Can't delete an incomplete type!");
			delete _ptr;
		}
	};

	template <class T>
	struct default_delete<T[]>
	{
		void operator()(T* _ptr) const
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
		unique_ptr() = default;
		unique_ptr(pointer obj) : _ptr(obj) {}
		unique_ptr(const_pointer obj) : _ptr(const_cast<pointer>(obj)) {}
		unique_ptr(nuts::nullptr_t _p) : _ptr(_p) {}
		unique_ptr(unique_ptr<T, Dx>&& src) { move(src); }
		unique_ptr(unique_ptr<T, Dx>& src)
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
		}

		~unique_ptr()
		{
			deleter(_ptr);
			_ptr = nullptr;
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

		inline pointer operator->()
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		inline const_pointer operator->() const
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		inline pointer get()
		        const { return const_cast<pointer>(_ptr); }

		pointer release()
		{
			auto tmp = _ptr;
			_ptr = nullptr;
			return tmp;
		}

		unique_ptr& operator=(unique_ptr<T, Dx>&& src)
		// Only allow moveable rvalue
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
			return *this;
		}

		unique_ptr& operator=(T* src)
		{
			_ptr = src;
			return *this;
		}

		unique_ptr<T>& move(unique_ptr<T, Dx>& src)
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
			return *this;
		}

		inline bool operator==(unique_ptr<T, Dx>& obj)
		        const { return _ptr == obj._ptr; }

		inline bool operator==(const T* obj)
		        const { return _ptr == obj; }

		inline bool operator!=(unique_ptr<T, Dx>& obj)
		        const { return _ptr != obj._ptr; }

		inline bool operator!=(const_pointer obj)
		        const { return _ptr != obj; }

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

		unique_ptr() = default;
		unique_ptr(pointer src) : _ptr(src) {}
		unique_ptr(const_pointer src) : _ptr(const_cast<pointer>(src)) {}
		unique_ptr(unique_ptr<T, Dx>& src)
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
		}

		~unique_ptr()
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

		inline const T& operator*() const
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		inline const_pointer operator->() const
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		inline pointer get() const { return const_cast<pointer>(_ptr); }

		pointer release()
		{
			auto tmp = _ptr;
			_ptr = nullptr;
			return tmp;
		}

		unique_ptr& operator=(unique_ptr<T, Dx>& src)
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
			return *this;
		}

		unique_ptr& operator=(T* src)
		{
			_ptr = src;
			return *this;
		}

		unique_ptr<T>& move(unique_ptr<T, Dx>& src)
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
			return *this;
		}

		bool operator==(unique_ptr<T, Dx>& obj)
		        const { return _ptr == obj._ptr; }

		bool operator==(const T* obj)
		        const { return _ptr == obj; }

		bool operator!=(unique_ptr<T, Dx>& obj)
		        const { return _ptr != obj._ptr; }

		bool operator!=(const_pointer obj)
		        const { return _ptr != obj; }

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
		using u64_ptr = u64*;

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

		shared_ptr(nuts::nullptr_t _p) : _ptr(_p)
		{
			_cnt = new u64(0);
			(*_cnt)++;
		}

		shared_ptr(const shared_ptr<T, Dx>& src) : _ptr(src._ptr), _cnt(src._cnt)
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

		T* operator->() const
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		inline T* get() const { return _ptr; }

		bool operator==(shared_ptr<T, Dx>& obj)
		        const { return _ptr == obj._ptr; }

		bool operator==(const T* obj)
		        const { return _ptr == obj; }

		bool operator!=(shared_ptr<T, Dx>& obj)
		        const { return _ptr != obj._ptr; }

		bool operator!=(const T* obj)
		        const { return _ptr != obj; }

		shared_ptr<T, Dx>& operator=(shared_ptr<T, Dx>& src)
		{
			if (_ptr != nullptr) *(_cnt) -= 1;
			_ptr = src._ptr, _cnt = src._cnt;
			*(_cnt) += 1;
			return *this;
		}

	protected:
		pointer _ptr = nullptr;
		u64_ptr _cnt = nullptr;
		static constexpr Dx deleter {};
	};

	template <class T, class Dx>
	T* get_raw(const unique_ptr<T, Dx>& src)
	{
		return const_cast<T*>(src.get());
	}

	template <class T, class Dx>
	T* get_raw(const shared_ptr<T, Dx>& src)
	{
		return const_cast<T*>(src.get());
	}

	template <class T, class... Args, class Dx = default_delete<T>>
	unique_ptr<T, Dx> make_unique(const Args&... pac)
	{
		return unique_ptr<T, Dx> {new T(pac...)};
	}

	template <class T, class... Args, class Dx = default_delete<T>>
	shared_ptr<T, Dx> make_shared(const Args&... pac)
	{
		return shared_ptr<T, Dx> {new T(pac...)};
	}
}

#endif
