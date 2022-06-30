#ifndef _NUTS_BIN_TREE_
#define _NUTS_BIN_TREE_

#include <cassert>
#include "type.h"
#include "queue.h"
#include "memory.h"
#include "algorithm.h"
#include "functional.h"

namespace nuts
{
	template <typename T>
	struct binary_tree_node
	{
		using tree_node = nuts::binary_tree_node<T>;
		using node_ptr = nuts::unique_ptr<tree_node>;
		using node_raw_ptr = binary_tree_node<T> *;

		T data;
		i8 bf = 0;
		node_ptr prev = nullptr, lc = nullptr, rc = nullptr;

		binary_tree_node() : prev(nullptr), lc(nullptr), rc(nullptr) {}

		explicit binary_tree_node(const T &_val) : data(_val),
												   prev(nullptr),
												   lc(nullptr), rc(nullptr) {}

		binary_tree_node(const tree_node &obj) : data(obj.data),
												 prev(obj.prev),
												 lc(obj.lc), rc(obj.rc) {}
		~binary_tree_node() { prev = nullptr; }
	};

	template <typename T>
	i64 get_w(const binary_tree_node<T> *st)
	{
		if (st == nullptr)
			return -1;
		if (st->lc == nullptr && st->rc == nullptr)
			return 0;
		return max(get_w(st->lc.get()), get_w(st->rc.get())) + 1;
	}

	template <typename T>
	i64 get_bf(const binary_tree_node<T> *st)
	{
		return get_w(st->lc.get()) - get_w(st->rc.get());
	}

	template <typename T>
	i64 get_w(const unique_ptr<binary_tree_node<T>> &st)
	{
		return get_w(st.get());
	}

	template <typename T>
	i64 get_bf(const unique_ptr<binary_tree_node<T>> &st)
	{
		return get_bf(st.get());
	}

	template <typename T, class Compare = nuts::less<T>>
	class binary_tree
	{
		using tree_node = nuts::binary_tree_node<T>;
		using node_ptr = nuts::unique_ptr<tree_node>;
		using node_raw_ptr = binary_tree_node<T> *;

	protected:
		node_ptr root = nullptr;
		u64 _size = 0;
		Compare cmp;

	private:
		template <class vistor>
		void pre_order_trav(node_ptr &st, const vistor &func);
		template <class vistor>
		void in_order_trav(node_ptr &st, const vistor &func);
		template <class vistor>
		void post_order_trav(node_ptr &st, const vistor &func);
		template <class vistor>
		void level_trav_helper(node_ptr &st, const vistor &func);

		node_raw_ptr min(const node_ptr &st) const;
		node_raw_ptr max(const node_ptr &st) const;

	public:
		class const_iterator
		{
			using tree_node = nuts::binary_tree_node<T>;
			using node_ptr = nuts::unique_ptr<tree_node>;
			using node_raw_ptr = binary_tree_node<T> *;

		protected:
			queue<node_raw_ptr> q;

		public:
			const_iterator() = default;
			const_iterator(const node_raw_ptr &src) { q.push(src); }

			const_iterator(const const_iterator &src)
			{
				if (!src.q.empty())
					q.push(src.q.front());
			}
			~const_iterator() { q.clear(); }

			const_iterator &operator++()
			{
				if (q.front() != nullptr)
				{
					if (q.front()->lc != nullptr)
						q.push(get_raw(q.front()->lc));
					if (q.front()->rc != nullptr)
						q.push(get_raw(q.front()->rc));
					q.pop();
				}
				return *this;
			}

			const_iterator operator++(int)
			{
				if (q.empty())
					return const_iterator();
				auto tmp = *this;
				++*this;
				return tmp;
			}

			const_iterator operator+(i64 step)
			{
				auto st = *this;
				while (step > 0)
				{
					st++;
					step--;
				}
				return st;
			}

			const T &operator*() const
			{
				assert(!q.empty());
				return q.front()->data;
			}

			const_iterator &operator=(const const_iterator &src)
			{
				this->q.clear();
				q.push(src.get());
				return *this;
			}

			node_raw_ptr get() { return q.front(); }
			T *operator->() { return &this->get()->data; }
			node_raw_ptr get() const { return q.front(); }
			T *operator->() const { return &this->get()->data; }

			bool operator==(const const_iterator &other) const
			{
				if (this->q.empty() && other.q.empty())
					return true;
				if (!this->q.empty() && !other.q.empty())
					return q.front() == other.q.front();
				return false;
			}

			bool operator!=(const const_iterator &other) const
			{
				return !(*this == other);
			}
		};

		static const_iterator npos; // NULL Position

		binary_tree() : root(nullptr), _size(0) {}
		binary_tree(std::initializer_list<T> ilist);
		virtual ~binary_tree();

		bool empty() const { return _size == 0 && root == nullptr; }
		u64 size() const { return _size; }

		const_iterator begin() const
		{
			if (this->empty())
				return npos;
			return const_iterator(root.get());
		}

		const_iterator end() const { return npos; }

		const T &min() const { return min(this->root)->data; }
		const T &max() const { return max(this->root)->data; }

		virtual bool insert(const T &_val);
		const_iterator insert_ret_pos(const T &_val);
		virtual const_iterator find(const T &_val);
		virtual bool erase(const T &_val);
		void clear();

		template <class vistor>
		void pre_trav(const vistor &func);
		template <class vistor>
		void in_trav(const vistor &func);
		template <class vistor>
		void post_trav(const vistor &func);
		template <class vistor>
		void level_trav(const vistor &func);

		binary_tree(const binary_tree<T, Compare> &src);
		binary_tree<T, Compare> &move(binary_tree<T, Compare> &src);
		binary_tree<T, Compare> &operator=(const binary_tree<T, Compare> &src);

		void printBT(const std::string &prefix, const node_ptr &st, bool isLeft) const;
		void printBT() const;
		void printBT(const const_iterator &st) const;
	};

	template <typename T, class Compare>
	typename binary_tree<T, Compare>::const_iterator
		binary_tree<T, Compare>::npos;

	template <typename T, class Compare>
	binary_tree<T, Compare>::binary_tree(const binary_tree<T, Compare> &src)
	{
		for (auto it = src.begin(); it != src.npos; it++)
		{
			this->insert(*it);
		}
	}

	template <typename T, class Compare>
	binary_tree<T, Compare> &
	binary_tree<T, Compare>::move(binary_tree<T, Compare> &src)
	{
		this->clear();
		this->root = src.root;
		this->_size = src._size;
		src.root = nullptr;
		src._size = 0;
		return *this;
	}

	template <typename T, class Compare>
	binary_tree<T, Compare> &
	binary_tree<T, Compare>::operator=(const binary_tree<T, Compare> &src)
	{
		this->clear();
		binary_tree<T, Compare> copy = src;
		this->move(src);
		return *this;
	}

	template <typename T, class Compare>
	i64 get_w(const typename binary_tree<T, Compare>::const_iterator &st)
	{
		return get_w(st.get());
	}

	template <typename T, class Compare>
	i64 get_bf(const typename binary_tree<T, Compare>::const_iterator &st)
	{
		return get_bf(st.get());
	}

	template <typename T, class Compare>
	binary_tree<T, Compare>::binary_tree(std::initializer_list<T> ilist)
	{
		for (auto it = ilist.begin(); it != ilist.end(); it++)
		{
			this->insert(*it);
		}
	}

	template <typename T, class Compare>
	binary_tree<T, Compare>::~binary_tree() { _size = 0; }

	template <typename T, class Compare>
	void binary_tree<T, Compare>::clear()
	{
		delete root.get();
		root = nullptr;
		_size = 0;
	}

	template <typename T, class Compare>
	bool binary_tree<T, Compare>::insert(const T &_val)
	{
		node_ptr new_node = new tree_node(_val);

		if (root == nullptr) // if the tree is empty, then store value in the root node
		{
			root = new_node;
			++_size;
			return true;
		}

		node_raw_ptr parent = nullptr;
		node_raw_ptr child = root.get();

		while (child != nullptr)
		{
			parent = child;
			if (cmp(_val, child->data))
				child = child->lc.get();
			else if (cmp(child->data, _val))
				child = child->rc.get();
			else // if value already exist, then just return
				return false;
		}

		if (cmp(_val, parent->data))
		{
			parent->lc.move(new_node);
			parent->lc->prev = parent;
		}
		else
		{
			parent->rc.move(new_node);
			parent->rc->prev = parent;
		}
		++_size;
		return true;
	}

	template <typename T, class Compare>
	typename binary_tree<T, Compare>::const_iterator
	binary_tree<T, Compare>::insert_ret_pos(const T &_val)
	{
		node_ptr new_node = new tree_node(_val);

		if (root == nullptr) // if the tree is empty, then store value in the root node
		{
			root = new_node;
			++_size;
			return const_iterator(root.get());
		}

		node_raw_ptr parent = nullptr;
		node_raw_ptr child = root.get();

		while (child != nullptr)
		{
			parent = child;
			if (cmp(_val, child->data))
				child = child->lc.get();
			else if (cmp(child->data, _val))
				child = child->rc.get();
			else // if value already exist, then just return
				return this->npos;
		}

		if (cmp(_val, parent->data))
		{
			parent->lc.move(new_node);
			parent->lc->prev = parent;
			++_size;
			return const_iterator(parent->lc.get());
		}
		else
		{
			parent->rc.move(new_node);
			parent->rc->prev = parent;
			++_size;
			return const_iterator(parent->rc.get());
		}
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::pre_order_trav(node_ptr &st, const vistor &func)
	{
		if (st != nullptr)
		{
			func(st->data);
			pre_order_trav(st->lc, func);
			pre_order_trav(st->rc, func);
		}
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::pre_trav(const vistor &func)
	{
		pre_order_trav(root, func);
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::in_order_trav(node_ptr &st, const vistor &func)
	{
		if (st != nullptr)
		{
			in_order_trav(st->lc, func);
			func(st->data);
			in_order_trav(st->rc, func);
		}
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::in_trav(const vistor &func)
	{
		in_order_trav(root, func);
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::post_order_trav(node_ptr &st, const vistor &func)
	{
		if (st != nullptr)
		{
			post_order_trav(st->lc, func);
			post_order_trav(st->rc, func);
			func(st->data);
		}
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::post_trav(const vistor &func)
	{
		post_order_trav(root, func);
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::level_trav_helper(node_ptr &st, const vistor &func)
	{
		queue<node_raw_ptr> q;
		if (st != nullptr)
			q.push(get_raw(st));
		while (!q.empty())
		{
			if (q.front() != nullptr)
			{
				func(q.front()->data);
				if (q.front()->lc != nullptr)
					q.push(get_raw(q.front()->lc));
				if (q.front()->rc != nullptr)
					q.push(get_raw(q.front()->rc));
				q.pop();
			}
			else
				break;
		}
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::level_trav(const vistor &func)
	{
		level_trav_helper(root, func);
	}

	template <typename T, class Compare>
	typename binary_tree<T, Compare>::const_iterator
	binary_tree<T, Compare>::find(const T &_val)
	{
		binary_tree_node<T> *st = get_raw(root);
		while (st != nullptr)
		{
			if (!cmp(_val, st->data) && !cmp(st->data, _val)) // Found or duplicated
			{
				const_iterator res{st};
				return res;
			}
			if (cmp(_val, st->data)) // Go left
			{
				st = get_raw(st->lc);
				continue;
			}
			if (cmp(st->data, _val)) // Go right
			{
				st = get_raw(st->rc);
				continue;
			}
		}
		return this->npos; // Not Found
	}

	template <typename T, class Compare>
	binary_tree_node<T> *binary_tree<T, Compare>::min(const node_ptr &st) const
	{
		auto res = st.get();
		while (res != nullptr && res->lc != nullptr)
			res = (res->lc).get();
		return res;
	}

	template <typename T, class Compare>
	binary_tree_node<T> *binary_tree<T, Compare>::max(const node_ptr &st) const
	{
		auto res = get_raw(st);
		while (res != nullptr && res->rc != nullptr)
			res = get_raw(res->rc);
		return res;
	}

	template <typename T, class Compare>
	bool binary_tree<T, Compare>::erase(const T &_val)
	{
		auto tmp = find(_val);
		if (tmp == this->npos)
			return false;
		binary_tree_node<T> *target = tmp.get();
		if (target->lc == nullptr && target->rc == nullptr)
		{
			node_ptr Delete_X;
			if (target->prev.get() != nullptr)
			{
				if (target == target->prev->lc.get())
				{
					Delete_X.move(target->prev->lc);
					Delete_X->prev = nullptr;
				}
				else
				{
					Delete_X.move(target->prev->rc);
					Delete_X->prev = nullptr;
				}
			}
			else
			{
				Delete_X.move(this->root);
				this->root = nullptr;
			}
			--_size;
			return true;
		}
		if (target->lc != nullptr && target->rc == nullptr)
		{
			binary_tree_node<T> *L_MAX = max(target->lc);
			node_ptr rec;
			rec.move(L_MAX->lc);
			swap(target->data, L_MAX->data);
			// this->swap(target, L_MAX);
			node_ptr Delete_X;
			if (L_MAX == L_MAX->prev->lc.get())
				Delete_X.move(L_MAX->prev->lc);
			else
				Delete_X.move(L_MAX->prev->rc);
			Delete_X->prev = nullptr;
			if (rec != nullptr)
				target->lc.move(rec);
			rec = nullptr;
			--_size;
			return true;
		}
		if (target->lc == nullptr && target->rc != nullptr)
		{
			binary_tree_node<T> *R_MIN = min(target->rc);
			node_ptr rec;
			rec.move(R_MIN->rc);
			swap(target->data, R_MIN->data);
			// this->swap(target, R_MIN);
			node_ptr Delete_X;
			if (R_MIN == R_MIN->prev->rc.get())
				Delete_X.move(R_MIN->prev->rc);
			else
				Delete_X.move(R_MIN->prev->lc);
			Delete_X->prev = nullptr;
			if (rec != nullptr)
				target->rc.move(rec);
			rec = nullptr;
			--_size;
			return true;
		}
		if (target->lc != nullptr && target->rc != nullptr)
		{
			binary_tree_node<T> *L_MAX = max(target->lc);
			node_ptr rec;
			rec.move(L_MAX->lc);
			swap(target->data, L_MAX->data);
			// this->swap(target, L_MAX);
			node_ptr Delete_X;
			if (L_MAX == L_MAX->prev->lc.get())
				Delete_X.move(L_MAX->prev->lc);
			else
				Delete_X.move(L_MAX->prev->rc);
			Delete_X->prev = nullptr;
			if (rec != nullptr)
				target->lc.move(rec);
			rec = nullptr;
			--_size;
			return true;
		}
		return false;
	}

	template <typename T, class Compare = nuts::less<T>>
	using BST = binary_tree<T, Compare>;

	template <typename T, class Compare = nuts::less<T>>
	class AVL : public binary_tree<T, Compare>
	{
		using tree_node = nuts::binary_tree_node<T>;
		using node_ptr = nuts::unique_ptr<tree_node>;
		using node_raw_ptr = binary_tree_node<T> *;

	private:
		void update_tree_bf();
		void update_tree_bf(node_ptr &ptr);
		void single_rotate_left(node_ptr &ptr);
		void single_rotate_right(node_ptr &ptr);
		void double_rotate_left(node_ptr &ptr);
		void double_rotate_right(node_ptr &ptr);
		void balance(node_ptr &ptr);

	public:
		AVL() { this->root = nullptr, this->_size = 0; }
		AVL(std::initializer_list<T> ilist);
		~AVL() { this->_size = 0; }

		bool insert(const T &_val);
		bool erase(const T &_val);
	};

	template <typename T, class Compare>
	void AVL<T, Compare>::update_tree_bf()
	{
		if (this->root == nullptr)
			return;
		for (auto it = BST<T, Compare>::begin();
			 it != this->npos;
			 it++)
		{
			it.get()->bf = get_bf(it.get());
			if (it.get()->lc != nullptr)
				it.get()->lc->prev = it.get();
			if (it.get()->rc != nullptr)
				it.get()->rc->prev = it.get();
		}
	}

	template <typename T, class Compare>
	void AVL<T, Compare>::update_tree_bf(node_ptr &ptr)
	{
		node_ptr it = ptr;
		while (it != nullptr)
		{
			it->bf = get_bf(it);
			it = it->prev.get();
		}
	}

	template <typename T, class Compare>
	void AVL<T, Compare>::single_rotate_left(unique_ptr<binary_tree_node<T>> &ptr)
	{
		node_raw_ptr foo = ptr->prev.get();
		node_raw_ptr rec = ptr.get();
		node_ptr left;
		left.move(ptr->lc);
		ptr->lc.move(left->rc);
		ptr->bf = get_bf(ptr.get());
		left->bf = get_bf(left.get());
		left->rc.move(ptr);

		if (left->rc->prev != nullptr)
			left->rc->prev = left.get();
		if (left->rc->lc != nullptr)
			left->rc->lc->prev = left->rc.get();

		ptr.move(left);
		ptr->prev = foo;

		if (foo != nullptr)
		{
			if (foo->lc != nullptr && foo->lc.get() == rec)
			{
				foo->lc = ptr.get();
				return;
			}
			if (foo->rc != nullptr && foo->rc.get() == rec)
			{
				foo->rc = ptr.get();
				return;
			}
		}
		else
			this->root = ptr.get();
	}

	template <typename T, class Compare>
	void AVL<T, Compare>::single_rotate_right(unique_ptr<binary_tree_node<T>> &ptr)
	{
		node_raw_ptr foo = ptr->prev.get();
		node_raw_ptr rec = ptr.get();
		node_ptr right;
		right.move(ptr->rc);
		ptr->rc.move(right->lc);
		ptr->bf = get_bf(ptr.get());
		right->bf = get_bf(right.get());
		right->lc.move(ptr);

		if (right->lc->prev != nullptr)
			right->lc->prev = right.get();
		if (right->lc->rc != nullptr)
			right->lc->rc->prev = right->lc.get();

		ptr.move(right);
		ptr->prev = foo;

		if (foo != nullptr)
		{
			if (foo->lc != nullptr && foo->lc.get() == rec)
			{
				foo->lc = ptr.get();
				return;
			}
			if (foo->rc != nullptr && foo->rc.get() == rec)
			{
				foo->rc = ptr.get();
				return;
			}
		}
		else
			this->root = ptr.get();
	}

	template <typename T, class Compare>
	void AVL<T, Compare>::double_rotate_left(unique_ptr<binary_tree_node<T>> &ptr)
	{
		single_rotate_right(ptr->lc);
		single_rotate_left(ptr);
	}

	template <typename T, class Compare>
	void AVL<T, Compare>::double_rotate_right(unique_ptr<binary_tree_node<T>> &ptr)
	{
		single_rotate_left(ptr->rc);
		single_rotate_right(ptr);
	}

	template <typename T, class Compare>
	void AVL<T, Compare>::balance(node_ptr &ptr)
	{
		if (ptr == nullptr)
			return;
		if (get_w(ptr->lc) >= get_w(ptr->rc) + 1)
		{
			if (get_w(ptr->lc->lc) >= get_w(ptr->lc->rc))
				single_rotate_left(ptr);
			else
				double_rotate_left(ptr);
		}
		else if (get_w(ptr->rc) >= get_w(ptr->lc) + 1)
		{
			if (get_w(ptr->rc->rc) >= get_w(ptr->rc->lc))
				single_rotate_right(ptr);
			else
				double_rotate_right(ptr);
		}
		this->update_tree_bf(ptr);
	}

	template <typename T, class Compare>
	bool AVL<T, Compare>::insert(const T &_val)
	{
		auto option = BST<T, Compare>::insert_ret_pos(_val);
		if (option != this->npos)
		{
			this->update_tree_bf();
			node_ptr loc = option.get();
			while (loc != nullptr)
			{
				if (loc->bf == 2 || loc->bf == -2)
					break;
				loc = loc->prev.get();
			}
			if (loc != nullptr)
			{
				balance(loc);
				loc = nullptr;
			}
			return true;
		}
		else
			return false;
	}

	template <typename T, class Compare>
	AVL<T, Compare>::AVL(std::initializer_list<T> ilist)
	{
		for (auto it = ilist.begin(); it != ilist.end(); it++)
		{
			AVL<T>::insert(*it);
		}
	}

	template <typename T, class Compare>
	bool AVL<T, Compare>::erase(const T &_val)
	{
		if (BST<T, Compare>::erase(_val))
		{
			if (this->root == nullptr)
				return true;
			this->update_tree_bf();
			node_ptr loc = nullptr;
			for (auto it = this->begin(); it != this->npos; ++it)
			{
				if (it.get()->bf == 2 || it.get()->bf == -2)
				{
					loc = it.get();
					balance(loc);
					loc = nullptr;
					return true;
				}
			}
			return true;
		}
		else
			return false;
	}

	template <typename T, class Compare>
	void binary_tree<T, Compare>::printBT(const std::string &prefix,
										  const unique_ptr<binary_tree_node<T>> &st,
										  bool isLeft) const
	{
		if (st != nullptr)
		{
			std::cout << prefix;
			if (st == this->root.get())
				std::cout << "\n#── ";
			else
				std::cout << (isLeft ? "├── " : "└── ");

			std::cout << st->data << "(" << (i32)st->bf << ")" << std::endl; // print the value of the node
			// std::cout << st->data << std::endl; // print the value of the node
			printBT(prefix + (isLeft ? "│   " : "    "), st->lc, true);
			printBT(prefix + (isLeft ? "│   " : "    "), st->rc, false);
		}
	}

	template <typename T, class Compare>
	void binary_tree<T, Compare>::printBT() const
	{
		if (this->root != nullptr)
			printBT("", this->root, false);
		else
			std::cout << "\n# ∧" << std::endl;
	}

	template <typename T, class Compare>
	void binary_tree<T, Compare>::printBT(const const_iterator &st) const
	{
		if (st != npos)
		{
			std::cout << std::endl;
			node_ptr k = st.get();
			printBT("", k, false);
			k = nullptr;
		}
		else
			std::cout << "\n└── ∧" << std::endl;
	}
}

#endif
