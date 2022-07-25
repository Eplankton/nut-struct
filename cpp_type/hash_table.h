#ifndef _NUTS_HASH_TABLE_
#define _NUTS_HASH_TABLE_

#include <iostream>
#include <cstddef>
#include <cassert>

#include "utility.h"
#include "vector.h"
#include "list.h"
#include "functional.h"

namespace nuts
{
	// Capacity of the bucket
	const u64 LONG_PRIME_LIST[28] = {
		53ul, 97ul, 193ul, 389ul, 769ul,
		1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
		49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
		1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
		50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 429496729ul};

	// Hash_Table
	template <typename Key,
			  typename Val,
			  class Hasher = nuts::hash<Key>>
	class hash_table
	{
		using Self_Type = hash_table<Key, Val, Hasher>;
		using Container_Type = list<pair<Key, Val>>;

	protected:
		u64 bucket_size = LONG_PRIME_LIST[9];
		u64 _size = 0;
		vector<Container_Type> bucket;
		Hasher hash_fn;

	public:
		hash_table();
		hash_table(const Self_Type &src);

		u64 size() const { return _size; }
		bool empty() const { return _size == 0; }
		bool contains(const Key &_k) const;
		Val &operator[](const Key &_k);
		const Val &operator[](const Key &_k) const;
		void insert(const Key &_k, const Val &_val);
		void insert(const pair<Key, Val> &_p);
		bool erase(const Key &_k);
		void clear();
	};

	template <class Key, class Val, class Hasher = nuts::hash<Key>>
	using unordered_map = hash_table<Key, Val, Hasher>; // Unordered_Map

	template <class Key, class Val, class Hasher>
	hash_table<Key, Val, Hasher>::hash_table()
	{
		vector<list<pair<Key, Val>>> tmp(bucket_size);
		this->bucket.move(tmp);
	}

	template <class Key, class Val, class Hasher>
	hash_table<Key, Val, Hasher>::hash_table(const Self_Type &src)
	{
		this->bucket_size = src.bucket_size;
		this->bucket = src.bucket;
		this->_size = src._size;
	}

	template <class Key, class Val, class Hasher>
	bool hash_table<Key, Val, Hasher>::contains(const Key &_k) const
	{
		u64 index = hash_fn(_k) % bucket_size;
		for (auto res = bucket[index].begin(); res != bucket[index].end() + 1; res++)
		{
			if (res->first == _k)
				return true;
		}
		return false;
	}

	template <class Key, class Val, class Hasher>
	Val &hash_table<Key, Val, Hasher>::operator[](const Key &_k)
	{
		u64 index = hash_fn(_k) % bucket_size;
		for (auto res = bucket[index].begin(); res != bucket[index].end() + 1; res++)
		{
			if (res->first == _k)
				return res->second;
		}
		bucket[index].push_back();
		bucket[index].back().first = _k;
		_size++;
		return bucket[index].back().second;
	}

	template <class Key, class Val, class Hasher>
	const Val &hash_table<Key, Val, Hasher>::operator[](const Key &_k) const
	{
		u64 index = hash_fn(_k) % bucket_size;
		assert(this->contains(_k));
		return (*this)[_k];
	}

	template <class Key, class Val, class Hasher>
	void hash_table<Key, Val, Hasher>::insert(const Key &_k, const Val &_val)
	{
		(*this)[_k] = _val;
	}

	template <class Key, class Val, class Hasher>
	void hash_table<Key, Val, Hasher>::insert(const pair<Key, Val> &_p)
	{
		(*this)[_p.first] = _p.second;
	}

	template <class Key, class Val, class Hasher>
	bool hash_table<Key, Val, Hasher>::erase(const Key &_k)
	{
		u64 index = hash_fn(_k) % bucket_size;
		for (auto res = bucket[index].begin(); res != bucket[index].end() + 1; res++)
		{
			if (res->first == _k)
			{
				bucket[index].erase(res);
				_size--;
				return true;
			}
		}
		return false;
	}

	template <class Key, class Val, class Hasher>
	void hash_table<Key, Val, Hasher>::clear()
	{
		for (u64 i = 0; i < bucket_size; i++)
		{
			bucket[i].clear();
		}
		_size = 0;
	}

	// Unordered_Set
	template <typename Key,
			  typename Hasher = nuts::hash<Key>>
	class unordered_set
	{
		using Self_Type = unordered_set<Key, Hasher>;
		using Bucket_Type = list<Key>;

	protected:
		u64 bucket_size = LONG_PRIME_LIST[9];
		u64 _size = 0;
		vector<Bucket_Type> bucket;
		Hasher hash_fn;

	public:
		unordered_set();
		unordered_set(const Self_Type &src);

		u64 size() const { return _size; }
		bool empty() const { return _size == 0; }
		bool contains(const Key &_k) const;
		void insert(const Key &_k);
		bool erase(const Key &_k);
		void clear();
	};

	template <class Key, class Hasher>
	unordered_set<Key, Hasher>::unordered_set()
	{
		vector<Bucket_Type> tmp(bucket_size);
		this->bucket.move(tmp);
	}

	template <class Key, class Hasher>
	unordered_set<Key, Hasher>::unordered_set(const Self_Type &src)
	{
		this->bucket_size = src.bucket_size;
		this->bucket = src.bucket;
		this->_size = src._size;
	}

	template <class Key, class Hasher>
	bool unordered_set<Key, Hasher>::contains(const Key &_k) const
	{
		u64 index = hash_fn(_k) % bucket_size;
		for (auto res = bucket[index].begin(); res != bucket[index].end() + 1; res++)
		{
			if (res != nullptr && *res == _k)
				return true;
		}
		return false;
	}

	template <class Key, class Hasher>
	void unordered_set<Key, Hasher>::insert(const Key &_k)
	{
		u64 index = hash_fn(_k) % bucket_size;
		for (auto res = bucket[index].begin(); res != bucket[index].end() + 1; res++)
		{
			if (res != nullptr && *res == _k)
				return;
		}
		bucket[index].push_back(_k);
		_size++;
	}

	template <class Key, class Hasher>
	bool unordered_set<Key, Hasher>::erase(const Key &_k)
	{
		u64 index = hash_fn(_k) % bucket_size;
		for (auto res = bucket[index].begin(); res != bucket[index].end() + 1; res++)
		{
			if (res != nullptr && *res == _k)
			{
				bucket[index].erase(res);
				_size--;
				return true;
			}
		}
		return false;
	}

	template <class Key, class Hasher>
	void unordered_set<Key, Hasher>::clear()
	{
		for (u64 i = 0; i < bucket_size; i++)
		{
			bucket[i].clear();
		}
		_size = 0;
	}
}

#endif
