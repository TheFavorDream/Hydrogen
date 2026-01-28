/*
	Definition and Implementation for the Hydrogen's internal matrix.
*/

#pragma once

#include "Common.h"
#include <vector>
#include "../Vector/VectorDef.h"




namespace Hydrogen
{



	//Matrix Class
	class Matrix
	{
	public:


		HYD Matrix(float pDefaultValue=1.0f);
		HYD Matrix(const Matrix& pOther);
		HYD Matrix(Matrix&& pOther);
		HYD Matrix(uint32 pWidth, uint32 pHeight);
	    HYD ~Matrix();


	    HYD void operator=(const Matrix& pOther);
	    HYD Matrix operator+(const Matrix& pOther);
	    HYD Matrix operator-(const Matrix& pOther);
	    HYD Matrix operator*(const Matrix& pOther);

		HYD void CopyDataToMatrix(const std::vector<float>& pData);

		HYD float& At(uint32 pI, uint32 pJ) const;

		HYD bool IsIdentity();

		//Temp
		HYD void PrintMatrix();

		HYD inline uint32 GetWidth() const { return m_Width; }
		HYD inline uint32 GetHeight() const { return m_Height; }
		HYD inline float* GetPtr() const { return m_Ptr; }

		inline const Vec3& GetScale() { return m_Scale; }
		inline const Vec3& GetTranslate() { return m_Translate; }
		inline const Vec4& GetRotate() { return m_Rotate; }

	
	private:
		float* m_Ptr = nullptr;
		uint32 m_Width = 0, m_Height = 0;

		Vec3 m_Scale;
		Vec3 m_Translate;
		Vec4 m_Rotate;
	};


	//Matrix Operations:


	HYD Matrix Scale(const Matrix& pMatrix, const Vec3& pScale);
	HYD Matrix Translation(const Matrix& pMatrix, const Vec3& pTranslation);
	HYD Matrix RotateQuaternion(const Matrix& pMatrix, const Vec4& pRotation);
};