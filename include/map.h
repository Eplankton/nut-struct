#ifndef _NUTS_MAP_
#define _NUTS_MAP_ 1

#include "functional.h"
#include "set.h"
#include "type.h"
#include "utility.h"
#include <cassert>

namespace nuts
{
	template <typename Pair>
	struct default_key_compare
	{
		default_key_compare() = default;
		bool operator()(const Pair& a,
		                const Pair& b) const
		{
			return a.get_first() < b.get_first();
		}
	};

	template <typename Key,
	          typename Val, class Compare = default_key_compare<pair<Key, Val>>>
	class map : public set<pair<Key, Val>, Compare>
	{
	public:
		using Val_type = Key;
		using Itr_type = typename AVL<pair<Key, Val>,
		                              Compare>::iterator;

		using Base_type = set<pair<Key, Val>, Compare>;

	public:
		map() = default;

		map(const map<Key, Val, Compare>& src)
		{
			for_each(this->begin(), this->end(),
			         [this](const auto& x) { this->insert(x); });
		}

		map(map<Key, Val, Compare>&& src)
		{
			Base_type::move(src);
		}
		~map() = default;

		map<Key, Val, Compare>& operator=(const map<Key, Val, Compare>& src);
		map<Key, Val, Compare>& operator=(map<Key, Val, Compare>&& src)
		{
			Base_type::move(src);
			return *this;
		}

		void print() const;

		bool insert(const Key& _k, const Val& _v)
		{
			return Base_type::insert({_k, _v});
		}

		bool insert(const pair<Key, Val>& _p)
		{
			return Base_type::insert(_p);
		}

		bool erase(const Key& _k)
		{
			pair<Key, Val> tmp;
			tmp.first = _k;
			return Base_type::erase(tmp);
		}

		Itr_type find(const Key& _k) const
		{
			pair<Key, Val> tmp;
			tmp.first = _k;
			return Base_type::find(tmp);
		}

		bool contains(const Key& _k) const
		{
			return this->find(_k) != this->npos;
		}

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
				auto at = Base_type::insert_ret_pos(tmp);
				return at->second;
			}
			else
				return loc->second;
		}

		const Val& operator[](const Key& _k) const { return at(_k); }
	};

	template <typename Key, typename Val, class Compare>
	void map<Key, Val, Compare>::print() const
	{
		auto pr = [this](const auto& x) {
			std::cout << x;
			if (&x != &this->back())
				printf(", ");
		};

		printf("\nmap @%#llx = {", (u64) this->root.get());
		if (!this->empty())
			for_each(this->begin(), this->end(), pr);
		printf("}\n");
	}

	template <typename Key, typename Val, class Compare>
	map<Key, Val, Compare>& map<Key, Val, Compare>::
	operator=(const map<Key, Val, Compare>& src)
	{
		Base_type::clear();
		for_each(this->begin(), this->end(),
		         [this](const auto& x) { this->insert(x); });
		return *this;
	}
}

#endif
