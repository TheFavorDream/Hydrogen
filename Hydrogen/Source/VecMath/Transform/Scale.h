#pragma once

#include "Common.h"
#include "../Vector/Vectors.h"
#include "../Matrix/MatDef.h"

namespace Hydrogen
{

	struct Scaler : public VecF3
	{
		HYD  Scaler() = default;
		HYD ~Scaler() = default;

		HYD Scaler(float pScaleX, float pScaleY, float pScaleZ);
		HYD Scaler(VecF3 pVec);


		HYD Scaler operator*(const Scaler& pOther) const;

		//Converts to matrix
		HYD MatF4 ToMatrix() const;
	};

};