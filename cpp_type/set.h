#ifndef _NUTS_SET_
#define _NUTS_SET_ 1

#include "binary_tree.h"
#include "functional.h"
#include "type.h"

namespace nuts
{
	template <typename T, class Compare = nuts::less<T>>
	class set : public AVL<T, Compare>
	{
	public:
		using value_type = T;

	public:
		set() { this->root = nullptr, this->_size = 0; }
		set(const std::initializer_list<T>& ilist);
		~set() { this->_size = 0; }
		void print() const;
	};

	template <typename T, class Compare>
	set<T, Compare>::set(const std::initializer_list<T>& ilist)
	{
		for_each(ilist.begin(), ilist.end() - 1,
		         [this](const T& x) { this->insert(x); });
	}

	template <typename T, class Compare>
	void set<T, Compare>::print() const
	{
		auto pr = [this](const auto& x) {
			std::cout << x;
			if (x != this->max())
				printf(", ");
		};

		printf("\nset @%#llx = {", (u64) this->root.get());
		if (!this->empty())
			for_each(this->begin(), this->end(), pr);
		printf("}\n");
	}
}

#endif