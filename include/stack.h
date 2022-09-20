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
	template <typename Box>
	concept Stack_Base = Container<Box> &&
	        requires(Box x, typename Box::value_type val)
	{
		x.push_back(val);
		x.pop_back();
	};

	template <class T, Stack_Base C = vector<T>>
	class stack
	{
	public:
		using value_type = T;
		using itr_type = typename C::iterator;

		stack() = default;
		stack(const stack<T, C>& src) = default;
		stack(const std::initializer_list<T>& ilist);
		stack(stack<T, C>&& src) { move(src); }

		~stack() = default;

		stack<T, C>& push(const T& obj);
		stack<T, C>& push(T&& obj);
		stack<T, C>& pop();
		stack<T, C>& clear();

		T& top();
		const T& top() const;

		bool empty() const { return impl.empty(); }
		u64 size() const { return impl.size(); }
		T& front() { return impl.front(); }
		T& back() { return impl.back(); }
		const T& front() const { return impl.front(); }
		const T& back() const { return impl.back(); }
		stack<T, C>& operator=(const stack<T, C>& obj);
		stack<T, C>& operator=(stack<T, C>&& src) { return move(src); }
		stack<T, C>& move(stack<T, C>& obj);

		itr_type begin() const;
		itr_type end() const;

		void print() const;

	protected:
		C impl;
	};

	template <class T, Stack_Base C>
	stack<T, C>::stack(const std::initializer_list<T>& ilist)
	{
		C copy {ilist};
		impl.move(copy);
	}

	template <class T, Stack_Base C>
	stack<T, C>& stack<T, C>::operator=(const stack<T, C>& obj)
	{
		clear();
		impl = obj.impl;
		return *this;
	}

	template <class T, Stack_Base C>
	stack<T, C>& stack<T, C>::move(stack<T, C>& obj)
	{
		clear();
		impl.move(obj.impl);
		return *this;
	}

	template <class T, Stack_Base C>
	typename C::iterator
	stack<T, C>::begin() const
	{
		return impl.begin();
	}

	template <class T, Stack_Base C>
	typename C::iterator
	stack<T, C>::end() const
	{
		return impl.end();
	}

	template <class T, Stack_Base C>
	stack<T, C>& stack<T, C>::push(const T& obj)
	{
		impl.push_back(obj);
		return *this;
	}

	template <class T, Stack_Base C>
	stack<T, C>& stack<T, C>::push(T&& obj)
	{
		impl.push_back(static_cast<T&&>(obj));
		return *this;
	}

	template <class T, Stack_Base C>
	stack<T, C>& stack<T, C>::pop()
	{
		impl.pop_back();
		return *this;
	}

	template <class T, Stack_Base C>
	stack<T, C>& stack<T, C>::clear()
	{
		impl.clear();
		return *this;
	}

	template <class T, Stack_Base C>
	T& stack<T, C>::top()
	{
		assert(!impl.empty());
		return impl.back();
	}

	template <class T, Stack_Base C>
	const T& stack<T, C>::top() const
	{
		assert(!impl.empty());
		return impl.back();
	}

	template <class T, Stack_Base C>
	void stack<T, C>::print() const
	{
		auto print = [this](const auto& x) {
			nuts::print(x);
			if (&x != &back()) printf(", ");
		};

		printf("stack @%#llx = [", (u64) impl.data());
		if (!empty())
			for_each(impl, print);
		printf("]\n");
	}
}

#endif
