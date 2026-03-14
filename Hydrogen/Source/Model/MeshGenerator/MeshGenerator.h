#pragma once


#include "Common.h"
#include "Model/Geometry/Mesh.h"
#include "Model/Geometry/Model.h"
#include "Core/Scene.h"
#include "VecMath/Math.h"
#include "VecMath/Transformation.h"
#include "Render/Platform/OpenGL/GLAttributeLayout.h"

namespace Hydrogen
{


	//Generator class
	class MeshGenerator
	{
	public:

		//Generates a Cube with normals and texture coordinates and adds it in scene and specified model
		//Returns HYD_INVALID_VALUE if TargetScene or TargetModel are invalid
		HYD static uint32 GenerateCube(Scene* pTargetScene, Id pTargetModel, const Transform& pTransform=Transform());

		//Generates a Sphere with normals and texture coordinates and adds it in scene and specified model
		//Returns HYD_INVALID_VALUE if TargetScene or TargetModel are invalid
		HYD static uint32 GenerateSphere(Scene* pTargetScene, Id pTargetModel, const Transform& pTransform=Transform());
		
		//Generates a Cone with normals and texture coordinates and adds it in scene and specified model
		//Returns HYD_INVALID_VALUE if TargetScene or TargetModel are invalid
		HYD static uint32 GenerateCone(Scene* pTargetScene, Id pTargetModel, const Transform& pTransform=Transform());
		
		//Generates a Plane with normals and texture coordinates and adds it in scene and specified model
		//Returns HYD_INVALID_VALUE if TargetScene or TargetModel are invalid
		HYD static uint32 GeneratePlane(Scene* pTargetScene, Id pTargetModel, const Transform& pTransform=Transform());

	};


};
