
/*
	Date: 2026/2/25
	Created By: Pouya Alizadeh 
*/


#pragma once

#include "../Common.h"
#include "../Geometry/Mesh.h"
#include "Render/Material/Material.h"
#include "../Render/InstructionSet.h"
#include "../Render/Vulkan/Pipeline.h"
#include "../VecMath/Transform/Transformation.h"
#include "../Camera/Camera.h"
#include "ResourcePool.h"
#include "Xenon/include/Xenon.h"
#include "../InternalEntities/Light.h"
#include <vector>


namespace Hydrogen
{


	enum SceneFlags 
	{
		HYD_SCENE_NONE		     = 0xffff
	};


	struct SceneShaderLayout
	{
		ShaderUniformBinding MVP = ShaderUniformBinding{
			.ShaderStage = HYD_SHADER_STAGE_VERTEX_BIT,
			.Type        = HYD_DESCRIPTOR_TYPE_UNIFORM_BUFFER
		};

		MaterialBinding Material;
	};


/*
	Node Class
	Purpose:
		Each node can have a Mesh, a Light source, a Camera & a transformation
*/
	class Node
	{
	public:
		HYD Node() = default;

		HYD Node(
			const std::string& pName
		) noexcept;

		HYD Node(
			const Node& pOther
		) noexcept;

		HYD Node(
			Node&& pOther
		) noexcept;

		HYD Node& operator=(const Node& pOther);
		HYD Node& operator=(Node&& pOther);

		
		HYD inline std::vector<Node>::iterator begin() const { return m_Children.begin(); }
		HYD inline std::vector<Node>::iterator end()   const { return m_Children.end(); }

		/*
			Purpose: Add a new child to this node
		*/
		HYD void InsertChildNode(
			Node pNode
		) noexcept;

		/*
			Purpose: Set this nodes light source
		*/
		HYD void SetLight(
			HYD_ID_SPACE pLight
		) noexcept;


		HYD inline HYD_STRING&     GetName()	  { return m_Name; }
		HYD inline Instance<Mesh>& GetMesh()	  { return m_Mesh; }
		HYD inline HYD_ID_SPACE    GetLight()	  { return m_Light; }
		HYD inline Transformation& GetTransform() { return m_Transform; }

		HYD inline bool HasMesh()  const {return !m_Mesh.IsNull();}
		HYD inline bool HasLight() const {return (m_Light != 0);}
		HYD inline bool HasChild() {return m_Children.size();} 


		HYD void Destroy() noexcept;

	private:
		Instance<Mesh>	m_Mesh;
		HYD_ID_SPACE    m_Light  = 0;
		HYD_ID_SPACE    m_Camera = 0;
		Ptr<Node>	    m_PointerToParent;
		
	private:
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
		HYD  Scene() noexcept;
		HYD ~Scene() noexcept;

		HYD Scene(Scene&& pOther) noexcept;
		HYD Scene(const Scene& pOther) = delete;

		HYD Scene& operator=(Scene&& pOther) noexcept;
		HYD Scene& operator=(const Scene& pOther) = delete;


		/*
			Purpose: Free Up the resources of the scene 
		*/
		HYD uint32 FreeScene() noexcept;

		/*
			Purpose: Returns an Instruction List to Render the Scene
		*/
		HYD InstructionSet Render() noexcept;

		/*
			Purpose: Load a scene from gltf file & Configure the buffers
		*/
		HYD uint32 LoadScene(
			std::vector<ShaderConfiguration> pShaders,
			const Xenon::Scene& 			 pSourceScene,
			SceneFlags						 pFlags		   = HYD_SCENE_NONE,
			GraphicsPipelineConfiguration    pPipelineConf = GraphicsPipelineConfiguration{} 
		) noexcept;



		/*
			Purpose: Create a New Light, Add to the Scene's Light Pool & and Return a handle to the light
			Note: Creating a Light doesn't Render it in the scene, User must Add the Light to a node 
		*/

		HYD HYD_ID_SPACE NewLight(
			Light pLight
		) noexcept;


		HYD inline Camera& 		     GetCamera()    		 { return m_Camera; }
		HYD inline Transformation&   GetTransform() 		 { return m_Transform;}
		HYD inline std::vector<Node> GetChildren() 		  	 {return m_Children;}

		HYD inline std::pair<HYD_ID_SPACE, DescriptorBindInfo> PrimitiveListBind() {	
			return std::pair<HYD_ID_SPACE, DescriptorBindInfo>(m_SceneDataDescSetID, DescriptorBindInfo{
				.Index 		    = 0,
				.BindingPoint   = HYD_PIPELINE_BIND_POINT_GRAPHICS,
				.PipelineLayout = m_PipelineLayout
			});
		};

		HYD inline std::pair<HYD_ID_SPACE, DescriptorBindInfo> CameraBind() {	
			return std::pair<HYD_ID_SPACE, DescriptorBindInfo>(m_CameraDataDescSetID, DescriptorBindInfo{
				.Index 		    = 1,
				.BindingPoint   = HYD_PIPELINE_BIND_POINT_GRAPHICS,
				.PipelineLayout = m_PipelineLayout
			});
		};

	private:


		/*
			Purpose: Creates the pipelines for Scene
		*/
		void CreatePiplines(
			GraphicsPipelineConfiguration pPipelineConf
		) noexcept;

		/*
			Purpose: Create the Descriptor Set Layouts
		*/
		void ConfigureDescriptors() noexcept;

		/*
			Purpose: Allocate Descriptor Sets & Attach Resources 
		*/
		void AllocateDescriptors(
			uint32 pPrimitiveCount,
			uint32 pMaterialCount,
			uint32 pImageCount,
			uint32 pSamplerCount
		) noexcept;

	private:

		ResourcePool<Mesh>		      			m_Meshes;
		ResourcePool<Material>		  			m_Materials;
		ResourcePool<Texture2D>		  			m_Textures;
		ResourcePool<Internal::Vulkan::Sampler> m_Samplers;

		Camera					      m_Camera;
		//LightCollection				  m_Lights;

		//For Bindless Rendering
		StorageBufferRef m_PrimitiveList;
		StorageBufferRef m_MaterialList;
		StorageBufferRef m_ImageList;
		StorageBufferRef m_SamplerList;


		PipelineLayoutRef	m_PipelineLayout;
		GraphicsPipelineRef m_Pipeline;
		

		//Descriptor Set Layouts:
		HYD_ID_SPACE m_SceneDataDescLayoutID  = 0;
		HYD_ID_SPACE m_CameraDataDescLayoutID = 0;

		HYD_ID_SPACE m_SceneDataDescSetID  = 0;
		HYD_ID_SPACE m_CameraDataDescSetID = 0;

		uint32 		 m_MeshCount = 0;
		
		UniformRef 	 m_CameraData;


		
	private:
		friend class Renderer;
		friend class MeshGenerator;
		friend class Core;
	};

};

