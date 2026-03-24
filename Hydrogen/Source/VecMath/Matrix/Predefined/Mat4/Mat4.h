#pragma once

#include "Common.h"
#include "../Mat3/Mat3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


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

		HYD void Scale(Vec3 pScale);
		HYD void Translate(Vec3 pTranslate);
		HYD void Rotate(Vec4 pRotate);


		HYD void Reset();

		HYD float& At(uint8 pRow, uint8 pColumn) const;

		HYD inline float* GetPtr() noexcept { return glm::value_ptr(m_Mat); }

	private:
		mutable glm::mat4 m_Mat = glm::mat4(1.0f);
	};

};