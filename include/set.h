#ifndef _NUTS_SET_
#define _NUTS_SET_

#include "binary_tree.h"

namespace nuts
{
	template <typename T, class Compare = nuts::less<T>>
	class set : public AVL<T, Compare>
	{
	public:
		using value_type = T;
		using base_type = AVL<T, Compare>;
		using self_type = set<T, Compare>;

	public:
		set() { this->root = nullptr, this->_size = 0; }
		set(const std::initializer_list<T>& ilist);
		set(const self_type& src);
		set(self_type&& src) { base_type::move(src); }
		~set() { this->_size = 0; }

		self_type& operator=(const self_type& src);
		self_type& operator=(self_type&& src);
		self_type& move(self_type& src);
		void print() const;
	};

	// Deduction Guide
	template <class K>
	set(const std::initializer_list<K>&) -> set<K>;

	template <typename T, class Compare>
	set<T, Compare>& set<T, Compare>::operator=(self_type&& src)
	{
		return move(src);
	}

	template <typename T, class Compare>
	set<T, Compare>& set<T, Compare>::move(self_type& src)
	{
		base_type::move(src);
		return *this;
	}

	template <typename T, class Compare>
	set<T, Compare>::set(const std::initializer_list<T>& ilist)
	{
		for (const auto& x: ilist) base_type::insert(x);
	}

	template <typename T, class Compare>
	set<T, Compare>::set(const self_type& src)
	{
		for_each(src, [](const T& x) { base_type::insert(x); });
	}

	template <typename T, class Compare>
	set<T, Compare>& set<T, Compare>::operator=(const self_type& src)
	{
		base_type::clear();
		for_each(src, [](const T& x) { base_type::insert(x); });
		return *this;
	}

	template <typename T, class Compare>
	void set<T, Compare>::print() const
	{
		auto pr = [&](const auto& x) {
			nuts::print(x);
			if (&x != &this->back())
				printf(", ");
		};

		printf("set @%#llx = {", (u64) this->root.get());
		if (!this->empty()) for_each(*this, pr);
		printf("}\n");
	}
}

#endif