#ifndef _NUTS_BITSET_
#define _NUTS_BITSET_ 1

#include "array.h"
#include "basic_string.h"
#include "functional.h"

#define BITSET_BLOCK_SIZE (u8) 8
#define GET_BLOCK_NUM(x) ((x / BITSET_BLOCK_SIZE) + 1)

namespace nuts
{
	template <u64 N>
	class bitset
	{
	public:
		using base_type = array<u8, GET_BLOCK_NUM(N)>;
		using self_type = bitset<N>;

		class bit_ref
		{
			friend class bitset;

		public:
			bit_ref() = default;
			bit_ref(const bit_ref&) = default;
			bit_ref(const bitset& obj, u64 pos)
					: _p(pos),
					  _m(const_cast<base_type *>(&obj.impl)) {}

			bit_ref& operator=(bool val)
			{
				u64 i = _p / 8,
						ofs = _p % 8,
						tmp = (u8) 1 << (u8) (7 - ofs);
				if (val)
					(*_m)[i] |= (tmp);
				else
					(*_m)[i] &= ~(tmp);
				return *this;
			}

			bool get() const
			{
				u64 i = _p / 8, ofs = _p % 8;
				return (((*_m)[i] >> (u8) (7 - ofs)) & (u8) 1) == (u8) 1;
			}

			void print() const { nuts::print(get()); }

			bool operator==(bool val) const { return get() == val; }
			bool operator!=(bool val) const { return get() != val; }

			bool operator==(const bit_ref& y) const { return get() == y.get(); }
			bool operator!=(const bit_ref& y) const { return get() != y.get(); }

		protected:
			base_type *_m = nullptr;
			u64 _p = 0;
		};

		friend class bit_ref;

		bitset() : impl(false) {}
		bitset(const bitset<N>& src) = default;
		explicit bitset(bool val);
		explicit bitset(const string& bit_str);
		explicit bitset(const char *src);

		static constexpr u64 size() { return N; }

		bit_ref operator[](u64 _n);
		bool operator[](u64 _n) const;

		u64 count() const;
		bool all() const;
		bool any() const;
		bool none() const;
		bool test(u64 pos) const
		{
			assert(pos < N);
			return bit_ref(*this, pos).get();
		}

		self_type& set();
		self_type& set(u64 pos, bool val = true);
		self_type& reset();
		self_type& reset(u64 pos);
		self_type& flip();
		self_type& flip(u64 pos);

		self_type& operator=(const string& bit_str);

		string to_string() const;
		void print_by_word() const;
		void print() const;

		bool operator==(const self_type& y) const;
		bool operator!=(const self_type& y) const;

		friend std::ostream&
		operator<<(std::ostream& output, const bitset<N>& obj)
		{
			for (int i = 0; i < obj.size(); i++)
				output << obj[i];
			return output;
		}

	protected:
		base_type impl;
	};

	template <u64 N>
	bitset<N>::bitset(bool val)
	{
		nuts::fill_n(impl.begin(), impl.size(), val ? ~(u8) 0 : (u8) 0);
	}

	template <u64 N>
	bitset<N>::bitset(const string& bit_str)
	{
		nuts::fill_n(impl.begin(), impl.size(), (u8) 0);
		for (u64 i = 0; i < bit_str.size() - 1 && i < N; i++)
			(*this)[i] = static_cast<bool>(bit_str[i] - 48);
	}

	template <u64 N>
	typename bitset<N>::bit_ref
	bitset<N>::operator[](u64 _n)
	{
		return bit_ref {*this, _n};
	}

	template <u64 N>
	bool bitset<N>::operator[](u64 _n) const
	{
		return bit_ref {*this, _n}.get();
	}

	template <u64 N>
	u64 bitset<N>::count() const
	{
		u64 res = 0;
		for (u64 i = 0; i < N; i++)
			if (test(i)) res++;
		return res;
	}

	template <u64 N>
	bool bitset<N>::all() const
	{
		for (u64 i = 0; i < N; i++)
			if (!test(i)) return false;
		return true;
	}

	template <u64 N>
	bool bitset<N>::any() const
	{
		for (u64 i = 0; i < N; i++)
			if (test(i)) return true;
		return false;
	}

	template <u64 N>
	bool bitset<N>::none() const
	{
		for (u64 i = 0; i < N; i++)
			if (test(i)) return false;
		return true;
	}

	template <u64 N>
	bitset<N>& bitset<N>::set()
	{
		nuts::fill_n(impl.begin(), impl.size(), ~((u8) 0));
		return *this;
	}

	template <u64 N>
	bitset<N>& bitset<N>::set(u64 pos, bool val)
	{
		(*this)[pos] = val;
		return *this;
	}

	template <u64 N>
	bitset<N>& bitset<N>::reset()
	{
		nuts::fill_n(impl.begin(), impl.size(), (u8) 0);
		return *this;
	}

	template <u64 N>
	bitset<N>& bitset<N>::reset(u64 pos)
	{
		(*this)[pos] = false;
		return *this;
	}

	template <u64 N>
	bitset<N>& bitset<N>::flip()
	{
		for (u64 i = 0; i < base_type::size(); i++)
			impl[i] = ~impl[i];
		return *this;
	}

	template <u64 N>
	bitset<N>& bitset<N>::flip(u64 pos)
	{
		(*this)[pos] = !test(pos);
		return *this;
	}

	template <u64 N>
	string bitset<N>::to_string() const
	{
		string res(N);
		for (u64 i = 0; i < N; i++)
			res[i] = static_cast<char>((u64) test(i) + 48);
		return res;
	}

	template <u64 N>
	void bitset<N>::print_by_word() const
	{
		for (u64 i = 0; i < 8 * base_type::size(); i++)
		{
			printf("%u", (*this)[i]);
			if ((i + 1) % 8 == 0) printf("\n");
		}
	}

	template <u64 N>
	bitset<N>& bitset<N>::operator=(const string& bit_str)
	{
		nuts::fill_n(impl.begin(), impl.size(), (u8) 0);
		for (u64 i = 0; i < bit_str.size() - 1 && i < N; i++)
			(*this)[i] = static_cast<bool>(bit_str[i] - 48);
		return *this;
	}

	template <u64 N>
	bool bitset<N>::operator==(const self_type& y) const
	{
		for (u64 i = 0; i < N; i++)
			if ((*this)[i] != y[i]) return false;
		return true;
	}

	template <u64 N>
	bool bitset<N>::operator!=(const self_type& y) const
	{
		return !((*this) == y);
	}

	template <u64 N>
	void bitset<N>::print() const
	{
		for (u64 i = 0; i < N; i++)
			printf("%u", test(i));
		printf("\n");
	}

	template <u64 N>
	bitset<N>::bitset(const char *src)
	{
		nuts::fill_n(impl.begin(), impl.size(), (u8) 0);
		for (u64 i = 0; i < strlen(src) - 1; i++)
			(*this)[i] = static_cast<bool>(src[i] - 48);
	}

	template <u64 N>
	struct hash<bitset<N>>
	{
		u64 operator()(const bitset<N>& x) const
		{
			return hash<u64> {}(x.count() + x.size());
		}
	};
}

#endif
