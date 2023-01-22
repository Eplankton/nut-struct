#ifndef _NUTS_MAP_
#define _NUTS_MAP_

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
		bool
		operator()(const pair<K, V>& a, const pair<K, V>& b) const
		{
			return a.get_first() < b.get_first();
		}
	};

	template <typename K,
	          typename V, class Compare = default_key_compare<K, V>>
	class map : public set<pair<K, V>, Compare>
	{
	public:
		using value_type = pair<K, V>;
		using key_type = K;
		using val_type = V;
		using itr_type = typename AVL<pair<K, V>, Compare>::iterator;
		using base_type = set<pair<K, V>, Compare>;

	public:
		map() = default;
		map(map<K, V, Compare>&& src) { base_type::move(src); }

		map(const map<K, V, Compare>& src)
		{
			for_each(*this, [&](const auto& x) { insert(x); });
		}

		map(const std::initializer_list<value_type>& ilist)
		{
			for (auto& i: ilist) insert(i);
		}

		~map() = default;

		map<K, V, Compare>& operator=(const map<K, V, Compare>& src);
		map<K, V, Compare>& operator=(map<K, V, Compare>&& src)
		{
			base_type::move(src);
			return *this;
		}

		void print() const;

		auto insert(const K& _k, const V& _v)
		{
			return base_type::insert({_k, _v});
		}

		auto insert(const pair<K, V>& _p)
		{
			return base_type::insert(_p);
		}

		auto insert(pair<K, V>&& _p)
		{
			return base_type::insert(nuts::move(_p));
		}

		auto erase(const K& _k)
		{
			pair<K, V> tmp;
			tmp.first = _k;
			return base_type::erase(tmp);
		}

		itr_type find(const K& _k) const
		{
			pair<K, V> tmp;
			tmp.first = _k;
			return base_type::find(tmp);
		}

		bool contains(const K& _k)
		        const { return this->find(_k) != this->npos; }

		V& at(const K& _k)
		{
			auto loc = this->find(_k);
			assert(loc != this->npos);
			return loc->second;
		}

		const V& at(const K& _k) const
		{
			auto loc = this->find(_k);
			assert(loc != this->npos);
			return loc->second;
		}

		V& operator[](const K& _k)
		{
			auto loc = this->find(_k);
			if (loc == this->npos)
			{
				pair<K, V> tmp;
				tmp.first = _k;
				auto at = base_type::insert_ret_pos(tmp);
				return at->second;
			}
			else
				return loc->second;
		}

		const V& operator[](const K& _k) const { return at(_k); }
	};

	// Deduction Guide
	template <class K, class V>
	map(const std::initializer_list<pair<K, V>>&) -> map<K, V>;

	template <typename K, typename V, class Compare>
	map<K, V, Compare>& map<K, V, Compare>::
	operator=(const map<K, V, Compare>& src)
	{
		base_type::clear();
		for_each(*this, [&](const auto& x) { insert(x); });
		return *this;
	}

	template <typename K, typename V, class Compare>
	void map<K, V, Compare>::print() const
	{
		auto printer = [&](const auto& x) {
			nuts::print(x);
			if (&x != &this->back()) nuts::print(", ");
		};

		printf("map @%#llx = {", (u64) this->root.get());
		if (!this->empty()) for_each(*this, printer);
		nuts::print("}\n");
	}
}

#endif
