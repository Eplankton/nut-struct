#ifndef _NUTS_LIST_
#define _NUTS_LIST_

#include <iostream>
#include <cstddef>
#include <cassert>

#include "type.h"

#ifndef _DEBUG
#define _DEBUG 1
#endif

#ifndef _WARNING
#define _WARNING 1
#endif

namespace nuts
{
	template <class T>
	struct ListNode // Element class
	{
	public:
		T data;
		ListNode<T> *prev = nullptr;
		ListNode<T> *next = nullptr;

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
	class list // Manager class
	{
	protected:
		ListNode<T> *head = nullptr;
		ListNode<T> *tail = nullptr;
		size_t length = 0;

	public:
		list() = default;                                                   // Void constructor
		explicit list(size_t userInputlength);                               // Init by several empty nodes
		explicit list(const T& userInputData, size_t userInputlength = 1); // Init by several valued nodes
		list(ListNode<T> *start_node, ListNode<T> *end_node);               // Init by a sub-list of another list(deep copy)
		list(const list<T>& obj);                                           // Init by another list(deep copy)
		list(std::initializer_list<T> ilist);                               // Init by a {ilist}
		~list() { this->clear(); }                                           // Clear and gain back memory

		bool empty() const // Whether the list is empty
		{
			return this->size() == 0 &&
			       head == nullptr &&
			       tail == nullptr;
		}

		bool exist() const { return this->empty(); } // Whether the list exists

#if _DEBUG
		void print(); // Print a list in console
#endif

		size_t size() const { return this->length; }// Get the length of the whole list

		size_t get_length(ListNode<T> *start_node) const;                  // Get the length of a sub-list(start from start_node)
		T& operator[](size_t N);                                          // Access to the address(return pointer) of node {WARNING: NOT A GOOD DESIGN}
		list<T>& operator=(const list<T>& obj);                              // Assign deeply
		list<T>& erase(ListNode<T> *start_node, size_t N_far = 0);          // Remove a node that N blocks from the start_node(reference argument)
		size_t erase_all(bool (*condition)(T& stock_val));                  // Remove all node match the condition,The number of elements removed
		list<T>& clear();                                                  // Clear the whole list, release all nodes
		list<T>& destroy(ListNode<T> *start_node, ListNode<T> *end_node); // Clear a part of the list(release nodes partly)
		list<T>& push_back();                                              // Add back an empty node
		list<T>& push_back(const T& obj, size_t num = 1);                  // Add back several nodes(add by init calue)
		list<T>& push_front();                                              // Add front an empty node
		list<T>& push_front(const T& obj, size_t num = 1);                  // Add frontseveral nodes(add by init value)
		list<T>& pop_back();                                              // Remove last element
		list<T>& pop_front();                                              // Remove first element
		list<T>& merge(list<T>& after);                                      // Merge lists together, the latter lost ownership
		list<T>& move(list<T>& after);                                      // A void manager can deprive other's ownership
		// list<T> &operator<<=(list<T> &obj);                                               // Overload as b.destroy(a)

		ListNode<T> *find(bool (*condition)(T& stock_val)); // Find the first element match the condition

#ifdef _DEBUG
		friend std::ostream& operator<<(std::ostream& output, list<T>& obj)
		{
			for (int i = 0; i < obj.size(); i++)
			{
				std::cout << obj[i] << ' ';
			}
			return output;
		}
#endif

		class iterator
		{
		protected:
			ListNode<T> *ptr_onNode = nullptr;

		public:
			iterator() = default;
			iterator(ListNode<T> *obj) : ptr_onNode(obj) {}
			iterator(const iterator& obj) : ptr_onNode(obj.ptr_onNode) {}

			T& operator*() { return (*ptr_onNode).data; }

			iterator& operator=(T *obj)
			{
				ptr_onNode = obj;
				return *this;
			}

			iterator& operator=(const iterator& obj)
			{
				this->ptr_onNode = obj.ptr_onNode;
				return *this;
			}

			bool operator==(ListNode<T> *obj) const { return ptr_onNode == obj; }
			bool operator!=(ListNode<T> *obj) const { return ptr_onNode != obj; }

			bool operator==(const iterator& obj) const { return ptr_onNode == obj.ptr_onNode; }
			bool operator!=(const iterator& obj) const { return ptr_onNode != obj.ptr_onNode; }

			iterator& operator++()
			{
				if (this->ptr_onNode != nullptr)
					this->ptr_onNode = this->ptr_onNode->next;
				return *this;
			}

			const iterator operator++(int)
			{
				iterator res = *this;
				++(*this);
				return res;
			}

			iterator& operator--()
			{
				if (this->ptr_onNode != nullptr)
					this->ptr_onNode = this->ptr_onNode->prev;
				return *this;
			}

			const iterator operator--(int)
			{
				iterator res = *this;
				--(*this);
				return res;
			}

			iterator operator+(const int& bias)
			{
				iterator res = *this;
				return advance(res, bias);
			}

			void operator+=(const int& bias)
			{
				return advance(*this, bias);
			}

			iterator operator-(const int& bias)
			{
				iterator res = *this;
				return advance(res, -bias);
			}

			void operator-=(const int& bias)
			{
				return advance(*this, -bias);
			}

			T *operator->() { return &ptr_onNode->data; }

			template <class>
			friend
			class list;
		};

		iterator begin() const// Return iterator to the first element
		{
			return iterator(this->head);
		}

		iterator end() const // Return iterator to the last element
		{
			return iterator(this->tail);
		}

		T& front() { return this->head->data; }
		T& back() { return this->tail->data; }

		const T& front() const { return this->head->data; }
		const T& back() const { return this->tail->data; }

		list(iterator st, iterator ed);
		list<T>& erase(iterator& it, size_t num = 0);
		list<T>& insert(ListNode<T> *position, const T& obj, size_t num = 1);
		typename list<T>::iterator insert(const iterator& position, const T& obj, size_t num = 1); // Insert several node at position
	};

	template <class T>
	list<T>::list(size_t userInputlength)
	{
		if (userInputlength >= 1)
		{
			this->length = userInputlength;
			this->head = new ListNode<T>; // Save the head node address.
			auto p = head;
			for (int i = 1; i < userInputlength; i++)
			{
				p->next = new ListNode<T>;
				p->next->prev = p;
				p = p->next;
				p->next = nullptr;
			}
			this->tail = p; // Save the last node address.
		}
	}

	template <class T>
	list<T>::list(const T& userInputData, size_t userInputlength)
	{
		this->push_back(userInputData, userInputlength); // better way
	}

	template <class T>
	list<T>::list(ListNode<T> *const start_node, ListNode<T> *const end_node)
	{
		if (start_node != nullptr && end_node != nullptr)
		{
			for (auto p = start_node; p != end_node; p = p->next)
				this->push_back(p->data);
			this->push_back(end_node->data);
		}
	}

	template <class T>
	list<T>::list(iterator st, iterator ed)
	{
		for (auto it = st; it != ed + 1; it++)
			this->push_back(*it);
	}

	template <class T>
	list<T>::list(const list<T>& obj)
	{
		if (!this->empty())
			this->operator=(obj);
	}

	template <class T>
	list<T>::list(std::initializer_list<T> ilist)
	{
		for (auto p = ilist.begin(); p != ilist.end(); ++p)
			this->push_back(*p);
	}

#if _DEBUG
	template <class T>
	void list<T>::print()
	{
		if (!this->empty())
		{
			std::cout << "+++++++\n";
			for (auto p = this->head; p != nullptr; p = p->next)
			{
				std::cout << "node[" << p->index << "]: " << p << " -> "
				          << p->data
				          << '\n';
			}
			std::cout << "+++++++\n\n";
		}
		else
		{
#if _WARNING
			std::cout << "[Error] Empty list\n";
#endif
		}
	}
#endif

	template <class T>
	size_t list<T>::get_length(ListNode<T> *const start_node) const
	{
		size_t count_list_size = 0;
		if (start_node != nullptr)
		{
			for (auto p = start_node; p != nullptr; p = p->next)
				count_list_size++;
			return count_list_size;
		}
		else
		{
			return count_list_size;
		}
	}

#ifdef _DEBUG
	template <class T>
	T& list<T>::operator[](size_t N)
	{
		assert(N < this->size());
		auto p = this->head;
		for (int i = 0; i < N; i++)
			p = p->next;
		T& tmp = p->data;
		return tmp;
	}
#endif

	template <class T>
	list<T>& list<T>::operator=(const list<T>& obj)
	{
		if (!this->empty())
			this->clear();
		auto start_node = obj.head;
		auto end_node = obj.tail;
		if (start_node != nullptr && end_node != nullptr)
		{
			for (auto p = start_node; p != end_node; p = p->next)
				this->push_back(p->data);
			this->push_back(end_node->data);
			return *this;
		}
		return *this;
	}

	template <class T>
	list<T>& list<T>::erase(ListNode<T> *start_node, size_t N_far)
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
	list<T>& list<T>::erase(iterator& it, size_t N)
	{
		return erase(it.ptr_onNode, N);
	}

	template <class T>
	size_t list<T>::erase_all(bool (*condition)(T& stock_val))
	{
		size_t remove_num = 0;
		for (auto p = this->head; p != nullptr;)
		{
			if (condition(p->data))
			{
				auto tmp = p->next;
				erase(p, 0);
				remove_num++;
				p = tmp;
				continue;
			}
			p = p->next;
		}
		this->length = get_length(this->head);
		return remove_num;
	}

	template <class T>
	list<T>& list<T>::clear()
	{
		if (!this->empty())
		{
			for (i64 i = this->size() - 1; i >= 0; i--)
				this->erase(this->head, i);
			head = nullptr, tail = nullptr;
		}
		return *this;
	}

	template <class T>
	list<T>& list<T>::destroy(ListNode<T> *start_node, ListNode<T> *end_node)
	{
		if (this->head != nullptr && start_node != nullptr && end_node != nullptr && !this->empty())
		{
			i64 start_index = this->size() - get_length(start_node);
			i64 end_index = this->get_length() - get_length(end_node);
			for (i64 i = end_index; i >= start_index; --i)
				erase(head, i);
			for (auto p = this->head; p != nullptr; p = p->next)
			{
				if (p->next == nullptr)
					this->tail = p;
			}
			return *this;
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
		else // If it's an empty list,add a node.
		{
			auto p = new ListNode<T>;
			this->head = p;
			this->length++;
			p->next = new ListNode<T>;
			p->next->prev = p;
			p = p->next;
			p->next = nullptr;
			this->tail = p;
			this->length++;
			return *this;
		}
	}

	template <class T>
	list<T>& list<T>::push_back(const T& obj, size_t num)
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
		else // If it's an empty list,add several node.
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
	list<T>& list<T>::push_front(const T& obj, size_t num)
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
		else // If it's an empty list,add several node.
		{
			auto *p = new ListNode<T>(obj);
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
				this->length = after.size(); // Pass ownership.
				this->head = after.head;
				this->tail = after.tail;
				after.head = nullptr; // Release the ownership of after.
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
	list<T>& list<T>::move(list<T>& after)
	{
		if (!this->empty()) { this->clear(); }

		// Pass ownership.
		this->length = after.length;
		this->head = after.head;
		this->tail = after.tail;
		// Release the ownership of after.
		after.head = nullptr;
		after.tail = nullptr;
		after.length = 0;
		return *this;
	}

	template <class T>
	list<T>& list<T>::insert(ListNode<T> *position, const T& obj, size_t num)
	{
		if (num != 0)
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
		else
		{
			return *this;
		}
	}

	template <class T>
	typename list<T>::iterator list<T>::insert(const iterator& position, const T& obj, size_t num)
	{
		if (num != 0)
		{
			for (auto p = this->head; p != nullptr; p = p->next)
			{
				if (p == position.ptr_onNode)
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
					if (position.ptr_onNode == this->tail)
					{
						this->tail = p;
					}
					return iterator(p);
				}
			}

			if (this->head == nullptr)
			{
				this->push_back(obj, num);
				return this->end();
			}
			return iterator(nullptr);

		}
		else
		{
			return iterator(nullptr);
		}
	}

	template <class T>
	ListNode<T> *list<T>::find(bool (*condition)(T& stock_val))
	{
		for (auto p = this->head; p != nullptr; p = p->next)
		{
			if (condition(p->data))
				return p;
		}
		return nullptr;
	}

	// template <class T>
	// list<T> &list<T>::operator<<=(list<T> &obj)
	// {
	//     return this->deprive(obj);
	// }
}

#endif
