#pragma once

#include "UniMat.h"

namespace Hydrogen
{

	template <typename Type>
	class Mat3 : public UniMat <Type, 3, 3>
	{
	public:

		HYD Mat3(Type pDefValue = Type())
		{
			this->m_Mat[0][0] = pDefValue;
			this->m_Mat[1][1] = pDefValue;
			this->m_Mat[2][2] = pDefValue;
		}


		HYD Mat3(const UniMat<Type, 3, 3>& pOther)
		{
			this->m_Mat = pOther.GetMatrix();
		}

		HYD Mat3& operator=(const UniMat<Type, 3, 3>& pOther)
		{
			if (this != &pOther)
			{
				this->m_Mat = pOther.GetMatrix();
			}
			return *this;
		}



		HYD Mat3 operator*(const Mat3& pOther) const
		{
			Mat3<Type> Res;
			UniMat<Type, 3, 3> Other = pOther.Transpose();

			Res[0][0] = this->m_Mat.At(0).Dot(Other.At(0));
			Res[0][1] = this->m_Mat.At(0).Dot(Other.At(1));
			Res[0][2] = this->m_Mat.At(0).Dot(Other.At(2));

			Res[1][0] = this->m_Mat.At(1).Dot(Other.At(0));
			Res[1][1] = this->m_Mat.At(1).Dot(Other.At(1));
			Res[1][2] = this->m_Mat.At(1).Dot(Other.At(2));

			Res[2][0] = this->m_Mat.At(2).Dot(Other.At(0));
			Res[2][1] = this->m_Mat.At(2).Dot(Other.At(1));
			Res[2][2] = this->m_Mat.At(2).Dot(Other.At(2));


			return Res;
		}
	};
};


