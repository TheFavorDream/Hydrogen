#pragma once

#include "../../Common.h"
#include <limits>
#include <cmath>
#ifdef TEST
	#define HYD
#endif


namespace Hydrogen
{
	


	//2 Component Vector
	
	template <typename Type>
	struct Vec2
	{
		Vec2() 
		{}

		Vec2(Type pX, Type pY) :
			X(pX), Y(pY)
		{}

		Vec2(Type pVal) :
			X(pVal), Y(pVal)
		{}

		Vec2(Type pVal[2]) :
			X(pVal[0]), Y(pVal[1])
		{}

		//Operators:
		Vec2 operator+(Vec2 pVec2) const noexcept
		{
			return Vec2(X + pVec2.X, Y + pVec2.Y);
		}

		Vec2 operator-(Vec2 pVec2) const noexcept
		{
			return Vec2(X - pVec2.X, Y - pVec2.Y);
		}

		Type operator[](uint32 Index) const noexcept
		{
			return *((Type*)(this + (sizeof(float)*Index)));
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

		Vec3() 
		{}

		Vec3(Type pX, Type pY, Type pZ) :
			X(pX), Y(pY), Z(pZ)
		{}

		Vec3(Type pVal) :
			X(pVal), Y(pVal), Z(pVal)
		{}

		Vec3(Vec2<Type> pXY, Type pZ = Type()) :
			X(pXY.X), Y(pXY.Y), Z(pZ)
		{}

		Vec3(Type pVal[3]) :
			X(pVal[0]), Y(pVal[1]), Z(pVal[2])
		{}

		//Operators:
		Vec3 operator+(Vec3 pVec3) const noexcept
		{
			return Vec3(X + pVec3.X, Y + pVec3.Y, Z + pVec3.Z);
		}

		Vec3 operator-(Vec3 pVec3) const noexcept
		{
			return Vec3(X - pVec3.X, Y - pVec3.Y, Z - pVec3.Z);
		}

		Vec3 operator*(Vec3 pVec3) const noexcept
		{
			return Vec3(X * pVec3.X, Y * pVec3.Y, Z * pVec3.Z);
		}


		bool operator==(Vec3<float> pVec3) const noexcept
		{
			return (abs(pVec3.X - X) < std::numeric_limits<float>::epsilon()) && (abs(pVec3.Y - Y) < std::numeric_limits<float>::epsilon()) && (abs(pVec3.Z - Z) < std::numeric_limits<float>::epsilon());
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

		Vec4() 
		{}

		Vec4(Type pX, Type pY, Type pZ, Type pW) :
			X(pX), Y(pY), Z(pZ), W(pW)
		{}

		Vec4(Vec2<Type> pXY, Type pZ = Type(), Type pW = Type()) :
			X(pXY.X), Y(pXY.Y), Z(pZ), W(pW)
		{}

		Vec4(Vec3<Type> pXYZ, Type pW = 0.0f) :
			X(pXYZ.X), Y(pXYZ.Y), Z(pXYZ.Z), W(pW)
		{}

		Vec4(Type pVal) :
			X(pVal), Y(pVal), Z(pVal), W(pVal)
		{}

		Vec4(Type pVal[4]) :
			X(pVal[0]), Y(pVal[1]), Z(pVal[2]), W(pVal[3])
		{}

		Vec4 operator+(Vec4 pVec4) const noexcept
		{
			return Vec4(X + pVec4.X, Y + pVec4.Y, Z + pVec4.Z, W + pVec4.W);
		}

		Vec4 operator-(Vec4 pVec4) const noexcept
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


	//Vec math def:
	HYD  VecF4 Average(const VecF4& pV1, const VecF4& pV2)  noexcept;
	HYD  VecF3 Average(const VecF3& pV1, const VecF3& pV2)	noexcept;
	HYD  VecF2 Average(const VecF2& pV1, const VecF2& pV2)	noexcept;
	HYD float  Average(float pV1, float pV2)				noexcept;


	HYD  VecF4  Clamp(const VecF4& pMin, const VecF4& pMax, const VecF4& pValue) noexcept;
	HYD  VecF3  Clamp(const VecF3& pMin, const VecF3& pMax, const VecF3& pValue) noexcept;
	HYD  VecF2  Clamp(const VecF2& pMin, const VecF2& pMax, const VecF2& pValue) noexcept;
	HYD float   Clamp(float pMin, float pMax, float pValue)						 noexcept;

	HYD float	Dot(VecF4 pVecA, VecF4 pVecB)	noexcept;
	HYD VecF3   Cross(VecF3 pVecA, VecF3 pVecB) noexcept;
	HYD float	Length(VecF4 pVec)				noexcept;

	HYD VecF2 Normalize(VecF2 pVec2) noexcept;
	HYD VecF3 Normalize(VecF3 pVec3) noexcept;
	HYD VecF4 Normalize(VecF4 pVec4) noexcept;

};