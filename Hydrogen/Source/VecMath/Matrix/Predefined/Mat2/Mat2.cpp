#include "Mat2.h"
#include "HydPch.h"


namespace Hydrogen
{

/*
	i -> column
	j -> row
*/

//Constrctors:
	Mat2::Mat2()
	{
		//Init matrix as identity:

		At(0, 0) = 1.0f;
		At(1, 1) = 1.0f;
	}

	Mat2::Mat2(std::initializer_list<float> pList)
	{
		if (pList.size() > 4)
			__debugbreak();

		uint8 Index = 0;
		for (auto &val : pList)
		{
			m_Ptr[Index] = val;
			Index++;
		}
	}


//Operations:

	Mat2 Mat2::operator+(Mat2 pMat2)
	{
		Mat2 Result;
		Result.At(0, 0) = this->At(0, 0) + pMat2.At(0, 0);
		Result.At(0, 1) = this->At(0, 1) + pMat2.At(0, 1);
		Result.At(1, 0) = this->At(1, 0) + pMat2.At(1, 0);
		Result.At(1, 1) = this->At(1, 1) + pMat2.At(1, 1);
		return Result;
	}

	Mat2 Mat2::operator-(Mat2 pMat2)
	{
		Mat2 Result;
		Result.At(0, 0) = this->At(0, 0) - pMat2.At(0, 0);
		Result.At(0, 1) = this->At(0, 1) - pMat2.At(0, 1);
		Result.At(1, 0) = this->At(1, 0) - pMat2.At(1, 0);
		Result.At(1, 1) = this->At(1, 1) - pMat2.At(1, 1);
		return Result;
	}

	Mat2 Mat2::operator*(Mat2 pMat2)
	{
		return Mat2({
				Dot(Vec2(m_Ptr[0], m_Ptr[2]), Vec2(pMat2.m_Ptr[0], pMat2.m_Ptr[1])),
				Dot(Vec2(m_Ptr[0], m_Ptr[2]), Vec2(pMat2.m_Ptr[2], pMat2.m_Ptr[3])),

				Dot(Vec2(m_Ptr[1], m_Ptr[3]), Vec2(pMat2.m_Ptr[0], pMat2.m_Ptr[1])),
				Dot(Vec2(m_Ptr[1], m_Ptr[3]), Vec2(pMat2.m_Ptr[2], pMat2.m_Ptr[3])),
			});
	}

	Mat2 Mat2::Transpose()
	{
		Mat2 Result;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				Result.At(i, j) = At(j, i);
			}
		}
		return Result;
	}

	float Mat2::Determinant()
	{
		return (At(0, 0)*At(1, 1) - At(1, 0)*At(0, 1));
	}

	float& Mat2::At(uint8 pColumn, uint8 pRow)
	{
		uint8 Index = (2 * pRow) + pColumn;
		//Check out of bound

		if (Index >= 4)
			__debugbreak(); //TODO: put ASSERT here

		return m_Ptr[Index];
	}

};