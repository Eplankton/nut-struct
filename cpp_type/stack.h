#ifndef _NUTS_STACK_
#define _NUTS_STACK_

#include "vector.h"
#include "list.h"

namespace nuts
{
	template <class T, class base = nuts::vector<T>>
	class stack
	{
	protected:
		base container;

	public:
		stack<T, base> &push(const T &obj);
		stack<T, base> &pop();
		stack<T, base> &clear();
		T &top();
		bool empty() { return container.empty(); }
		bool exist() { return container.exist(); }
		T &operator[](size_t N) { return this->container[N]; }
		size_t size() const { return container.size(); }

		T &front() { return container.front(); }
		T &back() { return container.back(); }

		const T &front() const { return container.front(); }
		const T &back() const { return container.back(); }

		stack<T, base> &operator=(stack<T, base> &obj);

		typename base::iterator begin();
		typename base::iterator end();
	};

	template <class T, class base>
	typename base::iterator stack<T, base>::begin()
	{
		return container.begin();
	}

	template <class T, class base>
	typename base::iterator stack<T, base>::end()
	{
		return container.end();
	}

	template <class T, class base>
	stack<T, base> &stack<T, base>::push(const T &obj)
	{
		container.push_back(obj);
		return *this;
	}

	template <class T, class base>
	stack<T, base> &stack<T, base>::pop()
	{
		assert(!container.empty());
		container.pop_back();
		return *this;
	}

	template <class T, class base>
	stack<T, base> &stack<T, base>::clear()
	{
		assert(!container.empty());
		container.clear();
		return *this;
	}

	template <class T, class base>
	T &stack<T, base>::top()
	{
		assert(!container.empty());
		return container.back();
	}

	template <class T, class base>
	stack<T, base> &stack<T, base>::operator=(stack<T, base> &obj)
	{
		container = obj.container;
		return *this;
	}
}
#endif
