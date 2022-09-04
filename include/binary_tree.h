#ifndef _NUTS_BIN_TREE_
#define _NUTS_BIN_TREE_ 1

#include "algorithm.h"
#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "queue.h"
#include "type.h"
#include <cassert>

namespace nuts
{
	template <typename T>
	struct binary_tree_node
	{
		using tree_node = nuts::binary_tree_node<T>;
		using node_ptr = nuts::unique_ptr<tree_node>;

		T data;
		i8 bf = 0;
		node_ptr prev = nullptr, lc = nullptr, rc = nullptr;

		binary_tree_node() : prev(nullptr), lc(nullptr), rc(nullptr) {}
		explicit binary_tree_node(const T& _val) : data(_val),
		                                           prev(nullptr),
		                                           lc(nullptr), rc(nullptr) {}
		explicit binary_tree_node(T&& _val) : data(static_cast<T&&>(_val)),
		                                      prev(nullptr),
		                                      lc(nullptr), rc(nullptr) {}
		binary_tree_node(const tree_node& obj) : data(obj.data),
		                                         prev(obj.prev),
		                                         lc(obj.lc), rc(obj.rc) {}
		~binary_tree_node() { prev = nullptr; }
	};

	template <typename T>
	i8 get_w(const binary_tree_node<T>* st)
	{
		if (st == nullptr)
			return -1;
		if (st->lc == nullptr && st->rc == nullptr)
			return 0;
		return max(get_w(st->lc.get()), get_w(st->rc.get())) + 1;
	}

	template <typename T>
	i8 get_bf(const binary_tree_node<T>* st)
	{
		return get_w(st->lc.get()) - get_w(st->rc.get());
	}

	template <typename T>
	i8 get_w(const unique_ptr<binary_tree_node<T>>& st)
	{
		return get_w(st.get());
	}

	template <typename T>
	i8 get_bf(const unique_ptr<binary_tree_node<T>>& st)
	{
		return get_bf(st.get());
	}

	template <typename T, class Compare = nuts::less<T>>
	class binary_tree
	{
	public:
		using value_type = T;
		using tree_node = nuts::binary_tree_node<T>;
		using node_ptr = nuts::unique_ptr<tree_node>;
		using node_raw_ptr = binary_tree_node<T>*;

	private:
		template <class vistor>
		void pre_order_trav(node_ptr& st, const vistor& func);
		template <class vistor>
		void in_order_trav(node_ptr& st, const vistor& func);
		template <class vistor>
		void post_order_trav(node_ptr& st, const vistor& func);
		template <class vistor>
		void level_trav_helper(node_ptr& st, const vistor& func);

		static node_raw_ptr min(const node_ptr& st);
		static node_raw_ptr max(const node_ptr& st);

	public:
		class iterator
		    : public bidirectional_iterator
		{
		public:
			using value_type = T;
			using tree_node = binary_tree_node<T>;
			using node_ptr = unique_ptr<tree_node>;
			using node_raw_ptr = binary_tree_node<T>*;

		protected:
			node_raw_ptr _ptr = nullptr;

		public:
			iterator() = default;
			explicit iterator(node_raw_ptr obj) : _ptr(obj) {}
			explicit iterator(const node_ptr& obj) : _ptr(obj.get()) {}
			iterator(const iterator& obj) : _ptr(obj._ptr) {}

			iterator& operator++()
			{
				node_raw_ptr p;
				if (_ptr == nullptr)
				{
					if (_ptr == nullptr)
						return const_cast<iterator&>(npos);
				}
				else if (_ptr->rc != nullptr)
				{
					_ptr = _ptr->rc.get();
					while (_ptr->lc != nullptr)
						_ptr = _ptr->lc.get();
				}
				else
				{
					p = _ptr->prev.get();
					while (p != nullptr && _ptr == p->rc.get())
					{
						_ptr = p;
						p = p->prev.get();
					}
					_ptr = p;
				}
				return *this;
			}

			iterator operator++(int)
			{
				auto res = *this;
				++(*this);
				return res;
			}

			iterator& operator--()
			{
				node_raw_ptr p;
				if (_ptr == nullptr)
				{
					if (_ptr == nullptr)
						return const_cast<iterator&>(npos);
				}
				else if (_ptr->lc != nullptr)
				{
					_ptr = _ptr->lc.get();
					while (_ptr->rc != nullptr)
						_ptr = _ptr->rc.get();
				}
				else
				{
					p = _ptr->prev.get();
					while (p != nullptr && _ptr == p->lc.get())
					{
						_ptr = p;
						p = p->prev.get();
					}
					_ptr = p;
				}
				return *this;
			}

			iterator operator--(int)
			{
				auto res = *this;
				--(*this);
				return res;
			}

			iterator operator+(i64 n) { return advance(*this, n); }
			iterator operator-(i64 n) { return advance(*this, -1 * n); }

			void operator+=(i64 n) { *this = advance(*this, n); }
			void operator-=(i64 n) { *this = advance(*this, -1 * n); }

			T& operator*() { return _ptr->data; }
			const T& operator*() const { return _ptr->data; }

			iterator& operator=(const iterator& src)
			{
				_ptr = src._ptr;
				return *this;
			}

			node_raw_ptr get() const { return _ptr; }
			T* operator->() const { return &_ptr->data; }

			bool operator==(const iterator& other)
			        const { return _ptr == other._ptr; }

			bool operator!=(const iterator& other)
			        const { return _ptr != other._ptr; }
		};

		using itr_type = typename binary_tree<T, Compare>::iterator;

		binary_tree() : root(nullptr), _size(0) {}
		binary_tree(const std::initializer_list<T>& ilist);
		binary_tree(const binary_tree<T, Compare>& src);
		binary_tree(binary_tree<T, Compare>&& src) { move(src); }
		~binary_tree();

		iterator begin() const
		{
			if (empty()) return npos;
			else
				return iterator {min(root)};
		}

		iterator end() const
		{
			if (empty()) return npos;
			else
				return iterator {max(root)};
		}

		T& front() { return *begin(); }
		T& back() { return *end(); }
		const T& front() const { return *begin(); }
		const T& back() const { return *end(); }

		bool empty() const { return _size == 0 && root == nullptr; }
		u64 size() const { return _size; }

		bool insert(const T& _val);
		auto insert_ret_pos(const T& _val) -> iterator;
		bool insert(T&& _val);
		auto insert_ret_pos(T&& _val) -> iterator;

		bool erase(const T& _val);
		auto erase_ret_pos(const T& _val) -> iterator;

		auto find(const T& _val) const -> iterator;
		bool contains(const T& _val) const { return find(_val) != npos; };
		auto lower_bound(const T& _val) const -> iterator;
		auto upper_bound(const T& _val) const -> iterator;

		binary_tree<T, Compare>& move(binary_tree<T, Compare>& src);
		binary_tree<T, Compare>& operator=(const binary_tree<T, Compare>& src);
		binary_tree<T, Compare>& operator=(binary_tree<T, Compare>&& src) { return move(src); }

		void print_as_tree() const;
		void clear();

	private:
		template <class vistor>
		void pre_trav(const vistor& func);
		template <class vistor>
		void in_trav(const vistor& func);
		template <class vistor>
		void post_trav(const vistor& func);
		template <class vistor>
		void level_trav(const vistor& func);

		void printBT(const std::string& prefix, const node_ptr& st, bool isLeft) const;
		void printBT(const iterator& st) const;

	protected:
		node_ptr root = nullptr;
		u64 _size = 0;
		constexpr static const Compare cmp {};

	public:
		constexpr static const itr_type npos {};
	};

	template <typename T, class Compare>
	binary_tree_node<T>* binary_tree<T, Compare>::min(const node_ptr& st)
	{
		auto res = get_raw(st);
		while (res != nullptr && res->lc != nullptr)
			res = get_raw(res->lc);
		return res;
	}

	template <typename T, class Compare>
	binary_tree_node<T>* binary_tree<T, Compare>::max(const node_ptr& st)
	{
		auto res = get_raw(st);
		while (res != nullptr && res->rc != nullptr)
			res = get_raw(res->rc);
		return res;
	}

	template <typename T, class Compare>
	binary_tree<T, Compare>::binary_tree(const binary_tree<T, Compare>& src)
	{
		for_each(src, [this](const auto& x) { insert(x); });
	}

	template <typename T, class Compare>
	binary_tree<T, Compare>&
	binary_tree<T, Compare>::move(binary_tree<T, Compare>& src)
	{
		clear();
		root = src.root;
		_size = src._size;
		src.root = nullptr;
		src._size = 0;
		return *this;
	}

	template <typename T, class Compare>
	binary_tree<T, Compare>&
	binary_tree<T, Compare>::operator=(const binary_tree<T, Compare>& src)
	{
		clear();
		binary_tree<T, Compare> copy = src;
		move(copy);
		return *this;
	}

	template <typename T, class Compare>
	i64 get_w(const typename binary_tree<T, Compare>::iterator& st)
	{
		return get_w(st.get());
	}

	template <typename T, class Compare>
	i64 get_bf(const typename binary_tree<T, Compare>::iterator& st)
	{
		return get_bf(st.get());
	}

	template <typename T, class Compare>
	binary_tree<T, Compare>::binary_tree(const std::initializer_list<T>& ilist)
	{
		for (const auto& x: ilist) insert(x);
	}

	template <typename T, class Compare>
	inline binary_tree<T, Compare>::~binary_tree()
	{
		clear();
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::
	        pre_order_trav(node_ptr& st, const vistor& func)
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
	void binary_tree<T, Compare>::pre_trav(const vistor& func)
	{
		pre_order_trav(root, func);
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::
	        in_order_trav(node_ptr& st, const vistor& func)
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
	void binary_tree<T, Compare>::in_trav(const vistor& func)
	{
		in_order_trav(root, func);
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::
	        post_order_trav(node_ptr& st, const vistor& func)
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
	void binary_tree<T, Compare>::post_trav(const vistor& func)
	{
		post_order_trav(root, func);
	}

	template <typename T, class Compare>
	template <class vistor>
	void binary_tree<T, Compare>::
	        level_trav_helper(node_ptr& st, const vistor& func)
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
	void binary_tree<T, Compare>::level_trav(const vistor& func)
	{
		level_trav_helper(root, func);
	}

	template <typename T, class Compare>
	void binary_tree<T, Compare>::clear()
	{
		delete root.get();
		root = nullptr;
		_size = 0;
	}

	template <typename T, class Compare>
	auto binary_tree<T, Compare>::lower_bound(const T& _val) const
	        -> itr_type
	{
		nuts::equal<T> eqr;
		for (auto it = begin(); it != npos; ++it)
		{
			if (eqr(_val, *it))
				return it;
		}
		return npos;
	}

	template <typename T, class Compare>
	auto binary_tree<T, Compare>::upper_bound(const T& _val) const
	        -> itr_type
	{
		nuts::greater<T> gtr;
		for (auto it = begin(); it != npos; ++it)
		{
			if (gtr(_val, *it))
				return it;
		}
		return npos;
	}

	template <typename T, class Compare>
	typename binary_tree<T, Compare>::iterator
	binary_tree<T, Compare>::find(const T& _val) const
	{
		node_raw_ptr st = root.get();
		while (st != nullptr)
		{
			if (!cmp(_val, st->data) &&
			    !cmp(st->data, _val))// Found or duplicated
			{
				return iterator {st};
			}
			if (cmp(_val, st->data))// Go left
			{
				st = get_raw(st->lc);
				continue;
			}
			if (cmp(st->data, _val))// Go right
			{
				st = get_raw(st->rc);
				continue;
			}
		}
		return npos;// Not Found
	}

	template <typename T, class Compare>
	bool binary_tree<T, Compare>::insert(const T& _val)
	{
		return insert_ret_pos(_val) != npos;
	}

	template <typename T, class Compare>
	auto binary_tree<T, Compare>::insert_ret_pos(const T& _val)
	        -> itr_type
	{
		node_ptr new_node(new tree_node(_val));

		if (root == nullptr)// if the tree is empty, then store value in the root node
		{
			root.move(new_node);
			++_size;
			return iterator {root.get()};
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
			else// if value already exist, then just return
				return npos;
		}

		if (cmp(_val, parent->data))
		{
			parent->lc.move(new_node);
			parent->lc->prev = parent;
			++_size;
			return iterator {parent->lc.get()};
		}
		else
		{
			parent->rc.move(new_node);
			parent->rc->prev = parent;
			++_size;
			return iterator {parent->rc.get()};
		}
	}

	template <typename T, class Compare>
	bool binary_tree<T, Compare>::insert(T&& _val)
	{
		return insert_ret_pos(_val) != npos;
	}

	template <typename T, class Compare>
	auto binary_tree<T, Compare>::insert_ret_pos(T&& _val)
	        -> itr_type
	{
		node_ptr new_node(new tree_node {static_cast<T&&>(_val)});

		if (root == nullptr)// if the tree is empty, then store value in the root node
		{
			root.move(new_node);
			++_size;
			return iterator {root.get()};
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
			else
				return npos;
		}

		if (cmp(_val, parent->data))
		{
			parent->lc.move(new_node);
			parent->lc->prev = parent;
			++_size;
			return iterator {parent->lc.get()};
		}
		else
		{
			parent->rc.move(new_node);
			parent->rc->prev = parent;
			++_size;
			return iterator {parent->rc.get()};
		}
	}

	template <typename T, class Compare>
	bool binary_tree<T, Compare>::erase(const T& _val)
	{
		u64 before = size();
		erase_ret_pos(_val);
		return size() < before;
	}

	template <typename T, class Compare>
	auto binary_tree<T, Compare>::erase_ret_pos(const T& _val)
	        -> itr_type
	{
		auto tmp = find(_val);
		if (tmp == npos) return npos;
		iterator res {tmp.get()->prev.get()};
		node_raw_ptr target = tmp.get();
		if (target->lc == nullptr && target->rc == nullptr)
		{
			node_ptr Delete_X;
			if (target->prev != nullptr)
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
				Delete_X.move(root);
				root = nullptr;
			}
			--_size;
			return res;
		}
		if (target->lc != nullptr && target->rc == nullptr)
		{
			target->lc->prev = target;
			node_raw_ptr L_MAX = max(target->lc);
			node_ptr rec;
			rec.move(L_MAX->lc);
			swap(target->data, L_MAX->data);
			node_ptr Delete_X;
			if (L_MAX == L_MAX->prev->lc.get())
				Delete_X.move(L_MAX->prev->lc);
			else
				Delete_X.move(L_MAX->prev->rc);
			Delete_X->prev = nullptr;
			if (rec != nullptr)
			{
				target->lc.move(rec);
				target->lc->prev = target;
			}
			rec = nullptr;
			--_size;
			return res;
		}
		if (target->lc == nullptr && target->rc != nullptr)
		{
			target->rc->prev = target;
			node_raw_ptr R_MIN = min(target->rc);
			node_ptr rec;
			rec.move(R_MIN->rc);
			swap(target->data, R_MIN->data);
			node_ptr Delete_X;
			if (R_MIN == R_MIN->prev->rc.get())
				Delete_X.move(R_MIN->prev->rc);
			else
				Delete_X.move(R_MIN->prev->lc);
			Delete_X->prev = nullptr;
			if (rec != nullptr)
			{
				target->rc.move(rec);
				target->rc->prev = target;
			}
			rec = nullptr;
			--_size;
			return res;
		}
		if (target->lc != nullptr && target->rc != nullptr)
		{
			target->lc->prev = target;
			target->rc->prev = target;
			node_raw_ptr L_MAX = max(target->lc);
			node_ptr rec;
			rec.move(L_MAX->lc);
			swap(target->data, L_MAX->data);
			node_ptr Delete_X;
			if (L_MAX == L_MAX->prev->lc.get())
				Delete_X.move(L_MAX->prev->lc);
			else
				Delete_X.move(L_MAX->prev->rc);
			Delete_X->prev = nullptr;
			if (rec != nullptr)
			{
				target->lc.move(rec);
				target->lc->prev = target;
			}
			rec = nullptr;
			--_size;
			return res;
		}
		return npos;
	}

	template <typename T, class Compare = nuts::less<T>>
	using BST = binary_tree<T, Compare>;

	template <typename T, class Compare = nuts::less<T>>
	class AVL : public BST<T, Compare>
	{
		using tree_node = nuts::binary_tree_node<T>;
		using node_ptr = nuts::unique_ptr<tree_node>;
		using node_raw_ptr = binary_tree_node<T>*;
		using itr_type = typename BST<T, Compare>::iterator;

	private:
		void single_rotate_left(node_ptr& ptr);
		void single_rotate_right(node_ptr& ptr);
		void double_rotate_left(node_ptr& ptr);
		void double_rotate_right(node_ptr& ptr);
		void update_upon(const node_ptr& ptr);
		void balance(node_ptr& ptr);

	public:
		AVL() { this->root = nullptr, this->_size = 0; }
		AVL(const std::initializer_list<T>& ilist);
		AVL(const AVL<T, Compare>& src);
		AVL(AVL<T, Compare>&& src) { BST<T, Compare>::move(src); }
		~AVL() { this->_size = 0; }

		bool insert(const T& _val);
		bool insert(T&& _val);
		bool erase(const T& _val);

		itr_type insert_ret_pos(const T& _val);
		itr_type insert_ret_pos(T&& _val);
		itr_type erase_ret_pos(const T& _val);

		AVL<T, Compare>& operator=(const AVL<T, Compare>& src);
		AVL<T, Compare>& operator=(AVL<T, Compare>&& src) { return BST<T, Compare>::move(src); }
	};

	template <typename T, class Compare>
	AVL<T, Compare>::AVL(const AVL<T, Compare>& src)
	{
		for_each(src, [this](const T& x) { this->insert(x); });
	}

	template <typename T, class Compare>
	AVL<T, Compare>::AVL(const std::initializer_list<T>& ilist)
	{
		for (const auto& x: ilist) insert(x);
	}

	template <typename T, class Compare>
	AVL<T, Compare>& AVL<T, Compare>::operator=(const AVL<T, Compare>& src)
	{
		AVL<T, Compare> copy(src);
		BST<T, Compare>::move(copy);
		return *this;
	}

	template <typename T, class Compare>
	void AVL<T, Compare>::update_upon(const node_ptr& ptr)
	{
		if (ptr == nullptr) return;
		node_ptr it {ptr.get()};
		while (it != nullptr)
		{
			if (it->lc != nullptr)
				it->lc->prev = it.get();
			if (it->rc != nullptr)
				it->rc->prev = it.get();

			it->bf = get_bf(it);
			it = it->prev.get();
		}
		it = nullptr;
	}

	template <typename T, class Compare>
	void AVL<T, Compare>::
	        single_rotate_left(node_ptr& ptr)
	{
		node_raw_ptr foo = ptr->prev.get();
		node_raw_ptr rec = ptr.get();
		node_ptr left;
		left.move(ptr->lc);
		ptr->lc.move(left->rc);
		ptr->bf = get_bf(ptr);
		left->bf = get_bf(left);
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
	void AVL<T, Compare>::
	        single_rotate_right(node_ptr& ptr)
	{
		node_raw_ptr foo = ptr->prev.get();
		node_raw_ptr rec = ptr.get();
		node_ptr right;
		right.move(ptr->rc);
		ptr->rc.move(right->lc);
		ptr->bf = get_bf(ptr);
		right->bf = get_bf(right);
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
	void AVL<T, Compare>::
	        double_rotate_left(node_ptr& ptr)
	{
		single_rotate_right(ptr->lc);
		single_rotate_left(ptr);
	}

	template <typename T, class Compare>
	void AVL<T, Compare>::
	        double_rotate_right(node_ptr& ptr)
	{
		single_rotate_left(ptr->rc);
		single_rotate_right(ptr);
	}

	template <typename T, class Compare>
	void AVL<T, Compare>::balance(node_ptr& ptr)
	{
		if (ptr == nullptr) return;
		if (get_w(ptr->rc) < get_w(ptr->lc))
		{
			if (get_w(ptr->lc->lc) >= get_w(ptr->lc->rc))
				single_rotate_left(ptr);
			else
				double_rotate_left(ptr);
		}
		else if (get_w(ptr->lc) < get_w(ptr->rc))
		{
			if (get_w(ptr->rc->rc) >= get_w(ptr->rc->lc))
				single_rotate_right(ptr);
			else
				double_rotate_right(ptr);
		}
		this->update_upon(ptr);
	}

	template <typename T, class Compare>
	bool AVL<T, Compare>::insert(const T& _val)
	{
		return insert_ret_pos(_val) != this->npos;
	}

	template <typename T, class Compare>
	typename BST<T, Compare>::iterator
	AVL<T, Compare>::insert_ret_pos(const T& _val)
	{
		auto opt = BST<T, Compare>::insert_ret_pos(_val);
		if (opt != this->npos)
		{
			node_ptr loc(opt.get());
			this->update_upon(loc);
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
		}
		return opt;
	}

	template <typename T, class Compare>
	bool AVL<T, Compare>::insert(T&& _val)
	{
		return insert_ret_pos(static_cast<T&&>(_val)) != this->npos;
	}

	template <typename T, class Compare>
	typename BST<T, Compare>::iterator
	AVL<T, Compare>::insert_ret_pos(T&& _val)
	{
		auto opt = BST<T, Compare>::insert_ret_pos(static_cast<T&&>(_val));
		if (opt != this->npos)
		{
			node_ptr loc(opt.get());
			this->update_upon(loc);
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
		}
		return opt;
	}

	template <typename T, class Compare>
	bool AVL<T, Compare>::erase(const T& _val)
	{
		u64 before = this->size();
		erase_ret_pos(_val);
		update_upon(this->root);
		return this->size() < before;
	}

	template <typename T, class Compare>
	typename BST<T, Compare>::iterator
	AVL<T, Compare>::erase_ret_pos(const T& _val)
	{
		auto opt = BST<T, Compare>::erase_ret_pos(_val);
		if (opt != this->npos)
		{
			node_ptr loc(opt.get());
			update_upon(loc);
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
		}
		return opt;
	}

	template <typename T, class Compare>
	void binary_tree<T, Compare>::
	        printBT(const std::string& prefix, const node_ptr& st, bool isLeft) const
	{
		if (st != nullptr)
		{
			std::cout << prefix;
			if (st == root.get())
				printf("\n└── ");
			else
				printf("%s", (isLeft ? "├── " : "└── "));
			std::cout << st->data
			          // << " $" << (i16) st->bf
			          << '\n';
			printBT(prefix + (isLeft ? "│   " : "    "), st->lc, true);
			printBT(prefix + (isLeft ? "│   " : "    "), st->rc, false);
		}
	}

	template <typename T, class Compare>
	void binary_tree<T, Compare>::print_as_tree() const
	{
		printf("\nbinary_tree @%#llx:", (u64) root.get());
		if (root != nullptr)
			printBT("", root, false);
		else
			printf("\n└── ∧\n");
	}

	template <typename T, class Compare>
	void binary_tree<T, Compare>::printBT(const iterator& st) const
	{
		if (st != npos)
		{
			std::cout << std::endl;
			node_ptr tmp = st.get();
			printBT("", tmp, false);
			tmp = nullptr;
		}
		else
			printf("\n└── ∧\n");
	}
}

#endif
