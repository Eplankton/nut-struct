#ifndef _NUTS_UOS_MAP_
#define _NUTS_UOS_MAP_ 1

#include "unordered_set.h"

namespace nuts
{
	template <typename Key, typename Val,
	          typename Hasher = nuts::hash<Key>>
	class unordered_map
	    : public unordered_set<pair<Key, Val>, Hasher>
	{
	public:
		using base_type = unordered_set<pair<Key, Val>, Hasher>;
		using Self_Type = unordered_map<Key, Val, Hasher>;
		using Bucket_Type = list<pair<Key, Val>>;
		using itr_type = typename base_type::iterator;

		unordered_map();
		unordered_map(const Self_Type& src);
		unordered_map(Self_Type&& src) { this->move(src); }
		~unordered_map() { base_type::clear(); }

		Self_Type& move(Self_Type& src);

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

		Self_Type& operator=(const Self_Type& src);
		Self_Type& operator=(Self_Type&& src) { return this->move(src); }
	};

	template <class Key, class Val, class Hasher>
	unordered_map<Key, Val, Hasher>::unordered_map()
	{
		vector<Bucket_Type> tmp(*this->bucket_size);
		this->bucket.move(tmp);
	}

	template <class Key, class Val, class Hasher>
	unordered_map<Key, Val, Hasher>::
	        unordered_map(const Self_Type& src)
	{
		this->bucket_size = src.bucket_size;
		this->bucket(src.bucket);
		this->_size = src._size;
	}

	template <class Key, class Val, class Hasher>
	unordered_map<Key, Val, Hasher>&
	unordered_map<Key, Val, Hasher>::move(Self_Type& src)
	{
		this->bucket_size = src.bucket_size;
		this->bucket.move(src.bucket);
		this->_size = src._size;
		src._size = 0;
		return *this;
	}

	template <class Key, class Val, class Hasher>
	bool unordered_map<Key, Val, Hasher>::contains(const Key& _k) const
	{
		u64 index = hash_fn(_k) % *this->bucket_size;
		auto ed = this->bucket[index].end() + 1;
		for (auto res = this->bucket[index].begin();
		     res != ed; res++)
		{
			if (res->first == _k)
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
		for (auto res = this->bucket[index].begin();
		     res != nullptr && res != ed; res++)
		{
			if (res->first == _k)
				return {this->bucket.begin() + index,
				        this->bucket.end(),
				        res};
		}
		return base_type::npos;// if not found
	}

	template <class Key, class Val, class Hasher>
	Val& unordered_map<Key, Val, Hasher>::at(const Key& _k)
	{
		auto it = this->find(_k);
		assert(it != base_type::npos);
		return it->second;
	}

	template <class Key, class Val, class Hasher>
	const Val& unordered_map<Key, Val, Hasher>::
	        at(const Key& _k) const
	{
		auto it = this->find(_k);
		assert(it != base_type::npos);
		return it->second;
	}

	template <class Key, class Val, class Hasher>
	Val& unordered_map<Key, Val, Hasher>::operator[](const Key& _k)
	{
		auto it = this->find(_k);
		if (it != base_type::npos)
			return it->second;
		else
		{
			if (this->_size == *this->bucket_size)
				this->rehash();
			u64 index = this->hash_fn(_k) % *this->bucket_size;
			this->bucket[index].push_back();
			this->bucket[index].back().first = _k;
			this->_size++;
			return this->bucket[index].back().second;
		}
	}

	template <class Key, class Val, class Hasher>
	const Val& unordered_map<Key, Val, Hasher>::
	operator[](const Key& _k) const
	{
		assert(this->find(_k) != base_type::npos);
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
		vector<Bucket_Type> tmp(*this->bucket_size);
		tmp.shrink_to_fit();

		auto fv = [this, &tmp](const pair<Key, Val>& x) {
			u64 index = this->hash_fn(x.first) % (*this->bucket_size);
			tmp[index].push_back(x);
		};

		for_each(this->begin(), this->end(), fv);
		this->bucket.move(tmp);
	}

	template <class Key, class Val, class Hasher>
	unordered_map<Key, Val, Hasher>& unordered_map<Key, Val, Hasher>::
	operator=(const Self_Type& src)
	{
		this->bucket_size = src.bucket_size;
		this->bucket = src.bucket;
		this->_size = src._size;
	}
}

#endif
