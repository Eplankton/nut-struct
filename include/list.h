#ifndef _NUTS_LIST_
#define _NUTS_LIST_

#include "algorithm.h"
#include "iterator.h"
#include "type.h"

namespace nuts
{
	template <class T>
	struct ListNode// Element class
	{
		T data;
		ListNode<T>* prev = nullptr;
		ListNode<T>* next = nullptr;

		ListNode() = default;
		explicit ListNode(const T& userInputData)
		    : data(userInputData), prev(nullptr), next(nullptr) {}
		explicit ListNode(T&& userInputData)
		    : data(userInputData), prev(nullptr), next(nullptr) {}
		~ListNode()
		{
			prev = next = nullptr;

			/* Don't have to delete anything here */
			/* Because the variable 'data' will call its own deconstructor  */
			/* And if 'data' is not a pointer, you can't delete it manually anyway */
		}
	};

	template <class T>
	class list// Manager class
	{
	public:
		using value_type = T;
		using node = ListNode<T>;
		using node_ptr = node*;

	private:
		list<T>& erase(node_ptr start_node, u64 N_far = 0);// Remove a node that N blocks from the start_node(reference argument)
		list<T>& insert(node_ptr position, const T& obj, u64 num = 1);

	public:
		list() = default;                                     // Void constructor
		list(const T& userInputData, u64 userInputlength = 1);// Init by several valued nodes
		list(const list<T>& obj);                             // Init by another list(deep copy)
		list(list<T>&& src) { move(src); }
		list(const std::initializer_list<T>& ilist);// Init by a {ilist}
		~list() { clear(); }                        // Clear and gain back memory

		inline bool empty() const// Whether the list is empty
		{
			return size() == 0 &&
			       head == nullptr &&
			       tail == nullptr;
		}

		node_ptr data() const { return head; }

		inline bool exist() const { return empty(); }// Whether the list exists
		inline u64 size() const { return length; }   // Get the length of the whole list
		void print() const;                          // Print a list in console
		list<T>& clear();                            // Clear the whole list, release all nodes

		list<T>& operator=(const list<T>& obj);// Copy
		list<T>& operator=(list<T>&& src) { return move(src); }

		list<T>& emplace_back();// Add back an empty node
		list<T>& emplace_back(const T& val);
		list<T>& emplace_back(T&& val);
		list<T>& push_back(const T& obj, u64 num = 1);// Add back several nodes(add by init calue)
		list<T>& push_back(T&& obj);

		list<T>& emplace_front();// Add front an empty node
		list<T>& emplace_front(const T& val);
		list<T>& emplace_front(T&& val);
		list<T>& push_front(const T& obj, u64 num = 1);// Add frontseveral nodes(add by init value)
		list<T>& push_front(T&& obj);

		list<T>& pop_back(); // Remove last element
		list<T>& pop_front();// Remove first element

		list<T>& merge(list<T>& after);// Merge lists together, the latter lost ownership
		list<T>& move(list<T>& src);   // A void manager can deprive other's ownership

		class iterator
		    : public bidirectional_iterator
		{
		public:
			using value_type = T;

		protected:
			node_ptr _ptr = nullptr;

		public:
			iterator() = default;
			iterator(node_ptr obj) : _ptr(obj) {}
			iterator(const iterator& obj) : _ptr(obj._ptr) {}

			node_ptr get() const { return _ptr; }

			T& operator*() { return (*_ptr).data; }
			const T& operator*() const { return (*_ptr).data; }

			iterator& operator=(node_ptr obj)
			{
				_ptr = obj;
				return *this;
			}

			iterator& operator=(const iterator& obj)
			{
				_ptr = obj._ptr;
				return *this;
			}

			inline bool operator==(node_ptr obj) const { return _ptr == obj; }
			inline bool operator!=(node_ptr obj) const { return _ptr != obj; }

			inline bool operator==(const iterator& obj) const { return _ptr == obj._ptr; }
			inline bool operator!=(const iterator& obj) const { return _ptr != obj._ptr; }

			iterator& operator++()
			{
				if (_ptr != nullptr)
					_ptr = _ptr->next;
				else
					_ptr = nullptr;
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
				if (_ptr != nullptr)
					_ptr = _ptr->prev;
				else
					_ptr = nullptr;
				return *this;
			}

			iterator operator--(int)
			{
				iterator res = *this;
				--(*this);
				return res;
			}

			iterator operator+(i64 bias) const
			{
				iterator res = *this;
				return nuts::advance(res, bias);
			}

			void operator+=(i64 bias) { *this = nuts::advance(*this, bias); }

			iterator operator-(i64 bias) const
			{
				iterator res = *this;
				return nuts::advance(res, -bias);
			}

			void operator-=(i64 bias) { *this = nuts::advance(*this, -bias); }

			T* operator->() const { return (T*) &_ptr->data; }
		};

		static constexpr iterator npos {};

		inline iterator begin() const { return iterator(const_cast<node_ptr>(head)); }
		inline iterator end() const { return iterator(const_cast<node_ptr>(tail)); }

		inline T& front() { return head->data; }
		inline T& back() { return tail->data; }

		inline const T& front() const { return head->data; }
		inline const T& back() const { return tail->data; }

		list<T>& insert(const iterator& pos,
		                const T& obj, u64 num = 1);// Insert several node at position

		list<T>& erase(iterator pos, u64 num = 0);

		template <typename Func>
		iterator find(Func fn) const;// Find the first element match the condition

		template <typename Func>
		void erase_all(const Func& fn);// Remove all node match the condition,The number of elements removed

		template <Forward_Itr Itr>
		void assign(Itr st, Itr ed);

		template <Forward_Itr Itr>
		list(Itr st, Itr ed);

	protected:
		node_ptr head = nullptr;
		node_ptr tail = nullptr;
		u64 length = 0;
	};

	// Deduction Guide
	template <class T>
	list(const std::initializer_list<T>&) -> list<T>;

	template <class T>
	list<T>::list(const T& userInputData, u64 userInputlength)
	{
		push_back(userInputData, userInputlength);// better way
	}

	template <class T>
	template <Forward_Itr Itr>
	list<T>::list(Itr st, Itr ed)
	{
		assign(st, ed);
	}

	template <class T>
	list<T>::list(const list<T>& obj)
	{
		if (!empty()) this->operator=(obj);
	}

	template <class T>
	list<T>::list(const std::initializer_list<T>& ilist)
	{
		for (const auto& x: ilist) push_back(x);
	}

	template <class T>
	list<T>& list<T>::operator=(const list<T>& obj)
	{
		if (!empty()) clear();
		assign(obj.begin(), obj.end());
		return *this;
	}

	template <class T>
	list<T>& list<T>::erase(node_ptr start_node, u64 N_far)
	{
		assert(N_far < size() && !empty());
		i64 i = 0;
		for (auto p = start_node; p != nullptr; p = p->next)
		{
			if (i == N_far)
			{
				if (p == head)
				{
					if (p->next != nullptr)
					{
						p->next->prev = nullptr;
						head = p->next;
						delete p;
						length--;
						return *this;
					}
					else
					{
						delete p;
						head = tail = nullptr;
						length--;
						return *this;
					}
				}
				if (p == tail)
				{
					if (p->prev != nullptr)
					{
						p->prev->next = nullptr;
						tail = p->prev;
						delete p;
						length--;
						return *this;
					}
					else
					{
						delete p;
						head = tail = nullptr;
						length--;
						return *this;
					}
				}
				p->next->prev = p->prev;
				p->prev->next = p->next;
				delete p;
				length--;
				return *this;
			}
			i++;
		}
		return *this;
	}

	template <class T>
	template <typename Func>
	void list<T>::erase_all(const Func& fn)
	{
		iterator safe;
		auto ed = end() + 1;
		for (auto p = begin(); p != ed;)
		{
			safe = p + 1;
			if (fn(*p))
			{
				erase(p);
				p = safe;
			}
			else
				++p;
		}
	}

	template <class T>
	list<T>& list<T>::clear()
	{
		if (!empty())
			while (!empty()) pop_back();
		return *this;
	}

	template <class T>
	list<T>& list<T>::emplace_back()
	{
		if (!empty())
		{
			auto p = tail;
			p->next = new node;
			p->next->prev = p;
			p = p->next;
			p->next = nullptr;
			tail = p;
			length++;
			return *this;
		}
		else// If it's an empty list,add a node.
		{
			auto p = new node;
			p->prev = p->next = nullptr;
			head = p;
			tail = p;
			length++;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::emplace_back(const T& val)
	{
		if (!empty())
		{
			auto p = tail;
			p->next = new node(val);
			p->next->prev = p;
			p = p->next;
			p->next = nullptr;
			tail = p;
			length++;
			return *this;
		}
		else// If it's an empty list,add a node.
		{
			auto p = new node(val);
			p->prev = p->next = nullptr;
			head = p;
			tail = p;
			length++;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::emplace_back(T&& val)
	{
		if (!empty())
		{
			auto p = tail;
			p->next = new node(nuts::move(val));
			p->next->prev = p;
			p = p->next;
			p->next = nullptr;
			tail = p;
			length++;
			return *this;
		}
		else// If it's an empty list,add a node.
		{
			auto p = new node(nuts::move(val));
			p->prev = p->next = nullptr;
			head = p;
			tail = p;
			length++;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::push_back(const T& obj, u64 num)
	{
		for (auto i: range(0, num))
			emplace_back(obj);
		return *this;
	}

	template <class T>
	list<T>& list<T>::push_back(T&& obj)
	{
		emplace_back(nuts::move(obj));
		return *this;
	}

	template <class T>
	list<T>& list<T>::emplace_front()
	{
		if (!empty())
		{
			auto tmp = head;
			head = new node;
			head->next = tmp;
			tmp->prev = head;
			length++;
			return *this;
		}
		else
		{
			head = new node;
			tail = head;
			length++;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::emplace_front(const T& val)
	{
		if (!empty())
		{
			auto tmp = head;
			head = new node(val);
			head->next = tmp;
			tmp->prev = head;
			length++;
			return *this;
		}
		else
		{
			head = new node(val);
			tail = head;
			length++;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::emplace_front(T&& val)
	{
		if (!empty())
		{
			auto tmp = head;
			head = new node(nuts::move(val));
			head->next = tmp;
			tmp->prev = head;
			length++;
			return *this;
		}
		else
		{
			head = new node(nuts::move(val));
			tail = head;
			length++;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::push_front(const T& obj, u64 num)
	{
		for (auto i: range(0, num))
			emplace_front(obj);
		return *this;
	}

	template <class T>
	list<T>& list<T>::push_front(T&& obj)
	{
		emplace_front(nuts::move(obj));
		return *this;
	}

	template <class T>
	list<T>& list<T>::pop_back()
	{
		erase(tail);
		return *this;
	}

	template <class T>
	list<T>& list<T>::pop_front()
	{
		erase(head);
		return *this;
	}

	template <class T>
	list<T>& list<T>::merge(list<T>& after)
	{
		if (!after.empty())
		{
			if (!empty())
			{
				length = size() + after.size();
				tail->next = after.head;
				after.head->prev = tail;
				tail = after.tail;
				after.head = after.tail = nullptr;
				after.length = 0;
				return *this;
			}
			else
			{
				length = after.size();
				head = after.head;
				tail = after.tail;
				after.head = after.tail = nullptr;
				after.length = 0;
				return *this;
			}
		}
		else
		{
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::move(list<T>& src)
	{
		if (!empty()) clear();

		length = src.length;
		head = src.head;
		tail = src.tail;

		src.head = src.tail = nullptr;
		src.length = 0;
		return *this;
	}

	template <class T>
	list<T>& list<T>::insert(node_ptr position, const T& obj, u64 num)
	{
		for (auto p = head; p != nullptr; p = p->next)
		{
			if (p == position)
			{
				auto temp = p->next;
				for (int i = 0; i < num; i++)
				{
					p->next = new node(obj);
					p->next->prev = p;
					p = p->next;
					p->next = nullptr;
					length++;
				}
				p->next = temp;
				if (temp != nullptr)
					temp->prev = p;
				if (position == tail)
					tail = p;
			}
		}
		return *this;
	}

	template <class T>
	list<T>& list<T>::insert(const iterator& pos, const T& obj, u64 num)
	{
		insert(pos.get(), obj, num);
		return *this;
	}

	template <class T>
	list<T>& list<T>::erase(iterator pos, u64 N)
	{
		return erase(pos.get(), N);
	}

	template <class T>
	template <typename Func>
	typename list<T>::iterator
	list<T>::find(Func fn) const
	{
		return nuts::find_if(begin(), end(), fn);
	}

	template <class T>
	template <Forward_Itr Itr>
	void list<T>::assign(Itr st, Itr ed)
	{
		for_each(st, ed, [&](const auto& x) { push_back(x); });
	}

	template <class T>
	void list<T>::print() const
	{
		auto print = [&](const auto& x) {
			nuts::print(x);
			if (&x != &back()) printf(", ");
		};

		printf("list @%#llx = [", (u64) data());
		if (!empty()) for_each(*this, print);
		printf("]\n");
	}
}

#endif
