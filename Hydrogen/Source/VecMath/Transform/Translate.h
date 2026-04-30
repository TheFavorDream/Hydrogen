#pragma once

#include "Common.h"
#include "../Vector/Vectors.h"
#include "../Matrix/MatDef.h"

#include "Scale.h"
#include "Rotate.h"

namespace Hydrogen
{

	struct Translation : public VecF3
	{
		HYD  Translation() = default;
		HYD ~Translation() = default;

		HYD Translation(float pX, float pY, float pZ);
		HYD Translation(VecF3 pVec);

		HYD Translation operator*(const Translation& pOther) const;
		 
		HYD MatF4 ToMatrix() const; 
	};

};
