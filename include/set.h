#ifndef _NUTS_SET_
#define _NUTS_SET_ 1

#include "binary_tree.h"
#include "concept.h"
#include "functional.h"
#include "type.h"

namespace nuts
{
	template <typename T, class Compare = nuts::less<T>>
	class set : public AVL<T, Compare>
	{
	public:
		using value_type = T;
		using Base_type = AVL<T, Compare>;


	public:
		set() { this->root = nullptr, this->_size = 0; }
		set(const std::initializer_list<T>& ilist);
		set(const set<T, Compare>& src);
		set(set<T, Compare>&& src) { Base_type::move(src); }
		~set() { this->_size = 0; }

		set<T, Compare>& operator=(const set<T, Compare>& src);
		set<T, Compare>& operator=(set<T, Compare>&& src)
		{
			Base_type::move(src);
			return *this;
		}

		void print() const;
	};

	template <typename T, class Compare>
	set<T, Compare>::set(const std::initializer_list<T>& ilist)
	{
		for_each(ilist.begin(), ilist.end() - 1,
		         [this](const T& x) { Base_type::insert(x); });
	}

	template <typename T, class Compare>
	set<T, Compare>::set(const set<T, Compare>& src)
	{
		for_each(src, [this](const T& x) { Base_type::insert(x); });
	}

	template <typename T, class Compare>
	set<T, Compare>& set<T, Compare>::operator=(const set<T, Compare>& src)
	{
		Base_type::clear();
		for_each(src, [this](const T& x) { Base_type::insert(x); });
		return *this;
	}

	template <typename T, class Compare>
	void set<T, Compare>::print() const
	{
		auto pr = [this](const auto& x) {
			std::cout << x;
			if (&x != &this->back())
				printf(", ");
		};

		printf("\nset @%#llx = {", (u64) this->root.get());
		if (!this->empty())
			for_each(*this, pr);
		printf("}\n");
	}
}

#endif