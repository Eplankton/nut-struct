#ifndef _NUTS_MEMO_
#define _NUTS_MEMO_

#include <cassert>
#include "type.h"

namespace nuts
{
	template <class T>
	struct default_delete
	{
		void operator()(T *_ptr) const
		{
			static_assert(0 < sizeof(T),
			              "Can't delete an incomplete type!");
				delete _ptr;
		}
	};

	template <typename T, class Dx = default_delete<T>>
	class unique_ptr
	{
	protected:
		T *_ptr = nullptr;
		Dx deleter;

	public:
		unique_ptr() = default;
		unique_ptr(T *obj) : _ptr(obj) {}
		unique_ptr(unique_ptr<T, Dx>& src)
		{
			_ptr = src._ptr;
			src._ptr = nullptr;
		}
		~unique_ptr()
		{
			if (_ptr != nullptr)
				deleter(_ptr);
			_ptr = nullptr;
		}

		T& operator*()
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		const T& operator*() const
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		T *operator->()
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		const T *operator->() const
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		T *get() const { return _ptr; }
		T *release()
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

		unique_ptr& operator=(T *src)
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

		bool operator==(unique_ptr<T, Dx>& obj) const
		{
			return this->_ptr == obj._ptr;
		}

		bool operator==(const T *obj) const
		{
			return this->_ptr == obj;
		}

		bool operator!=(unique_ptr<T, Dx>& obj) const
		{
			return this->_ptr != obj._ptr;
		}

		bool operator!=(const T *obj) const
		{
			return this->_ptr != obj;
		}

		unique_ptr<T, Dx>& operator++()
		{
			_ptr++;
			return *this;
		}

		unique_ptr<T, Dx> operator++(int) const
		{
			unique_ptr<T, Dx> tmp(_ptr);
			_ptr++;
			return tmp;
		}

		unique_ptr<T, Dx>& operator--()
		{
			_ptr--;
			return *this;
		}

		unique_ptr<T, Dx> operator--(int) const
		{
			unique_ptr<T, Dx> tmp(_ptr);
			_ptr--;
			return tmp;
		}
	};

	template <typename T>
	class shared_ptr
	{
	protected:
		T *_ptr = nullptr;
		u64 *_cnt = nullptr;

	public:
		shared_ptr() = default;
		shared_ptr(T *obj) : _ptr(obj)
		{
			_cnt = new u64(0);
			(*_cnt)++;
		}
		shared_ptr(shared_ptr<T>& src) : _ptr(src._ptr), _cnt(src._cnt)
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
					delete _cnt, delete _ptr;
					_ptr = nullptr, _cnt = nullptr;
				}
			}
		}

		T& operator*()
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		const T& operator*() const
		{
			assert(_ptr != nullptr);
			return *_ptr;
		}

		T *operator->()
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		const T *operator->() const
		{
			assert(_ptr != nullptr);
			return _ptr;
		}

		T *get() const { return _ptr; }

		bool operator==(shared_ptr<T>& obj) const
		{
			return this->_ptr == obj._ptr;
		}

		bool operator==(const T *obj) const
		{
			return this->_ptr == obj;
		}

		bool operator!=(shared_ptr<T>& obj) const
		{
			return this->_ptr != obj._ptr;
		}

		bool operator!=(const T *obj) const
		{
			return this->_ptr != obj;
		}

		shared_ptr<T>& operator=(shared_ptr<T>& src)
		{
			if (_ptr != nullptr)
				*(_cnt) -= 1;
			_ptr = src._ptr, _cnt = src._cnt;
			*(_cnt) += 1;
			return *this;
		}

		shared_ptr<T>& operator++()
		{
			_ptr++;
			return *this;
		}

		shared_ptr<T> operator++(int) const
		{
			shared_ptr<T> tmp(_ptr);
			_ptr++;
			return tmp;
		}

		shared_ptr<T>& operator--()
		{
			_ptr--;
			return *this;
		}

		shared_ptr<T> operator--(int) const
		{
			shared_ptr<T> tmp(_ptr);
			_ptr--;
			return tmp;
		}
	};

	template <class T>
	T *get_raw(const unique_ptr<T>& src) { return src.get(); }

	template <class T>
	T *get_raw(const shared_ptr<T>& src) { return src.get(); }
}

#endif
