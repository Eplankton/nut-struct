#ifndef _NUTS_QUEUE_
#define _NUTS_QUEUE_ 1

#include "type.h"
#include "list.h"
#include "deque.h"

namespace nuts
{
	template <class T, class base = nuts::deque<T>>
	class queue
	{
		using itr_type = typename base::iterator;

	protected:
		base container;

	public:
		queue() = default;
		queue(const std::initializer_list<T> &ilist);
		queue(const queue<T, base> &src)
		{
			container = src.container;
		}
		~queue() = default;

		queue<T, base> &push(const T &obj);
		queue<T, base> &pop();
		queue<T, base> &clear();
		bool empty() const { return container.empty(); }
		u64 size() const { return container.size(); }

		T &front() { return container.front(); }
		T &back() { return container.back(); }
		const T &front() const { return container.front(); }
		const T &back() const { return container.back(); }
		queue<T, base> &operator=(queue<T, base> &obj);

		itr_type begin();
		itr_type end();

		itr_type begin() const;
		itr_type end() const;

		void print() const;
	};

	template <class T, class base>
	queue<T, base>::queue(const std::initializer_list<T> &ilist)
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
	queue<T, base> &queue<T, base>::push(const T &obj)
	{
		container.push_back(obj);
		return *this;
	}

	template <class T, class base>
	queue<T, base> &queue<T, base>::pop()
	{
		if (!empty())
			container.pop_front();
		return *this;
	}

	template <class T, class base>
	queue<T, base> &queue<T, base>::clear()
	{
		container.clear();
		return *this;
	}

	template <class T, class base>
	queue<T, base> &queue<T, base>::operator=(queue<T, base> &obj)
	{
		this->clear();
		container = obj.container;
		return *this;
	}

	template <class T, class base>
	void queue<T, base>::print() const
	{
		auto print = [this](const auto &x)
		{
			std::cout << x;
			if (&x != &this->back())
				printf(", ");
		};

		printf("\nqueue @%#llx = [", (u64)container.data());
		if (!empty())
			for_each(begin(), end(), print);
		printf("]\n");
	}

}
#endif
