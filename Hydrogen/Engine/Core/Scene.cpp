#include "Scene.h"
#include "HydPch.h"

namespace Hydrogen
{

	Scene::Scene()
	{
		m_Camera.SetupCamera(60.0f, glm::vec3(0.0f), 2.0f, 0.001f, 10000.0f);
	}

	Scene::~Scene()
	{
	}


	Scene::Scene(Scene&& pOther)
	{
		m_Meshes    = std::move(pOther.m_Meshes);
		m_Name		= std::move(pOther.m_Name);
		m_Children	= std::move(pOther.m_Children);
	}

	Scene& Scene::operator=(Scene&& pOther)
	{
		if (&pOther == this)
			return *this;
		m_Meshes    = std::move(pOther.m_Meshes);
		m_Name      = std::move(pOther.m_Name);
		m_Children  = std::move(pOther.m_Children);

		return *this;
	}


	uint32 Scene::PushNode(Node pNode)
	{
		m_Children.push_back(std::move(pNode));
		return HYD_OK;
	}

	uint32 Scene::FreeScene()
	{
		m_Meshes.Shutdown();
		return HYD_OK;
	}

	uint32 Scene::Render()
	{
		//Start the Travers:
		std::stack<Node> Travers;


		for (auto node : m_Children)
		{
			node.GetTransform() = m_Transform * node.GetTransform();
			Travers.push(node);
		}



		while (!Travers.empty())
		{
			Node Current = Travers.top();
			Travers.pop();

			
			if (!Current.GetMesh().IsNull())
				Current.GetMesh()->Render(Current.GetTransform());

			for (auto node : Current)
			{
				node.GetTransform() = Current.GetTransform() * node.GetTransform();
				Travers.push(node);
			}
		
		}

		return HYD_OK;
	}



/*
	Node Implementation:
*/



	Node::Node(const Node& pOther)
	{
		m_Name	    = pOther.m_Name;
		m_Mesh	    = pOther.m_Mesh;
		m_Children  = pOther.m_Children;
		m_Transform = pOther.m_Transform;

		m_PointerToParent = pOther.m_PointerToParent;
		

	}

	Node::Node(Node&& pOther)
	{
		m_Name		= std::move(pOther.m_Name);
		m_Mesh		= std::move(pOther.m_Mesh);
		m_Children	= std::move(pOther.m_Children);
		m_Transform = std::move(pOther.m_Transform);

		m_PointerToParent	     = pOther.m_PointerToParent;
		pOther.m_PointerToParent = nullptr;
	}
		
	Node& Node::operator=(const Node& pOther)
	{
		if (&pOther == this)
			return *this;

		m_Name		= pOther.m_Name;
		m_Mesh		= pOther.m_Mesh;
		m_Children  = pOther.m_Children;
		m_Transform = pOther.m_Transform;

		m_PointerToParent = pOther.m_PointerToParent;

		return *this;
	}

	Node& Node::operator=(Node&& pOther)
	{

		if (&pOther == this)
			return *this;

		m_Name		= std::move(pOther.m_Name);
		m_Mesh		= std::move(pOther.m_Mesh);
		m_Children  = std::move(pOther.m_Children);
		m_Transform = std::move(pOther.m_Transform);

		m_PointerToParent		 = pOther.m_PointerToParent;
		pOther.m_PointerToParent = nullptr;

		return *this;

	}

};