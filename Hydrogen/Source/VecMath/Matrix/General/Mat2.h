#pragma once

#include "Common.h"

namespace Hydrogen
{

	template <typename Type>
	class Mat2 : public UniMat <Type, 2, 2>
	{
	public:

		HYD Mat2(Type pDefValue = Type())
		{
			m_Mat[0][0] = pDefValue;
			m_Mat[1][1] = pDefValue;
		}


		HYD Mat2(const UniMat<Type, 2, 2>& pOther)
		{
			m_Mat = pOther.GetMatrix();
		}

		HYD Mat2& operator=(const UniMat<Type, 2, 2>& pOther)
		{
			if (this != &pOther)
			{
				m_Mat = pOther.GetMatrix();
			}
			return *this;
		}


		HYD Mat2 operator*(const Mat2& pOther) const
		{
			Mat2<Type> Res;
			UniMat<Type, 2, 2> Other = pOther.Transpose();

			Res[0][0] = m_Mat.At(0).Dot(Other.At(0));
			Res[0][1] = m_Mat.At(0).Dot(Other.At(1));

			Res[1][0] = m_Mat.At(1).Dot(Other.At(0));
			Res[1][1] = m_Mat.At(1).Dot(Other.At(1));

			return Res;
		}
	};

};