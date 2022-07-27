#ifndef _NUTS_MATRIX_
#define _NUTS_MATRIX_ 1

#include "array.h"
#include "type.h"

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
		using Self_Type = matrix<T, Row, Col>;

	protected:
		array<array<T, Col>, Row> m;

	public:
		matrix() = default;
		matrix(const T& _val);
		matrix(const Self_Type& src);
		matrix(const std::initializer_list<array<T, Col>>& ilist);
		matrix(const std::initializer_list<T>& ilist);
		~matrix() = default;

		u64 row() const { return Row; }
		u64 col() const { return Col; }
		void print() const;

		array<T, Col>& operator[](u64 _r) { return m[_r]; }
		const array<T, Col>& operator[](u64 _r) const { return m[_r]; }

		T& at(u64 _r, u64 _c)
		{
			assert(_r < row() && _c < col());
			return m[_r][_c];
		}

		const T& at(u64 _r, u64 _c) const
		{
			assert(_r < row() && _c < col());
			return m[_r][_c];
		}

		template <typename Ty, u64 R, u64 C>
		friend matrix<Ty, R, C> operator+(const matrix<Ty, R, C>& A,
		                                  const matrix<Ty, R, C>& B);

		template <typename Ty, u64 R, u64 C>
		friend matrix<Ty, R, C> operator-(const matrix<Ty, R, C>& A,
		                                  const matrix<Ty, R, C>& B);

		template <typename Ty, u64 R1, u64 C1, u64 C2>
		friend matrix<Ty, R1, C2> operator*(const matrix<Ty, R1, C1>& A,
		                                    const matrix<Ty, C1, C2>& B);

		Self_Type& operator=(const Self_Type& src);
	};

	template <typename T, u64 Row, u64 Col>
	matrix<T, Row, Col>::
	        matrix(const T& _val)
	{
		for (u64 i = 0; i < row(); ++i)
			for (u64 j = 0; j < col(); ++j)
				m[i][j] = _val;
	}

	template <typename T, u64 Row, u64 Col>
	matrix<T, Row, Col>::
	        matrix(const std::initializer_list<array<T, Col>>& ilist)
	{
		auto st = ilist.begin();
		for (u64 i = 0; i < row(); ++i)
			m[i] = *(st++);
	}

	template <typename T, u64 Row, u64 Col>
	matrix<T, Row, Col>::
	        matrix(const std::initializer_list<T>& ilist)
	{
		auto st = ilist.begin();
		for (u64 i = 0; i < row(); ++i)
			for (u64 j = 0; j < col(); ++j)
				m[i][j] = *(st++);
	}

	template <typename T, u64 Row, u64 Col>
	matrix<T, Row, Col>::matrix(const Self_Type& src)
	{
		for (u64 i = 0; i < row(); ++i)
			for (u64 j = 0; j < col(); ++j)
				m[i][j] = src[i][j];
	}

	template <typename T, u64 Row, u64 Col>
	matrix<T, Row, Col>&
	matrix<T, Row, Col>::operator=(const Self_Type& src)
	{
		for (u64 i = 0; i < row(); ++i)
			for (u64 j = 0; j < col(); ++j)
				m[i][j] = src[i][j];
		return *this;
	}

	template <typename T, u64 Row, u64 Col>
	void matrix<T, Row, Col>::print() const
	{
		printf("\n");
		auto row_array_print = [](const auto& r) {
			auto trav_in_col = [&r](const auto& c) {
				std::cout << (T) c;
				if (&c != &r.back())
					printf(", ");
			};

			printf("[");
			nuts::for_each(r.begin(), r.end(), trav_in_col);
			printf("]\n");
		};

		for (u64 i = 0; i < row(); ++i)
		{
			row_array_print(m[i]);
		}
	}

	template <typename T, u64 R, u64 C>
	matrix<T, R, C> operator+(const matrix<T, R, C>& A, const matrix<T, R, C>& B)
	{
		matrix<T, R, C> res;
		for (u64 i = 0; i < res.row(); ++i)
			for (u64 j = 0; j < res.col(); ++j)
				res[i][j] = A[i][j] + B[i][j];
		return res;
	}

	template <typename T, u64 R, u64 C>
	matrix<T, R, C> operator-(const matrix<T, R, C>& A, const matrix<T, R, C>& B)
	{
		matrix<T, R, C> res;
		for (u64 i = 0; i < res.row(); ++i)
			for (u64 j = 0; j < res.col(); ++j)
				res[i][j] = A[i][j] - B[i][j];
		return res;
	}

	template <typename Ty, u64 R1, u64 C1, u64 C2>
	matrix<Ty, R1, C2>
	operator*(const matrix<Ty, R1, C1>& A,
	          const matrix<Ty, C1, C2>& B)
	{
		matrix<Ty, R1, C2> res(0);
		for (u64 i = 0; i < A.row(); ++i)
			for (u64 j = 0; j < B.col(); ++j)
				for (u64 c = 0; c < A.col(); ++c)
					res[i][j] += A[i][c] * B[c][j];
		return res;
	}
}

#endif