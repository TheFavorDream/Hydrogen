#include "Scene.h"
#include "HydPch.h"

namespace Hydrogen
{

	Scene::Scene()
	{
		m_Camera.SetupCamera(45.0f, glm::vec3(0.0f, 0.0f, -5.0f), 2.0f, 0.01f, 200000.0f);

		m_Buffers.InitPool();
		m_Materials.InitPool();
		m_Models.InitPool();
	}

	Scene::~Scene()
	{
	}


	Id Scene::LoadModel(const std::string& pPath)
	{
		Model* NewModel = GLTFLoader::Load(pPath, this, NO_MATERIAL);
		if (NewModel == nullptr)
			return 0;
		return m_Models.PushModel(&NewModel);
	}

	Id Scene::NewModel(const std::string& pName, std::vector<Mesh*>& pMeshes, const Transform& pTransform)
	{
		Model* NewModel = ResourcePool<Model>::New();
		NewModel->SetupModel(pName, pTransform, pMeshes);
		return m_Models.PushModel(&NewModel);
	}


	uint32 Scene::FreeScene()
	{
		m_Models.ShutdownPool();
		m_Materials.ShutdownPool();
		m_Buffers.ShutdownPool();
		return HYD_OK;
	}


	Id Scene::CreateMesh(Id pModel, const std::vector<Vertex>& pVertices, const std::vector<uint16>& pIndices)
	{
		Primitive primitive;

		primitive.Count = pIndices.size();
		primitive.Type = GL_UNSIGNED_SHORT;

		primitive.m_VertexArrays = m_Buffers.CreateVertexArray();
		m_Buffers.BindArray(primitive.m_VertexArrays);

		primitive.m_VertexBuffer =  m_Buffers.CreateVertexBuffer( pVertices.size() * sizeof(Vertex), (void*)&pVertices[0]);
		primitive.m_ElementBuffer = m_Buffers.CreateElementBuffer(pIndices.size() * sizeof(uint16),  (void*)&pIndices[0]);


		m_Buffers.BindVertexBuffer(primitive.m_VertexBuffer);
		m_Buffers.BindElementBuffer(primitive.m_ElementBuffer);


		m_Buffers.AddVertexAttributes(primitive.m_VertexArrays, 
			{ 
				Layout(TYPE_FLOAT, 3, POSITION),
				Layout(TYPE_FLOAT, 3, NORMALS),
				Layout(TYPE_FLOAT, 2, TEXCOORDS_0)
			});

		Mesh* NewMesh = ResourcePool<Mesh>::New();
		NewMesh->PushPremitive(primitive);
		return GetModel(pModel).AddMesh(&NewMesh);
	}

	uint32 Scene::Render()
	{
		m_Models.RenderModels();
		return HYD_OK;
	}


	Model& Scene::GetModel(Id pModel)
	{
		return m_Models.GetModel(pModel);
	}

	Model& Scene::operator[](Id pModel)
	{
		return m_Models.GetModel(pModel);
	}

};