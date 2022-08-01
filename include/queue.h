#ifndef _NUTS_QUEUE_
#define _NUTS_QUEUE_ 1

#include "deque.h"
#include "list.h"
#include "type.h"

/** @file queue
     * push() list as same as deque 
	 * pop() list as same as deque 
     */

namespace nuts
{
	template <class T, class base = deque<T>>
	class queue
	{
	public:
		using Value_type = T;
		using Itr_type = typename base::iterator;

	public:
		queue() = default;
		queue(const std::initializer_list<T>& ilist);
		queue(const queue<T, base>& src) { container = src.container; }
		queue(queue<T, base>&& src) { this->move(src); }
		~queue() = default;

		queue<T, base>& push(const T& obj);
		queue<T, base>& pop();
		queue<T, base>& clear();
		bool empty() const { return container.empty(); }
		u64 size() const { return container.size(); }

		T& front() { return container.front(); }
		T& back() { return container.back(); }
		const T& front() const { return container.front(); }
		const T& back() const { return container.back(); }
		queue<T, base>& operator=(const queue<T, base>& obj);
		queue<T, base>& operator=(queue<T, base>&& src) { return this->move(src); }
		queue<T, base>& move(queue<T, base>& src);

		Itr_type begin();
		Itr_type end();

		Itr_type begin() const;
		Itr_type end() const;

		void print() const;

	protected:
		base container;
	};

	template <class T, class base>
	queue<T, base>::queue(const std::initializer_list<T>& ilist)
	{
		base copy(ilist);
		this->container.move(copy);
	}

	template <class T, class base>
	typename base::iterator queue<T, base>::begin()
	{
		return container.begin();
	}

	template <class T, class base>
	typename base::iterator queue<T, base>::end()
	{
		return container.end();
	}

	template <class T, class base>
	typename base::iterator queue<T, base>::begin() const
	{
		return container.begin();
	}

	template <class T, class base>
	typename base::iterator queue<T, base>::end() const
	{
		return container.end();
	}

	template <class T, class base>
	queue<T, base>& queue<T, base>::push(const T& obj)
	{
		container.push_back(obj);
		return *this;
	}

	template <class T, class base>
	queue<T, base>& queue<T, base>::pop()
	{
		if (!empty())
			container.pop_front();
		return *this;
	}

	template <class T, class base>
	queue<T, base>& queue<T, base>::clear()
	{
		container.clear();
		return *this;
	}

	template <class T, class base>
	queue<T, base>& queue<T, base>::operator=(const queue<T, base>& obj)
	{
		this->clear();
		container = obj.container;
		return *this;
	}

	template <class T, class base>
	queue<T, base>& queue<T, base>::move(queue<T, base>& src)
	{
		this->clear();
		container.move(src.container);
		return *this;
	}

	template <class T, class base>
	void queue<T, base>::print() const
	{
		auto print = [this](const auto& x) {
			std::cout << x;
			if (&x != &this->back())
				printf(", ");
		};

		printf("\nqueue @%#llx = [", (u64) container.data());
		if (!empty())
			for_each(begin(), end(), print);
		printf("]\n");
	}
}

#endif
