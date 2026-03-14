#include "Vector/VectorDef.h"
#include "Math.h"

#define LIT_PI 3.14159f

namespace Hydrogen
{
	Vec4 Average(const Vec4& pV1, const Vec4& pV2) noexcept
	{
		return Vec4((pV1.X + pV2.X) / 2.0f, (pV1.Y + pV2.Y) / 2.0f, (pV1.Z + pV2.Z) / 2.0f, (pV1.W + pV2.W) / 2.0f);
	}

	Vec3 Average(const Vec3& pV1, const Vec3& pV2) noexcept
	{
		return Vec3((pV1.X + pV2.X) / 2.0f, (pV1.Y + pV2.Y) / 2.0f, (pV1.Z + pV2.Z) / 2.0f);
	}

	Vec2 Average(const Vec2& pV1, const Vec2& pV2) noexcept
	{
		return Vec2((pV1.X + pV2.X) / 2.0f, (pV1.Y + pV2.Y) / 2.0f);
	}

	float Average(float pV1, float pV2) noexcept
	{
		return (pV1 + pV2) / 2.0f;
	}

	Vec4 Clamp(const Vec4 & pMin, const Vec4 & pMax, const Vec4 & pValue) noexcept
	{
		return Vec4(
			Clamp(pMin.X, pMax.X, pValue.X),
			Clamp(pMin.Y, pMax.Y, pValue.Y),
			Clamp(pMin.Z, pMax.Z, pValue.Z),
			Clamp(pMin.W, pMax.W, pValue.W)
		);
	}

	Vec3 Clamp(const Vec3 & pMin, const Vec3 & pMax, const Vec3 & pValue) noexcept
	{
		return Vec3(
			Clamp(pMin.X, pMax.X, pValue.X),
			Clamp(pMin.Y, pMax.Y, pValue.Y),
			Clamp(pMin.Z, pMax.Z, pValue.Z)
		);
	}

	Vec2 Clamp(const Vec2 & pMin, const Vec2 & pMax, const Vec2 & pValue) noexcept
	{
		return Vec2(
			Clamp(pMin.X, pMax.X, pValue.X), 
			Clamp(pMin.Y, pMax.Y, pValue.Y)
		);
	}

	float Clamp(float pMin, float pMax, float pValue) noexcept
	{
		if (pValue <= pMin)
			return pMin;
		if (pValue >= pMax)
			return pMax;
		return pValue;
	}

	//Dot Product Definition
	float Dot(Vec4 pVecA, Vec4 pVecB) noexcept
	{
		float Product = 0.0f;
		Product += (pVecA.X * pVecB.X);
		Product += (pVecA.Y * pVecB.Y);
		Product += (pVecA.Z * pVecB.Z);
		Product += (pVecA.W * pVecB.W);
		return Product;
	}

	//Cross product
	Vec3 Cross(Vec3 pVecA, Vec3 pVecB) noexcept
	{
		return Vec3(
			(pVecA.Y * pVecB.Z) - (pVecA.Z * pVecB.Y),
			(pVecA.X * pVecB.Z) - (pVecA.Z * pVecB.X),
			(pVecA.X * pVecB.Y) - (pVecA.Y * pVecB.X)
		);
	}

	//Length:
	float Length(Vec4 pVec) noexcept
	{
		return sqrt( (pVec.X*pVec.X) + (pVec.Y*pVec.Y) + (pVec.Z*pVec.Z) + (pVec.W*pVec.W));
	}

	//Vector normalize:
	Vec2 Normalize(Vec2 pVec2) noexcept
	{
		float length = Length(pVec2);
		return Vec2(
			pVec2.X / length,
			pVec2.Y / length
		);
	}

	Vec3 Normalize(Vec3 pVec3) noexcept
	{
		float length = Length(pVec3);
		return Vec3(
			pVec3.X / length,
			pVec3.Y / length,
			pVec3.Z / length
		);
	}

	Vec4 Normalize(Vec4 pVec4) noexcept
	{
		float length = Length(pVec4);
		return Vec4(
			pVec4.X / length,
			pVec4.Y / length,
			pVec4.Z / length,
			pVec4.W / length
		);
	}


	float SinA(float pDegrees)
	{
		return sin(pDegrees*(LIT_PI/180.0f));
	}

	float CosA(float pDegrees)
	{
		return cos(pDegrees*(LIT_PI / 180.0f));
	}

	float TanA(float pDegrees)
	{
		return tan(pDegrees*(LIT_PI / 180.0f));
	}
};