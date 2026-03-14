#pragma once

#include "Common.h"
#include "Rotate.h"

#include "Vector/VectorDef.h"
#include "Matrix/Predefined/Mat4/Mat4.h"
#include "Matrix/Predefined/Mat3/Mat3.h"
#include "Matrix/Predefined/Mat2/Mat2.h"


namespace Hydrogen
{

	//Stores Transformation
	struct Transform
	{
		bool IsDirty = true; //if true, recalculate matrix 

		Vec3 Scale        = Vec3(1.0f, 1.0f, 1.0f);			    //Scale Factor
		Vec3 Translation  = Vec3(0.0f, 0.0f, 0.0f);             //Translate
		Quat Rotation     = Quat(1.0f, Vec3(0.0f, 0.0f, 0.0f)); //Rotation with quaternions
	};


	HYD Mat4 CalculateMatrix(const Transform& pTransform);


	HYD Mat4 Scale     (Mat4& pMat4, const Vec3& pScale);
	HYD Mat4 Translate (Mat4& pMat4, const Vec3& pTranslate);
	HYD Mat4 Rotate    (Mat4& pMat4, const Quat& pRotate);


};