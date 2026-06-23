
/*
	Date: 2026/2/25
	Created By: Pouya Alizadeh 
*/


#pragma once

#include "Common.h"
#include "Geometry/Mesh.h"
#include "Render/ShaderPool.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Render/ElementBuffer.h"
#include "Render/Material/Material.h"
#include "Camera/Camera.h"
//#include "Xenon/include/Xenon.h"


namespace Hydrogen
{



	struct Node
	{
		HYD Node() = default;
		
		HYD_VEC<Node>   m_Children;
		HYD_STRING      m_Name;
		Instance<Mesh>	m_Mesh;
		Ptr<Node>       m_PointerToParent;
		Transformation  m_Transform;
	};


	class Scene
	{
	public:


		//Constructor & Destructor
		HYD  Scene();
		HYD ~Scene();

		HYD Scene(Scene&& pOther);
		HYD Scene(const Scene& pOther) = delete;

		HYD Scene& operator=(Scene&& pOther);
		HYD Scene& operator=(const Scene& pOther) = delete;


		//Free ups the Resource Pools
		HYD uint32 FreeScene();

		//Renders the Scene
		HYD uint32 Render();

		HYD Camera& GetCamera() { return m_Camera; }

	private:

		ResourcePool<Mesh>		    m_Meshes;
		ResourcePool<Material>	    m_Materials;
		ResourcePool<VertexBuffer>	m_VBOs;
		ResourcePool<ElementBuffer>	m_EBOs;
		ResourcePool<VertexArray>	m_VAOs;

		Camera			m_Camera;

		HYD_STRING      m_Name;
		HYD_VEC<Node>   m_Nodes;

		friend class Renderer;
		friend class MeshGenerator;
		friend class World;
	};

};

