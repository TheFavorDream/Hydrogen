#pragma once

#include "Common.h"
#include "VecTemplate.h"
#include "UniVec.h"

#ifdef TEST
#define HYD
#endif

#define Column UniVec
#define Row    UniVec

namespace Hydrogen
{

	

	//Vec math def:
	HYD  VecF4 Average(const VecF4& pV1, const VecF4& pV2)  noexcept;
	HYD  VecF3 Average(const VecF3& pV1, const VecF3& pV2)	noexcept;
	HYD  VecF2 Average(const VecF2& pV1, const VecF2& pV2)	noexcept;
	HYD float Average(float pV1, float pV2)				noexcept;


	HYD  VecF4  Clamp(const VecF4& pMin, const VecF4& pMax, const VecF4& pValue) noexcept;
	HYD  VecF3  Clamp(const VecF3& pMin, const VecF3& pMax, const VecF3& pValue) noexcept;
	HYD  VecF2  Clamp(const VecF2& pMin, const VecF2& pMax, const VecF2& pValue) noexcept;
	HYD float  Clamp(float pMin, float pMax, float pValue)				   noexcept;

	HYD float Dot(VecF4 pVecA, VecF4 pVecB) noexcept;
	HYD VecF3  Cross(VecF3 pVecA, VecF3 pVecB) noexcept;
	HYD float Length(VecF4 pVec) noexcept;

	HYD VecF2 Normalize(VecF2 pVec2) noexcept;
	HYD VecF3 Normalize(VecF3 pVec3) noexcept;
	HYD VecF4 Normalize(VecF4 pVec4) noexcept;


};