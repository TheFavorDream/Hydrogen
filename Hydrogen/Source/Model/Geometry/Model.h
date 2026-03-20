/*

	By: Pouya Alizadeh
	Created On: 26/1/17

	Purpose:
		Model contains Meshes, and it's own Global Transformation 
		Curcial for Scene construction.

*/



#pragma once

#include "Common.h"

#include "Mesh.h"
#include "VecMath/Transformation.h"
#include "Model/MeshPool.h"
#include "glm/glm.hpp"
#include "Log/Log.h"

namespace Hydrogen
{

	class Model
	{

	public:


		HYD Model(const std::string& pName="Unamed", const Transform& pTransform=Transform());

		HYD ~Model(); //Destroy the Model

		HYD Model(const Model& pOther) = delete; //Copy 
		HYD Model(Model&& pOther);	    //Move

		HYD Model& operator=(const Model& pOther) = delete;
		HYD Model& operator=(Model&& pOther);

		//Setup Model
		HYD uint32 SetupModel(const std::string& pName, const Transform& pTransform = Transform(), std::vector<Mesh*>& pMeshes=std::vector<Mesh*>());
		
		//Destroy Model:
		HYD uint32 DestroyModel();
		 
		//Transform the Model with Matrix
		HYD uint32 SetTransform(const Transform& pTransform);
		//Transform the Model with TRS properties
		HYD uint32 SetTransform(Vec3 pScale=Vec3(1.0f), Vec3 pTranslate=Vec3(0.0f), Vec4 pRotate=Vec4(0.0f));
		
		//HYD uint32 BakeTransform(Vec3 pScale = Vec3(1.0f), Vec3 pTranslate = Vec3(0.0f), Vec4 pRotate = Vec4(0.0f));

		HYD uint32 RenderModel();

		//Get Transformer
		HYD inline const Mat4& GetModelMatrix() { return m_ModelMatrix; }

		//Name Setter & Getter
		HYD uint32 SetName(std::string pName);
		HYD inline const std::string& GetName() { return m_Name; };

//Mesh Specific Operations:

		HYD Mesh& GetMesh(const std::string& pName);
		HYD Mesh& GetMesh(const Id pId);

		HYD Mesh& operator[](const std::string& pName);
		HYD Mesh& operator[](const Id pId);

		HYD inline MeshPool& GetMeshPool() noexcept { return m_Meshes; }

		HYD Id     AddMesh(Mesh** pMesh);
		HYD uint32 PopMesh(Id*    pMesh);

		//bakes the input transform into each mesh
		//recommended for models that are static.
		HYD uint32 BakeTransform(Transform& pTransform);
		//Retrive a specific mesh


	private:

		//Meshes that make up the model
		MeshPool m_Meshes;
		//Name of the Model (not guaranteed to be unique)
		std::string m_Name;
		//Global Transformation of model which applies to all the meshes
		Mat4 m_ModelMatrix;
	};

};