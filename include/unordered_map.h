#ifndef _NUTS_HASH_MAP_
#define _NUTS_HASH_MAP_

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
		using k_type = K;
		using v_type = V;
		using base_type = unordered_set<pair<K, V>, Hasher>;
		using self_type = unordered_map<K, V, Hasher>;
		using bucket_type = list<pair<K, V>>;
		using itr_type = typename base_type::iterator;

		using base_type::_size;
		using base_type::bucket;
		using base_type::bucket_size;
		using base_type::hash_fn;
		using base_type::npos;

		unordered_map();
		unordered_map(const self_type& src);
		unordered_map(self_type&& src) { move(src); }
		unordered_map(const std::initializer_list<value_type>& ilist);
		~unordered_map() { base_type::clear(); }

		inline u64 get_index(const K& _k)
		        const { return hash_fn(_k) % *bucket_size; }

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

	// Deduction Guide
	template <class K, class V>
	unordered_map(const std::initializer_list<pair<K, V>>&)
	        -> unordered_map<K, V>;

	template <class K, class V, class Hasher = nuts::hash<K>>
	using hash_map = unordered_map<K, V, Hasher>;

	template <class K, class V, class Hasher>
	unordered_map<K, V, Hasher>::unordered_map()
	{
		vector<bucket_type> tmp(*bucket_size);
		bucket.move(tmp);
	}

	template <class K, class V, class Hasher>
	unordered_map<K, V, Hasher>::
	        unordered_map(const std::initializer_list<value_type>& ilist)
	{
		for (auto& i: ilist) insert(i);
	}

	template <class K, class V, class Hasher>
	unordered_map<K, V, Hasher>::
	        unordered_map(const self_type& src)
	{
		bucket_size = src.bucket_size;
		bucket = src.bucket;
		_size = src._size;
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
		return find(_k) != npos;
	}

	template <class K, class V, class Hasher>
	typename unordered_set<nuts::pair<K, V>, Hasher>::iterator
	unordered_map<K, V, Hasher>::find(const K& _k) const
	{
		u64 i = get_index(_k);
		auto it = nuts::find_if(bucket[i],
		                        [&](const auto& p) { return p._0() == _k; });
		if (it == bucket_type::npos)
			return npos;
		else
			return {advance(bucket.begin(), i), bucket.end(), it};
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
		assert(it != npos);
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
			if (_size == *bucket_size)
				rehash();
			u64 i = get_index(_k);
			bucket[i].emplace_back();
			bucket[i].back().first = _k;
			++_size;
			return bucket[i].back().second;
		}
	}

	template <class K, class V, class Hasher>
	const V& unordered_map<K, V, Hasher>::
	operator[](const K& _k) const
	{
		assert(find(_k) != npos);
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
		bucket_size++;
		vector<bucket_type> tmp(*bucket_size);

		auto opr = [&](pair<K, V>& x) {
			u64 index = get_index(x.first);
			tmp[index].push_back(nuts::move(x));
		};

		for_each(*this, opr);
		bucket.move(tmp);
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
		auto pr = [&](const auto& x) {
			nuts::print(x);
			if (&x != &base_type::back()) printf(", ");
		};

		printf("hash_map = {");
		for_each(*this, pr);
		printf("}\n");
	}
}

#endif
