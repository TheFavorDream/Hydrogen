/*

	By: Pouya Alizadeh
	Created On: 26/1/17

	Purpose:
		Model contains Meshes, and it's own Global Transformation 
		Curcial for Scene construction.

*/



#pragma once

#include "Common.h"

#include <vector>
#include <string>
#include "Math/Matrix/Matrix.h"
#include "Geometry/Mesh.h"
#include "Shader/Shader.h"
#include "Platform/OpenGL/Buffer.h"
#include "Platform/OpenGL/VertexArray.h"
#include "3rdParty/glm/glm.hpp"
#include "Log/Log.h"
#include "Material/Material.h"

namespace Hydrogen
{

	class Model
	{

	public:

		HYD Model();																					 //Create an Empty Model
		HYD Model(const std::string& pName);														     //Create an Empty Model with Specified name
		HYD Model(const std::string& pName, std::vector<Mesh>& pMeshes);							 //Create Model with a set of input meshes
		HYD Model(const std::string& pName, std::vector<Mesh>& pMeshes, Matrix& pTransform); //Create a Model with Meshes and Transformation;

		HYD ~Model(); //Destroy the Model

		HYD Model(const Model& pOther) = delete; //Copy 
		HYD Model(Model&& pOther);	    //Move

		HYD Model& operator=(const Model& pOther) = delete;
		HYD Model& operator=(Model&& pOther);

		//Setup Model
		HYD uint32 SetupModel( const std::string& pName,  std::vector<Mesh>& pMeshes);
		
		//Destroy Model:
		HYD uint32 DestroyModel();
		 
		//Transform the Model with Matrix
		HYD uint32 SetTransform(const glm::mat4& pTransform);
		//Transform the Model with TRS properties
		HYD uint32 SetTransform(Vec3 pScale=Vec3(1.0f), Vec3 pTranslate=Vec3(0.0f), Vec4 pRotate=Vec4(0.0f));
		
		//bakes the input transform into each mesh
		//recommended for models that are static.
		HYD uint32 BakeTransform(const glm::mat4& pTransform);
		HYD uint32 BakeTransform(Vec3 pScale = Vec3(1.0f), Vec3 pTranslate = Vec3(0.0f), Vec4 pRotate = Vec4(0.0f));


		//Get Transformer
		HYD inline const glm::mat4& GetTransform() { return m_Transform; }

		//Name Setter & Getter
		HYD uint32 SetName(std::string pName);
		HYD inline const std::string& GetName() { return m_Name; };

		//Iterator for Meshes
		HYD std::vector<Mesh>::iterator begin() { return m_Meshes.begin(); }
		HYD std::vector<Mesh>::iterator end() { return m_Meshes.end(); }


		//the "pTransform" will get multiplied by Global Transformation of the model
		//Render the Entire model
		//this function is going to be used by the scene
		//the owner scene of this model should supply the shader when calling.
		HYD uint32 RenderModel(const Shader& pShader);

		//Retrive a specific mesh
		HYD const Mesh& GetMeshByIndex(uint32 pIndex) noexcept;
		HYD const Mesh& GetMeshByName(std::string pName);


	private:
		//Meshes that make up the model
		std::vector<Mesh> m_Meshes;

		//Name of the Model (not guaranteed to be unique)
		std::string m_Name;
		//Global Transformation of model which applies to all the meshes
		//Matrix m_Transform;
		glm::mat4 m_Transform=glm::mat4(1.0f);

		friend class Mesh;
		friend class GLTFLoader;

	};

};