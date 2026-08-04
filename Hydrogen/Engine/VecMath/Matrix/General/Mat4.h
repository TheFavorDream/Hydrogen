#pragma once

#include "UniMat.h"

namespace Hydrogen
{

	template <typename Type>
	class Mat4 : public UniMat <Type, 4, 4>
	{
	public:

		HYD Mat4(Type pDefValue = Type())
		{
			this->m_Mat[0][0] = pDefValue;
			this->m_Mat[1][1] = pDefValue;
			this->m_Mat[2][2] = pDefValue;
			this->m_Mat[3][3] = pDefValue;
		}


		HYD Mat4(const UniMat<Type, 4, 4>& pOther)
		{
			this->m_Mat = pOther.GetMatrix();
		}

		HYD Mat4& operator=(const UniMat<Type, 4, 4>& pOther)
		{
			if (this != &pOther)
			{
				this->m_Mat = pOther.GetMatrix();
			}
			return *this;
		}




		HYD Mat4 operator*(const Mat4& pOther) const
		{
			Mat4<Type> Res;
			UniMat<Type, 4, 4> Other = pOther.Transpose();

			for (uint16 i = 0; i < 4; i++)
			{
				for (uint16 j = 0; j < 4; j++)
					Res[i][j] = this->m_Mat.At(i).Dot(Other.At(j));	
			}

			return Res;
		}
	};


};