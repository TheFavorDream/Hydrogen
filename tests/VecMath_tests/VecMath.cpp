
#include "VecMath/Math.h"
#include "VecMath/Vector/VectorDef.h"
#include "VecMath/Matrix/Matrix.h"
#include "VecMath/Matrix/Predefined/Mat4/Mat4.h"
#include "VecMath/Matrix/Predefined/Mat3/Mat3.h"
#include "VecMath/Matrix/Predefined/Mat2/Mat2.h"



using namespace Hydrogen;

void PrintVector(Vec4 Vec);
void PrintMatrix(Mat4 Mat);

int main(int argc, char** arvg)
{

	Mat4 A = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
	Mat4 B = {2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f};



	//PrintMatrix();
	//std::cout << '\n';
	PrintMatrix(A*B);
	return 0;
}

void PrintVector(Vec4 Vec)
{
	std::cout << "(" << Vec.X << ',' << Vec.Y << ',' << Vec.Z << ',' << Vec.W << ")\n";
}

void PrintMatrix(Mat4 Mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << Mat.At(i, j) << ',';
		}
		std::cout << '\n';
	}
}
