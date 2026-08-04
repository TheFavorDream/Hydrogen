#include "Quaternion.h"


namespace Hydrogen
{



	Quaternion::Quaternion(VecF3 pVec, float pVal)
	{
		X = pVec.X;
		Y = pVec.Y;
		Z = pVec.Z;
		W = pVal;
	}

	Quaternion::Quaternion(float pX, float pY, float pZ, float pW)
	{
		X = pX;
		Y = pY;
		Z = pZ;
		W = pW;
	}

	Quaternion::Quaternion(const VecF4& pOther)
	{
		X = pOther.X;
		Y = pOther.Y;
		Z = pOther.Z;
		W = pOther.W;
	}



	Quaternion Quaternion::operator*(const Quaternion& pOther) const
	{
		Quaternion Quat;

		Quat.X = (X*pOther.W + Y*pOther.Z - Z*pOther.Y + W*pOther.X);
		Quat.Y = (-X*pOther.Z + Y*pOther.W + Z*pOther.X + W*pOther.Y);
		Quat.Z = (X*pOther.Y - Y*pOther.X + Z*pOther.W + W*pOther.Z);
		Quat.W = (-X*pOther.X - Y*pOther.Y - Z*pOther.Z + W*pOther.W);

		return Quat;
	}

	void Quaternion::Euler(float pX, float pY, float pZ)
	{
		X = SinA(pX / 2.0f)*CosA(pY / 2.0f)*CosA(pZ/2.0f) + CosA(pX / 2.0f)*SinA(pY / 2.0f)*SinA(pZ / 2.0f);
		Y = CosA(pX / 2.0f)*SinA(pY / 2.0f)*CosA(pZ/2.0f) - SinA(pX / 2.0f)*CosA(pY / 2.0f)*SinA(pZ / 2.0f);
		Z = SinA(pX / 2.0f)*SinA(pY / 2.0f)*CosA(pZ/2.0f) + CosA(pX / 2.0f)*CosA(pY / 2.0f)*SinA(pZ / 2.0f);
		W = CosA(pX / 2.0f)*CosA(pY / 2.0f)*CosA(pZ/2.0f) - SinA(pX / 2.0f)*SinA(pY / 2.0f)*SinA(pZ / 2.0f);
	}

	MatF4 Quaternion::CalculateMatrix() const
	{
		MatF4 Rotate = MatF4(1.0);

		Rotate[0][0] = (1.0f - 2.0f*Sq(Y) - 2.0f*Sq(Z));
		Rotate[0][1] = (2.0f*X*Y - 2.0f*W*Z);
		Rotate[0][2] = (2.0f*X*Z + 2.0f*W*Y);

		Rotate[1][0] = 2.0f*X*Y + 2.0f*W*Z;
		Rotate[1][1] = 1.0f - 2.0f*Sq(X) - 2.0f*Sq(Z);
		Rotate[1][2] = 2.0f*Y*Z - 2.0f*W*X;

		Rotate[2][0] = 2.0f*X*Z - 2.0f*W*Y;
		Rotate[2][1] = 2.0f*Y*Z + 2.0f*W*X;
		Rotate[2][2] = 1.0f - 2.0f*Sq(X) - 2.0f*Sq(Y);

		return Rotate;
	}

	Quaternion Quaternion::ToQuaternion(const MatF4& pRotate)
	{


		float Diag = sqrt(pRotate.At(0).At(0) + pRotate.At(1).At(1) + pRotate.At(2).At(2) + pRotate.At(3).At(3));

		float X = (pRotate.At(2).At(1) - pRotate.At(1).At(2)) / (Diag);
		float Y = (pRotate.At(2).At(0) - pRotate.At(0).At(2)) / (Diag);
		float Z = (pRotate.At(1).At(0) - pRotate.At(0).At(1)) / (Diag);
		float W = Diag*0.5f;

		return Quaternion(X, Y, Z, W);
	}

};