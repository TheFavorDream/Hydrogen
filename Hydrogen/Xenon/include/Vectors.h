#pragma once


namespace Xenon
{


	template <typename Type>
	struct Vec2
	{
		Vec2(Type pVal = Type()) :
			X(pVal), Y(pVal)
		{}
		Vec2(Type pVals[2])
		{
			X = pVals[0];
			Y = pVals[1];

		}
		Vec2(Type pX, Type pY) :
			X(pX), Y(pY)
		{}

		union {
			Type X = Type();
			Type R;
		};

		union {
			Type Y = Type();
			Type G;
		};
	};



	template <typename Type>
	struct Vec3
	{
		Vec3(Type pVal = Type()) :
			X(pVal), Y(pVal), Z(pVal)
		{}
		Vec3(Type pVals[3])
		{
			X = pVals[0];
			Y = pVals[1];
			Z = pVals[2];

		}
		Vec3(Type pX, Type pY, Type pZ) :
			X(pX), Y(pY), Z(pZ)
		{}

		union {
			Type X = Type();
			Type R;
		};

		union {
			Type Y = Type();
			Type G;
		};

		union {
			Type Z = Type();
			Type B;
		};
	};


	template <typename Type>
	struct Vec4
	{
		Vec4(Type pVal = Type()) :
			X(pVal), Y(pVal), Z(pVal), W(pVal)
		{}
		Vec4(Type pVals[4]) {
			X = pVals[0];
			Y = pVals[1];
			Z = pVals[2];
			W = pVals[3];
		}
		Vec4(Type pX, Type pY, Type pZ, Type pW) :
			X(pX), Y(pY), Z(pZ), W(pW)
		{}

		Type operator[](uint16_t pIndex)
		{
			XE_ASSERT(pIndex < 4, "Out of bound");

			return *(&X + pIndex);
		}

		Type& At(uint16_t pIndex)
		{
			XE_ASSERT(pIndex < 4, "Out of bound");

			return *(&X + pIndex);
		}

		union {
			Type X = Type();
			Type R;
		};

		union {
			Type Y = Type();
			Type G;
		};
		union {
			Type Z = Type();
			Type B;
		};
		union {
			Type W = Type();
		};
	};



	//Implementation of Xenon's matrix
	//Stored as Column Major
	template <typename Type>
	struct Mat4
	{

		//By Default Initializes a identity matrix
		Mat4(Type pVal=Type())
		{
			for (uint16_t i = 0; i < 4; i++)
			{
				for (uint16_t j = 0; j < 4; j++)
				{

					if (i == j)
					{
						Mat.At(i).At(i) = pVal;
						continue;
					}

					At(i, j) = Type();
				}
			}
		}

		Mat4(Type pValues[16])
		{
			memcpy(RawPtr(), &pValues[0], sizeof(Type) * 16);
		}


		Type& At(uint16_t pI, uint16_t pJ)
		{

			return Mat.At(pJ).At(pI);
		
		}

		Vec4<Type> operator[](uint16_t pIndex) const
		{
			return Mat[pIndex];
		}

		void* RawPtr() { return reinterpret_cast<void*>(&Mat.X); }

	private:
		Vec4<Vec4<Type>> Mat;
	};


};