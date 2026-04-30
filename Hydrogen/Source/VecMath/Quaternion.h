#pragma once

#include "Common.h"
#include "Vector/Vectors.h"
#include "Matrix/MatDef.h"
#include "Math.h"

namespace Hydrogen
{


	/*
		Definition for Quaternions, used for rotation
		Q = <X, Y, Z, W> = <Vec3(X, Y, Z), W>
	*/

	struct Quaternion;

	typedef Quaternion Quat;


	struct Quaternion : public VecF4
	{

		HYD  Quaternion() = default;
		HYD ~Quaternion() = default;
		
		HYD Quaternion(VecF3 pVec, float pVal);
		HYD Quaternion(float pX, float pY, float pZ, float pW);
		HYD Quaternion(const VecF4& pOther);


		//Cast back to Vec4
		HYD operator VecF4() const;

		HYD Quaternion operator*(const Quaternion& pOther) const;

		HYD void Euler(float pX, float pY, float pZ) ;

		//Calculates Rotation matrix of the quaternion:
		HYD MatF4 CalculateMatrix() const;
		HYD static Quaternion  ToQuaternion(const MatF4& pRotate);

	};



};