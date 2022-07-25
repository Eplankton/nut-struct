#ifndef _NUTS_QUEUE_
#define _NUTS_QUEUE_ 1

#include "list.h"
//#include "deque.h"

namespace nuts
{
	template <class T, class base = nuts::list<T>>
	class queue
	{
	protected:
		base container;

	public:
		queue<T, base>& push(const T& obj);
		queue<T, base>& pop();
		queue<T, base>& clear();
		bool empty() const { return container.empty(); }
		bool exist() const { return container.exist(); }
		unsigned int size() const { return container.size(); }

		T& front() { return *begin(); }
		T& back() { return *end(); }

		const T& front() const { return container.front(); }
		const T& back() const { return container.back(); }

		queue<T, base>& operator=(queue<T, base>& obj);

		typename base::iterator begin();
		typename base::iterator end();
	};

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
	queue<T, base>& queue<T, base>::push(const T& obj)
	{
		container.push_back(obj);
		return *this;
	}

	template <class T, class base>
	queue<T, base>& queue<T, base>::pop()
	{
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
	queue<T, base>& queue<T, base>::operator=(queue<T, base>& obj)
	{
		container = obj.container;
		return *this;
	}

	// template <class T, class base>

}
#endif
