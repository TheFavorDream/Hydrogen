#include "Matrix.h"


namespace Hydrogen
{
	Matrix::Matrix(float pDefaultValue):
		m_Height(4), m_Width(4)
	{
		m_Ptr = new float[m_Height*m_Width];

		//Construct an Identity Matrix:
		for (uint32 j = 0; j < m_Height; j++)
		{
			for (uint32 i = 0; i < m_Width; i++)
			{
				At(i, j) = (i == j) ? pDefaultValue : 0.0f;
			}
		}
	}

	Matrix::Matrix(const Matrix& Other)
	{
		
		m_Height = Other.m_Height;
		m_Width = Other.m_Width;

		m_Ptr = new float[m_Height*m_Width];
		memcpy((void*)m_Ptr, (void*)Other.m_Ptr, (m_Height*m_Width)*sizeof(float));
	}

	Matrix::Matrix(Matrix&& pOther)
	{
		m_Height = pOther.m_Height;
		m_Width = pOther.m_Width;
		m_Ptr = pOther.m_Ptr;
		pOther.m_Ptr = nullptr;
	}

	Matrix::Matrix(uint32 pWidth, uint32 pHeight) :
		m_Height(pHeight), m_Width(pWidth)
	{
		m_Ptr = new float[pWidth*pHeight];
		//Construct an Identity Matrix:
		for (uint32 j = 0; j < m_Height; j++)
		{
			for (uint32 i = 0; i < m_Width; i++)
			{
				At(i, j) = (i == j) ? 1.0f : 0.0f;
			}
		}
	}

	Matrix::~Matrix()
	{
		delete[] m_Ptr;
	}


	void Matrix::operator=(const Matrix& pOther)
	{
		
		m_Height = pOther.m_Height;
		m_Width = pOther.m_Width;

		m_Ptr = new float[m_Width*m_Height];
		memcpy((void*)m_Ptr, (void*)pOther.m_Ptr, (m_Height*m_Width*sizeof(float)));
	}

	Matrix Matrix::operator+(const Matrix & pOther)
	{
		uint32 Width = (m_Width > pOther.m_Width) ? pOther.m_Width : m_Width;
		uint32 Height = (m_Height > pOther.m_Height) ? pOther.m_Height : m_Height;

		Matrix Result(Width, Height);
		for (uint32 j = 0; j < Height; j++)
		{
			for (uint32 i = 0; i < Width; i++)
			{
				Result.At(i, j) = (At(i, j) + pOther.At(i, j));
			}
		}

		return Result;
	}

	Matrix Matrix::operator-(const Matrix & pOther)
	{
		uint32 Width = (m_Width > pOther.m_Width) ? pOther.m_Width : m_Width;
		uint32 Height = (m_Height > pOther.m_Height) ? pOther.m_Height : m_Height;

		Matrix Result(Width, Height);
		for (uint32 j = 0; j < Height; j++)
		{
			for (uint32 i = 0; i < Width; i++)
			{
				Result.At(i, j) = (At(i, j) - pOther.At(i, j));
			}
		}

		return Result;
	}

	Matrix Matrix::operator*(const Matrix & pOther)
	{

		//If "this" matrix is identity we skip the multipication:

		if (m_Width != pOther.m_Height) // If two matrix didn't meet the multipication needs we return "This" Matrix: 
			return *this;

		Matrix Result(m_Height, m_Height);
		for (uint32 j = 0; j < m_Height; j++)
		{
			for (uint32 i = 0; i < pOther.m_Width; i++)
			{
				float Sum = 0.0f;
				for (uint32 k = 0; k < m_Width; k++)
				{
					Sum += (At(k, j) * pOther.At(i, k));
				}
				Result.At(i, j) = Sum;
			}
		}

		return Result;
	}

	void Matrix::CopyDataToMatrix(const std::vector<float>& pData)
	{
		memcpy((void*)m_Ptr, (void*)&pData[0], (m_Width*m_Height)*sizeof(float));

	}

	float& Matrix::At(uint32 pI, uint32 pJ) const
	{

		//Retriving Elements in Column-Major Order
		return m_Ptr[(pI*m_Height)+pJ];
	}

	bool Matrix::IsIdentity()
	{
		for (uint32 i = 0; i < m_Width; i++)
		{
			if (At(i, i) != 1.0f)
				return false;
		}
		return true;
	}

	void Matrix::PrintMatrix()
	{
		for (uint32 j = 0; j < m_Height; j++)
		{
			for (uint32 i = 0; i < m_Width; i++)
			{
				//std::cout << At(i, j) << ' ';
			}
			//std::cout << '\n';
		}
	}




};