#include "VecMath/Transformation.h"
#include "Mat4.h"
#include "HydPch.h"

namespace Hydrogen
{



	Mat4::Mat4()
	{
		m_Mat = glm::mat4(1.0f);
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

	}

	Mat4::Mat4(const Mat4& pOther)
	{
		m_Mat = pOther.m_Mat;
	}

	Mat4& Mat4::operator=(const Mat4 & pOther)
	{
		if (this != &pOther)
		{
			m_Mat = pOther.m_Mat;
		}
		return *this;
	}

	Mat4 Mat4::operator+(const Mat4& pMat4) const
	{
		Mat4 Result;
		Result.m_Mat = pMat4.m_Mat + m_Mat;
		return Result;
	}

	Mat4 Mat4::operator-(const Mat4& pMat4) const
	{
		Mat4 Result;
		Result.m_Mat = pMat4.m_Mat - m_Mat;
		return Result;
	}

	Mat4 Mat4::operator*(const Mat4& pMat4) const
	{
		Mat4 Result;
		Result.m_Mat = (pMat4.m_Mat*m_Mat);
		return Result;
	}

	Mat4 Mat4::Transpose()
	{
		Mat4 Result;
		Result.m_Mat = glm::transpose(m_Mat);
		return Result;
	}

	float Mat4::Determinant()
	{
		return glm::determinant(m_Mat);
	}



	void Mat4::Scale(Vec3 pScale)
	{
		m_Mat = glm::scale(m_Mat, glm::vec3(pScale.X, pScale.Y, pScale.Z));
	}

	void Mat4::Translate(Vec3 pTranslate)
	{
		m_Mat = glm::translate(m_Mat, glm::vec3(pTranslate.X, pTranslate.Y, pTranslate.Z));
	}

	void Mat4::Rotate(Vec4 pRotate)
	{
		
	}


	void Mat4::Reset()
	{
		m_Mat = glm::mat4(1.0f);
	}

	float& Mat4::At(uint8 pRow, uint8 pColumn) const
	{
		uint8 Index = (4 * pRow) + pColumn;
		if (Index >= 16)
			__debugbreak();
		return m_Mat[pColumn][pRow];
	}

};