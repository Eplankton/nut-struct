#ifndef _NUTS_HASH_MAP_
#define _NUTS_HASH_MAP_ 1

#include "unordered_set.h"
#include "utility.h"

namespace nuts
{
	template <typename K, typename V,
	          typename Hasher = nuts::hash<K>>
	class unordered_map
	    : public unordered_set<pair<K, V>, Hasher>
	{
	public:
		using value_type = pair<K, V>;
		using key_type = K;
		using val_type = V;
		using base_type = unordered_set<pair<K, V>, Hasher>;
		using self_type = unordered_map<K, V, Hasher>;
		using bucket_type = list<pair<K, V>>;
		using itr_type = typename base_type::iterator;

		unordered_map();
		unordered_map(const self_type& src);
		unordered_map(self_type&& src) { move(src); }
		~unordered_map() { base_type::clear(); }

		inline u64 get_index(const K& _k)
		        const { return this->hash_fn(_k) % *(this->bucket_size); }

		self_type& move(self_type& src);
		void rehash();
		bool contains(const K& _k) const;
		itr_type find(const K& _k) const;

		V& at(const K& _k);
		const V& at(const K& _k) const;

		V& operator[](const K& _k);
		const V& operator[](const K& _k) const;

		void insert(const K& _k, const V& _val);
		void insert(const pair<K, V>& _p);
		bool erase(const K& _k);

		self_type& operator=(const self_type& src);
		self_type& operator=(self_type&& src) { return move(src); }

		void print() const;
	};

	template <class K, class V, class Hasher = nuts::hash<K>>
	using hash_map = unordered_map<K, V, Hasher>;

	template <class K, class V, class Hasher>
	unordered_map<K, V, Hasher>::unordered_map()
	{
		vector<bucket_type> tmp(*this->bucket_size);
		this->bucket.move(tmp);
	}

	template <class K, class V, class Hasher>
	unordered_map<K, V, Hasher>::
	        unordered_map(const self_type& src)
	{
		this->bucket_size = src.bucket_size;
		this->bucket = src.bucket;
		this->_size = src._size;
	}

	template <class K, class V, class Hasher>
	unordered_map<K, V, Hasher>&
	unordered_map<K, V, Hasher>::move(self_type& src)
	{
		base_type::move(src);
		return *this;
	}

	template <class K, class V, class Hasher>
	bool unordered_map<K, V, Hasher>::contains(const K& _k) const
	{
		u64 index = get_index(_k);
		auto ed = this->bucket[index].end() + 1;
		for (auto it = this->bucket[index].begin();
		     it != ed; ++it)
		{
			if (it->first == _k)
				return true;
		}
		return false;
	}

	template <class K, class V, class Hasher>
	typename unordered_set<nuts::pair<K, V>, Hasher>::iterator
	unordered_map<K, V, Hasher>::find(const K& _k) const
	{
		u64 index = get_index(_k);
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

	template <class K, class V, class Hasher>
	V& unordered_map<K, V, Hasher>::at(const K& _k)
	{
		auto it = find(_k);
		assert(it != base_type::npos);
		return it->second;
	}

	template <class K, class V, class Hasher>
	const V& unordered_map<K, V, Hasher>::
	        at(const K& _k) const
	{
		auto it = find(_k);
		assert(it != base_type::npos);
		return it->second;
	}

	template <class K, class V, class Hasher>
	V& unordered_map<K, V, Hasher>::operator[](const K& _k)
	{
		auto it = find(_k);
		if (it != base_type::npos)
			return it->second;
		else
		{
			if (this->_size == *this->bucket_size)
				this->rehash();
			u64 index = get_index(_k);
			this->bucket[index].emplace_back();
			this->bucket[index].back().first = _k;
			++this->_size;
			return this->bucket[index].back().second;
		}
	}

	template <class K, class V, class Hasher>
	const V& unordered_map<K, V, Hasher>::
	operator[](const K& _k) const
	{
		assert(find(_k) != base_type::npos);
		return (*this)[_k];
	}

	template <class K, class V, class Hasher>
	void unordered_map<K, V, Hasher>::
	        insert(const K& _k, const V& _val)
	{
		(*this)[_k] = _val;
	}

	template <class K, class V, class Hasher>
	void unordered_map<K, V, Hasher>::
	        insert(const pair<K, V>& _p)
	{
		(*this)[_p.first] = _p.second;
	}

	template <class K, class V, class Hasher>
	bool unordered_map<K, V, Hasher>::
	        erase(const K& _k)
	{
		pair<K, V> tmp;
		tmp.first = _k;
		return base_type::erase(tmp);
	}

	template <class K, class V, class Hasher>
	void unordered_map<K, V, Hasher>::rehash()
	{
		this->bucket_size++;
		vector<bucket_type> tmp(*this->bucket_size);

		auto opr = [this, &tmp](pair<K, V>& x) {
			u64 index = get_index(x.first);
			tmp[index].push_back(nuts::move(x));
		};

		for_each(*this, opr);
		this->bucket.move(tmp);
	}

	template <class K, class V, class Hasher>
	unordered_map<K, V, Hasher>& unordered_map<K, V, Hasher>::
	operator=(const self_type& src)
	{
		base_type::operator=(src);
		return *this;
	}

	template <class K, class V, class Hasher>
	void unordered_map<K, V, Hasher>::print() const
	{
		auto pr = [this](const auto& x) {
			nuts::print(x);
			if (&x != &(*this->end())) printf(", ");
		};

		printf("hash_map = {");
		for_each(*this, pr);
		printf("}\n");
	}
}

#endif
