#ifndef _NUTS_UNO_SET_
#define _NUTS_UNO_SET_

#include "utility.h"
#include "vector.h"
#include "list.h"
#include "functional.h"

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
        using Self_Type = unordered_set<Key, Hasher>;
        using Bucket_Type = list<Key>;

    protected:
        const u64 *bucket_size = PRIME_LIST;
        u64 _size = 0;
        vector<Bucket_Type> bucket;
        Hasher hash_fn;

    public:
        class iterator
        {
            using Outside = typename vector<Bucket_Type>::iterator;
            using Inside = typename Bucket_Type::iterator;

        protected:
            Outside out_itr, bkt_end;
            Inside in_itr;

        public:
            iterator() = default;
            iterator(const Outside &o,
                     const Inside &i,
                     const Outside &bt) : out_itr(o),
                                          in_itr(i),
                                          bkt_end(bt) {}

            iterator(const iterator &src) : out_itr(src.out_itr),
                                            in_itr(src.in_itr),
                                            bkt_end(src.bkt_end) {}

            ~iterator() = default;

            Key &operator*() { return *in_itr; }
            const Key &operator*() const { return *in_itr; }

            iterator &operator++()
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
                while (bias > 0)
                {
                    ++res;
                    --bias;
                }
                return res;
            }

            bool operator==(const iterator &x) const
            {
                return this->in_itr == x.in_itr;
            }

            bool operator!=(const iterator &x) const
            {
                return this->in_itr != x.in_itr;
            }
        };

        iterator begin()
        {
            for (auto o = bucket.begin(); o != bucket.end() + 1; ++o)
            {
                if (!o->empty())
                {
                    return iterator{o, o->begin(), bucket.end()};
                }
            }
            return iterator();
        }

        iterator begin() const
        {
            for (auto o = bucket.begin(); o != bucket.end() + 1; ++o)
            {
                if (!o->empty())
                {
                    return iterator{o, o->begin(), bucket.end()};
                }
            }
            return iterator();
        }

        iterator end()
        {
            for (auto o = bucket.end(); o != bucket.begin() - 1; --o)
            {
                if (!o->empty())
                {
                    return iterator{o, o->begin(), bucket.end()};
                }
            }
            return iterator();
        }

        iterator end() const
        {
            for (auto o = bucket.end(); o != bucket.begin() - 1; --o)
            {
                if (!o->empty())
                {
                    return iterator{o, o->begin(), bucket.end()};
                }
            }
            return iterator();
        }

        static const iterator npos;

        unordered_set();
        unordered_set(const Self_Type &src);
        unordered_set(std::initializer_list<Key> ilist);
        ~unordered_set() { this->clear(); }

        Self_Type &move(Self_Type &src);
        u64 size() const { return _size; }
        bool empty() const { return _size == 0; }
        iterator find(const Key &_k) const;
        bool contains(const Key &_k) const;
        void rehash();
        void insert(const Key &_k);
        bool erase(const Key &_k);
        void clear();
        void print();
    };

    template <class Key, class Hasher>
    const typename unordered_set<Key, Hasher>::iterator
        unordered_set<Key, Hasher>::npos;

    template <class Key, class Hasher>
    unordered_set<Key, Hasher>::unordered_set()
    {
        vector<Bucket_Type> tmp(*bucket_size);
        tmp.shrink_to_fit();
        this->bucket.move(tmp);
    }

    template <class Key, class Hasher>
    unordered_set<Key, Hasher>::
        unordered_set(const Self_Type &src)
    {
        this->bucket_size = src.bucket_size;
        this->bucket(src.bucket);
        this->_size = src._size;
    }

    template <class Key, class Hasher>
    unordered_set<Key, Hasher>::
        unordered_set(std::initializer_list<Key> ilist)
    {
        while (ilist.size() > *bucket_size)
            bucket_size++;
        vector<Bucket_Type> tmp(*bucket_size);
        tmp.shrink_to_fit();
        this->bucket.move(tmp);
        for_each(ilist.begin(), ilist.end() - 1, [this](const Key &x)
                 { this->insert(x); });
    }

    template <class Key, class Hasher>
    typename unordered_set<Key, Hasher>::iterator
    unordered_set<Key, Hasher>::find(const Key &_k) const
    {
        u64 index = hash_fn(_k) % *bucket_size;
        for (auto res = bucket[index].begin();
             res != bucket[index].end() + 1;
             res++)
        {
            if (res != nullptr && *res == _k)
                return iterator{bucket.begin() + index,
                                res,
                                bucket.end()};
        }
        return npos;
    }

    template <class Key, class Hasher>
    unordered_set<Key, Hasher> &unordered_set<Key, Hasher>::
        move(Self_Type &src)
    {
        this->bucket_size = src.bucket_size;
        this->bucket.move(src.bucket);
        this->_size = src._size;
        src._size = 0;
        return *this;
    }

    template <class Key, class Hasher>
    bool unordered_set<Key, Hasher>::
        contains(const Key &_k) const
    {
        return this->find(_k) != npos;
    }

    template <class Key, class Hasher>
    void unordered_set<Key, Hasher>::insert(const Key &_k)
    {
        auto it = this->find(_k);
        if (it == npos)
        {
            if (_size == *bucket_size - 1)
                // if (_size >= *bucket_size - 1)
                this->rehash();
            u64 index = this->hash_fn(_k) % *this->bucket_size;
            this->bucket[index].push_back();
            this->bucket[index].back() = _k;
            this->_size++;
        }
    }

    template <class Key, class Hasher>
    bool unordered_set<Key, Hasher>::erase(const Key &_k)
    {
        u64 index = hash_fn(_k) % *bucket_size;
        for (auto res = bucket[index].begin();
             res != bucket[index].end() + 1;
             res++)
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
        if (!this->empty())
        {
            for_each(bucket.begin(), bucket.end(),
                     [](Bucket_Type &x)
                     { x.clear(); });
            _size = 0;
        }
    }

    template <class Key, class Hasher>
    void unordered_set<Key, Hasher>::print()
    {
        printf("\n");
        u64 collison = 0;
        for (u64 n = 0; n < *bucket_size; n++)
        {
            if (!bucket[n].empty())
            {
                collison += bucket[n].size() - 1;
                std::cout << '#' << n << ": ";
                for_each(bucket[n].begin(), bucket[n].end(),
                         [](const auto &x)
                         { std::cout << x << " ~ "; });
                std::cout << '\n';
            }
        }
        printf("[Buckect: %lld || Collision: %.2lf%% || Load: %.2lf%% ]\n",
               *bucket_size, (f64)collison * 100.0 / _size,
               (f64)_size * 100.0 / *bucket_size);
    }

    template <class Key, class Hasher>
    void unordered_set<Key, Hasher>::rehash()
    {
        bucket_size++;
        vector<Bucket_Type> tmp(*bucket_size);
        tmp.shrink_to_fit();

        auto fv = [this, &tmp](const Key &x)
        {
            u64 index = this->hash_fn(x) % (*this->bucket_size);
            tmp[index].push_back(x);
        };

        for_each(begin(), end(), fv);
        this->bucket.move(tmp);
    }
}

#endif
