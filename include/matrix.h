#ifndef _NUTS_MATRIX_
#define _NUTS_MATRIX_

#include "algorithm.h"
#include "array.h"
#include "concept.h"

namespace nuts
{
	/** @file matrix
     *  Create small matrix on stack.
     */

	template <typename T, u64 Row, u64 Col>
	class matrix
	{
	public:
		using value_type = T;
		using self_type = matrix<T, Row, Col>;
		using base_type = array<array<T, Col>, Row>;

	protected:
		base_type impl;

	public:
		matrix() = default;
		matrix(const T& _val);
		matrix(const self_type& src) = default;
		matrix(const std::initializer_list<array<T, Col>>& ilist);
		matrix(const std::initializer_list<T>& ilist);
		~matrix() = default;

		static constexpr u64 row() { return Row; }
		static constexpr u64 col() { return Col; }
		void print() const;

		inline array<T, Col>& operator[](u64 _r) { return impl[_r]; }
		inline const array<T, Col>& operator[](u64 _r) const { return impl[_r]; }

		T& at(u64 _r, u64 _c)
		{
			assert(_r < row() && _c < col());
			return impl[_r][_c];
		}

		const T& at(u64 _r, u64 _c) const
		{
			assert(_r < row() && _c < col());
			return impl[_r][_c];
		}

		template <typename RetType, u64 R, u64 C>
		friend matrix<RetType, R, C> operator+(const matrix<RetType, R, C>& A,
		                                       const matrix<RetType, R, C>& B);

		template <typename RetType, u64 R, u64 C>
		friend matrix<RetType, R, C> operator-(const matrix<RetType, R, C>& A,
		                                       const matrix<RetType, R, C>& B);

		template <typename RetType, u64 R1, u64 C1, u64 C2>
		friend matrix<RetType, R1, C2> operator*(const matrix<RetType, R1, C1>& A,
		                                         const matrix<RetType, C1, C2>& B);

		self_type& operator=(const self_type& src);
	};

	template <typename T, u64 Row, u64 Col>
	matrix<T, Row, Col>::
	        matrix(const T& _val)
	{
		for (u64 i = 0; i < row(); ++i)
			for (u64 j = 0; j < col(); ++j)
				impl[i][j] = _val;
	}

	template <typename T, u64 Row, u64 Col>
	matrix<T, Row, Col>::
	        matrix(const std::initializer_list<array<T, Col>>& ilist)
	{
		auto st = ilist.begin();
		for (u64 i = 0; i < row(); ++i)
			impl[i] = *(st++);
	}

	template <typename T, u64 Row, u64 Col>
	matrix<T, Row, Col>::
	        matrix(const std::initializer_list<T>& ilist)
	{
		auto st = ilist.begin();
		for (u64 i = 0; i < row(); ++i)
			for (u64 j = 0; j < col(); ++j)
				impl[i][j] = *(st++);
	}

	template <typename T, u64 Row, u64 Col>
	matrix<T, Row, Col>& matrix<T, Row, Col>::operator=(const self_type& src)
	{
		for (u64 i = 0; i < row(); ++i)
			for (u64 j = 0; j < col(); ++j)
				impl[i][j] = src[i][j];
		return *this;
	}

	template <typename T, u64 R, u64 C>
	matrix<T, R, C>
	operator+(const matrix<T, R, C>& A, const matrix<T, R, C>& B)
	{
		matrix<T, R, C> res;
		for (u64 i = 0; i < res.row(); ++i)
			for (u64 j = 0; j < res.col(); ++j)
				res[i][j] = A[i][j] + B[i][j];
		return res;
	}

	template <typename T, u64 R, u64 C>
	matrix<T, R, C>
	operator-(const matrix<T, R, C>& A, const matrix<T, R, C>& B)
	{
		matrix<T, R, C> res;
		for (u64 i = 0; i < res.row(); ++i)
			for (u64 j = 0; j < res.col(); ++j)
				res[i][j] = A[i][j] - B[i][j];
		return res;
	}

	template <typename RetType, u64 R1, u64 C1, u64 C2>
	matrix<RetType, R1, C2>
	operator*(const matrix<RetType, R1, C1>& A,
	          const matrix<RetType, C1, C2>& B)
	{
		matrix<RetType, R1, C2> res(0);
		for (u64 i = 0; i < A.row(); ++i)
			for (u64 j = 0; j < B.col(); ++j)
				for (u64 c = 0; c < A.col(); ++c)
					res[i][j] += A[i][c] * B[c][j];
		return res;
	}

	template <typename T, u64 Row, u64 Col>
	void matrix<T, Row, Col>::print() const
	{
		auto row_array_print = [](const auto& r) {
			auto trav_in_col = [&r](const auto& c) {
				nuts::print(c);
				if (&c != &r.back()) printf(", ");
			};
			printf("[");
			for_each(r, trav_in_col);
			printf("]\n");
		};

		for (u64 i = 0; i < row(); ++i)
			row_array_print(impl[i]);
		printf("\n");
	}
}

#endif
