#include "Matrix.h"


namespace Hydrogen
{
	Matrix::Matrix():
		m_Height(4), m_Width(4)
	{
		m_Ptr = new float[m_Height*m_Width];

		//Construct an Identity Matrix:
		for (int j = 0; j < m_Height; j++)
		{
			for (int i = 0; i < m_Width; i++)
			{
				At(i, j) = (i == j) ? 1.0f : 0.0f;
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
		for (int j = 0; j < m_Height; j++)
		{
			for (int i = 0; i < m_Width; i++)
			{
				At(i, j) = (i == j) ? 1.0f : 0.0f;
			}
		}
	}

	Matrix::~Matrix()
	{
		delete[] m_Ptr;
	}


	Matrix Matrix::operator+(Matrix & pOther)
	{
		uint32 Width = (m_Width > pOther.m_Width) ? pOther.m_Width : m_Width;
		uint32 Height = (m_Height > pOther.m_Height) ? pOther.m_Height : m_Height;

		Matrix Result(Width, Height);
		for (int j = 0; j < Height; j++)
		{
			for (int i = 0; i < Width; i++)
			{
				Result.At(i, j) = (At(i, j) + pOther.At(i, j));
			}
		}

		return Result;
	}

	Matrix Matrix::operator-(Matrix & pOther)
	{
		uint32 Width = (m_Width > pOther.m_Width) ? pOther.m_Width : m_Width;
		uint32 Height = (m_Height > pOther.m_Height) ? pOther.m_Height : m_Height;

		Matrix Result(Width, Height);
		for (int j = 0; j < Height; j++)
		{
			for (int i = 0; i < Width; i++)
			{
				Result.At(i, j) = (At(i, j) - pOther.At(i, j));
			}
		}

		return Result;
	}

	float& Matrix::At(uint32 pI, uint32 pJ)
	{
		//Retriving Elements in Column-Major Order
		return m_Ptr[(pI*m_Height)+pJ];
	}

	void Matrix::PrintMatrix()
	{
		for (int j = 0; j < m_Height; j++)
		{
			for (int i = 0; i < m_Width; i++)
			{
				std::cout << At(i, j) << ' ';
			}
			std::cout << '\n';
		}
	}


};
