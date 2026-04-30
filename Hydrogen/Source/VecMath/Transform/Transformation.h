#pragma once

#include "Common.h"

#include "Scale.h"
#include "Rotate.h"
#include "Translate.h"

#include "../Vector/Vectors.h"
#include "../Matrix/MatDef.h"
#include "../Quaternion.h"

namespace Hydrogen
{

	//Describes the Transformation of a primitive
	struct Transformation
	{

		HYD Transformation(Scaler pScale = VecF3(1.0f), Rotation pRotate = VecF4(VecF3(0.0f), 1.0f), Translation pTranslate = VecF3(0.0f), bool pIsDirty=true);

		//TRS properties:
		Rotation    t_Rotate       = Quat(VecF3(0.0f), 1.0f);
		Scaler      t_Scale		   = VecF3(1.0f);
		Translation t_Translate    = VecF3(0.0f);
		//indicates if TRS values has been changed if so, recalculate model matrix
		mutable bool IsDirty = true;

		HYD Transformation operator*(const Transformation& pTransfrom) const;

		HYD static inline MatF4 CalculateMatrix(const Transformation& pTransform){
			return (pTransform.t_Translate.ToMatrix() * pTransform.t_Rotate.ToMatrix() * pTransform.t_Scale.ToMatrix());
		}

	};
};