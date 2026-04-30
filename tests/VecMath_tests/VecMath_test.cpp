
#include "VecMath/Math.h"
#include "VecMath/Vector/VectorDef.h"
#include "VecMath/Matrix/MatDef.h"
#include "VecMath/Vector/UniVec.h"
#include <iostream>

#include <thread>
#include "Hydrofiler.h"


using namespace std::chrono_literals;
using namespace Hydrogen;


void PrintVector(VecF4 Vec);

int main(int argc, char** arvg)
{

	START_PROFILE("Test")
		std::this_thread::sleep_for(1000us);
	auto Profile = STOP_PROFILE

	std::cout << Tools::Hydrofiler::GetDurationInMicroSeconds(Profile);

	return 0;
}

void PrintVector(VecF4 Vec)
{
	std::cout << "(" << Vec.X << ',' << Vec.Y << ',' << Vec.Z << ',' << Vec.W << ")\n";
}
