#include "Vectors.h"

namespace Hydrogen
{

	VecF4 Average(const VecF4& pV1, const VecF4& pV2) noexcept
	{
		return VecF4((pV1.X + pV2.X) / 2.0f, (pV1.Y + pV2.Y) / 2.0f, (pV1.Z + pV2.Z) / 2.0f, (pV1.W + pV2.W) / 2.0f);
	}

	VecF3 Average(const VecF3& pV1, const VecF3& pV2) noexcept
	{
		return VecF3((pV1.X + pV2.X) / 2.0f, (pV1.Y + pV2.Y) / 2.0f, (pV1.Z + pV2.Z) / 2.0f);
	}

	VecF2 Average(const VecF2& pV1, const VecF2& pV2) noexcept
	{
		return VecF2((pV1.X + pV2.X) / 2.0f, (pV1.Y + pV2.Y) / 2.0f);
	}

	float Average(float pV1, float pV2) noexcept
	{
		return (pV1 + pV2) / 2.0f;
	}

	VecF4 Clamp(const VecF4 & pMin, const VecF4 & pMax, const VecF4 & pValue) noexcept
	{
		return VecF4(
			Clamp(pMin.X, pMax.X, pValue.X),
			Clamp(pMin.Y, pMax.Y, pValue.Y),
			Clamp(pMin.Z, pMax.Z, pValue.Z),
			Clamp(pMin.W, pMax.W, pValue.W)
		);
	}

	VecF3 Clamp(const VecF3& pMin, const VecF3& pMax, const VecF3& pValue) noexcept
	{
		return VecF3(
			Clamp(pMin.X, pMax.X, pValue.X),
			Clamp(pMin.Y, pMax.Y, pValue.Y),
			Clamp(pMin.Z, pMax.Z, pValue.Z)
		);
	}

	VecF2 Clamp(const VecF2& pMin, const VecF2& pMax, const VecF2& pValue) noexcept
	{
		return VecF2(
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
	float Dot(VecF4 pVecA, VecF4 pVecB) noexcept
	{
		float Product = 0.0f;
		Product += (pVecA.X * pVecB.X);
		Product += (pVecA.Y * pVecB.Y);
		Product += (pVecA.Z * pVecB.Z);
		Product += (pVecA.W * pVecB.W);
		return Product;
	}

	//Cross product
	VecF3 Cross(VecF3 pVecA, VecF3 pVecB) noexcept
	{
		return VecF3(
			(pVecA.Y * pVecB.Z) - (pVecA.Z * pVecB.Y),
			(pVecA.X * pVecB.Z) - (pVecA.Z * pVecB.X),
			(pVecA.X * pVecB.Y) - (pVecA.Y * pVecB.X)
		);
	}

	//Length:
	float Length(VecF4 pVec) noexcept
	{
		return sqrt((pVec.X*pVec.X) + (pVec.Y*pVec.Y) + (pVec.Z*pVec.Z) + (pVec.W*pVec.W));
	}

	//Vector normalize:
	VecF2 Normalize(VecF2 pVec2) noexcept
	{
		float length = Length(pVec2);
		return VecF2(
			pVec2.X / length,
			pVec2.Y / length
		);
	}

	VecF3 Normalize(VecF3 pVec3) noexcept
	{
		float length = Length(pVec3);
		return VecF3(
			pVec3.X / length,
			pVec3.Y / length,
			pVec3.Z / length
		);
	}

	VecF4 Normalize(VecF4 pVec4) noexcept
	{
		float length = Length(pVec4);
		return VecF4(
			pVec4.X / length,
			pVec4.Y / length,
			pVec4.Z / length,
			pVec4.W / length
		);
	}



};