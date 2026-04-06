#pragma once

#include "Common.h"


#include "Vector/VectorDef.h"



namespace Hydrogen
{

	//Describes the Transformation of a primitive
	struct Transformation
	{
		//TRS properties:
		VecF4 Rotate     = VecF4(VecF3(0.0f), 1.0f);
		VecF3 Scale		 = VecF3(1.0f);
		VecF3 Translate  = VecF3(0.0f);
		//indicates if TRS values has been changed if so, recalculate model matrix
		bool IsDirty = false;

	};
};