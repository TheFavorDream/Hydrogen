#pragma once

#include "Common.h"


#ifdef TEST
	#define HYD
#endif

namespace Hydrogen
{

	//2 Component Vector
	
	template <typename Type>
	struct Vec2
	{
		HYD Vec2() 
		{}

		HYD Vec2(Type pX, Type pY) :
			X(pX), Y(pY)
		{}

		HYD Vec2(Type pVal) :
			X(pVal), Y(pVal)
		{}

		//Operators:
		Vec2 operator+(Vec2 pVec2) noexcept
		{
			return Vec2(X + pVec2.X, Y + pVec2.Y);
		}

		Vec2 operator-(Vec2 pVec2) noexcept
		{
			return Vec2(X - pVec2.X, Y - pVec2.Y);
		}

		Type operator[](uint32 Index) noexcept
		{
			return *(Type*(this + (sizeof(float)*Index)));
		}

		union
		{
			Type X= Type();
			Type R;
			Type S;
		};

		union
		{
			Type Y= Type();
			Type G;
			Type T;
		};
	};

	//3 Component Vector
	template <typename Type>
	struct Vec3
	{

		HYD Vec3() 
		{}

		HYD Vec3(Type pX, Type pY, Type pZ) :
			X(pX), Y(pY), Z(pZ)
		{}

		HYD Vec3(Type pVal) :
			X(pVal), Y(pVal), Z(pVal)
		{}

		HYD Vec3(Vec2<Type> pXY, Type pZ = Type()) :
			X(pXY.X), Y(pXY.Y), Z(pZ)
		{}


		//Operators:
		Vec3 operator+(Vec3 pVec3) noexcept
		{
			return Vec3(X + pVec3.X, Y + pVec3.Y, Z + pVec3.Z);
		}

		Vec3 operator-(Vec3 pVec3) noexcept
		{
			return Vec3(X - pVec3.X, Y - pVec3.Y, Z - pVec3.Z);
		}

		Vec3 operator*(Vec3 pVec3) noexcept
		{
			return Vec3(X * pVec3.X, Y * pVec3.Y, Z * pVec3.Z);
		}

		union
		{
			Type X = Type();
			Type R;
			Type U;
		};

		union
		{
			Type Y = Type();
			Type G;
			Type V;
		};

		union
		{
			Type Z = Type();
			Type B;
		};
	};


	//4 Component Vector
	template <typename Type>
	struct Vec4
	{

		HYD Vec4() 
		{}

		HYD Vec4(Type pX, Type pY, Type pZ, Type pW) :
			X(pX), Y(pY), Z(pZ), W(pW)
		{}

		HYD Vec4(Vec2<Type> pXY, Type pZ = Type(), Type pW = Type()) :
			X(pXY.X), Y(pXY.Y), Z(pZ), W(pW)
		{}

		HYD Vec4(Vec3<Type> pXYZ, Type pW = 0.0f) :
			X(pXYZ.X), Y(pXYZ.Y), Z(pXYZ.Z), W(pW)
		{}

		HYD Vec4(Type pVal) :
			X(pVal), Y(pVal), Z(pVal), W(pVal)
		{}

		Vec4 operator+(Vec4 pVec4) noexcept
		{
			return Vec4(X + pVec4.X, Y + pVec4.Y, Z + pVec4.Z, W + pVec4.W);
		}

		Vec4 operator-(Vec4 pVec4) noexcept
		{
			return Vec4(X - pVec4.X, Y - pVec4.Y, Z - pVec4.Z, W - pVec4.W);
		}

		union
		{
			Type X = Type();
			Type R;
			Type U;
		};

		union
		{
			Type Y = Type();
			Type G;
			Type V;
		};
		union
		{
			Type Z = Type();
			Type B;
		};
		union
		{
			Type W = Type();
			Type A;
		};
	};

	//Common types:

	typedef Vec2<float> VecF2;
	typedef Vec3<float> VecF3;
	typedef Vec4<float> VecF4;

	typedef Vec2<double> VecD2;
	typedef Vec3<double> VecD3;
	typedef Vec4<double> VecD4;

	typedef Vec2<uint32> VecI2;
	typedef Vec3<uint32> VecI3;
	typedef Vec4<uint32> VecI4;

};