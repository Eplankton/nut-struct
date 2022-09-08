#ifndef _NUTS_MAP_
#define _NUTS_MAP_ 1

#include "set.h"
#include "type.h"
#include "utility.h"
#include <cassert>

namespace nuts
{
	template <typename K, typename V>
	struct default_key_compare
	{
		default_key_compare() = default;
		bool operator()(const pair<K, V>& a,
		                const pair<K, V>& b) const
		{
			return a.get_first() < b.get_first();
		}
	};

	template <typename Key,
	          typename Val, class Compare = default_key_compare<Key, Val>>
	class map : public set<pair<Key, Val>, Compare>
	{
	public:
		using value_type = Key;
		using itr_type = typename AVL<pair<Key, Val>, Compare>::iterator;
		using base_type = set<pair<Key, Val>, Compare>;

	public:
		map() = default;
		map(const map<Key, Val, Compare>& src)
		{
			for_each(*this, [this](const auto& x) { insert(x); });
		}

		map(map<Key, Val, Compare>&& src) { base_type::move(src); }
		~map() = default;

		map<Key, Val, Compare>& operator=(const map<Key, Val, Compare>& src);
		map<Key, Val, Compare>& operator=(map<Key, Val, Compare>&& src)
		{
			base_type::move(src);
			return *this;
		}

		void print() const;

		auto insert(const Key& _k, const Val& _v)
		{
			return base_type::insert({_k, _v});
		}

		auto insert(const pair<Key, Val>& _p)
		{
			return base_type::insert(_p);
		}

		auto insert(pair<Key, Val>&& _p)
		{
			return base_type::insert(static_cast<pair<Key, Val>&&>(_p));
		}

		auto erase(const Key& _k)
		{
			pair<Key, Val> tmp;
			tmp.first = _k;
			return base_type::erase(tmp);
		}

		itr_type find(const Key& _k) const
		{
			pair<Key, Val> tmp;
			tmp.first = _k;
			return base_type::find(tmp);
		}

		bool contains(const Key& _k)
		        const { return this->find(_k) != this->npos; }

		Val& at(const Key& _k)
		{
			auto loc = this->find(_k);
			assert(loc != this->npos);
			return loc->second;
		}

		const Val& at(const Key& _k) const
		{
			auto loc = this->find(_k);
			assert(loc != this->npos);
			return loc->second;
		}

		Val& operator[](const Key& _k)
		{
			auto loc = this->find(_k);
			if (loc == this->npos)
			{
				pair<Key, Val> tmp;
				tmp.first = _k;
				auto at = base_type::insert_ret_pos(tmp);
				return at->second;
			}
			else
				return loc->second;
		}

		const Val& operator[](const Key& _k) const { return at(_k); }
	};

	template <typename Key, typename Val, class Compare>
	map<Key, Val, Compare>& map<Key, Val, Compare>::
	operator=(const map<Key, Val, Compare>& src)
	{
		base_type::clear();
		for_each(*this, [this](const auto& x) { insert(x); });
		return *this;
	}

	template <typename Key, typename Val, class Compare>
	void map<Key, Val, Compare>::print() const
	{
		auto printer = [this](const auto& x) {
			std::cout << x;
			if (&x != &this->back()) printf(", ");
		};

		printf("\nmap @%#llx = {", (u64) this->root.get());
		if (!this->empty()) for_each(*this, printer);
		printf("}\n");
	}
}

#endif
