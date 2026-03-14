#pragma once

#include "Common.h"
#include "VecMath/Vector/VectorDef.h"

#ifdef TEST
#define HYD
#endif

namespace Hydrogen
{

	/*
		Matrices are stored as Column-major.
		At(Column, Row)
	*/

	class Mat2
	{
	public:

		//Constructors
		HYD  Mat2();
		HYD  Mat2(std::initializer_list<float> pList);

		//Operations

		HYD Mat2 operator+(Mat2 pMat2);
		HYD Mat2 operator-(Mat2 pMat2);
		HYD Mat2 operator*(Mat2 pMat2);
		 
		HYD Mat2  Transpose();
		HYD float Determinant();

		HYD float& At(uint8 pColumn, uint8 pRow);

		HYD inline float* GetPtr() noexcept { return m_Ptr; }

	private:
		//Matrix Data
		float m_Ptr[4] = {
			1.0f, 0.0f,
			0.0f, 1.0f
		};
	};

};