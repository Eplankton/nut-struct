#ifndef _NUTS_MAP_
#define _NUTS_MAP_

#include <cassert>
#include "type.h"
#include "utility.h"
#include "functional.h"
#include "binary_tree.h"

namespace nuts
{
	template <typename Pair>
	struct default_key_compare
	{
		default_key_compare() = default;
		bool operator()(const Pair &a, const Pair &b) const
		{
			if (a.get_first() == b.get_first())
				return false;
			else
				return a.get_first() < b.get_first();
		}
	};

	template <typename Key,
			  typename Value,
			  class Compare = default_key_compare<pair<Key, Value>>>
	class map
	{
	protected:
		AVL<pair<Key, Value>, Compare> Tree;

	public:
		map() = default;
		~map() { Tree.clear(); }

		static typename AVL<pair<Key, Value>,
							Compare>::const_iterator npos;

		u64 size() const { return Tree.size(); }
		bool empty() const { return Tree.empty(); }
		void clear() { Tree.clear(); }
		void print() { Tree.printBT(); }

		auto begin() { return Tree.begin(); }
		auto end() { return Tree.end(); }

		bool insert(const Key &_k, const Value &_v)
		{
			return Tree.insert(make_pair(_k, _v));
		}

		bool erase(const Key &_k)
		{
			pair<Key, Value> tmp;
			tmp.first = _k;
			return Tree.erase(tmp);
		}

		typename AVL<pair<Key, Value>, Compare>::const_iterator
		find(const Key &_k)
		{
			pair<Key, Value> tmp;
			tmp.first = _k;
			return Tree.find(tmp);
		}

		bool contains(const Key &_k)
		{
			return this->find(_k) != npos;
		}

		Value &at(const Key &_k)
		{
			auto loc = this->find(_k);
			assert(loc != npos);
			auto shit = loc.get();
			return loc->second;
		}

		const Value &at(const Key &_k) const
		{
			auto loc = this->find(_k);
			assert(loc != npos);
			auto shit = loc.get();
			return loc->second;
		}

		Value &operator[](const Key &_k)
		{
			auto loc = this->find(_k);
			if (loc == npos)
			{
				pair<Key, Value> tmp;
				tmp.first = _k;
				auto at = Tree.insert_ret_pos(tmp);
				return at->second;
			}
			else
			{
				return loc->second;
			}
		}

		const Value &operator[](const Key &_k) const
		{
			return this->at(_k);
		}
	};

	template <typename Key, typename Val, class Compare>
	typename AVL<pair<Key, Val>, Compare>::const_iterator
		map<Key, Val, Compare>::npos;

	// template <typename Key, typename Value,
	// 		  class Compare = default_key_compare<pair<Key, Value>>>
	// class map : public AVL<pair<Key, Value>, Compare>
	// {
	// public:
	// 	map() { this->root = nullptr, this->_size = 0; }
	// 	~map() { this->_size = 0; }

	// 	bool insert(const Key &_k, const Value &_v)
	// 	{
	// 		return AVL<pair<Key, Value>, Compare>::insert(make_pair(_k, _v));
	// 	}

	// 	bool erase(const Key &_k)
	// 	{
	// 		pair<Key, Value> tmp;
	// 		tmp.first = _k;
	// 		return AVL<pair<Key, Value>, Compare>::erase(tmp);
	// 	}

	// 	auto find(const Key &_k)
	// 	{
	// 		pair<Key, Value> tmp;
	// 		tmp.first = _k;
	// 		return AVL<pair<Key, Value>, Compare>::find(tmp);
	// 	}

	// 	bool contains(const Key &_k)
	// 	{
	// 		return this->find(_k) != this->npos;
	// 	}

	// 	Value &operator[](const Key &_k)
	// 	{
	// 		auto loc = this->find(_k);
	// 		if (loc == this->npos)
	// 		{
	// 			pair<Key, Value> tmp;
	// 			tmp.first = _k;
	// 			auto at = Tree.insert_ret_pos(tmp);
	// 			return at->second;
	// 		}
	// 		else
	// 		{
	// 			return loc->second;
	// 		}
	// 	}

	// 	const Value &operator[](const Key &_k) const
	// 	{
	// 		auto loc = this->find(_k);
	// 		assert(loc != this->npos);
	// 		auto shit = loc.get();
	// 		return loc->second;
	// 	}
	// };
}

#endif
