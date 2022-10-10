#ifndef _NUTS_HASH_MAP_
#define _NUTS_HASH_MAP_ 1

#include "unordered_set.h"
#include "utility.h"

namespace nuts
{
	template <typename Key, typename Val,
	          typename Hasher = nuts::hash<Key>>
	class unordered_map
	    : public unordered_set<pair<Key, Val>, Hasher>
	{
	public:
		using value_type = pair<Key, Val>;
		using key_type = Key;
		using val_type = Val;
		using base_type = unordered_set<pair<Key, Val>, Hasher>;
		using self_type = unordered_map<Key, Val, Hasher>;
		using bucket_type = list<pair<Key, Val>>;
		using itr_type = typename base_type::iterator;

		unordered_map();
		unordered_map(const self_type& src);
		unordered_map(self_type&& src) { move(src); }
		~unordered_map() { base_type::clear(); }

		self_type& move(self_type& src);
		void rehash();
		bool contains(const Key& _k) const;
		itr_type find(const Key& _k) const;

		Val& at(const Key& _k);
		const Val& at(const Key& _k) const;

		Val& operator[](const Key& _k);
		const Val& operator[](const Key& _k) const;

		void insert(const Key& _k, const Val& _val);
		void insert(const pair<Key, Val>& _p);
		bool erase(const Key& _k);

		self_type& operator=(const self_type& src);
		self_type& operator=(self_type&& src) { return move(src); }

		void print() const;
	};

	template <class Key, class Val, class Hasher = nuts::hash<Key>>
	using hash_map = unordered_map<Key, Val, Hasher>;

	template <class Key, class Val, class Hasher>
	unordered_map<Key, Val, Hasher>::unordered_map()
	{
		vector<bucket_type> tmp(*this->bucket_size);
		this->bucket.move(tmp);
	}

	template <class Key, class Val, class Hasher>
	unordered_map<Key, Val, Hasher>::
	        unordered_map(const self_type& src)
	{
		this->bucket_size = src.bucket_size;
		this->bucket = src.bucket;
		this->_size = src._size;
	}

	template <class Key, class Val, class Hasher>
	unordered_map<Key, Val, Hasher>&
	unordered_map<Key, Val, Hasher>::move(self_type& src)
	{
		base_type::move(src);
		return *this;
	}

	template <class Key, class Val, class Hasher>
	bool unordered_map<Key, Val, Hasher>::contains(const Key& _k) const
	{
		u64 index = hash_fn(_k) % *this->bucket_size;
		auto ed = this->bucket[index].end() + 1;
		for (auto it = this->bucket[index].begin();
		     it != ed; ++it)
		{
			if (it->first == _k)
				return true;
		}
		return false;
	}

	template <class Key, class Val, class Hasher>
	typename unordered_set<nuts::pair<Key, Val>, Hasher>::iterator
	unordered_map<Key, Val, Hasher>::find(const Key& _k) const
	{
		u64 index = this->hash_fn(_k) % *this->bucket_size;
		auto ed = this->bucket[index].end() + 1;
		for (auto it = this->bucket[index].begin();
		     it != nullptr && it != ed; ++it)
		{
			if (it->first == _k)
				return {this->bucket.begin() + index,
				        this->bucket.end(), it};
		}
		return base_type::npos;
	}

	template <class Key, class Val, class Hasher>
	Val& unordered_map<Key, Val, Hasher>::at(const Key& _k)
	{
		auto it = find(_k);
		assert(it != base_type::npos);
		return it->second;
	}

	template <class Key, class Val, class Hasher>
	const Val& unordered_map<Key, Val, Hasher>::
	        at(const Key& _k) const
	{
		auto it = find(_k);
		assert(it != base_type::npos);
		return it->second;
	}

	template <class Key, class Val, class Hasher>
	Val& unordered_map<Key, Val, Hasher>::operator[](const Key& _k)
	{
		auto it = find(_k);
		if (it != base_type::npos)
			return it->second;
		else
		{
			if (this->_size == *this->bucket_size)
				this->rehash();
			u64 index = this->hash_fn(_k) % *this->bucket_size;
			this->bucket[index].emplace_back();
			this->bucket[index].back().first = _k;
			++this->_size;
			return this->bucket[index].back().second;
		}
	}

	template <class Key, class Val, class Hasher>
	const Val& unordered_map<Key, Val, Hasher>::
	operator[](const Key& _k) const
	{
		assert(find(_k) != base_type::npos);
		return (*this)[_k];
	}

	template <class Key, class Val, class Hasher>
	void unordered_map<Key, Val, Hasher>::
	        insert(const Key& _k, const Val& _val)
	{
		(*this)[_k] = _val;
	}

	template <class Key, class Val, class Hasher>
	void unordered_map<Key, Val, Hasher>::
	        insert(const pair<Key, Val>& _p)
	{
		(*this)[_p.first] = _p.second;
	}

	template <class Key, class Val, class Hasher>
	bool unordered_map<Key, Val, Hasher>::
	        erase(const Key& _k)
	{
		pair<Key, Val> tmp;
		tmp.first = _k;
		return base_type::erase(tmp);
	}

	template <class Key, class Val, class Hasher>
	void unordered_map<Key, Val, Hasher>::rehash()
	{
		this->bucket_size++;
		vector<bucket_type> tmp(*this->bucket_size);

		auto opr = [this, &tmp](pair<Key, Val>& x) {
			u64 index = this->hash_fn(x.first) % (*this->bucket_size);
			tmp[index].push_back(nuts::move(x));
		};

		for_each(*this, opr);
		this->bucket.move(tmp);
	}

	template <class Key, class Val, class Hasher>
	unordered_map<Key, Val, Hasher>& unordered_map<Key, Val, Hasher>::
	operator=(const self_type& src)
	{
		base_type::operator=(src);
		return *this;
	}

	template <class Key, class Val, class Hasher>
	void unordered_map<Key, Val, Hasher>::print() const
	{
		auto pr = [this](const auto& x) {
			nuts::print(x);
			if (&x != &(*this->end())) printf(", ");
		};

		printf("hash_map @%#llx = {", (u64) this->bucket.data());
		for_each(*this, pr);
		printf("}\n");
	}
}

#endif
