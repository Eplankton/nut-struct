#ifndef _NUTS_QUEUE_
#define _NUTS_QUEUE_ 1

#include "concept.h"
#include "deque.h"
#include "list.h"

/** @file queue
     * push() list as same as deque 
	 * pop() list as same as deque 
     */

namespace nuts
{
	template <typename Box>
	concept Queue_Base = Container<Box> &&
	        requires(Box x, typename Box::value_type val)
	{
		x.push_back(val);
		x.pop_front();
	};

	template <class T, Queue_Base C = deque<T>>
	class queue
	{
	public:
		using value_type = T;
		using itr_type = typename C::iterator;

		queue() = default;
		queue(const queue<T, C>& src) = default;
		queue(const std::initializer_list<T>& ilist);
		queue(queue<T, C>&& src) { move(src); }
		~queue() = default;

		queue<T, C>& push(const T& obj);
		queue<T, C>& push(T&& obj);
		queue<T, C>& pop();
		queue<T, C>& clear();
		bool empty() const { return impl.empty(); }
		u64 size() const { return impl.size(); }

		T& front() { return impl.front(); }
		T& back() { return impl.back(); }
		const T& front() const { return impl.front(); }
		const T& back() const { return impl.back(); }
		queue<T, C>& operator=(const queue<T, C>& obj);
		queue<T, C>& operator=(queue<T, C>&& src) { return move(src); }
		queue<T, C>& move(queue<T, C>& src);

		itr_type begin() const;
		itr_type end() const;

		void print() const;

	protected:
		C impl;
	};

	template <class T, Queue_Base C>
	queue<T, C>::queue(const std::initializer_list<T>& ilist)
	{
		C copy {ilist};
		impl.move(copy);
	}

	template <class T, Queue_Base C>
	typename C::iterator
	queue<T, C>::begin() const
	{
		return impl.begin();
	}

	template <class T, Queue_Base C>
	typename C::iterator
	queue<T, C>::end() const
	{
		return impl.end();
	}

	template <class T, Queue_Base C>
	queue<T, C>& queue<T, C>::push(const T& obj)
	{
		impl.push_back(obj);
		return *this;
	}

	template <class T, Queue_Base C>
	queue<T, C>& queue<T, C>::push(T&& obj)
	{
		impl.push_back(static_cast<T&&>(obj));
		return *this;
	}

	template <class T, Queue_Base C>
	queue<T, C>& queue<T, C>::pop()
	{
		if (!empty())
			impl.pop_front();
		return *this;
	}

	template <class T, Queue_Base C>
	queue<T, C>& queue<T, C>::clear()
	{
		impl.clear();
		return *this;
	}

	template <class T, Queue_Base C>
	queue<T, C>& queue<T, C>::operator=(const queue<T, C>& obj)
	{
		clear();
		impl = obj.impl;
		return *this;
	}

	template <class T, Queue_Base C>
	queue<T, C>& queue<T, C>::move(queue<T, C>& src)
	{
		clear();
		impl.move(src.impl);
		return *this;
	}

	template <class T, Queue_Base C>
	void queue<T, C>::print() const
	{
		auto print = [this](const auto& x) {
			std::cout << x;
			if (&x != &back()) printf(", ");
		};

		printf("queue @%#llx = [", (u64) impl.data());
		if (!empty()) for_each(impl, print);
		printf("]\n");
	}
}

#endif
