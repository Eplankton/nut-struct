#ifndef _NUTS_HASH_TABLE_
#define _NUTS_HASH_TABLE_

#include <iostream>
#include <cstddef>
#include <cassert>

#include "utility.h"
#include "vector.h"
#include "array.h"
#include "list.h"
#include "functional.h"

namespace nuts
{
	const u64 LONG_PRIME_LIST[28] = {
		53u, 97u, 193u, 389u, 769u,
		1543u, 3079u, 6151u, 12289u, 24593u};

	// Abstract hash function
	template <class key>
	u64 hash(const key &val) { return 0; }

	// Full specialization of hash function
	template <>
	u64 hash(const int &i) { return i % LONG_PRIME_LIST[1]; }

	function<u64(const int &)> HASH_INT = hash;

	template <class key, class value, class hash_func = function<u64(const key &)>>
	class hash_table
	{
	protected:
		u64 table_size = LONG_PRIME_LIST[1]; // Give a prime number
		vector<list<pair<key, value>>> bucket;
		hash_func hash_fn;

	public:
		hash_table();
		hash_table(const hash_func &src);
		value &operator[](const key &_k);
		void insert(const key &_k, const value &_val);
		void insert(const pair<key, value> &_p);
		bool remove(const key &_k);
		void clear();
	};

	template <class key, class value, class hash_func>
	hash_table<key, value, hash_func>::hash_table()
	{
		vector<list<pair<key, value>>> tmp(table_size);
		this->bucket.move(tmp);
		hash_fn = HASH_INT;
	}

	template <class key, class value, class hash_func>
	hash_table<key, value, hash_func>::hash_table(const hash_func &src)
	{
		vector<list<pair<key, value>>> tmp(table_size);
		this->bucket.move(tmp);
		hash_fn = src;
	}

	template <class key, class value, class hash_func>
	value &hash_table<key, value, hash_func>::operator[](const key &_k)
	{
		u64 index = hash_fn(_k);
		for (auto res = bucket[index].begin(); res != bucket[index].end() + 1; res++)
		{
			if (res->first == _k)
				return res->second;
		}
		bucket[index].push_back();
		bucket[index].back().first = _k;
		return bucket[index].back().second;
	}

	template <class key, class value, class hash_func>
	void hash_table<key, value, hash_func>::insert(const key &_k, const value &_val)
	{
		(*this)[_k] = _val;
	}

	template <class key, class value, class hash_func>
	void hash_table<key, value, hash_func>::insert(const pair<key, value> &_p)
	{
		(*this)[_p.first] = _p.second;
	}

	template <class key, class value, class hash_func>
	bool hash_table<key, value, hash_func>::remove(const key &_k)
	{
		u64 index = hash_fn(_k);
		for (auto res = bucket[index].begin(); res != bucket[index].end() + 1; res++)
		{
			if (res->first == _k)
			{
				bucket[index].erase(res, 0);
				return true;
			}
		}
		return false;
	}

	template <class key, class value, class hash_func>
	void hash_table<key, value, hash_func>::clear()
	{
		for (u64 i = 0; i < table_size; i++)
		{
			bucket[i].clear();
		}
	}

}

#endif
