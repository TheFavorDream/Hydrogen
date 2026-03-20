#include "Transformation.h"
#include "HydPch.h"


namespace Hydrogen
{
	Mat4 CalculateMatrix(const Transform& pTransform)
	{
		//Multiply by TRS

		Mat4 ModelMat;

		ModelMat = Translate(ModelMat, pTransform.Translation);
		ModelMat = Rotate(ModelMat,    pTransform.Rotation);
		ModelMat = Scale(ModelMat,     pTransform.Scale);

		return ModelMat;
	}

	Mat4 Scale(Mat4& pMat4, const Vec3& pScale)
	{
		Mat4 ScaleMat;
		ScaleMat.At(0, 0) = pScale.X;
		ScaleMat.At(1, 1) = pScale.Y;
		ScaleMat.At(2, 2) = pScale.Z;
		return (pMat4 * ScaleMat);
	}

	Mat4 Translate(Mat4& pMat4, const Vec3& pTranslate)
	{
		Mat4 TranslateMat;
		TranslateMat.At(0, 3) = pTranslate.X;
		TranslateMat.At(1, 3) = pTranslate.Y;
		TranslateMat.At(2, 3) = pTranslate.Z;
		return (pMat4 * TranslateMat);
	}

	Mat4 Rotate(Mat4& pMat4, const Quat& pRotate)
	{
		Mat4 RotMat;

		RotMat.At(0, 0) = 2.0f * (pow(pRotate.Scaler, 2) + pow(pRotate.Vector.X, 2)) - 1.0f;

		RotMat.At(0, 1) = 2.0f * (pRotate.Vector.X*pRotate.Vector.Y - pRotate.Scaler*pRotate.Vector.Z);

		RotMat.At(0, 2) = 2.0f * (pRotate.Vector.X*pRotate.Vector.Z + pRotate.Scaler*pRotate.Vector.Y);


		RotMat.At(1, 0) = 2.0f * (pRotate.Vector.X*pRotate.Vector.Y + pRotate.Scaler*pRotate.Vector.Z);

		RotMat.At(1, 1) = 2.0f * (pow(pRotate.Scaler, 2) + pow(pRotate.Vector.Y, 2)) - 1.0f;

		RotMat.At(1, 2) = 2.0f * (pRotate.Vector.Y*pRotate.Vector.Z - pRotate.Scaler*pRotate.Vector.X);


		RotMat.At(2, 0) = 2.0f * (pRotate.Vector.X*pRotate.Vector.Z - pRotate.Scaler*pRotate.Vector.Y);

		RotMat.At(2, 1) = 2.0f * (pRotate.Vector.Y*pRotate.Vector.Z + pRotate.Scaler*pRotate.Vector.X);

		RotMat.At(2, 2) = 2.0f * (pow(pRotate.Scaler, 2) + pow(pRotate.Vector.Z, 2)) - 1.0f;

		return (pMat4 * RotMat);
	}



};