
/*
	Date: 2026/2/25
	Created By: Pouya Alizadeh 
*/


#pragma once

#include "Common.h"
#include "Geometry/Mesh.h"
#include "Render/Material/Material.h"
#include "Camera/Camera.h"
#include "Xenon/include/Xenon.h"


namespace Hydrogen
{



	class Node
	{
	public:
		HYD Node() = default;


		HYD Node(const Node& pOther); 
		HYD Node(Node&& pOther);

		HYD Node& operator=(const Node& pOther);
		HYD Node& operator=(Node&& pOther);

		
		HYD inline std::vector<Node>::iterator begin() const { return m_Children.begin(); }
		HYD inline std::vector<Node>::iterator end()   const { return m_Children.end(); }


		inline HYD_STRING&     GetName()	  { return m_Name; }
		inline Instance<Mesh>& GetMesh()	  { return m_Mesh; }
		inline Transformation& GetTransform() { return m_Transform; }



	private:
		Instance<Mesh>	m_Mesh;
		Ptr<Node>	    m_PointerToParent;
		friend class Core;

	protected:
		mutable HYD_VEC<Node>   m_Children;
		HYD_STRING				m_Name;
		Transformation		    m_Transform;
		friend class Core;
	};


	class Scene : public Node
	{
	public:


		//Constructor & Destructor
		HYD  Scene();
		HYD ~Scene();

		HYD Scene(Scene&& pOther);
		HYD Scene(const Scene& pOther) = delete;

		HYD Scene& operator=(Scene&& pOther);
		HYD Scene& operator=(const Scene& pOther) = delete;


		//Pushes the node to the tree
		HYD uint32 PushNode(Node pNode);

		//Free ups the Resource Pools
		HYD uint32 FreeScene();

		//Renders the Scene
		HYD uint32 Render();

		HYD Camera& GetCamera() { return m_Camera; }

	private:

		ResourcePool<Mesh>		    m_Meshes;
		Camera					    m_Camera;

		friend class Renderer;
		friend class MeshGenerator;
		friend class Core;
	};

};

