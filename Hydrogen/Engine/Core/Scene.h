
/*
	Date: 2026/2/25
	Created By: Pouya Alizadeh 
*/


#pragma once

#include "../Common.h"
#include "../Geometry/Mesh.h"
#include "Render/Material/Material.h"
#include "../Render/Vulkan/Pipeline.h"
#include "../VecMath/Transform/Transformation.h"
#include "../Camera/Camera.h"
#include "Xenon/include/Xenon.h"
#include "../InternalEntities/Light.h"


namespace Hydrogen
{


	enum SceneFlags 
	{
		HYD_SCENE_NONE		     = 0xffff,
		HYD_SCENE_EXTERNAL_LIGHT = 1 << 1
	};


	struct SceneShaderLayout
	{
		ShaderUniformBinding MVP = ShaderUniformBinding{
			.ShaderStage = HYD_SHADER_STAGE_VERTEX_BIT,
			.Type        = HYD_DESCRIPTOR_TYPE_UNIFORM_BUFFER
		};

		MaterialBinding Material;
	};



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

		HYD void Destroy() noexcept;


	private:
		Instance<Mesh>	m_Mesh;
		Ptr<Node>	    m_PointerToParent;
		friend class Scene;
	
	protected:
		mutable HYD_VEC<Node>   m_Children;
		HYD_STRING				m_Name;
		Transformation		    m_Transform;
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

		/*
			Purpose: Returns an Instruction List to Render the Scene
		*/
		HYD std::vector<Instruction> Render() noexcept;

		//Load Models
		HYD uint32 LoadScene(
			const Xenon::Scene& pSourceScene
		) noexcept;


		HYD uint32 ConfigurePipeline(
			const GraphicsPipelineConfiguration& pConf,
			const SceneShaderLayout&			 pShaderLayout,
			SceneFlags							 pFlags = HYD_SCENE_NONE
		) noexcept;

		HYD inline Camera& 		    GetCamera()    		 { return m_Camera; }
		HYD inline Transformation&  GetTransform() 		 { return m_Transform;}
		HYD inline LightCollection& GetLightCollection() { return m_Lights;}

	private:

		ResourcePool<Mesh>		      m_Meshes;
		Camera					      m_Camera;
		LightCollection				  m_Lights;

		GraphicsPipelineConfiguration m_PipelineConf;
		GraphicsPipelineRef 		  m_Pipeline;
		HYD_ID_SPACE 				  m_PipelineLayoutID = 0;
		HYD_ID_SPACE			      m_MVPLayoutID 	 = 0;
		HYD_ID_SPACE			      m_MaterialLayoutID = 0;
		HYD_ID_SPACE			      m_LightLayoutID    = 0;
		SceneShaderLayout   	      m_ShaderBinding;
		UniformRef 				      m_Uniforms;
		
	private:
		friend class Renderer;
		friend class MeshGenerator;
		friend class Core;
	};

};

