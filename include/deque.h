#ifndef _NUTS_DEQUE_
#define _NUTS_DEQUE_ 1

#include "array.h"
#include "iterator.h"
#include "list.h"
#include "type.h"

#define DEQUE_BUF_SIZE 512ULL
#define GET_BLOCK_CAPACITY(x) (sizeof(x) > DEQUE_BUF_SIZE \
	                                   ? sizeof(x)        \
	                                   : DEQUE_BUF_SIZE / sizeof(x))

namespace nuts
{
	template <typename T, u64 Buf = GET_BLOCK_CAPACITY(T)>
	class deque
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using buf_type = array<T, Buf>;
		using map_type = list<buf_type>;

		class iterator
		    : public bidirectional_iterator
		{
		public:
			using value_type = T;
			using pointer = T*;
			using out_itr = typename map_type::iterator;

		protected:
			pointer cur, st, ed;
			out_itr itr;

		public:
			iterator() : cur(nullptr), st(nullptr), ed(nullptr) {}
			~iterator() = default;
			iterator(const_pointer& c, const_pointer& s,
			         const_pointer& e, const out_itr& i)
			    : cur(const_cast<pointer>(c)), st(const_cast<pointer>(s)),
			      ed(const_cast<pointer>(e)), itr(i) {}

			iterator(const iterator& src)
			    : cur(src.cur), st(src.st), ed(src.ed), itr(src.itr) {}

			pointer get() const { return cur; }
			pointer operator->() const { return cur; }

			T& operator*() { return *cur; }
			const T& operator*() const { return *cur; }

			iterator& operator++()
			{
				auto tmp = itr;
				if (cur == ed && ++tmp == nullptr)
				{
					cur = st = ed = nullptr;
					itr = nullptr;
					return *this;
				}
				if (cur == ed)
				{
					++itr;
					st = &itr->front(), ed = &itr->back();
					cur = st;
					return *this;
				}
				++cur;
				return *this;
			}

			iterator operator++(int)
			{
				iterator res = *this;
				++(*this);
				return res;
			}

			iterator& operator--()
			{
				auto tmp = itr;
				if (cur == st && --tmp == nullptr)
				{
					cur = st = ed = nullptr;
					itr = nullptr;
					return *this;
				}
				if (cur == st)
				{
					--itr;
					st = &itr->front(), ed = &itr->back();
					cur = ed;
					return *this;
				}
				--cur;
				return *this;
			}

			iterator operator--(int)
			{
				iterator res = *this;
				--(*this);
				return res;
			}

			iterator operator+(i64 bias)
			        const { return nuts::advance(*this, bias); }

			void operator+=(i64 bias) { *this = nuts::advance(*this, bias); }

			iterator operator-(i64 bias)
			        const { return nuts::advance(*this, -bias); }

			void operator-=(i64 bias) { *this = nuts::advance(*this, -bias); }

			inline bool operator==(const iterator& obj)
			        const { return cur == obj.cur; }

			inline bool operator!=(const iterator& obj)
			        const { return cur != obj.cur; }

			inline bool operator==(pointer obj)
			        const { return cur == obj; }

			inline bool operator!=(pointer obj)
			        const { return cur != obj; }

			iterator& operator=(const iterator& src)
			{
				cur = src.cur, st = src.st,
				ed = src.ed, itr = src.itr;
				return *this;
			}
		};

		iterator begin() const
		{
			const_pointer st = &impl.front()[0],
			              ed = &impl.front()[Buf - 1],
			              cur = first;
			return {cur, st, ed, impl.begin()};
		}

		iterator end() const
		{
			const_pointer st = &impl.back()[0],
			              ed = &impl.back()[Buf - 1],
			              cur = last;
			return {cur, st, ed, impl.end()};
		}

		deque() = default;
		deque(const std::initializer_list<T>& ilist);
		deque(const deque<T, Buf>& src);
		deque(deque<T, Buf>&& src) { move(src); }
		~deque() { clear(); }

		T* data() const { return (pointer) impl.data(); }
		u64 size() const { return _size; }
		bool empty() const { return size() == 0; }
		void clear();
		static constexpr u64 block_capacity() { return Buf; }

		T& front() { return *begin(); }
		T& back() { return *end(); }
		const T& front() const { return *begin(); }
		const T& back() const { return *end(); }

		T& operator[](u64 _n);
		const T& operator[](u64 _n) const;

		T& at(u64 _n);
		const T& at(u64 _n) const;

		deque<T, Buf>& operator=(const deque<T, Buf>& src);
		deque<T, Buf>& operator=(deque<T, Buf>&& src) { return move(src); }
		deque<T, Buf>& move(deque<T, Buf>& src);

		void emplace_back();
		void emplace_front();

		void push_back(const T& _val);
		void push_front(const T& _val);

		void push_back(T&& _val);
		void push_front(T&& _val);

		void pop_back();
		void pop_front();

		void print() const;
		void print_detail() const;

	private:
		bool is_front_full() const;
		bool is_back_full() const;
		void allocate_front();
		void allocate_back();
		void free_front();
		void free_back();

	protected:
		map_type impl;
		u64 _size = 0;
		pointer first = nullptr, last = nullptr;
	};

	template <typename T, u64 Buf>
	deque<T, Buf>::deque(const std::initializer_list<T>& ilist)
	{
		for (const T& x: ilist) push_back(x);
	}

	template <typename T, u64 Buf>
	deque<T, Buf>::deque(const deque<T, Buf>& src)
	{
		for (const T& x: range(src)) push_back(x);
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::clear()
	{
		if (!empty())
		{
			impl.clear();
			first = last = nullptr, _size = 0;
		}
	}

	template <typename T, u64 Buf>
	deque<T, Buf>& deque<T, Buf>::move(deque<T, Buf>& src)
	{
		impl.move(src.impl);
		_size = src.size();
		first = src.first, last = src.last;
		src._size = 0;
		src.first = src.last = nullptr;
		return *this;
	}

	template <typename T, u64 Buf>
	bool deque<T, Buf>::is_back_full() const
	{
		return last == &impl.back()[Buf - 1] ||
		       last == (&impl.back()[0]) - 1;
	}

	template <typename T, u64 Buf>
	bool deque<T, Buf>::is_front_full() const
	{
		return first == &impl.front()[0] ||
		       first == (&impl.front()[Buf - 1]) + 1;
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::allocate_back()
	{
		impl.emplace_back();
		last = &impl.back()[0];
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::allocate_front()
	{
		impl.emplace_front();
		first = &impl.front()[Buf - 1];
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::free_front()
	{
		impl.pop_front();
		first = &impl.front()[0];
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::free_back()
	{
		impl.pop_back();
		last = &impl.back()[Buf - 1];
	}

	template <class T, u64 Buf>
	deque<T, Buf>& deque<T, Buf>::operator=(const deque<T, Buf>& src)
	{
		clear();
		for_each(src, [this](const T& x) { push_back(x); });
		return *this;
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::push_back(const T& _val)
	{
		emplace_back();
		back() = _val;
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::push_front(const T& _val)
	{
		emplace_front();
		front() = _val;
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::push_back(T&& _val)
	{
		emplace_back();
		back() = static_cast<T&&>(_val);
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::push_front(T&& _val)
	{
		emplace_front();
		front() = static_cast<T&&>(_val);
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::emplace_back()
	{
		if (empty())
		{
			allocate_back();
			first = last;
		}
		else
		{
			if (is_back_full()) allocate_back();
			else
				++last;
		}
		++_size;
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::emplace_front()
	{
		if (empty())
		{
			allocate_front();
			last = first;
		}
		else
		{
			if (is_front_full()) allocate_front();
			else
				--first;
		}
		++_size;
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::pop_back()
	{
		if (!empty())
		{
			last--;
			if (last == &impl.back()[0] - 1)
				free_back();
			_size--;
		}
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::pop_front()
	{
		if (!empty())
		{
			first++;
			if (first == &impl.front()[Buf - 1] + 1)
				free_front();
			_size--;
		}
	}

	template <typename T, u64 Buf>
	T& deque<T, Buf>::operator[](u64 _n)
	{
		u64 head_len = (&impl.front().back() - first) + 1,
		    tail_len = (last - &impl.back().front()) + 1,
		    mid_len = (impl.size() <= 2) ? 0 : Buf * (impl.size() - 2);
		if (_n < head_len)
		{
			return impl.front()[_n + Buf - head_len];
		}
		if (_n < head_len + mid_len)
		{
			_n -= head_len;
			u64 bias = (_n / Buf) + 1;
			_n %= Buf;
			auto it = nuts::advance(impl.begin(), bias);
			return (*it)[_n];
		}
		if (_n < size())
		{
			return impl.back()[_n - (head_len + mid_len)];
		}
		// undefined
		return *(last + 1);
	}

	template <typename T, u64 Buf>
	const T& deque<T, Buf>::operator[](u64 _n) const
	{
		u64 head_len = &impl.front().back() - first + 1,
		    tail_len = last - &impl.back().front() + 1,
		    mid_len = (impl.size() <= 2) ? 0 : Buf * (impl.size() - 2);
		if (_n < head_len)
		{
			return impl.front()[_n + Buf - head_len];
		}
		if (_n < head_len + mid_len)
		{
			_n -= head_len;
			u64 bias = (_n / Buf) + 1;
			_n %= Buf;
			auto it = nuts::advance(impl.begin(), bias);
			return (*it)[_n];
		}
		if (_n < size())
		{
			return impl.back()[_n - (head_len + mid_len)];
		}
		// undefined
		return *(last + 1);
	}

	template <typename T, u64 Buf>
	T& deque<T, Buf>::at(u64 _n)
	{
		assert(_n > size());
		u64 head_len = &impl.front().back() - first + 1,
		    tail_len = last - &impl.back().front() + 1,
		    mid_len = (impl.size() <= 2) ? 0 : Buf * (impl.size() - 2);
		if (_n < head_len)
		{
			return impl.front()[_n + Buf - head_len];
		}
		if (_n < head_len + mid_len)
		{
			_n -= head_len;
			u64 bias = (_n / Buf) + 1;
			_n %= Buf;
			auto it = nuts::advance(impl.begin(), bias);
			return (*it)[_n];
		}
		if (_n < size())
		{
			return impl.back()[_n - (head_len + mid_len)];
		}
		// undefined
		return *(last + 1);
	}

	template <typename T, u64 Buf>
	const T& deque<T, Buf>::at(u64 _n) const
	{
		assert(_n < size());
		u64 head_len = &impl.front().back() - first + 1,
		    tail_len = last - &impl.back().front() + 1,
		    mid_len = (impl.size() <= 2) ? 0 : Buf * (impl.size() - 2);
		if (_n < head_len)
		{
			return impl.front()[_n + Buf - head_len];
		}
		if (_n < head_len + mid_len)
		{
			_n -= head_len;
			u64 bias = (_n / Buf) + 1;
			_n %= Buf;
			auto it = nuts::advance(impl.begin(), bias);
			return (*it)[_n];
		}
		if (_n < size())
		{
			return impl.back()[_n - (head_len + mid_len)];
		}
		// undefined
		return *(last + 1);
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::print() const
	{
		auto print = [this](const auto& x) {
			nuts::print(x);
			if (&x != last) printf(", ");
		};

		printf("deque @%#llx = [", (u64) impl.data());
		if (!empty()) for_each(*this, print);
		printf("]\n");
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::print_detail() const
	{
		auto array_print = [this](const buf_type& arr) {
			printf("[");
			for_each(arr, [&arr, this](const T& x) { 
					if (&x == first || &x == last)
						printf("*"); nuts::print(x);
                    if (&x != &arr.back()) printf(", "); });
			printf("]\n");
		};

		printf("deque @%#llx: \n", (u64) impl.data());
		if (!empty()) for_each(impl, array_print);
	}
}

#endif
