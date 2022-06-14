#ifndef _NUTS_BIN_TREE_
#define _NUTS_BIN_TREE_

#include "type.h"
#include "queue.h"
#include "memory.h"
#include "algorithm.h"

namespace nuts
{
	template <typename T>
	struct binary_tree_node
	{
		using tree_node = nuts::binary_tree_node<T>;
		using node_ptr = nuts::unique_ptr<tree_node>;
		using node_raw_ptr = binary_tree_node<T> *;

		T data;
		i64 bf = 0;
		node_ptr prev, lc, rc;

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

	template <typename T>
	class binary_tree
	{
		using tree_node = nuts::binary_tree_node<T>;
		using node_ptr = nuts::unique_ptr<tree_node>;
		using node_raw_ptr = binary_tree_node<T> *;

	protected:
		node_ptr root = nullptr;
		u64 _size = 0;

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
			i64 index = 0;
			queue<node_raw_ptr> q;

		public:
			const_iterator() = default;
			const_iterator(const node_raw_ptr &src, const i64 i = 0)
				: index(i) { q.push(src); }

			const_iterator(const const_iterator &src)
			{
				index = src.get_index();
				if (!src.q.empty())
					q.push(src.q.front());
			}

			const_iterator(i64 i) : index(i) {}
			~const_iterator() { index = 0; }

			const_iterator &operator++()
			{
				if (q.front() != nullptr)
				{
					if (q.front()->lc != nullptr)
						q.push(get_raw(q.front()->lc));
					if (q.front()->rc != nullptr)
						q.push(get_raw(q.front()->rc));
					q.pop();
					index++;
				}
				return *this;
			}

			const_iterator operator++(int)
			{
				if (q.empty())
					return const_iterator{-1};
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

			const T &operator*() const { return q.front()->data; }
			const node_raw_ptr get() const { return q.front(); }
			constexpr i64 get_index() const { return index; }

			const T *operator->() { return this->get()->data; }

			bool operator==(const const_iterator &other) const
			{
				if (this->q.empty() && other.q.empty())
					return true;
				return index == other.index;
			}

			bool operator!=(const const_iterator &other) const
			{
				return !(*this == other);
			}
		};

		const const_iterator npos{-1};

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

		const T &min() { return min(this->root)->data; }
		const T &max() { return max(this->root)->data; }

		bool insert(const T &_val);
		const_iterator search(const T &_val);
		bool erase(const T &_val);
		void clear();

		template <class vistor>
		void pre_trav(const vistor &func);

		template <class vistor>
		void in_trav(const vistor &func);

		template <class vistor>
		void post_trav(const vistor &func);

		template <class vistor>
		void level_trav(const vistor &func);
	};

	template <typename T>
	binary_tree<T>::binary_tree(std::initializer_list<T> ilist)
	{
		for (auto it = ilist.begin(); it != ilist.end(); it++)
		{
			this->insert(*it);
		}
	}

	template <typename T>
	binary_tree<T>::~binary_tree() { _size = 0; }

	template <typename T>
	void binary_tree<T>::clear()
	{
		delete root.get();
		root = nullptr;
		_size = 0;
	}

	template <typename T>
	bool binary_tree<T>::insert(const T &_val)
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
			if (_val < child->data)
			{
				child = child->lc.get();
			}
			else if (_val > child->data)
			{
				child = child->rc.get();
			}
			else // if value already exist, then just return
			{
				return false;
			}
		}

		if (_val < parent->data)
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

	template <typename T>
	template <class vistor>
	void binary_tree<T>::pre_order_trav(node_ptr &st, const vistor &func)
	{
		if (st != nullptr)
		{
			func(st->data);
			pre_order_trav(st->lc, func);
			pre_order_trav(st->rc, func);
		}
	}

	template <typename T>
	template <class vistor>
	void binary_tree<T>::pre_trav(const vistor &func)
	{
		pre_order_trav(root, func);
	}

	template <typename T>
	template <class vistor>
	void binary_tree<T>::in_order_trav(node_ptr &st, const vistor &func)
	{
		if (st != nullptr)
		{
			in_order_trav(st->lc, func);
			func(st->data);
			in_order_trav(st->rc, func);
		}
	}

	template <typename T>
	template <class vistor>
	void binary_tree<T>::in_trav(const vistor &func)
	{
		in_order_trav(root, func);
	}

	template <typename T>
	template <class vistor>
	void binary_tree<T>::post_order_trav(node_ptr &st, const vistor &func)
	{
		if (st != nullptr)
		{
			post_order_trav(st->lc, func);
			post_order_trav(st->rc, func);
			func(st->data);
		}
	}

	template <typename T>
	template <class vistor>
	void binary_tree<T>::post_trav(const vistor &func)
	{
		post_order_trav(root, func);
	}

	template <typename T>
	template <class vistor>
	void binary_tree<T>::level_trav_helper(node_ptr &st, const vistor &func)
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

	template <typename T>
	template <class vistor>
	void binary_tree<T>::level_trav(const vistor &func)
	{
		level_trav_helper(root, func);
	}

	template <typename T>
	typename binary_tree<T>::const_iterator binary_tree<T>::search(const T &_val)
	{
		for (auto it = this->begin(); it != this->npos; it++)
		{
			if (*it == _val)
				return const_iterator(it.get(), it.get_index());
		}
		return this->npos;

		// binary_tree_node<T> *st = get_raw(root);
		// while (st)
		// {
		// 	if (_val == st->data)
		// 	{
		// 		return st;
		// 	}
		// 	if (_val < st->data)
		// 	{
		// 		st = get_raw(st->lc);
		// 		continue;
		// 	}
		// 	if (_val > st->data)
		// 	{
		// 		st = get_raw(st->rc);
		// 		continue;
		// 	}
		// }
		// return st;
	}

	template <typename T>
	binary_tree_node<T> *binary_tree<T>::min(const node_ptr &st) const
	{
		auto res = st.get();
		while (res != nullptr && res->lc != nullptr)
			res = (res->lc).get();
		return res;
	}

	template <typename T>
	binary_tree_node<T> *binary_tree<T>::max(const node_ptr &st) const
	{
		auto res = get_raw(st);
		while (res != nullptr && res->rc != nullptr)
			res = get_raw(res->rc);
		return res;
	}

	template <typename T>
	bool binary_tree<T>::erase(const T &_val)
	{
		binary_tree_node<T> *target = search(_val);
		if (target == nullptr)
		{
			return false;
		}
		if (target->lc == nullptr && target->rc == nullptr)
		{
			node_ptr Delete_X;
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
			--_size;
			return true;
		}
		if (target->lc != nullptr && target->rc == nullptr)
		{
			binary_tree_node<T> *L_MAX = max(target->lc);
			swap(target->data, L_MAX->data);
			node_ptr Delete_X;
			if (L_MAX == L_MAX->prev->lc.get())
				Delete_X.move(L_MAX->prev->lc);
			else
				Delete_X.move(L_MAX->prev->rc);
			Delete_X->prev = nullptr;
			--_size;
			return true;
		}
		if (target->lc == nullptr && target->rc != nullptr)
		{
			binary_tree_node<T> *R_MIN = min(target->rc);
			swap(target->data, R_MIN->data);
			node_ptr Delete_X;
			if (R_MIN == R_MIN->prev->rc.get())
				Delete_X.move(R_MIN->prev->rc);
			else
				Delete_X.move(R_MIN->prev->lc);
			Delete_X->prev = nullptr;
			--_size;
			return true;
		}
		if (target->lc != nullptr && target->rc != nullptr)
		{
			binary_tree_node<T> *L_MAX = max(target->lc);
			swap(target->data, L_MAX->data);
			node_ptr Delete_X;
			if (L_MAX == L_MAX->prev->lc.get())
				Delete_X.move(L_MAX->prev->lc);
			else
				Delete_X.move(L_MAX->prev->rc);
			Delete_X->prev = nullptr;
			--_size;
			return true;
		}
		return false;
	}

	template <typename T>
	using BST = binary_tree<T>;

	template <typename T>
	class AVL : public binary_tree<T>
	{
		using tree_node = nuts::binary_tree_node<T>;
		using node_ptr = nuts::unique_ptr<tree_node>;
		using node_raw_ptr = binary_tree_node<T> *;

	private:
		void update_tree_bf();
		void single_rotate_left(node_ptr &ptr);
		void single_rotate_right(node_ptr &ptr);
		void double_rotate_left(node_ptr &ptr);
		void double_rotate_right(node_ptr &ptr);

	public:
		AVL() { this->root = nullptr, this->_size = 0; }
		AVL(std::initializer_list<T> ilist);
		~AVL() { this->_size = 0; }

		void balance(node_ptr &ptr);
		bool insert(const T &_val);
		bool erase(const T &_val);
	};

	template <typename T>
	void AVL<T>::update_tree_bf()
	{
		for (auto it = BST<T>::begin(); it != this->npos; it++)
		{
			it.get()->bf = get_bf(it.get());
		}
	}

	template <typename T>
	void AVL<T>::single_rotate_left(unique_ptr<binary_tree_node<T>> &ptr)
	{
		using node_ptr = unique_ptr<binary_tree_node<T>>;
		using node_raw_ptr = binary_tree_node<T> *;

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
		{
			this->root = ptr.get();
		}
	}

	template <typename T>
	void AVL<T>::single_rotate_right(unique_ptr<binary_tree_node<T>> &ptr)
	{
		using node_ptr = unique_ptr<binary_tree_node<T>>;
		using node_raw_ptr = binary_tree_node<T> *;

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
		{
			this->root = ptr.get();
		}
	}

	template <typename T>
	void AVL<T>::double_rotate_left(unique_ptr<binary_tree_node<T>> &ptr)
	{
		single_rotate_right(ptr->lc);
		single_rotate_left(ptr);
	}

	template <typename T>
	void AVL<T>::double_rotate_right(unique_ptr<binary_tree_node<T>> &ptr)
	{
		single_rotate_left(ptr->rc);
		single_rotate_right(ptr);
	}

	template <typename T>
	void AVL<T>::balance(node_ptr &ptr)
	{
		if (ptr == nullptr)
			return;
		if (get_w(ptr->lc) >= get_w(ptr->rc) + 1)
		{
			if (get_w(ptr->lc->lc) >= get_w(ptr->lc->rc))
			{
				single_rotate_left(ptr);
			}
			else
			{
				double_rotate_left(ptr);
			}
		}
		else if (get_w(ptr->rc) >= get_w(ptr->lc) + 1)
		{
			if (get_w(ptr->rc->rc) >= get_w(ptr->rc->lc))
			{
				single_rotate_right(ptr);
			}
			else
			{
				double_rotate_right(ptr);
			}
		}
		ptr->bf = get_bf(ptr);
	}

	template <typename T>
	bool AVL<T>::insert(const T &_val)
	{
		if (BST<T>::insert(_val))
		{
			update_tree_bf();
			node_raw_ptr at = BST<T>::search(_val).get();
			node_raw_ptr loc = nullptr;
			while (at)
			{
				if (at->bf >= 2 || at->bf <= -2)
				{
					loc = at;
					break;
				}
				at = at->prev.get();
			}
			node_ptr zs = loc;
			if (loc != nullptr)
				balance(zs);
			zs = nullptr;
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	AVL<T>::AVL(std::initializer_list<T> ilist)
	{
		for (auto it = ilist.begin(); it != ilist.end(); it++)
		{
			AVL<T>::insert(*it);
		}
		this->update_tree_bf();
	}

	// template <typename T>
	// AVL<T>::AVL(std::initializer_list<T> ilist)
	// {
	// 	for (auto it = ilist.begin(); it != ilist.end(); it++)
	// 	{
	// 		BST<T>::insert(*it);
	// 	}
	// 	this->update_tree_bf();
	// }

	//
	//	template <typename T>
	//	bool AVL<T>::erase(const T &_val)
	//	{
	//		if (BST<T>::erase(_val))
	//		{
	//			balance(this->root);
	//			return true;
	//		}
	//		else
	//		{
	//			return false;
	//		}
	//	}

}
#endif
