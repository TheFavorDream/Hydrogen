#include "Mat3.h"

namespace Hydrogen
{

//Constructor
	Mat3::Mat3()
	{
		//Init as Identity
		At(0, 0) = 1.0f;
		At(1, 1) = 1.0f;
		At(2, 2) = 1.0f;
	}

	Mat3::Mat3(std::initializer_list<float> pList)
	{
		if (pList.size() > 9)
			__debugbreak();

		uint8 Index = 0;
		for (auto &val : pList)
		{
			m_Ptr[Index] = val;
			Index++;
		}
	}

	Mat3::Mat3(Mat2 pMat2)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				At(i, j) = pMat2.At(i, j);
			}
		}
	}

//Operations
	Mat3 Mat3::operator+(Mat3 pMat3)
	{
		Mat3 Result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				Result.At(i, j) = this->At(i, j) + pMat3.At(i, j);
		return Result;
	}
	Mat3 Mat3::operator-(Mat3 pMat3)
	{
		Mat3 Result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				Result.At(i, j) = this->At(i, j) - pMat3.At(i, j);
		return Result;
	}


	Mat3 Mat3::operator*(Mat3 pMat3)
	{
		return Mat3({
				Dot(Vec3(m_Ptr[0], m_Ptr[3], m_Ptr[6]), Vec3(pMat3.m_Ptr[0], pMat3.m_Ptr[1], pMat3.m_Ptr[2])),
				Dot(Vec3(m_Ptr[0], m_Ptr[3], m_Ptr[6]), Vec3(pMat3.m_Ptr[3], pMat3.m_Ptr[4], pMat3.m_Ptr[5])),
				Dot(Vec3(m_Ptr[0], m_Ptr[3], m_Ptr[6]), Vec3(pMat3.m_Ptr[6], pMat3.m_Ptr[7], pMat3.m_Ptr[8])),

				Dot(Vec3(m_Ptr[1], m_Ptr[4], m_Ptr[7]), Vec3(pMat3.m_Ptr[0], pMat3.m_Ptr[1], pMat3.m_Ptr[2])),
				Dot(Vec3(m_Ptr[1], m_Ptr[4], m_Ptr[7]), Vec3(pMat3.m_Ptr[3], pMat3.m_Ptr[4], pMat3.m_Ptr[5])),
				Dot(Vec3(m_Ptr[1], m_Ptr[4], m_Ptr[7]), Vec3(pMat3.m_Ptr[6], pMat3.m_Ptr[7], pMat3.m_Ptr[8])),

				Dot(Vec3(m_Ptr[2], m_Ptr[5], m_Ptr[8]), Vec3(pMat3.m_Ptr[0], pMat3.m_Ptr[1], pMat3.m_Ptr[2])),
				Dot(Vec3(m_Ptr[2], m_Ptr[5], m_Ptr[8]), Vec3(pMat3.m_Ptr[3], pMat3.m_Ptr[4], pMat3.m_Ptr[5])),
				Dot(Vec3(m_Ptr[2], m_Ptr[5], m_Ptr[8]), Vec3(pMat3.m_Ptr[6], pMat3.m_Ptr[7], pMat3.m_Ptr[8])),
			});
	}


	Mat3 Mat3::Transpose()
	{
		Mat3 Result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				Result.At(i, j) = this->At(j, i);
		return Result;
	}

	float Mat3::Determinant()
	{
		return (
				 m_Ptr[0]*(m_Ptr[4]*m_Ptr[8] - m_Ptr[7]*m_Ptr[5])
				-m_Ptr[3]*(m_Ptr[1]*m_Ptr[8] - m_Ptr[7]*m_Ptr[2])
				+m_Ptr[6]*(m_Ptr[1]*m_Ptr[5] - m_Ptr[4]*m_Ptr[2])
			);
	}

	float & Mat3::At(uint8 pColumn, uint8 pRow)
	{
		uint8 Index = (3 * pRow) + pColumn;
		if (Index >= 9)
			__debugbreak();
		return m_Ptr[Index];
	}
};