#ifndef _NUTS_UNO_SET_
#define _NUTS_UNO_SET_ 1

#include "functional.h"
#include "list.h"
#include "range.h"
#include "vector.h"

namespace nuts
{
	// Capacity of the bucket
	static const u64 PRIME_LIST[] = {
	        17ul, 29ul, 53ul, 97ul, 113ul, 193ul, 271ul, 389ul,
	        769ul, 1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
	        49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
	        1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
	        50331653ul, 100663319ul, 201326611ul, 402653189ul,
	        805306457ul, 1610612741ul, 3221225473ul, 429496729ul};

	template <typename Key,
	          typename Hasher = nuts::hash<Key>>
	class unordered_set
	{
	public:
		using prime_pointer = const u64*;
		using value_type = Key;
		using self_type = unordered_set<Key, Hasher>;
		using bucket_type = list<Key>;

	public:
		class iterator
		    : public forward_iterator
		{
		public:
			using value_type = Key;
			using Outside = typename vector<bucket_type>::iterator;
			using Inside = typename bucket_type::iterator;

		protected:
			Outside out_itr, bkt_end;
			Inside in_itr;

		public:
			iterator() = default;
			iterator(const Outside& o, const Outside& bt,
			         const Inside& i) : out_itr(o),
			                            bkt_end(bt),
			                            in_itr(i) {}

			iterator(const iterator& src) : out_itr(src.out_itr),
			                                bkt_end(src.bkt_end),
			                                in_itr(src.in_itr) {}
			~iterator() = default;

			Key& operator*() { return *in_itr; }
			Key& operator*() const { return *in_itr; }

			Inside& operator->() { return in_itr; }
			Inside& operator->() const { return in_itr; }

			iterator& operator++()
			{
				++in_itr;
				if (in_itr == nullptr)
				{
					while (out_itr != bkt_end)
					{
						++out_itr;
						if (!out_itr->empty())
						{
							in_itr = out_itr->begin();
							return *this;
						}
					}
				}
				return *this;
			}

			iterator operator++(int)
			{
				iterator res = *this;
				++(*this);
				return res;
			}

			iterator operator+(i64 bias) const
			{
				iterator res = *this;
				while (bias--)
					++res;
				return res;
			}

			void operator+=(i64 bias)
			{
				while (bias--) ++(*this);
			}

			bool operator==(const iterator& x) const
			{
				return in_itr == x.in_itr;
			}

			bool operator!=(const iterator& x) const
			{
				return in_itr != x.in_itr;
			}
		};

		iterator begin() const
		{
			auto ed = bucket.end() + 1;
			for (auto o = bucket.begin(); o != ed; ++o)
				if (!o->empty())
					return {o, bucket.end(), o->begin()};
			return npos;
		}

		iterator end() const
		{
			auto ed = bucket.begin() - 1;
			for (auto o = bucket.end(); o != ed; --o)
				if (!o->empty())
					return {o, bucket.end(), o->begin()};
			return npos;
		}

		unordered_set();
		unordered_set(const self_type& src);
		unordered_set(self_type&& src) { this->move(src); }
		unordered_set(const std::initializer_list<Key>& ilist);
		~unordered_set() { this->clear(); }

		Key& front() { return *begin(); }
		Key& back() { return *end(); }
		const Key& front() const { return *begin(); }
		const Key& back() const { return *end(); }

		u64 size() const { return _size; }
		bool empty() const { return _size == 0; }
		void rehash();
		self_type& move(self_type& src);

		iterator find(const Key& _k) const;
		bool contains(const Key& _k) const;
		void insert(const Key& _k);
		bool erase(const Key& _k);
		void clear();

		self_type& operator=(const self_type& src);
		self_type& operator=(self_type&& src) { return this->move(src); }

		void print() const;
		void print_as_table() const;

	protected:
		prime_pointer bucket_size = PRIME_LIST;
		u64 _size = 0;
		vector<bucket_type> bucket;
		constexpr static const Hasher hash_fn {};
		constexpr static const iterator npos {};
	};

	template <class Key, class Hasher>
	unordered_set<Key, Hasher>::unordered_set()
	{
		vector<bucket_type> tmp(*bucket_size);
		this->bucket.move(tmp);
	}

	template <class Key, class Hasher>
	unordered_set<Key, Hasher>::
	        unordered_set(const self_type& src)
	{
		this->bucket_size = src.bucket_size;
		this->bucket = src.bucket;
		this->_size = src._size;
	}

	template <class Key, class Hasher>
	unordered_set<Key, Hasher>::
	        unordered_set(const std::initializer_list<Key>& ilist)
	{
		while (ilist.size() > *bucket_size)
			bucket_size++;
		vector<bucket_type> tmp(*bucket_size);
		tmp.shrink_to_fit();
		this->bucket.move(tmp);
		for (const auto& x: ilist) this->insert(x);
	}

	template <class Key, class Hasher>
	typename unordered_set<Key, Hasher>::iterator
	unordered_set<Key, Hasher>::find(const Key& _k) const
	{
		u64 index = hash_fn(_k) % *bucket_size;
		auto ed = bucket[index].end() + 1;
		for (auto it = bucket[index].begin();
		     it != ed; ++it)
		{
			if (it != nullptr && *it == _k)
				return {bucket.begin() + index,
				        bucket.end(), it};
		}
		return npos;
	}

	template <class Key, class Hasher>
	unordered_set<Key, Hasher>& unordered_set<Key, Hasher>::
	        move(self_type& src)
	{
		bucket_size = src.bucket_size;
		_size = src._size;
		bucket.move(src.bucket);
		src._size = 0;
		return *this;
	}

	template <class Key, class Hasher>
	unordered_set<Key, Hasher>& unordered_set<Key, Hasher>::
	operator=(const self_type& src)
	{
		bucket_size = src.bucket_size;
		bucket = src.bucket;
		_size = src._size;
		return *this;
	}

	template <class Key, class Hasher>
	bool unordered_set<Key, Hasher>::
	        contains(const Key& _k) const
	{
		return find(_k) != npos;
	}

	template <class Key, class Hasher>
	void unordered_set<Key, Hasher>::insert(const Key& _k)
	{
		auto it = find(_k);
		if (it == npos)
		{
			if (_size == *bucket_size - 1) rehash();
			u64 index = hash_fn(_k) % *bucket_size;
			bucket[index].push_back(_k);
			++_size;
		}
	}

	template <class Key, class Hasher>
	bool unordered_set<Key, Hasher>::erase(const Key& _k)
	{
		u64 index = hash_fn(_k) % *bucket_size;
		auto ed = bucket[index].end() + 1;
		for (auto it = bucket[index].begin();
		     it != ed && it != nullptr; ++it)
		{
			if (*it == _k)
			{
				bucket[index].erase(it);
				_size--;
				return true;
			}
		}
		return false;
	}

	template <class Key, class Hasher>
	void unordered_set<Key, Hasher>::clear()
	{
		if (!this->empty())
		{
			for_each(bucket, [](bucket_type& x) { x.clear(); });
			_size = 0;
		}
	}

	template <class Key, class Hasher>
	void unordered_set<Key, Hasher>::rehash()
	{
		bucket_size++;
		vector<bucket_type> tmp(*bucket_size);
		tmp.shrink_to_fit();

		auto opr = [this, &tmp](Key& x) {
			u64 index = hash_fn(x) % (*bucket_size);
			tmp[index].emplace_back();
			tmp[index].back() = std::move(x);
		};

		for_each(*this, opr);
		bucket.move(tmp);
	}

	template <class Key, class Hasher>
	void unordered_set<Key, Hasher>::print() const
	{
		auto pr = [this](const auto& x) {
			std::cout << x;
			if (x != *this->end())
				printf(", ");
		};

		printf("\nhash_table @%#llx = {", (u64) bucket.data());
		for_each(*this, pr);
		printf("}\n");
	}

	template <class Key, class Hasher>
	void unordered_set<Key, Hasher>::print_as_table() const
	{
		printf("\n");
		u64 collison = 0;
		for (u64 n = 0; n < *bucket_size; ++n)
		{
			if (!bucket[n].empty())
			{
				collison += bucket[n].size() - 1;
				printf("#%lld: ", n);
				for_each(bucket[n],
				         [](const auto& x) { std::cout << x << " ~ "; });
				printf("\n");
			}
		}
		printf("[Buckect: %lld || Collision: %.2lf%% || Average: %.2lf ]\n",
		       *bucket_size, static_cast<f64>(collison) * 100 / _size,
		       static_cast<f64>(_size) / *bucket_size);
	}
}

#endif
