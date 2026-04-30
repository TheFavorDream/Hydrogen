
/*
	Date: 2026/2/25
	Created By: Pouya Alizadeh aka Voltage
*/


#pragma once

#include "Common.h"
#include "Model/ModelPool.h"
#include "Model/MeshPool.h"
#include "Render/RenderPool/ShaderPool.h"
#include "Render/RenderPool/BufferPool.h"
#include "Render/RenderPool/MaterialPool.h"
#include "Render/MatrixPool.h"
#include "Camera/Camera.h"
#include "Loaders/GLTF.h"


namespace Hydrogen
{

	class Scene
	{
	public:


		//Constructor & Destructor
		HYD  Scene();
		HYD ~Scene();

		//Free ups the Resource Pools
		HYD uint32 FreeScene();

		//Model & mesh Loading & Creation
		HYD Id      LoadModel(const std::string& pPath);
		HYD Id      NewModel(const std::string& pName, std::vector<Ptr<Mesh>>& pMeshes = std::vector<Ptr<Mesh>>(), const Transformation& pTransform = Transformation());
		HYD Id		CreateMesh(Id pModel, const std::vector<Vertex>& pVertices, const std::vector<uint16>& pIndices);


		//Model:
		HYD uint32 SetModelTransform(Id pModel, const Transformation& pTransform);
		HYD uint32 SetModelTransform(Id pModel, const Scaler& pScale=VecF3(1.0f), const Rotation& pRotation=VecF4(VecF3(0.0f), 1.0f), const Translation& pTranslate=VecF3(0.0f));

		HYD Transformation GetModelTransformation(Id pModel);


		//Renders the Scene
		HYD uint32 Render();


		//Getters:

		HYD Model& GetModel(Id pModel);
		HYD Model& operator[](Id pModel);

		HYD inline Camera&          GetCamera() noexcept { return m_Camera; }
		HYD inline const ModelPool& GetModels() noexcept { return m_Models; }

	private:

		ModelPool		m_Models;
		BufferPool		m_Buffers;
		MaterialPool	m_Materials;
		MatrixPool      m_Matrices;

		Camera			m_Camera;

		friend class GLTFLoader;
		friend class Renderer;
		friend class MeshGenerator;
		friend class Mesh;
	};

};

