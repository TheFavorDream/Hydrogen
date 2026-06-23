#include "Scene.h"
#include "HydPch.h"

namespace Hydrogen
{

	Scene::Scene()
	{
		m_Camera.SetupCamera(60.0f, glm::vec3(0.0f));
	}

	Scene::~Scene()
	{
	}


	Scene::Scene(Scene&& pOther)
	{
		m_VBOs = std::move(pOther.m_VBOs);
		m_VAOs = std::move(pOther.m_VAOs);
		m_EBOs = std::move(pOther.m_EBOs);

		m_Meshes    = std::move(pOther.m_Meshes);
		m_Materials = std::move(pOther.m_Materials);
		m_Name		= std::move(pOther.m_Name);
		m_Nodes		= std::move(pOther.m_Nodes);
	}

	Scene& Scene::operator=(Scene&& pOther)
	{
		if (&pOther == this)
			return *this;

		m_VBOs = std::move(pOther.m_VBOs);
		m_VAOs = std::move(pOther.m_VAOs);
		m_EBOs = std::move(pOther.m_EBOs);

		m_Meshes    = std::move(pOther.m_Meshes);
		m_Materials = std::move(pOther.m_Materials);
		m_Name      = std::move(pOther.m_Name);
		m_Nodes     = std::move(pOther.m_Nodes);
		return *this;
	}


	uint32 Scene::FreeScene()
	{

		return HYD_OK;
	}

/*
	Id Scene::CreateMesh(Id pModel, const std::vector<Vertex>& pVertices, const std::vector<uint16>& pIndices)
	{
		Primitive primitive;

		primitive.m_VertexArrays = m_Buffers.CreateVertexArray();
		m_Buffers.BindArray(primitive.m_VertexArrays);

		primitive.m_VertexBuffer =  m_Buffers.CreateVertexBuffer((uint32)pVertices.size() * sizeof(Vertex), (void*)&pVertices[0]);
		primitive.m_ElementBuffer = m_Buffers.CreateElementBuffer((uint32)pIndices.size(), GL_UNSIGNED_SHORT, (void*)&pIndices[0]);


		m_Buffers.BindVertexBuffer(primitive.m_VertexBuffer);
		m_Buffers.BindElementBuffer(primitive.m_ElementBuffer);


		m_Buffers.AddVertexAttributes(primitive.m_VertexArrays, 
			{ 
				Layout(TYPE_FLOAT, 3, POSITION, false, 0, sizeof(Vertex)),
				Layout(TYPE_FLOAT, 3, NORMALS,  false, 12,  sizeof(Vertex)),
				Layout(TYPE_FLOAT, 2, TEXCOORDS_0, false, 24, sizeof(Vertex))
			});

		Mesh* NewMesh = ResourcePool<Mesh>::New();
		NewMesh->PushPremitive(primitive);
		return GetModel(pModel).AddMesh(&NewMesh);
	}
	*/




	uint32 Scene::Render()
	{
		//Start the Travers:
		std::stack<Node> Travers;


		for (auto& node : m_Nodes)
		{
			Travers.push(node);
		}



		while (!Travers.empty())
		{
			Node Current = Travers.top();
			Travers.pop();

			
			if (!Current.m_Mesh.IsNull())
				Current.m_Mesh->Render(Current.m_Transform);

			for (auto node : Current.m_Children)
			{
				node.m_Transform = Current.m_Transform * node.m_Transform;
				Travers.push(node);
			}
		
		}

		return HYD_OK;
	}



};