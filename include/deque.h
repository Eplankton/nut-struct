#ifndef _NUTS_DEQUE_
#define _NUTS_DEQUE_ 1

#include "array.h"
#include "iterator.h"
#include "list.h"
#include "type.h"

#define DEQUE_BUF_SIZE 8

namespace nuts
{
	template <typename T, u64 Buf = sizeof(T) * DEQUE_BUF_SIZE>
	class deque
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using buf_type = array<T, Buf>;
		using map_type = list<array<T, Buf>>;

	private:
		bool is_back_full() const;
		bool is_front_full() const;

	public:
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
			        const { return advance(*this, bias); }

			void operator+=(i64 bias) { *this = advance(*this, bias); }

			iterator operator-(i64 bias)
			        const { return advance(*this, -bias); }

			void operator-=(i64 bias) { *this = advance(*this, -bias); }

			bool operator==(const iterator& obj)
			        const { return this->cur == obj.cur; }

			bool operator!=(const iterator& obj)
			        const { return this->cur != obj.cur; }

			bool operator==(pointer obj)
			        const { return this->cur == obj; }

			bool operator!=(pointer obj)
			        const { return this->cur != obj; }

			iterator& operator=(const iterator& src)
			{
				cur = src.cur, st = src.st,
				ed = src.ed, itr = src.itr;
				return *this;
			}
		};

		iterator begin() const
		{
			const_pointer st = &map.front()[0],
			              ed = &map.front()[Buf - 1],
			              cur = first;
			return {cur, st, ed, map.begin()};
		}

		iterator end() const
		{
			const_pointer st = &map.back()[0],
			              ed = &map.back()[Buf - 1],
			              cur = last;
			return {cur, st, ed, map.end()};
		}

		deque() = default;
		deque(const std::initializer_list<T>& ilist);

		deque(const deque<T, Buf>& src);
		deque(deque<T, Buf>&& src) { move(src); }

		~deque() { clear(); }

		deque<T, Buf>& move(deque<T, Buf>& src)
		{
			map.move(src.map);
			_size = src.size();
			first = src.first, last = src.last;
			src._size = 0;
			src.first = src.last = nullptr;
			return *this;
		}

		T* data() const { return (pointer) map.data(); }
		u64 size() const { return _size; }
		bool empty() const { return size() == 0; }
		void clear()
		{
			if (!empty())
			{
				map.clear();
				first = last = nullptr, _size = 0;
			}
		}

		T& front() { return *begin(); }
		T& back() { return *end(); }
		const T& front() const { return *begin(); }
		const T& back() const { return *end(); }

		T& operator[](u64 _n);
		const T& operator[](u64 _n) const;

		T& at(u64 _n);
		const T& at(u64 _n) const;

		deque<T, Buf>& operator=(const deque<T, Buf>& src);
		deque<T, Buf>& operator=(deque<T, Buf>&& src) { return this->move(src); }

		void emplace_back();
		void emplace_front();

		void push_back(const T& _val);
		void push_front(const T& _val);
		void pop_back();
		void pop_front();

		void print() const;
		void print_detail() const;

	protected:
		map_type map;
		u64 _size = 0;
		pointer first = nullptr, last = nullptr;
	};

	template <class T, u64 Buf>
	deque<T, Buf>::deque(const std::initializer_list<T>& ilist)
	{
		for (const auto& x: ilist) push_back(x);
	}

	template <class T, u64 Buf>
	deque<T, Buf>::deque(const deque<T, Buf>& src)
	{
		for_each(src, [this](const auto& x) { this->push_back(x); });
	}

	template <typename T, u64 Buf>
	bool deque<T, Buf>::is_back_full() const
	{
		auto tmp = (&map.back()[0]) - 1;
		return last == &map.back()[Buf - 1] || last == tmp;
	}

	template <typename T, u64 Buf>
	bool deque<T, Buf>::is_front_full() const
	{
		auto tmp = (&map.front()[Buf - 1]) + 1;
		return first == &map.front()[0] || first == tmp;
	}

	template <class T, u64 Buf>
	deque<T, Buf>& deque<T, Buf>::operator=(const deque<T, Buf>& src)
	{
		clear();
		for_each(src, [this](const auto& x) { this->push_back(x); });
		return *this;
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::push_back(const T& _val)
	{
		if (empty())
		{
			map.emplace_back();
			first = &map.back()[0];
			last = first;
			*last = _val;
		}
		else
		{
			if (is_back_full())
			{
				map.emplace_back();
				last = &map.back()[0];
				*last = _val;
			}
			else
				*(++last) = _val;
		}
		++_size;
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::push_front(const T& _val)
	{
		if (empty())
		{
			map.emplace_front();
			last = &map.front()[Buf - 1];
			first = last;
			*first = _val;
		}
		else
		{
			if (is_front_full())
			{
				map.emplace_front();
				first = &map.front()[Buf - 1];
				*first = _val;
			}
			else
				*(--first) = _val;
		}
		++_size;
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::emplace_back()
	{
		if (empty())
		{
			map.emplace_back();
			first = &map.back()[0];
			last = first;
		}
		else
		{
			if (is_back_full())
			{
				map.emplace_back();
				last = &map.back()[0];
			}
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
			map.emplace_front();
			last = &map.front()[Buf - 1];
			first = last;
		}
		else
		{
			if (is_front_full())
			{
				map.emplace_front();
				first = &map.front()[Buf - 1];
			}
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
			if (last == &map.back()[0] - 1)
			{
				map.pop_back();
				last = &map.back()[Buf - 1];
			}
			--_size;
		}
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::pop_front()
	{
		if (!empty())
		{
			first++;
			if (first == &map.front()[Buf - 1] + 1)
			{
				map.pop_front();
				first = &map.front()[0];
			}
			--_size;
		}
	}

	template <typename T, u64 Buf>
	T& deque<T, Buf>::operator[](u64 _n)
	{
		u64 head_len = (&map.front().back() - first) + 1,
		    tail_len = (last - &map.back().front()) + 1,
		    mid_len = (map.size() <= 2) ? 0 : Buf * (map.size() - 2);
		if (_n < head_len)
		{
			return map.front()[_n + Buf - head_len];
		}
		if (_n < head_len + mid_len)
		{
			_n -= head_len;
			u64 bias = (_n / Buf) + 1;
			_n %= Buf;
			auto it = advance(map.begin(), bias);
			return (*it)[_n];
		}
		if (_n < size())
		{
			return map.back()[_n - (head_len + mid_len)];
		}
		// undefined
		return *(last + 1);
	}

	template <typename T, u64 Buf>
	const T& deque<T, Buf>::operator[](u64 _n) const
	{
		u64 head_len = &map.front().back() - first + 1,
		    tail_len = last - &map.back().front() + 1,
		    mid_len = (map.size() <= 2) ? 0 : Buf * (map.size() - 2);
		if (_n < head_len)
		{
			return map.front()[_n + Buf - head_len];
		}
		if (_n < head_len + mid_len)
		{
			_n -= head_len;
			u64 bias = (_n / Buf) + 1;
			_n %= Buf;
			auto it = advance(map.begin(), bias);
			return (*it)[_n];
		}
		if (_n < size())
		{
			return map.back()[_n - (head_len + mid_len)];
		}
		// undefined
		return *(last + 1);
	}

	template <typename T, u64 Buf>
	T& deque<T, Buf>::at(u64 _n)
	{
		assert(_n > size());
		u64 head_len = &map.front().back() - first + 1,
		    tail_len = last - &map.back().front() + 1,
		    mid_len = (map.size() <= 2) ? 0 : Buf * (map.size() - 2);
		if (_n < head_len)
		{
			return map.front()[_n + Buf - head_len];
		}
		if (_n < head_len + mid_len)
		{
			_n -= head_len;
			u64 bias = (_n / Buf) + 1;
			_n %= Buf;
			auto it = advance(map.begin(), bias);
			return (*it)[_n];
		}
		if (_n < size())
		{
			return map.back()[_n - (head_len + mid_len)];
		}
		// undefined
		return *(last + 1);
	}

	template <typename T, u64 Buf>
	const T& deque<T, Buf>::at(u64 _n) const
	{
		assert(_n < size());
		u64 head_len = &map.front().back() - first + 1,
		    tail_len = last - &map.back().front() + 1,
		    mid_len = (map.size() <= 2) ? 0 : Buf * (map.size() - 2);
		if (_n < head_len)
		{
			return map.front()[_n + Buf - head_len];
		}
		if (_n < head_len + mid_len)
		{
			_n -= head_len;
			u64 bias = (_n / Buf) + 1;
			_n %= Buf;
			auto it = advance(map.begin(), bias);
			return (*it)[_n];
		}
		if (_n < size())
		{
			return map.back()[_n - (head_len + mid_len)];
		}
		// undefined
		return *(last + 1);
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::print() const
	{
		auto print = [this](const auto& x) {
			std::cout << x;
			if (&x != &this->back()) printf(", ");
		};

		printf("\ndeque @%#llx = [", (u64) map.data());
		if (!empty()) for_each(*this, print);
		printf("]\n");
	}

	template <typename T, u64 Buf>
	void deque<T, Buf>::print_detail() const
	{
		auto array_print = [this](const buf_type& arr) {
			printf("[");
			for_each(arr.begin(), arr.end(),
			         [&arr, this](const T& x) { 
						if (&x == this->begin().get() ||
							&x == this->end().get())
							 printf("*"); std::cout << x; 
                        if (&x != &arr.back()) printf(", "); });
			printf("]\n");
		};

		printf("\ndeque @%#llx: \n", (u64) map.data());
		if (!empty())
			for_each(map, array_print);
	}
}

#endif
