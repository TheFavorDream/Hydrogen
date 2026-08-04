#include "Transformation.h"
#include "HydPch.h"
#include "../Math.h"

namespace Hydrogen
{



	Transformation::Transformation(Scaler pScale, Rotation pRotate, Translation pTranslate, bool pIsDirty)
	{
		t_Scale		= pScale;
		t_Rotate	= pRotate;
		t_Translate = pTranslate;
		IsDirty		= pIsDirty;
	}

	Transformation Transformation::operator*(const Transformation& pTransfrom) const
	{

		if (!(this->IsDirty || pTransfrom.IsDirty))
			return Transformation(VecF3(1.0f), VecF4(VecF3(0.0f), 1.0f), VecF3(0.0f), (this->IsDirty | pTransfrom.IsDirty));


		const Translation &LT = t_Translate;
		const Rotation    &LR = t_Rotate;
		const Scaler      &LS = t_Scale;

		const Translation &RT = pTransfrom.t_Translate;
		const Rotation    &RR = pTransfrom.t_Rotate;
		const Scaler      &RS = pTransfrom.t_Scale;


		Scaler   scale = LS * RS;

		Rotation rotate = LR * RR;

		//TODO: Fix the percision rounding problem when computing translation 

		Translation translate = Translation(
		
			(RT.X*( LS.X* ( 1.0f - 2.0f*Sq(LR.Y) - 2.0f*Sq(LR.Z) ) )) + (RT.Y*2.0f*( (LR.X*LR.Y) - (LR.W*LR.Z) )) + (RT.Z*2.0f*(LR.X*LR.Z + LR.W*LR.Y)) + LT.X,
		
			RT.X*2.0f* ( (LR.X*LR.Y) + (LR.W*LR.Z) )+ RT.Y*( LS.Y* (1.0f - 2.0f*Sq(LR.X) - 2.0f*Sq(LR.Z))) + RT.Z*2.0f*((LR.Y*LR.Z) - (LR.W*LR.X)) + LT.Y,
		
			RT.X*2.0f*((LR.X*LR.Z) - (LR.W*LR.Y) ) + RT.Y*2.0f*( (LR.Y*LR.Z) + (LR.W*LR.X) ) + RT.Z*(LS.Z * (1.0f - 2.0f*Sq(LR.X) - 2.0f*Sq(LR.Y)) ) + LT.Z
		);
		

		return Transformation(scale, rotate, translate, (this->IsDirty | pTransfrom.IsDirty));
	}



//--------------------------------------TRS-----------------------------------------

	Scaler::Scaler(float pScaleX, float pScaleY, float pScaleZ)
	{
		X = pScaleX;
		Y = pScaleY;
		Z = pScaleZ;
	}

	Scaler::Scaler(VecF3 pVec)
	{
		X = pVec.X;
		Y = pVec.Y;
		Z = pVec.Z;
	}

	Scaler Scaler::operator*(const Scaler& pOther) const
	{
		return Scaler(X*pOther.X, Y*pOther.Y, Z*pOther.Z);
	}

	MatF4 Scaler::ToMatrix() const
	{
		MatF4 Scale = MatF4(1.0f);
		Scale[0][0] = X;
		Scale[1][1] = Y;
		Scale[2][2] = Z;
		return Scale;
	}




	Rotation::Rotation(VecF4 pVec)
	{
		X = pVec.X;
		Y = pVec.Y;
		Z = pVec.Z;
		W = pVec.W;

	}

	Rotation::Rotation(Quat pVec)
	{
		X = pVec.X;
		Y = pVec.Y;
		Z = pVec.Z;
		W = pVec.W;
	}

	Rotation::Rotation(VecF3 pVec, float pVal)
	{
		X = pVec.X;
		Y = pVec.Y;
		Z = pVec.Z;
		W = pVal;
	}

	Rotation& Rotation::operator=(const Rotation& pOther)
	{
		X = pOther.X;
		Y = pOther.Y;
		Z = pOther.Z;
		W = pOther.W;
		return *this;
	}


	Rotation Rotation::operator*(const Rotation& pOther) const
	{
		return Quat(X, Y, Z, W) * Quat(pOther.X, pOther.Y, pOther.Z, pOther.W);
	}


	MatF4 Rotation::ToMatrix() const
	{
		return Quat(X, Y, Z, W).CalculateMatrix();
	}

	Rotation Rotation::ToRotaion(const MatF4& pRotate)
	{
		return Quat::ToQuaternion(pRotate);
	}





	Translation::Translation(float pX, float pY, float pZ)
	{
		X = pX;
		Y = pY;
		Z = pZ;
	}

	Translation::Translation(VecF3 pVec)
	{
		X = pVec.X;
		Y = pVec.Y;
		Z = pVec.Z;
	}

	Translation Translation::operator*(const Translation& pOther) const
	{
		return Translation(X+pOther.X, Y + pOther.Y, Z + pOther.Z);
	}

	MatF4 Translation::ToMatrix() const
	{
		MatF4 Translate = MatF4(1.0f);
		Translate[0][3] = X;
		Translate[1][3] = Y;
		Translate[2][3] = Z;
		return Translate;
	}


};