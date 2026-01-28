#pragma once

#include "../../Common.h"

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


		float X = 0.0f, Y = 0.0f;
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

		float X = 0.0f, Y = 0.0f, Z=0.0f;
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

		HYD Vec4(float pVal) :
			X(pVal), Y(pVal), Z(pVal), W(pVal)
		{}
		float X = 0.0f, Y = 0.0f, Z = 0.0f, W=0.0f;
	};


	//Vec math def:
	HYD  Vec4 Average(const Vec4& pV1, const Vec4& pV2)  noexcept;
	HYD  Vec3 Average(const Vec3& pV1, const Vec3& pV2)	noexcept;
	HYD  Vec2 Average(const Vec2& pV1, const Vec2& pV2)	noexcept;
	HYD float Average(float pV1, float pV2)				noexcept;


	HYD  Vec4  Clamp(const Vec4& pMin, const Vec4& pMax, const Vec4& pValue) noexcept;
	HYD  Vec3  Clamp(const Vec3& pMin, const Vec3& pMax, const Vec3& pValue) noexcept;
	HYD  Vec2  Clamp(const Vec2& pMin, const Vec2& pMax, const Vec2& pValue) noexcept;
	HYD float Clamp(float pMin, float pMax, float pValue)				   noexcept;

};