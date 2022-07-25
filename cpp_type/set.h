#ifndef _NUTS_SET_
#define _NUTS_SET_

#include "type.h"
#include "functional.h"
#include "binary_tree.h"

namespace nuts
{
	template <typename T, class Compare = nuts::less<T>>
	class set : public AVL<T, Compare>
	{
	public:
		set() { this->root = nullptr, this->_size = 0; }
		set(std::initializer_list<T> ilist);
		~set() { this->_size = 0; }
		void print() const;
	};

	template <typename T, class Compare>
	set<T, Compare>::set(std::initializer_list<T> ilist)
	{
		for (auto it = ilist.begin(); it != ilist.end(); it++)
		{
			this->insert(*it);
		}
	}

	// template <typename T, class Compare>
	// bool set<T, Compare>::contains(const T &_k) const
	// {
	// 	return AVL<T, Compare>::find(_k) != this->npos;
	// }

	template <typename T, class Compare>
	void set<T, Compare>::print() const
	{
		auto pr = [this](const auto &x)
		{
			std::cout << x;
			if (x != this->max())
				printf(", ");
		};

		printf("\nset@%#llx = {", (u64)this->root.get());
		for_each(this->begin(), this->end(), pr);
		printf("}\n");
	}
}

#endif