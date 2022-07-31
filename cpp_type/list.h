#ifndef _NUTS_LIST_
#define _NUTS_LIST_ 1

#include <cassert>
#include <cstddef>
#include <iostream>

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
		explicit ListNode(const T& userInputData) : data(userInputData), prev(nullptr), next(nullptr) {}
		~ListNode()
		{
			prev = nullptr;
			next = nullptr;

			/* Don't have to delete anything here */
			/* Because the variable 'data' will call its own deconstructor  */
			/* And if 'data' is not a pointer, you can't delete it manually anyway */
		}

		// template <class>
		// friend class list;
		/* Don't need an explicit class/typename T */
		/* Very import, this authorizes the manager class */
	};

	template <class T>
	class list// Manager class
	{
	public:
		using value_type = T;
		using node = ListNode<T>;
		using node_ptr = node*;

	protected:
		node_ptr head = nullptr;
		node_ptr tail = nullptr;
		u64 length = 0;

	private:
		list<T>& erase(node_ptr start_node, u64 N_far = 0);// Remove a node that N blocks from the start_node(reference argument)
		list<T>& insert(node_ptr position, const T& obj, u64 num = 1);

	public:
		list() = default;                                              // Void constructor
		explicit list(u64 userInputlength);                            // Init by several empty nodes
		explicit list(const T& userInputData, u64 userInputlength = 1);// Init by several valued nodes
		
		list(const list<T>& obj);                                      // Init by another list(deep copy)
		list(list<T>&& src) { this->move(src); }
		
		list(const std::initializer_list<T>& ilist);// Init by a {ilist}
		
		~list() { this->clear(); }                  // Clear and gain back memory

		bool empty() const// Whether the list is empty
		{
			return this->size() == 0 &&
			       head == nullptr &&
			       tail == nullptr;
		}

		node_ptr data() const { return const_cast<node_ptr>(head); }
		bool exist() const { return this->empty(); }// Whether the list exists
		u64 size() const { return length; }         // Get the length of the whole list
		void print() const;                         // Print a list in console

		list<T>& operator=(const list<T>& obj);// Copy
		list<T>& operator=(list<T>&& src) { return this->move(src); }

		list<T>& clear();// Clear the whole list, release all nodes

		list<T>& push_back();                         // Add back an empty node
		list<T>& push_back(const T& obj, u64 num = 1);// Add back several nodes(add by init calue)

		list<T>& push_front();                         // Add front an empty node
		list<T>& push_front(const T& obj, u64 num = 1);// Add frontseveral nodes(add by init value)

		list<T>& pop_back(); // Remove last element
		list<T>& pop_front();// Remove first element

		list<T>& merge(list<T>& after);// Merge lists together, the latter lost ownership
		list<T>& move(list<T>& src); // A void manager can deprive other's ownership

		class iterator : public bidirectional_iterator
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
				this->_ptr = obj._ptr;
				return *this;
			}

			bool operator==(node_ptr obj) const { return _ptr == obj; }
			bool operator!=(node_ptr obj) const { return _ptr != obj; }

			bool operator==(const iterator& obj) const { return _ptr == obj._ptr; }
			bool operator!=(const iterator& obj) const { return _ptr != obj._ptr; }

			iterator& operator++()
			{
				if (this->_ptr != nullptr)
					this->_ptr = this->_ptr->next;
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
				return advance(res, bias);
			}

			void operator+=(i64 bias)
			{
				return advance(*this, bias);
			}

			iterator operator-(i64 bias) const
			{
				iterator res = *this;
				return advance(res, -bias);
			}

			void operator-=(i64 bias)
			{
				return advance(*this, -bias);
			}

			T* operator->() { return &_ptr->data; }
			const T* operator->() const { return &_ptr->data; }
		};

		iterator begin() { return iterator(this->head); }
		iterator end() { return iterator(this->tail); }
		iterator begin() const { return {const_cast<node_ptr>(this->head)}; }
		iterator end() const { return {const_cast<node_ptr>(this->tail)}; }

		T& front() { return this->head->data; }
		T& back() { return this->tail->data; }

		const T& front() const { return this->head->data; }
		const T& back() const { return this->tail->data; }

		list<T>& insert(const iterator& pos,
		                const T& obj, u64 num = 1);// Insert several node at position
		list<T>& erase(iterator pos, u64 num = 0);

		template <typename Itr>
		void assign(Itr st, Itr ed);

		template <typename Func>
		iterator find(const Func& fn) const;// Find the first element match the condition

		template <typename Func>
		void erase_all(const Func& fn);// Remove all node match the condition,The number of elements removed

		template <typename Itr>
		list(Itr st, Itr ed);
	};

	template <class T>
	list<T>::list(u64 userInputlength)
	{
		if (userInputlength >= 1)
		{
			this->length = userInputlength;
			this->head = new ListNode<T>;// Save the head node address.
			auto p = head;
			for (int i = 1; i < userInputlength; i++)
			{
				p->next = new ListNode<T>;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
			}
			this->tail = p;// Save the last node address.
		}
	}

	template <class T>
	list<T>::list(const T& userInputData, u64 userInputlength)
	{
		this->push_back(userInputData, userInputlength);// better way
	}

	template <class T>
	template <typename Itr>
	list<T>::list(Itr st, Itr ed)
	{
		this->assign(st, ed);
	}

	template <class T>
	list<T>::list(const list<T>& obj)
	{
		if (!this->empty())
			this->operator=(obj);
	}

	template <class T>
	list<T>::list(const std::initializer_list<T>& ilist)
	{
		for (auto p = ilist.begin(); p != ilist.end(); ++p)
			this->push_back(*p);
	}

	template <class T>
	list<T>& list<T>::operator=(const list<T>& obj)
	{
		if (!this->empty())
			this->clear();
		this->assign(obj.begin(), obj.end());
		return *this;
	}

	template <class T>
	list<T>& list<T>::erase(ListNode<T>* start_node, u64 N_far)
	{
		assert(N_far < this->size() && !this->empty());
		int i = 0;
		for (auto p = start_node; p != nullptr; p = p->next)
		{
			if (i == N_far)
			{
				if (p == this->head)
				{
					if (p->next != nullptr)
					{
						p->next->prev = nullptr;
						this->head = p->next;
						delete p;
						this->length--;
						return *this;
					}
					else
					{
						delete p;
						this->head = nullptr;
						this->tail = nullptr;
						this->length--;
						return *this;
					}
				}
				if (p == this->tail)
				{
					if (p->prev != nullptr)
					{
						p->prev->next = nullptr;
						this->tail = p->prev;
						delete p;
						this->length--;
						return *this;
					}
					else
					{
						delete p;
						this->head = nullptr;
						this->tail = nullptr;
						this->length--;
						return *this;
					}
				}
				p->next->prev = p->prev;
				p->prev->next = p->next;
				delete p;
				this->length--;
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
				this->erase(p);
				p = safe;
			}
			else
				++p;
		}
	}

	template <class T>
	list<T>& list<T>::clear()
	{
		if (!this->empty())
		{
			for (i64 i = static_cast<i64>(this->size() - 1);
			     i >= 0; i--)
				this->erase(this->head, i);
			head = nullptr, tail = nullptr;
		}
		return *this;
	}

	template <class T>
	list<T>& list<T>::push_back()
	{
		if (!this->empty())
		{
			auto p = this->tail;
			p->next = new ListNode<T>;
			p->next->prev = p;
			p = p->next;
			p->next = nullptr;
			this->tail = p;
			this->length++;
			return *this;
		}
		else// If it's an empty list,add a node.
		{
			auto p = new ListNode<T>;
			p->prev = nullptr;
			p->next = nullptr;
			this->head = p;
			this->tail = p;
			this->length++;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::push_back(const T& obj, u64 num)
	{
		if (!this->empty())
		{
			auto p = this->tail;
			for (int i = 0; i < num; i++)
			{
				p->next = new ListNode<T>(obj);
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				this->length++;
			}
			this->tail = p;
			return *this;
		}
		else// If it's an empty list,add several node.
		{
			auto p = new ListNode<T>(obj);
			this->head = p;
			this->length++;
			for (int i = 1; i < num; i++)
			{
				p->next = new ListNode<T>(obj);
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				this->length++;
			}
			this->tail = p;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::push_front()
	{
		if (!this->empty())
		{
			auto tmp = this->head;
			this->head = new ListNode<T>;
			this->head->next = tmp;
			tmp->prev = this->head;
			this->length++;
			return *this;
		}
		else
		{
			this->head = new ListNode<T>;
			this->tail = this->head;
			this->length++;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::push_front(const T& obj, u64 num)
	{
		if (!this->empty())
		{
			auto tmp = this->head;
			this->head = new ListNode<T>(obj);
			this->length++;
			auto p = this->head;
			for (int i = 1; i < num; i++)
			{
				p->next = new ListNode<T>(obj);
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				this->length++;
			}
			p->next = tmp;
			tmp->prev = p;
			return *this;
		}
		else// If it's an empty list,add several node.
		{
			auto* p = new ListNode<T>(obj);
			this->head = p;
			this->length++;
			for (int i = 1; i < num; i++)
			{
				p->next = new ListNode<T>(obj);
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
				this->length++;
			}
			this->tail = p;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::pop_back()
	{
		this->erase(this->tail);
		return *this;
	}

	template <class T>
	list<T>& list<T>::pop_front()
	{
		this->erase(this->head);
		return *this;
	}

	template <class T>
	list<T>& list<T>::merge(list<T>& after)
	{
		if (!after.empty())
		{
			if (!this->empty())
			{
				// Pass ownership.
				this->length = this->size() + after.size();
				this->tail->next = after.head;
				after.head->prev = this->tail;
				this->tail = after.tail;
				// Release the ownership of after.
				after.head = nullptr;
				after.tail = nullptr;
				after.length = 0;
				return *this;
			}
			else
			{
				this->length = after.size();// Pass ownership.
				this->head = after.head;
				this->tail = after.tail;
				after.head = nullptr;// Release the ownership of after.
				after.tail = nullptr;
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
		if (!this->empty())// Pass ownership.
			this->clear();

		this->length = src.length;// Release the ownership of after.
		this->head = src.head;
		this->tail = src.tail;

		src.head = nullptr;
		src.tail = nullptr;
		src.length = 0;
		return *this;
	}

	template <class T>
	list<T>& list<T>::insert(ListNode<T>* position, const T& obj, u64 num)
	{
		for (auto p = this->head; p != nullptr; p = p->next)
		{
			if (p == position)
			{
				auto temp = p->next;
				for (int i = 0; i < num; i++)
				{
					p->next = new ListNode<T>(obj);
					p->next->prev = p;
					p = p->next;
					p->next = nullptr;
					this->length++;
				}
				p->next = temp;
				if (temp != nullptr)
				{
					temp->prev = p;
				}
				if (position == this->tail)
				{
					this->tail = p;
				}
				return *this;
			}
		}
		return *this;
	}

	template <class T>
	list<T>& list<T>::insert(const iterator& pos, const T& obj, u64 num)
	{
		this->insert(pos.get(), obj, num);
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
	list<T>::find(const Func& fn) const
	{
		auto ed = end() + 1;
		for (auto p = begin(); p != ed; ++p)
		{
			if (fn(*p))
				return p;
		}
		return iterator {nullptr};
	}

	template <class T>
	template <typename Itr>
	void list<T>::assign(Itr st, Itr ed)
	{
		for_each(st, ed,
		         [=](const auto& x) { this->push_back(x); });
	}

	template <class T>
	void list<T>::print() const
	{
		auto print = [this](const auto& x) {
			std::cout << x;
			if (&x != &this->back())
				printf(", ");
		};

		printf("\nlist @%#llx = [", (u64) this->head);
		if (!empty())
			for_each(begin(), end(), print);
		printf("]\n");
	}

}// namespace nuts

#endif
