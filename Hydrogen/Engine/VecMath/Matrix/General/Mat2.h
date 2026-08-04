#pragma once

#include "../../../Common.h"
#include "UniMat.h"

namespace Hydrogen
{

	template <typename Type>
	class Mat2 final: public UniMat<Type, 2, 2>
	{
	public:

		HYD Mat2(Type pDefValue = Type())
		{
			this->m_Mat[0][0] = pDefValue;
			this->m_Mat[1][1] = pDefValue;
		}


		HYD Mat2(const UniMat<Type, 2, 2>& pOther)
		{
			this->m_Mat = pOther.GetMatrix();
		}

		HYD Mat2& operator=(const UniMat<Type, 2, 2>& pOther)
		{
			if (this != &pOther)
			{
				this->m_Mat = pOther.GetMatrix();
			}
			return *this;
		}


		HYD Mat2 operator*(const Mat2& pOther) const
		{
			Mat2<Type> Res;
			UniMat<Type, 2, 2> Other = pOther.Transpose();

			Res[0][0] = this->m_Mat.At(0).Dot(Other.At(0));
			Res[0][1] = this->m_Mat.At(0).Dot(Other.At(1));

			Res[1][0] = this->m_Mat.At(1).Dot(Other.At(0));
			Res[1][1] = this->m_Mat.At(1).Dot(Other.At(1));

			
			return Res;
		}
	};

};