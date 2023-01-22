#ifndef _NUTS_HASH_SET_
#define _NUTS_HASH_SET_

#include "functional.h"
#include "list.h"
#include "move.h"
#include "range.h"
#include "vector.h"

namespace nuts
{
	// Capacity of the bucket
	static constexpr u64 PRIME_LIST[] = {
	        /* 0     */ 5ul,
	        /* 1     */ 11ul,
	        /* 2     */ 23ul,
	        /* 3     */ 47ul,
	        /* 4     */ 97ul,
	        /* 5     */ 199ul,
	        /* 6     */ 409ul,
	        /* 7     */ 823ul,
	        /* 8     */ 1741ul,
	        /* 9     */ 3469ul,
	        /* 10    */ 6949ul,
	        /* 11    */ 14033ul,
	        /* 12    */ 28411ul,
	        /* 13    */ 57557ul,
	        /* 14    */ 116731ul,
	        /* 15    */ 236897ul,
	        /* 16    */ 480881ul,
	        /* 17    */ 976369ul,
	        /* 18    */ 1982627ul,
	        /* 19    */ 4026031ul,
	        /* 20    */ 8175383ul,
	        /* 21    */ 16601593ul,
	        /* 22    */ 33712729ul,
	        /* 23    */ 68460391ul,
	        /* 24    */ 139022417ul,
	        /* 25    */ 282312799ul,
	        /* 26    */ 573292817ul,
	        /* 27    */ 1164186217ul,
	        /* 28    */ 2364114217ul,
	        /* 29    */ 4294967291ul,
	        /* 30    */ 8589934583ULL,
	        /* 31    */ 17179869143ULL,
	        /* 32    */ 34359738337ULL,
	        /* 33    */ 68719476731ULL,
	        /* 34    */ 137438953447ULL,
	        /* 35    */ 274877906899ULL,
	        /* 36    */ 549755813881ULL,
	        /* 37    */ 1099511627689ULL,
	        /* 38    */ 2199023255531ULL,
	        /* 39    */ 4398046511093ULL,
	        /* 40    */ 8796093022151ULL,
	        /* 41    */ 17592186044399ULL,
	        /* 42    */ 35184372088777ULL,
	        /* 43    */ 70368744177643ULL,
	        /* 44    */ 140737488355213ULL,
	        /* 45    */ 281474976710597ULL,
	        /* 46    */ 562949953421231ULL,
	        /* 47    */ 1125899906842597ULL,
	        /* 48    */ 2251799813685119ULL,
	        /* 49    */ 4503599627370449ULL,
	        /* 50    */ 9007199254740881ULL,
	        /* 51    */ 18014398509481951ULL,
	        /* 52    */ 36028797018963913ULL,
	        /* 53    */ 72057594037927931ULL,
	        /* 54    */ 144115188075855859ULL,
	        /* 55    */ 288230376151711717ULL,
	        /* 56    */ 576460752303423433ULL,
	        /* 57    */ 1152921504606846883ULL,
	        /* 58    */ 2305843009213693951ULL,
	        /* 59    */ 4611686018427387847ULL,
	        /* 60    */ 9223372036854775783ULL,
	        /* 61    */ 18446744073709551557ULL,
	};

	template <typename K, typename Hasher = hash<K>>
	class unordered_set
	{
	public:
		using prime_pointer = const u64*;
		using value_type = K;
		using self_type = unordered_set<K, Hasher>;
		using bucket_type = list<K>;

		class iterator
		    : public forward_iterator
		{
		public:
			using value_type = K;
			using Outside = typename vector<bucket_type>::iterator;
			using Inside = typename bucket_type::iterator;

		protected:
			Outside out_itr, bkt_end;
			Inside in_itr;

		public:
			iterator() = default;
			iterator(const Outside& o, const Outside& b,
			         const Inside& i) : out_itr(o), bkt_end(b),
			                            in_itr(i) {}

			iterator(const iterator& src) : out_itr(src.out_itr),
			                                bkt_end(src.bkt_end),
			                                in_itr(src.in_itr) {}
			~iterator() = default;

			K& operator*() { return *in_itr; }
			const K& operator*() const { return *in_itr; }
			const Inside& operator->() const { return in_itr; }

			iterator& operator++()
			{
				++in_itr;
				if (in_itr == bucket_type::npos)
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

			iterator operator+(u64 bias)
			        const { return nuts::advance(*this, bias); }

			void operator+=(i64 bias)
			{
				while (bias--) ++(*this);
			}

			inline bool operator==(const iterator& x) const
			{
				return in_itr == x.in_itr;
			}

			inline bool operator!=(const iterator& x) const
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
		unordered_set(self_type&& src) { move(src); }
		unordered_set(const std::initializer_list<K>& ilist);
		~unordered_set() { clear(); }

		K& front() { return *begin(); }
		K& back() { return *end(); }
		const K& front() const { return *begin(); }
		const K& back() const { return *end(); }

		inline u64 size() const { return _size; }
		inline bool empty() const { return _size == 0; };
		self_type& move(self_type& src);

		inline u64 get_index(const K& _k)
		        const { return hash_fn(_k) % *bucket_size; }

		iterator find(const K& _k) const;
		bool contains(const K& _k) const;
		void insert(const K& _k);
		void insert(K&& _k);
		bool erase(const K& _k);
		void rehash();
		void clear();

		self_type& operator=(const self_type& src);
		self_type& operator=(self_type&& src) { return move(src); }

		void print() const;
		void print_as_table() const;

	protected:
		prime_pointer bucket_size = PRIME_LIST;
		u64 _size = 0;
		vector<bucket_type> bucket;

	public:
		static constexpr Hasher hash_fn {};
		static constexpr iterator npos {};
	};

	// Deduction Guide
	template <class K>
	unordered_set(const std::initializer_list<K>&) -> unordered_set<K>;

	template <class K, class Hasher = nuts::hash<K>>
	using hash_set = unordered_set<K, Hasher>;

	template <class K, class Hasher>
	unordered_set<K, Hasher>::unordered_set()
	{
		vector<bucket_type> tmp(*bucket_size);
		bucket.move(tmp);
	}

	template <class K, class Hasher>
	unordered_set<K, Hasher>::
	        unordered_set(const self_type& src)
	{
		bucket_size = src.bucket_size;
		bucket = src.bucket;
		_size = src._size;
	}

	template <class K, class Hasher>
	unordered_set<K, Hasher>::
	        unordered_set(const std::initializer_list<K>& ilist)
	{
		while (ilist.size() > *bucket_size &&
		       *bucket_size != PRIME_LIST[31]) ++bucket_size;
		vector<bucket_type> tmp(*bucket_size);
		bucket.move(tmp);
		for (const auto& x: ilist) insert(x);
	}

	template <class K, class Hasher>
	typename unordered_set<K, Hasher>::iterator
	unordered_set<K, Hasher>::find(const K& _k) const
	{
		u64 i = get_index(_k);
		auto it = nuts::find(bucket[i], _k);
		if (it == bucket_type::npos)
			return npos;
		else
			return {advance(bucket.begin(), i), bucket.end(), it};
	}

	template <class K, class Hasher>
	unordered_set<K, Hasher>& unordered_set<K, Hasher>::
	        move(self_type& src)
	{
		bucket_size = src.bucket_size;
		_size = src._size;
		bucket.move(src.bucket);
		bucket_size = PRIME_LIST;
		src._size = 0;
		return *this;
	}

	template <class K, class Hasher>
	unordered_set<K, Hasher>& unordered_set<K, Hasher>::
	operator=(const self_type& src)
	{
		bucket_size = src.bucket_size;
		bucket = src.bucket;
		_size = src._size;
		return *this;
	}

	template <class K, class Hasher>
	bool unordered_set<K, Hasher>::
	        contains(const K& _k) const
	{
		return find(_k) != npos;
	}

	template <class K, class Hasher>
	void unordered_set<K, Hasher>::insert(const K& _k)
	{
		K tmp = _k;
		insert(nuts::move(tmp));
	}

	template <class K, class Hasher>
	void unordered_set<K, Hasher>::insert(K&& _k)
	{
		auto it = find(_k);
		if (it == npos)
		{
			if (_size == *bucket_size - 1) rehash();
			u64 index = get_index(_k);
			bucket[index].push_back(static_cast<K&&>(_k));
			++_size;
		}
	}

	template <class K, class Hasher>
	bool unordered_set<K, Hasher>::erase(const K& _k)
	{
		u64 i = get_index(_k);
		auto it = nuts::find(bucket[i], _k);
		if (it == bucket_type::npos)
			return false;
		else
		{
			bucket[i].erase(it);
			_size--;
			return true;
		}
	}

	template <class K, class Hasher>
	void unordered_set<K, Hasher>::clear()
	{
		if (!empty())
		{
			for_each(bucket, [](bucket_type& x) { x.clear(); });
			bucket_size = PRIME_LIST;
			_size = 0;
		}
	}

	template <class K, class Hasher>
	void unordered_set<K, Hasher>::rehash()
	{
		bucket_size++;
		vector<bucket_type> tmp(*bucket_size);

		auto opr = [&](K& x) {
			u64 index = get_index(x);
			tmp[index].push_back(nuts::move(x));
		};

		for_each(*this, opr);
		bucket.move(tmp);
	}

	template <class K, class Hasher>
	void unordered_set<K, Hasher>::print() const
	{
		auto pr = [&](const auto& x) {
			nuts::print(x);
			if (&x != &back()) nuts::print(", ");
		};

		nuts::print("hash_set = {");
		for_each(*this, pr);
		nuts::println("}");
	}

	template <class K, class Hasher>
	void unordered_set<K, Hasher>::print_as_table() const
	{
		u64 collison = 0;
		for (u64 n = 0; n < *bucket_size; ++n)
		{
			if (!bucket[n].empty())
			{
				collison += bucket[n].size() - 1;
				printf("#%lld: ", n);
				for_each(bucket[n],
				         [](const auto& x) { nuts::print(x, "->"); });
				printf("\n");
			}
		}
		printf("[Buckect: %lld || Collision: %.2lf%% || Average: %.2lf ]\n",
		       *bucket_size, static_cast<f64>(collison) * 100 / _size,
		       static_cast<f64>(_size) / *bucket_size);
	}
}

#endif
