
#include "VecMath/Math.h"
#include "VecMath/Vector/VectorDef.h"
#include "VecMath/Matrix/MatDef.h"
#include "VecMath/Vector/UniVec.h"
#include <iostream>

#include <thread>
#include "Hydrofiler.h"

using namespace Hydrogen;

void PrintVector(VecF4 Vec);

int main(int argc, char** arvg)
{

	START_PROFILE("Matrix Creation");
		MatI4 Mat;
	STOP_PROFILE;

	auto Profile = Tools::Hydrofiler::GetLatestProfile();
	std::cout << "Profile: " << Profile.BlockName << " Time: " << Tools::Hydrofiler::GetDurationInMicroSeconds(Profile) << " us\n";

	START_PROFILE("Matrix Copy");
		MatI4 MatC = Mat;
	STOP_PROFILE;

	Profile = Tools::Hydrofiler::GetLatestProfile();
	std::cout << "Profile: " << Profile.BlockName << " Time: " << Tools::Hydrofiler::GetDurationInMicroSeconds(Profile) << " us\n";

	START_PROFILE("Matrix Addition");
		MatC + Mat;
	STOP_PROFILE;

	Profile = Tools::Hydrofiler::GetLatestProfile();
	std::cout << "Profile: " << Profile.BlockName << " Time: " << Tools::Hydrofiler::GetDurationInMicroSeconds(Profile) << " us\n";

	START_PROFILE("Matrix multipication");
		MatC * Mat;
	STOP_PROFILE;

	Profile = Tools::Hydrofiler::GetLatestProfile();
	std::cout << "Profile: " << Profile.BlockName << " Time: " << Tools::Hydrofiler::GetDurationInMicroSeconds(Profile) << " us\n";


	return 0;
}

void PrintVector(VecF4 Vec)
{
	std::cout << "(" << Vec.X << ',' << Vec.Y << ',' << Vec.Z << ',' << Vec.W << ")\n";
}
