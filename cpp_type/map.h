#ifndef _NUTS_MAP_
#define _NUTS_MAP_

#include <cassert>
#include "type.h"
#include "utility.h"
#include "functional.h"
#include "set.h"

namespace nuts
{
	template <typename Pair>
	struct default_key_compare
	{
		default_key_compare() = default;
		bool operator()(const Pair &a,
						const Pair &b) const
		{
			return a.get_first() < b.get_first();
		}
	};

	template <typename Key,
			  typename Value, class Compare = default_key_compare<pair<Key, Value>>>
	class map : public set<pair<Key, Value>, Compare>
	{
		using itr_type = typename AVL<pair<Key, Value>,
									  Compare>::const_iterator;

		using base_type = set<pair<Key, Value>, Compare>;

	public:
		map() = default;
		~map() = default;
		static itr_type npos;

		void print() const;

		bool insert(const Key &_k, const Value &_v)
		{
			return this->insert(make_pair(_k, _v));
		}

		bool erase(const Key &_k)
		{
			pair<Key, Value> tmp;
			tmp.first = _k;
			return base_type::erase(tmp);
		}

		itr_type find(const Key &_k)
		{
			pair<Key, Value> tmp;
			tmp.first = _k;
			return base_type::find(tmp);
		}

		bool contains(const Key &_k)
		{
			return this->find(_k) != npos;
		}

		Value &at(const Key &_k)
		{
			auto loc = this->find(_k);
			if (loc == npos)
			{
				pair<Key, Value> tmp;
				tmp.first = _k;
				auto at = base_type::insert_ret_pos(tmp);
				return at->second;
			}
			else
				return loc->second;
		}

		Value &operator[](const Key &_k)
		{
			return this->at(_k);
		}

		const Value &at(const Key &_k) const
		{
			auto loc = this->find(_k);
			assert(loc != npos);
			auto shit = loc.get();
			return loc->second;
		}

		const Value &operator[](const Key &_k) const
		{
			return this->at(_k);
		}
	};

	template <typename Key, typename Val, class Compare>
	typename AVL<pair<Key, Val>, Compare>::const_iterator
		map<Key, Val, Compare>::npos;

	template <typename Key, typename Val, class Compare>
	void map<Key, Val, Compare>::print() const
	{
		auto pr = [this](const auto &x)
		{
			std::cout << x;
			if (x != this->max())
				printf(", ");
		};

		printf("\nmap@%#llx = {", (u64)this->root.get());
		for_each(this->begin(), this->end(), pr);
		printf("}\n");
	}
}

#endif
