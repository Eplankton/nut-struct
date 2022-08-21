#ifndef _NUTS_STACK_
#define _NUTS_STACK_ 1

#include "concept.h"
#include "deque.h"
#include "vector.h"


/** @file stack
     * push() vector as same as deque 
	 * pop() vector usually three times faster than deque
     */

namespace nuts
{
	template <typename T>
	concept Stack_Base = Container<T> &&
	        requires(T x, typename T::value_type val)
	{
		x.push_back(val);
		x.pop_back();
	};

	template <class T, Stack_Base base = vector<T>>
	class stack
	{
	public:
		using value_type = T;
		using itr_type = typename base::iterator;

	public:
		stack() = default;
		stack(const std::initializer_list<T>& ilist);
		stack(stack<T, base>&& src) { move(src); }
		stack(const stack<T, base>& src) { _Base = src._Base; }
		~stack() = default;

		stack<T, base>& push(const T& obj);
		stack<T, base>& push(T&& obj);
		stack<T, base>& pop();
		stack<T, base>& clear();

		T& top();
		const T& top() const;

		bool empty() const { return _Base.empty(); }
		u64 size() const { return _Base.size(); }
		T& front() { return _Base.front(); }
		T& back() { return _Base.back(); }
		const T& front() const { return _Base.front(); }
		const T& back() const { return _Base.back(); }
		stack<T, base>& operator=(const stack<T, base>& obj);
		stack<T, base>& operator=(stack<T, base>&& src) { return move(src); }
		stack<T, base>& move(stack<T, base>& obj);

		itr_type begin() const;
		itr_type end() const;

		void print() const;

	protected:
		base _Base;
	};

	template <class T, Stack_Base base>
	stack<T, base>::stack(const std::initializer_list<T>& ilist)
	{
		base copy {ilist};
		_Base.move(copy);
	}

	template <class T, Stack_Base base>
	stack<T, base>& stack<T, base>::operator=(const stack<T, base>& obj)
	{
		clear();
		_Base = obj._Base;
		return *this;
	}

	template <class T, Stack_Base base>
	stack<T, base>& stack<T, base>::move(stack<T, base>& obj)
	{
		clear();
		_Base.move(obj._Base);
		return *this;
	}

	template <class T, Stack_Base base>
	typename base::iterator
	stack<T, base>::begin() const
	{
		return _Base.begin();
	}

	template <class T, Stack_Base base>
	typename base::iterator
	stack<T, base>::end() const
	{
		return _Base.end();
	}

	template <class T, Stack_Base base>
	stack<T, base>& stack<T, base>::push(const T& obj)
	{
		_Base.push_back(obj);
		return *this;
	}

	template <class T, Stack_Base base>
	stack<T, base>& stack<T, base>::push(T&& obj)
	{
		_Base.push_back(static_cast<T&&>(obj));
		return *this;
	}

	template <class T, Stack_Base base>
	stack<T, base>& stack<T, base>::pop()
	{
		_Base.pop_back();
		return *this;
	}

	template <class T, Stack_Base base>
	stack<T, base>& stack<T, base>::clear()
	{
		_Base.clear();
		return *this;
	}

	template <class T, Stack_Base base>
	T& stack<T, base>::top()
	{
		assert(!_Base.empty());
		return _Base.back();
	}

	template <class T, Stack_Base base>
	const T& stack<T, base>::top() const
	{
		assert(!_Base.empty());
		return _Base.back();
	}

	template <class T, Stack_Base base>
	void stack<T, base>::print() const
	{
		auto print = [this](const auto& x) {
			std::cout << x;
			if (&x != &this->back())
				printf(", ");
		};

		printf("\nstack @%#llx = [", (u64) _Base.data());
		if (!empty())
			for_each(_Base, print);
		printf("]\n");
	}
}

#endif
