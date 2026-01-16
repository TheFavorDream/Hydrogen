/*
	Definition and Implementation for the Hydrogen's internal matrix.
*/

#pragma once

#include "../../Common.h"
#include <vector>
#include "../Vector/VectorDef.h"

namespace Hydrogen
{

	//Matrix Operations:


	Matrix Scale(const Matrix& pMatrix, const Vec3& pScale);
	Matrix Translation(const Matrix& pMatrix, const Vec3& pTranslation);
	Matrix RotateQuaternion(const Matrix& pMatrix, const Vec4& pRotation);

	//Matrix Class
	class Matrix
	{
	public:

		Matrix(float pDefaultValue=1.0f);
		Matrix(const Matrix& pOther);
		Matrix(Matrix&& pOther);
		Matrix(uint32 pWidth, uint32 pHeight);
	   ~Matrix();


	   void operator=(Matrix&& pOther);
	   Matrix operator+(const Matrix& pOther);
	   Matrix operator-(const Matrix& pOther);
	   Matrix operator*(const Matrix& pOther);

	   void CopyDataToMatrix(const std::vector<float>& pData);

		float& At(uint32 pI, uint32 pJ) const;

		bool IsIdentity();

		//Temp
		void PrintMatrix();

		inline uint32 GetWidth() const { return m_Width; }
		inline uint32 GetHeight() const { return m_Height; }
		inline float* GetPtr() const { return m_Ptr; }
	private:
		float* m_Ptr = nullptr;
		uint32 m_Width = 0, m_Height = 0;
	};
};