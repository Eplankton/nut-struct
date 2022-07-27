#ifndef _NUTS_STACK_
#define _NUTS_STACK_ 1

#include "deque.h"
#include "type.h"
#include "vector.h"

/** @file stack
     * push() vector as same as deque 
	 * pop() vector usually three times faster than deque
     */

namespace nuts
{
	template <class T, class base = vector<T>>
	class stack
	{
	public:
		using value_type = T;
		using itr_type = typename base::iterator;

	protected:
		base container;

	public:
		stack() = default;
		stack(const std::initializer_list<T>& ilist);
		stack(const stack<T, base>& src)
		{
			container = src.container;
		};
		~stack() = default;

		stack<T, base>& push(const T& obj);
		stack<T, base>& pop();
		stack<T, base>& clear();

		T& top();
		const T& top() const;

		bool empty() const { return container.empty(); }
		u64 size() const { return container.size(); }
		T& front() { return container.front(); }
		T& back() { return container.back(); }
		const T& front() const { return container.front(); }
		const T& back() const { return container.back(); }
		stack<T, base>& operator=(stack<T, base>& obj);

		itr_type begin();
		itr_type end();

		itr_type begin() const;
		itr_type end() const;

		void print() const;
	};

	template <class T, class base>
	stack<T, base>::stack(const std::initializer_list<T>& ilist)
	{
		base copy(ilist);
		this->container.move(copy);
	}

	template <class T, class base>
	stack<T, base>& stack<T, base>::operator=(stack<T, base>& obj)
	{
		this->clear();
		container = obj.container;
		return *this;
	}

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
	typename base::iterator
	stack<T, base>::begin() const
	{
		return container.begin();
	}

	template <class T, class base>
	typename base::iterator
	stack<T, base>::end() const
	{
		return container.end();
	}

	template <class T, class base>
	stack<T, base>& stack<T, base>::push(const T& obj)
	{
		container.push_back(obj);
		return *this;
	}

	template <class T, class base>
	stack<T, base>& stack<T, base>::pop()
	{
		container.pop_back();
		return *this;
	}

	template <class T, class base>
	stack<T, base>& stack<T, base>::clear()
	{
		container.clear();
		return *this;
	}

	template <class T, class base>
	T& stack<T, base>::top()
	{
		assert(!container.empty());
		return container.back();
	}

	template <class T, class base>
	const T& stack<T, base>::top() const
	{
		assert(!container.empty());
		return container.back();
	}

	template <class T, class base>
	void stack<T, base>::print() const
	{
		auto print = [this](const auto& x) {
			std::cout << x;
			if (&x != &this->back())
				printf(", ");
		};

		printf("\nstack @%#llx = [", (u64) container.data());
		if (!empty())
			for_each(begin(), end(), print);
		printf("]\n");
	}
}

#endif
