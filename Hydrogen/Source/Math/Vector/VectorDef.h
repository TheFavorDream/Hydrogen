#pragma once

namespace Hydrogen
{
	//2 Component Vector
	struct Vec2
	{
		Vec2():
			X(0.0f), Y(0.0f)
		{}

		Vec2(float pX, float pY) :
			X(pX), Y(pY)
		{}

		Vec2(float pVal) :
			X(pVal), Y(pVal)
		{}


		float X = 0.0f, Y = 0.0f;
	};

	//3 Component Vector
	struct Vec3
	{

		Vec3() :
			X(0.0f), Y(0.0f), Z(0.0f)
		{}

		Vec3(float pX, float pY, float pZ) :
			X(pX), Y(pY), Z(pZ)
		{}

		Vec3(float pVal):
			X(pVal), Y(pVal), Z(pVal)
		{}

		float X = 0.0f, Y = 0.0f, Z=0.0f;
	};

	//4 Component Vector
	struct Vec4
	{

		Vec4() :
			X(0.0f), Y(0.0f), Z(0.0f), W(0.0f)
		{}

		Vec4(float pX, float pY, float pZ, float pW) :
			X(pX), Y(pY), Z(pZ), W(pW)
		{}

		Vec4(float pVal) :
			X(pVal), Y(pVal), Z(pVal), W(pVal)
		{}
		float X = 0.0f, Y = 0.0f, Z = 0.0f, W=0.0f;
	};


	//Vec math def:
	Vec4 Average(const Vec4& pV1, const Vec4& pV2)  noexcept;
	Vec3 Average(const Vec3& pV1, const Vec3& pV2)	noexcept;
	Vec2 Average(const Vec2& pV1, const Vec2& pV2)	noexcept;
	float Average(float pV1, float pV2)				noexcept;


	Vec4 Clamp(const Vec4& pMin, const Vec4& pMax, const Vec4& pValue) noexcept;
	Vec3 Clamp(const Vec3& pMin, const Vec3& pMax, const Vec3& pValue) noexcept;
	Vec2 Clamp(const Vec2& pMin, const Vec2& pMax, const Vec2& pValue) noexcept;
	float Clamp(float pMin, float pMax, float pValue)				   noexcept;

};