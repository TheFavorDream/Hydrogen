#pragma once

#include "Common.h"
#include "../Mat2/Mat2.h"
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

	class Mat3
	{
	public:

		//Constructor & Destructor
		HYD  Mat3();
		HYD  Mat3(std::initializer_list<float> pList);
		HYD  Mat3(Mat2 pMat2);
		//Operations

		HYD Mat3 operator+(Mat3 pMat3);
		HYD Mat3 operator-(Mat3 pMat3);
		HYD Mat3 operator*(Mat3 pMat3);
		
		HYD Mat3  Transpose();
		HYD float Determinant();

		HYD float& At(uint8 pColumn, uint8 pRow);

		HYD inline float* GetPtr() noexcept { return m_Ptr; }

	private:
		//Matrix Data
		float m_Ptr[9] = {
			1.0f , 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
	};

};