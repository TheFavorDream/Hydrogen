#pragma once


#include "Common.h"
#include "Geometry/Mesh.h"
#include "Core/Scene.h"
#include "VecMath/Math.h"
#include "VecMath/Transform/Transformation.h"


namespace Hydrogen
{


	//Generator class
	class MeshGenerator
	{
	public:

		////Generates a Cube with normals and texture coordinates and adds it in scene and specified model
		////Returns HYD_INVALID_VALUE if TargetScene or TargetModel are invalid
		//HYD static uint32 GenerateCube(Ptr<Scene> pTargetScene, Ptr<Model> pTargetModel, const Transformation& pTransform= Transformation());
		//
		////Generates a Sphere with normals and texture coordinates and adds it in scene and specified model
		////Returns HYD_INVALID_VALUE if TargetScene or TargetModel are invalid
		//HYD static uint32 GenerateSphere(Scene* pTargetScene, Id pTargetModel, const Transformation& pTransform= Transformation());
		//
		////Generates a Cone with normals and texture coordinates and adds it in scene and specified model
		////Returns HYD_INVALID_VALUE if TargetScene or TargetModel are invalid
		//HYD static uint32 GenerateCone(Scene* pTargetScene, Id pTargetModel, const Transformation& pTransform= Transformation());
		//
		////Generates a Plane with normals and texture coordinates and adds it in scene and specified model
		////Returns HYD_INVALID_VALUE if TargetScene or TargetModel are invalid
		//HYD static uint32 GeneratePlane(Scene* pTargetScene, Id pTargetModel, const Transformation& pTransform= Transformation());

	};


};
