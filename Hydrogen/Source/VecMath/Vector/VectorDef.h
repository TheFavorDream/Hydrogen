#pragma once

#include "../../Common.h"

#ifdef TEST
#define HYD
#endif

namespace Hydrogen
{
	//2 Component Vector
	struct Vec2
	{
		HYD Vec2():
			X(0.0f), Y(0.0f)
		{}

		HYD Vec2(float pX, float pY) :
			X(pX), Y(pY)
		{}

		HYD Vec2(float pVal) :
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

		Vec2 operator*(float pScaler) noexcept
		{
			return Vec2(X * pScaler, Y * pScaler);
		}


		union
		{
			float X = 0.0f;
			float R;
			float S;
		};

		union
		{
			float Y = 0.0f;
			float G;
			float T;
		};
	};

	//3 Component Vector
	struct Vec3
	{

		HYD Vec3() :
			X(0.0f), Y(0.0f), Z(0.0f)
		{}

		HYD Vec3(float pX, float pY, float pZ) :
			X(pX), Y(pY), Z(pZ)
		{}

		HYD Vec3(float pVal):
			X(pVal), Y(pVal), Z(pVal)
		{}

		HYD Vec3(Vec2 pXY, float pZ=0.0f):
			X(pXY.X), Y(pXY.Y), Z(pZ)
		{}


		//Operators:
		Vec3 operator+(Vec3 pVec3) noexcept
		{
			return Vec3(X + pVec3.X, Y + pVec3.Y, Z+pVec3.Z);
		}

		Vec3 operator-(Vec3 pVec3) noexcept
		{
			return Vec3(X - pVec3.X, Y - pVec3.Y, Z - pVec3.Z);
		}

		Vec3 operator*(float pScaler) noexcept
		{
			return Vec3(X * pScaler, Y * pScaler, Z * pScaler);
		}

		Vec3 operator*(Vec3 pVec3) noexcept
		{
			return Vec3(X * pVec3.X, Y * pVec3.Y, Z * pVec3.Z);
		}

		union
		{
			float X = 0.0f;
			float R;
			float U;
		};

		union
		{
			float Y = 0.0f;
			float G;
			float V;
		};

		union 
		{
			float Z = 0.0f;
			float B;
		};
	};

	//4 Component Vector
	struct Vec4
	{

		HYD Vec4() :
			X(0.0f), Y(0.0f), Z(0.0f), W(0.0f)
		{}

		HYD Vec4(float pX, float pY, float pZ, float pW) :
			X(pX), Y(pY), Z(pZ), W(pW)
		{}

		HYD Vec4(Vec2 pXY, float pZ=0.0f, float pW=0.0f) :
			X(pXY.X), Y(pXY.Y), Z(pZ), W(pW)
		{}

		HYD Vec4(Vec3 pXYZ, float pW=0.0f) :
			X(pXYZ.X), Y(pXYZ.Y), Z(pXYZ.Z), W(pW)
		{}

		HYD Vec4(float pVal) :
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

		Vec4 operator*(float pScaler) noexcept
		{
			return Vec4(X * pScaler, Y * pScaler, Z * pScaler, W * pScaler);
		}

		union
		{
			float X = 0.0f;
			float R;
			float U;
		};

		union
		{
			float Y = 0.0f;
			float G;
			float V;
		};
		union
		{
			float Z = 0.0f;
			float B;
		};
		union
		{
			float W = 0.0f;
			float A;
		};
	};


	//Vec math def:
	HYD  Vec4 Average(const Vec4& pV1, const Vec4& pV2)  noexcept;
	HYD  Vec3 Average(const Vec3& pV1, const Vec3& pV2)	noexcept;
	HYD  Vec2 Average(const Vec2& pV1, const Vec2& pV2)	noexcept;
	HYD float Average(float pV1, float pV2)				noexcept;


	HYD  Vec4  Clamp(const Vec4& pMin, const Vec4& pMax, const Vec4& pValue) noexcept;
	HYD  Vec3  Clamp(const Vec3& pMin, const Vec3& pMax, const Vec3& pValue) noexcept;
	HYD  Vec2  Clamp(const Vec2& pMin, const Vec2& pMax, const Vec2& pValue) noexcept;
	HYD float  Clamp(float pMin, float pMax, float pValue)				   noexcept;

	HYD float Dot(Vec4 pVecA, Vec4 pVecB) noexcept;
	HYD Vec3  Cross(Vec3 pVecA, Vec3 pVecB) noexcept;
	HYD float Length(Vec4 pVec) noexcept;

	HYD Vec2 Normalize(Vec2 pVec2) noexcept;
	HYD Vec3 Normalize(Vec3 pVec3) noexcept;
	HYD Vec4 Normalize(Vec4 pVec4) noexcept;


};