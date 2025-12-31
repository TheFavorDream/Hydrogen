/*
	Definition and Implementation for the Hydrogen's internal matrix.
*/

#pragma once

#include "../../Common.h"

namespace Hydrogen
{
	class Matrix
	{
	public:

		Matrix();
		Matrix(const Matrix& pOther);
		Matrix(Matrix&& pOther);
		Matrix(uint32 pWidth, uint32 pHeight);
	   ~Matrix();


	   Matrix operator+(Matrix& pOther);
	   Matrix operator-(Matrix& pOther);


		float& At(uint32 pI, uint32 pJ);

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