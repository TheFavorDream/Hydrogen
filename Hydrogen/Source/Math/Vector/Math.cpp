#include "VectorDef.h"
#include "Math.h"

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
};