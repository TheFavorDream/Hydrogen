#pragma once

#include "Common.h"
#include "../Vector/Vectors.h"
#include "../Quaternion.h"
#include "../Matrix/MatDef.h"

#include "Scale.h"

namespace Hydrogen
{
	struct Rotation : VecF4
	{
		HYD  Rotation() = default;
		HYD ~Rotation() = default;

		HYD Rotation(VecF4 pVec);
		HYD Rotation(Quat pVec);
		HYD Rotation(VecF3 pVec, float pVal);

		HYD Rotation& operator=(const Rotation& pOther);

		HYD Rotation  operator*(const Rotation& pOther) const;

		HYD MatF4  ToMatrix() const;
		HYD static Rotation ToRotaion(const MatF4& pRotate);
	};

};