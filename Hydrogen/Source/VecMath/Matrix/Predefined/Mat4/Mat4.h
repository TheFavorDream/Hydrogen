#pragma once

#include "Common.h"
#include "../Mat3/Mat3.h"

#ifdef TEST
#define HYD
#endif

namespace Hydrogen
{

	/*
		Matrices are stored as Row-major.
		At(Row, Column)
	*/

	class Mat4
	{
	public:

		//Constructor & Destructor
		HYD  Mat4();
		HYD  Mat4(Mat3 pMat3);
		HYD  Mat4(std::initializer_list<float> pList);
		HYD  Mat4(const Mat4& pOther);

		//Operations

		HYD Mat4& operator=(const Mat4& pOther);
		HYD Mat4  operator+(const Mat4& pMat4) const;
		HYD Mat4  operator-(const Mat4& pMat4) const;
		HYD Mat4  operator*(const Mat4& pMat4) const;
		

		HYD Mat4  Transpose();
		HYD float Determinant();

		HYD void Reset();

		HYD float& At(uint8 pRow, uint8 pColumn) const;

		HYD inline float* GetPtr() noexcept { return m_Ptr; }


		HYD void Print()
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					std::cout << At(i, j) << ',';
				}
				std::cout << '\n';
			}
		}

	private:
		//Matrix Data
		mutable float m_Ptr[16] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	};

};