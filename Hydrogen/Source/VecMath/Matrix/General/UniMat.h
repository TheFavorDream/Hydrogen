#pragma once


#include "Common.h"
#include "VecMath/Vector/Vectors.h"
#include "VecMath/Vector/UniVec.h"

namespace Hydrogen
{


	/*
	
		Definition for Universal matrix.
		Universal matrix lets user to define a m*n matrix in Row major and performe operations such as Addition, subtraction, transpose etc.
		Multipication isn't defined for universal matrix, user should inhert the universal matrix and implement their own multipication operator.
		
	*/

	template <typename Type, uint32 tRow, uint32 tColumn>
	class UniMat
	{
	public:

		HYD  UniMat() = default;
		HYD ~UniMat() = default;


		//Move Constructor:
		HYD UniMat(UniMat&& pOther)
		{
			m_Mat = std::move(pOther.m_Mat);
		}
		//Copy Constructor:
		HYD UniMat(const UniMat& pOther)
		{
			m_Mat = pOther.m_Mat;
		}


		//Move operator:
		HYD UniMat& operator=(UniMat&& pOther)
		{
			if (this != &pOther)
			{
				m_Mat = std::move(pOther.m_Mat);
			}
			return *this;
		}

		//Copy operator:
		HYD UniMat& operator=(const UniMat& pOther)
		{
			if (this != &pOther)
			{
				m_Mat = pOther.m_Mat;
			}
			return *this;
		}


		//Operations:

		//Transpose matrix:
		HYD UniMat Transpose() const
		{
			UniMat<Type, tRow, tColumn> Res;

			for (uint16 i = 0; i < tRow; i++)
			{
				for (uint16 j = 0; j < tColumn; j++)
					Res[j][i] = At(i, j);
			}

			return Res;
		}



		HYD UniMat operator+(const UniMat& pOther)
		{
			UniMat<Type, tRow, tColumn> Res;
			for (uint16 i = 0; i < tRow; i++)
				Res[i] = m_Mat.At(i) + pOther.m_Mat.At(i);
			
			return Res;
		}

		HYD UniMat operator-(const UniMat& pOther) const
		{
			UniMat<Type, tRow, tColumn> Res;
			for (uint16 i = 0; i < tRow; i++)
				Res[i] = m_Mat.At(i) - pOther.m_Mat.At(i);
			return Res;
		}



		//matrix member data access:
		HYD UniVec<Type, tRow>&  operator[](uint32 pIndex)
		{
			return m_Mat[pIndex];
		}


		HYD UniVec<Type, tRow> At(uint32 pIndex) const
		{
			return m_Mat.At(pIndex);
		}

		HYD Type At(uint16 pRow, uint16 pColumn) const
		{
			return m_Mat.At(pRow).At(pColumn);
		}

		//gets matrix 
		HYD UniVec<UniVec<Type, tColumn>, tRow> GetMatrix() const
		{
			return m_Mat;
		}

		//Get Raw memory address
		HYD Type* GetPointer() 
		{
			return &m_Mat[0][0];
		}

	protected:
		UniVec<UniVec<Type, tColumn>, tRow> m_Mat;
	};

};
