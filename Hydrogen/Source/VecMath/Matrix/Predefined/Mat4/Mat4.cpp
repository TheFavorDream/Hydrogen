#include "Mat4.h"

namespace Hydrogen
{



	Mat4::Mat4()
	{
		At(0, 0) = 1.0f;
		At(1, 1) = 1.0f;
		At(2, 2) = 1.0f;
		At(3, 3) = 1.0f;

	}

	Mat4::Mat4(Mat3 pMat3)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				At(i, j) = pMat3.At(i, j);
			}
		}
	}

	Mat4::Mat4(std::initializer_list<float> pList)
	{
		if (pList.size() > 16)
			__debugbreak();

		uint8 Index = 0;
		for (auto &val : pList)
		{
			m_Ptr[Index] = val;
			Index++;
		}
	}

	Mat4::Mat4(const Mat4 & pOther)
	{
		memcpy(m_Ptr, pOther.m_Ptr, (16) * sizeof(float));
	}

	Mat4& Mat4::operator=(const Mat4 & pOther)
	{
		if (this != &pOther)
		{
			memcpy(m_Ptr, pOther.m_Ptr, (16)*sizeof(float));
		}
		return *this;
	}

	Mat4 Mat4::operator+(const Mat4& pMat4) const
	{
		Mat4 Result;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				Result.At(i, j) = this->At(i, j) + pMat4.At(i, j);
		return Result;
	}

	Mat4 Mat4::operator-(const Mat4& pMat4) const
	{
		Mat4 Result;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				Result.At(i, j) = this->At(i, j) - pMat4.At(i, j);
		return Result;
	}

	Mat4 Mat4::operator*(const Mat4& pMat4) const
	{
		//TODO: Optimize this using SIMD
		return Mat4({
				Dot(Vec4(m_Ptr[0], m_Ptr[1], m_Ptr[2] , m_Ptr[3]), Vec4(pMat4.m_Ptr[0],  pMat4.m_Ptr[4],  pMat4.m_Ptr[8],  pMat4.m_Ptr[12])),
				Dot(Vec4(m_Ptr[0], m_Ptr[1], m_Ptr[2] , m_Ptr[3]), Vec4(pMat4.m_Ptr[1],  pMat4.m_Ptr[5],  pMat4.m_Ptr[9],  pMat4.m_Ptr[13])),
				Dot(Vec4(m_Ptr[0], m_Ptr[1], m_Ptr[2] , m_Ptr[3]), Vec4(pMat4.m_Ptr[2],  pMat4.m_Ptr[6],  pMat4.m_Ptr[10], pMat4.m_Ptr[14])),
				Dot(Vec4(m_Ptr[0], m_Ptr[1], m_Ptr[2] , m_Ptr[3]), Vec4(pMat4.m_Ptr[3],  pMat4.m_Ptr[7],  pMat4.m_Ptr[11], pMat4.m_Ptr[15])),

				Dot(Vec4(m_Ptr[4], m_Ptr[5], m_Ptr[6], m_Ptr[7]), Vec4(pMat4.m_Ptr[0], pMat4.m_Ptr[4], pMat4.m_Ptr[8], pMat4.m_Ptr[12])),
				Dot(Vec4(m_Ptr[4], m_Ptr[5], m_Ptr[6], m_Ptr[7]), Vec4(pMat4.m_Ptr[1],  pMat4.m_Ptr[5],  pMat4.m_Ptr[9],  pMat4.m_Ptr[13])),
				Dot(Vec4(m_Ptr[4], m_Ptr[5], m_Ptr[6], m_Ptr[7]), Vec4(pMat4.m_Ptr[2],  pMat4.m_Ptr[6],  pMat4.m_Ptr[10], pMat4.m_Ptr[14])),
				Dot(Vec4(m_Ptr[4], m_Ptr[5], m_Ptr[6], m_Ptr[7]), Vec4(pMat4.m_Ptr[3],  pMat4.m_Ptr[7],  pMat4.m_Ptr[11], pMat4.m_Ptr[15])),

				Dot(Vec4(m_Ptr[8], m_Ptr[9], m_Ptr[10], m_Ptr[11]), Vec4(pMat4.m_Ptr[0], pMat4.m_Ptr[4], pMat4.m_Ptr[8], pMat4.m_Ptr[12])),
				Dot(Vec4(m_Ptr[8], m_Ptr[9], m_Ptr[10], m_Ptr[11]), Vec4(pMat4.m_Ptr[1],  pMat4.m_Ptr[5],  pMat4.m_Ptr[9],  pMat4.m_Ptr[13])),
				Dot(Vec4(m_Ptr[8], m_Ptr[9], m_Ptr[10], m_Ptr[11]), Vec4(pMat4.m_Ptr[2],  pMat4.m_Ptr[6],  pMat4.m_Ptr[10], pMat4.m_Ptr[14])),
				Dot(Vec4(m_Ptr[8], m_Ptr[9], m_Ptr[10], m_Ptr[11]), Vec4(pMat4.m_Ptr[3],  pMat4.m_Ptr[7],  pMat4.m_Ptr[11], pMat4.m_Ptr[15])),

				Dot(Vec4(m_Ptr[12], m_Ptr[13], m_Ptr[14], m_Ptr[15]), Vec4(pMat4.m_Ptr[0], pMat4.m_Ptr[4], pMat4.m_Ptr[8], pMat4.m_Ptr[12])),
				Dot(Vec4(m_Ptr[12], m_Ptr[13], m_Ptr[14], m_Ptr[15]), Vec4(pMat4.m_Ptr[1],  pMat4.m_Ptr[5],  pMat4.m_Ptr[9],  pMat4.m_Ptr[13])),
				Dot(Vec4(m_Ptr[12], m_Ptr[13], m_Ptr[14], m_Ptr[15]), Vec4(pMat4.m_Ptr[2],  pMat4.m_Ptr[6],  pMat4.m_Ptr[10], pMat4.m_Ptr[14])),
				Dot(Vec4(m_Ptr[12], m_Ptr[13], m_Ptr[14], m_Ptr[15]), Vec4(pMat4.m_Ptr[3],  pMat4.m_Ptr[7],  pMat4.m_Ptr[11], pMat4.m_Ptr[15])),

			});

	}

	Mat4 Mat4::Transpose()
	{
		Mat4 Result;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				Result.At(i, j) = this->At(j, i);
		return Result;
	}

	float Mat4::Determinant()
	{
		return (
				 m_Ptr[0] *(Mat3({ m_Ptr[5], m_Ptr[6] , m_Ptr[7] , m_Ptr[9] , m_Ptr[10] , m_Ptr[11] , m_Ptr[13] , m_Ptr[14] , m_Ptr[15] }).Determinant())
				-m_Ptr[4] *(Mat3({ m_Ptr[1], m_Ptr[2] , m_Ptr[3] , m_Ptr[9] , m_Ptr[10] , m_Ptr[11] , m_Ptr[13] , m_Ptr[14] , m_Ptr[15] }).Determinant())
			    +m_Ptr[8] *(Mat3({ m_Ptr[1], m_Ptr[2] , m_Ptr[3] , m_Ptr[5] , m_Ptr[6]  , m_Ptr[7]  , m_Ptr[13] , m_Ptr[14] , m_Ptr[15] }).Determinant())
				-m_Ptr[14]*(Mat3({ m_Ptr[1], m_Ptr[2] , m_Ptr[3] , m_Ptr[5] , m_Ptr[6]  , m_Ptr[7]  , m_Ptr[9]  , m_Ptr[10] , m_Ptr[11] }).Determinant())
			);
	}

	HYD void Mat4::Reset()
	{
		for (int i = 0; i < 4; i++)
		{
			At(i, i) = 1.0f;
		}
	}

	float& Mat4::At(uint8 pRow, uint8 pColumn) const
	{
		uint8 Index = (4 * pRow) + pColumn;
		if (Index >= 16)
			__debugbreak();
		return m_Ptr[Index];
	}

};