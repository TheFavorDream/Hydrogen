#pragma once

#include "Common.h"

#ifdef TEST
#define HYD
#endif 

namespace Hydrogen
{

	struct LongAverage
	{
		float Sum     = 0; 
		int   Count   = 0;
		int   Samples = 10000; //Number Of Samples to be done before reseting the Count

		HYD void AddValue(float pVal) noexcept
		{ 
			if (Count > Samples)
			{
				Sum = 0;
				Count = 0;
			}

			Sum += pVal; Count += 1; 
		}
		
		HYD float CalcAvg() noexcept
		{
			return (Count != 0.0f)?(Sum / float(Count)) : Sum;
		}
	};


	HYD float SinA(float pDegrees);
	HYD float CosA(float pDegrees);
	HYD float TanA(float pDegrees);
	
	HYD inline float Sq(float pBase) { return (pBase*pBase); };


};


