#pragma once

#include "Common.h"
#include "VecMath/Vector/VectorDef.h"
#include "Matrix/Matrix.h"

namespace Hydrogen
{
	//Quaternion Definition
	struct Quat
	{
		float Scaler = 1.0f;
		Vec3  Vector = Vec3(0.0f);

		Quat():
			Scaler(1.0f), Vector(0.0f)
		{}

		Quat(float pScaler, Vec3 pVec):
			Scaler(pScaler), Vector(pVec)
		{}

		Quat operator+(Quat pQuaternion)
		{
			return Quat(Scaler + pQuaternion.Scaler, Vector + pQuaternion.Vector);
		}

		Quat operator-(Quat pQuaternion)
		{
			return Quat(Scaler - pQuaternion.Scaler, Vector - pQuaternion.Vector);
		}

		Quat operator*(Quat pQuaternion)
		{
			Vec3 Vec = Cross(Vector, pQuaternion.Vector) + (Vector*pQuaternion.Scaler) + (pQuaternion.Vector*Scaler);
			return Quat(Scaler*pQuaternion.Scaler-Dot(Vector, pQuaternion.Vector) , Vec);
		}
		Quat operator*(Vec3 pVec3)
		{
			return Quat(Scaler, Vec3(Vector.X*pVec3.X, Vector.Y*pVec3.Y, Vector.Z*pVec3.Z));
		}
	};


//Euler Angles
	struct Euler
	{
		float Angle = 0.0f;      //Angle in Degrees
		Vec3  Axis = Vec3(0.0f); //Defines the Axis of rotation
	};

//Operations:

	HYD Euler QuaternionToEuler(const  Quat& pQuaternion);
	HYD Quat  EulerToQuaternion (const Euler& pEuler);

	HYD Euler ExtractRotationInEuler(const Mat4& pMat4);
	HYD Quat  ExtractRotationInQuaternion(const Mat4& pMat4);



};