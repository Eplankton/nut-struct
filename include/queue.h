#ifndef _NUTS_QUEUE_
#define _NUTS_QUEUE_ 1

#include "concept.h"
#include "deque.h"
#include "list.h"
#include "type.h"

/** @file queue
     * push() list as same as deque 
	 * pop() list as same as deque 
     */

namespace nuts
{
	template <typename T>
	concept Queue_Base = Container<T> &&
	        requires(T x, typename T::value_type val)
	{
		x.push_back(val);
		x.pop_front();
	};

	template <class T, Queue_Base base = deque<T>>
	class queue
	{
	public:
		using value_type = T;
		using itr_type = typename base::iterator;

	public:
		queue() = default;
		queue(const std::initializer_list<T>& ilist);
		queue(const queue<T, base>& src) { _Base = src._Base; }
		queue(queue<T, base>&& src) { this->move(src); }
		~queue() = default;

		queue<T, base>& push(const T& obj);
		queue<T, base>& pop();
		queue<T, base>& clear();
		bool empty() const { return _Base.empty(); }
		u64 size() const { return _Base.size(); }

		T& front() { return _Base.front(); }
		T& back() { return _Base.back(); }
		const T& front() const { return _Base.front(); }
		const T& back() const { return _Base.back(); }
		queue<T, base>& operator=(const queue<T, base>& obj);
		queue<T, base>& operator=(queue<T, base>&& src) { return this->move(src); }
		queue<T, base>& move(queue<T, base>& src);

		itr_type begin() const;
		itr_type end() const;

		void print() const;

	protected:
		base _Base;
	};

	template <class T, Queue_Base base>
	queue<T, base>::queue(const std::initializer_list<T>& ilist)
	{
		base copy{ilist};
		this->_Base.move(copy);
	}

	template <class T, Queue_Base base>
	typename base::iterator queue<T, base>::begin() const
	{
		return _Base.begin();
	}

	template <class T, Queue_Base base>
	typename base::iterator queue<T, base>::end() const
	{
		return _Base.end();
	}

	template <class T, Queue_Base base>
	queue<T, base>& queue<T, base>::push(const T& obj)
	{
		_Base.push_back(obj);
		return *this;
	}

	template <class T, Queue_Base base>
	queue<T, base>& queue<T, base>::pop()
	{
		if (!empty())
			_Base.pop_front();
		return *this;
	}

	template <class T, Queue_Base base>
	queue<T, base>& queue<T, base>::clear()
	{
		_Base.clear();
		return *this;
	}

	template <class T, Queue_Base base>
	queue<T, base>& queue<T, base>::operator=(const queue<T, base>& obj)
	{
		this->clear();
		_Base = obj._Base;
		return *this;
	}

	template <class T, Queue_Base base>
	queue<T, base>& queue<T, base>::move(queue<T, base>& src)
	{
		this->clear();
		_Base.move(src._Base);
		return *this;
	}

	template <class T, Queue_Base base>
	void queue<T, base>::print() const
	{
		auto print = [this](const auto& x) {
			std::cout << x;
			if (&x != &this->back())
				printf(", ");
		};

		printf("\nqueue @%#llx = [", (u64) _Base.data());
		if (!empty())
			for_each(_Base, print);
		printf("]\n");
	}
}

#endif
