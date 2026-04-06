#include "Vector/VectorDef.h"
#include "Math.h"
#include "HydPch.h"


#define LIT_PI 3.14159f

namespace Hydrogen
{

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