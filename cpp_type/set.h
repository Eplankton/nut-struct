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

		bool contains(const T &_k)
		{
			return AVL<T, Compare>::find(_k) != this->npos;
		}
	};

	template <typename T, class Compare>
	set<T, Compare>::set(std::initializer_list<T> ilist)
	{
		for (auto it = ilist.begin(); it != ilist.end(); it++)
		{
			AVL<T>::insert(*it);
		}
	}
}

#endif