#ifndef _NUTS_STACK_
#define _NUTS_STACK_

#include "vector.h"
#include "list.h"

namespace nuts
{
	template <class T, class base = nuts::vector<T>>
	class stack
	{
		using itr_type = typename base::iterator;

	protected:
		base container;

	public:
		stack() = default;
		stack(std::initializer_list<T> ilist);
		~stack() = default;

		stack<T, base> &push(const T &obj);
		stack<T, base> &pop();
		stack<T, base> &clear();

		T &top();
		const T &top() const;

		bool empty() const { return container.empty(); }
		bool exist() const { return container.exist(); }

		size_t size() const { return container.size(); }
		T &front() { return container.front(); }
		T &back() { return container.back(); }
		const T &front() const { return container.front(); }
		const T &back() const { return container.back(); }
		stack<T, base> &operator=(stack<T, base> &obj);

		itr_type begin();
		itr_type end();

		const itr_type begin() const { return this->begin(); }
		const itr_type end() const { return this->end(); };

		void print();
	};

	template <class T, class base>
	stack<T, base>::stack(std::initializer_list<T> ilist)
	{
		vector<T> copy = ilist;
		this->container.move(copy);
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
	const T &stack<T, base>::top() const
	{
		return container.back();
	}

	template <class T, class base>
	stack<T, base> &stack<T, base>::operator=(stack<T, base> &obj)
	{
		container = obj.container;
		return *this;
	}

	template <class T, class base>
	void stack<T, base>::print()
	{
		auto print = [this](const auto &x)
		{
			std::cout << x;
			if (x != this->back())
				std::cout << ", ";
		};

		printf("\nstack@%#llx = [", (u64)container.data());
		for_each(begin(), end(), print);
		printf("]\n");
	}
}
#endif
