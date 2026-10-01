#include "Scene.h"
#include "../HydPch.h"
#include "../Render/Renderer.h"
#include <cstddef>
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{



/*
	Node Implementation:
*/


	Node::Node(
		const std::string& pName
	) noexcept
		: m_Name(pName)
	{

	}

	Node::Node(
		const Node& pOther
	) noexcept
	{
		m_Name	    = pOther.m_Name;
		m_Mesh	    = pOther.m_Mesh;
		m_Children  = pOther.m_Children;
		m_Transform = pOther.m_Transform;
		m_Light     = pOther.m_Light;
		m_Camera    = pOther.m_Camera;
		m_PointerToParent = pOther.m_PointerToParent;
	}

	Node::Node(
		Node&& pOther
	) noexcept
	{
		m_Name		= std::move(pOther.m_Name);
		m_Mesh		= std::move(pOther.m_Mesh);
		m_Children	= std::move(pOther.m_Children);
		m_Transform = std::move(pOther.m_Transform);
		m_Camera    = pOther.m_Camera;
		m_Light     = pOther.m_Light;

		pOther.m_Light  = 0;
		pOther.m_Camera = 0;

		m_PointerToParent	     = pOther.m_PointerToParent;
		pOther.m_PointerToParent = nullptr;
	}
		
	Node& Node::operator=(
		const Node& pOther
	)
	{
		if (&pOther == this)
			return *this;

		m_Name		= pOther.m_Name;
		m_Mesh		= pOther.m_Mesh;
		m_Children  = pOther.m_Children;
		m_Transform = pOther.m_Transform;
		m_Light     = pOther.m_Light;
		m_Camera    = pOther.m_Camera;

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

		m_Light     = pOther.m_Light;
		m_Camera    = pOther.m_Camera;
		
		pOther.m_Light  = 0;
		pOther.m_Camera = 0;

		m_PointerToParent		 = pOther.m_PointerToParent;
		pOther.m_PointerToParent = nullptr;

		return *this;

	}

	
	void Node::SetLight(
		HYD_ID_SPACE pLight
	) noexcept
	{
		m_Light = pLight;
	}


	void Node::InsertChildNode(
		Node pNode
	) noexcept
	{
		m_Children.push_back(std::move(pNode));
	}


	void Node::Destroy() noexcept
	{
		m_Mesh.Reset();
		m_Light  = 0;
		m_Camera = 0;
		for (auto& node : m_Children)
			node.Destroy();
	}


//-------------------------Scene-------------------------------------------------------

	Scene::Scene() noexcept
	{
		m_Camera.SetupCamera(60.0f, glm::vec3(0.0f), 2.0f, 0.001f, 10000.0f);

		Renderer::Self().SetCamera(&m_Camera);
	}

	Scene::~Scene() noexcept
	{
		FreeScene();
	}


	Scene::Scene(
		Scene&& pOther
	) noexcept
	{
		m_Meshes    = std::move(pOther.m_Meshes);
		m_Materials = std::move(pOther.m_Materials);
		m_Textures  = std::move(pOther.m_Textures);
		m_Samplers  = std::move(pOther.m_Samplers);
		m_Camera    = std::move(pOther.m_Camera);

		//LightCollection				  m_Lights;

		//For Bindless Rendering
		m_PrimitiveList = std::move(pOther.m_PrimitiveList);
		m_MaterialList  = std::move(pOther.m_MaterialList);
		m_ImageList     = std::move(pOther.m_ImageList);
		m_SamplerList   = std::move(pOther.m_SamplerList);

		m_PipelineLayout = std::move(m_PipelineLayout);
		m_Pipeline 		 = std::move(m_Pipeline);
		m_CameraData 	 = std::move(m_CameraData);
		

		//Descriptor Set Layouts:
		m_SceneDataDescLayoutID  = pOther.m_SceneDataDescLayoutID;
		m_CameraDataDescLayoutID = pOther.m_CameraDataDescLayoutID;

		pOther.m_SceneDataDescLayoutID  = 0;
		pOther.m_CameraDataDescLayoutID = 0;

	}

	Scene& Scene::operator=(
		Scene&& pOther
	) noexcept
	{
		if (&pOther == this)
			return *this;

		m_Meshes    = std::move(pOther.m_Meshes);
		m_Materials = std::move(pOther.m_Materials);
		m_Textures  = std::move(pOther.m_Textures);
		m_Samplers  = std::move(pOther.m_Samplers);
		m_Camera    = std::move(pOther.m_Camera);

		//LightCollection				  m_Lights;

		//For Bindless Rendering
		m_PrimitiveList = std::move(pOther.m_PrimitiveList);
		m_MaterialList  = std::move(pOther.m_MaterialList);
		m_ImageList     = std::move(pOther.m_ImageList);
		m_SamplerList   = std::move(pOther.m_SamplerList);

		m_PipelineLayout = std::move(m_PipelineLayout);
		m_Pipeline 		 = std::move(m_Pipeline);
		m_CameraData 	 = std::move(m_CameraData);
		

		//Descriptor Set Layouts:
		m_SceneDataDescLayoutID  = pOther.m_SceneDataDescLayoutID;
		m_CameraDataDescLayoutID = pOther.m_CameraDataDescLayoutID;

		pOther.m_SceneDataDescLayoutID  = 0;
		pOther.m_CameraDataDescLayoutID = 0;

		return *this;
	}


	uint32 Scene::FreeScene() noexcept
	{
		for (auto& node : m_Children)
		{
			node.Destroy();
		}

		m_Meshes.Shutdown();
		return HYD_OK;
	}

	InstructionSet Scene::Render() noexcept
	{
		//Start the Travers:
		std::stack<Node> Travers;

		for (auto node : m_Children)
		{
			node.GetTransform() = m_Transform * node.GetTransform();
			Travers.push(node);
		}


		//Upload Camera Data:

		Renderer::Self().AccessUniformBuffer(m_CameraData).UploadData(
			m_Camera.GetViewPtr(),
			sizeof(glm::mat4),
			0
		);
		
		Renderer::Self().AccessUniformBuffer(m_CameraData).UploadData(
			m_Camera.GetProjectionPtr(),
			sizeof(glm::mat4),
			sizeof(glm::mat4)
		);
		
		InstructionSet Instructions;

		while (!Travers.empty())
		{
			Node Current = Travers.top();
			Travers.pop();



			if (Current.HasMesh())
			{

				Instance<Mesh>& mesh = Current.GetMesh();

				Instructions.Append(mesh->Render(
					Current.GetTransform()
				));

				MatF4 Model 	  = mesh->GetModelMatrix();
				
				uint32 BaseOffset =	(m_MeshCount*sizeof(MeshData))*Renderer::Self().CurrentFrame(); 
				m_PrimitiveList->UploadData(
					(void*)Model.GetPointer(),sizeof(MeshData), BaseOffset +  (mesh->GetID()*sizeof(MeshData))
				);
			}

			
			for (auto node : Current)
			{
				node.GetTransform() = Current.GetTransform() * node.GetTransform();
				Travers.push(node);
			}
		
		}

		return std::move(Instructions);
	}


	/*
		Purpose: Load a scene from gltf file & Configure the buffers
	*/
	uint32 Scene::LoadScene(
		std::vector<ShaderConfiguration> pShaders,
		const Xenon::Scene& 			 pSourceScene,
		SceneFlags						 pFlags,
		GraphicsPipelineConfiguration    pPipelineConf// = GraphicsPipelineConfiguration{} 
	) noexcept
	{


		//Shaders:
		for(auto& shader : pShaders)
		{
			pPipelineConf.AttachShader(shader);
		}
		
		CreatePiplines(pPipelineConf);

		
		std::vector<Instance<Mesh>>		 				MeshInstances;
		//std::vector<Instance<Material>>					MaterialInstances;
		//std::unordered_map<uint64, Instance<Texture2D>> TextureInstances;


		//std::set<Xenon::Sampler> XenonSamplers;

		//Set the name for the scene:
		m_Name = pSourceScene.GetName();

		/*
		//Texture Loading:
		for (auto& texture : pSourceScene.GetTextures())
		{
			TextureConfiguration TextureConfiguration;

			Xenon::BinaryData Img 	   		 = texture.second.RetriveImageData();
			TextureConfiguration.Data  		 = Buffer(Img.Ptr, Img.ByteLength);
			Img.Ptr 	   			   		 = nullptr;
			Img.ByteLength 			   		 = 0;
			TextureConfiguration.TexCoordSet = texture.second.TexCoordSet;

			XenonSamplers.emplace(texture.second.TextureSample);

			Texture2DRef NewTexture = m_Textures.Resource();
			NewTexture->CreateTexture(TextureConfiguration);

			TextureInstances.emplace(texture.first, std::move(NewTexture));
		}

		*/




		uint32 MeshIDGen = 0;		
		for (auto& mesh : pSourceScene.GetMeshes())
		{
			Mesh NewMesh = Mesh::CreateGLTFMesh(mesh, MeshIDGen++, m_Pipeline);
			m_MeshCount++;

			MeshInstances.push_back(
				m_Meshes.PushObject(std::move(NewMesh))
			);
		}



		//Node-Tree:
		using NODE = std::pair<Xenon::Node, Ptr<Node>>;

		std::stack<NODE> Travers;

		for (auto& node : pSourceScene)
		{
			Travers.push(NODE(node, (Node*)this));
		}

		Ptr<Node> Parent = nullptr;
		while (!Travers.empty())
		{
			NODE node = std::move(Travers.top());
			Travers.pop();

			//Create and Configure the node:
			Node NewNode;
			NewNode.GetName()		  = node.first.GetName();
			NewNode.m_PointerToParent = node.second;
			if (!node.first.IsMeshEmpty())
			{
				NewNode.GetMesh() = MeshInstances.at(node.first.GetMeshIndex());
			}
			NewNode.m_Transform.t_Scale      = node.first.Scale();
			NewNode.m_Transform.t_Rotate     = node.first.Rotation();
			NewNode.m_Transform.t_Translate  = node.first.Translation();

				

			//Dumb shit
			//Push it to the parent node: (starting with scene as parent)
			NewNode.m_Children.reserve(node.first.ChildCount());
			node.second->m_Children.push_back(std::move(NewNode));
			Parent = &node.second->m_Children.at(node.second->m_Children.size() - 1);
			for (auto& Child : node.first)
			{
				Travers.push(NODE(Child, Parent));
			}
		}



		//Create the Pipeline & Descriptors:

		//Storage Buffers:
		m_PrimitiveList = Renderer::Self().InstanceStorageBuffer();
		m_PrimitiveList->CreateBuffer(
			sizeof(MeshData)*m_MeshCount*Renderer::Self().FramesInFlight()
		);

		//Camera Uniform
		m_CameraData = Renderer::Self().CreateUniformBuffer(sizeof(MatF4)*2);

		
		//Pipeline Layout:
		AllocateDescriptors(
			m_MeshCount, 0, 0, 0
		);

		return HYD_OK;
	}

		

	/*
		Purpose: Create a New Light, Add to the Scene's Light Pool & and Return a handle to the light
		Note: Create a Light doesn't Render it in the scene, User must Add the Light to a node 
	*/
	
	HYD HYD_ID_SPACE Scene::NewLight(
		Light pLight
	) noexcept
	{
		return 0;//m_Lights.CreateLight(std::move(pLight));
	}


	void Scene::CreatePiplines(
		GraphicsPipelineConfiguration pPipelineConf
	) noexcept
	{
		//Vertex Buffer Layout Config:
		Internal::Vulkan::VertexAttribute Attrib;
		Attrib.m_BindingDescriptions.push_back(
			{
				.binding   = 0,
				.stride    = sizeof(Vertex),
				.inputRate = VK_VERTEX_INPUT_RATE_VERTEX 
			}
		);

		Attrib.m_AttributeDescriptions.push_back(
			{
				.location = 0,
				.binding  = 0,
				.format   = VK_FORMAT_R32G32B32_SFLOAT,
				.offset   = offsetof(Vertex, Position)
			}
		);

		Attrib.m_AttributeDescriptions.push_back(
		{
				.location = 1,
				.binding  = 0,
				.format   = VK_FORMAT_R32G32B32_SFLOAT,
				.offset   = offsetof(Vertex, Normal)
			}
		);

		Attrib.m_AttributeDescriptions.push_back(
		{
				.location = 2,
				.binding  = 0,
				.format   = VK_FORMAT_R32G32B32A32_SFLOAT,
				.offset   = offsetof(Vertex, Tangent)
			}
		);

		Attrib.m_AttributeDescriptions.push_back(
		{
				.location = 3,
				.binding  = 0,
				.format   = VK_FORMAT_R32G32_SFLOAT,
				.offset   = offsetof(Vertex, TexCoord00)
			}
		);

		Attrib.m_AttributeDescriptions.push_back(
		{
				.location = 4,
				.binding  = 0,
				.format   = VK_FORMAT_R32G32_SFLOAT,
				.offset   = offsetof(Vertex, TexCoord01)
			}
		);

		pPipelineConf.AddVertexBufferLayout(Attrib);
		pPipelineConf.SetPrimitiveTogology();

		pPipelineConf.SetRasterizer(HYD_POLYGON_MODE_FILL, HYD_CULL_MODE_BACK, HYD_FRONT_FACE_COUNTER_CLOCKWISE);
		pPipelineConf.SetDepthStencil(true, true,HYD_COMPARE_OP_GREATER);

		ConfigureDescriptors();		
		pPipelineConf.SetPipelineLayout(m_PipelineLayout);
		m_Pipeline = Renderer::Self().CreatePipeline(pPipelineConf);
	}

	void Scene::ConfigureDescriptors() noexcept
	{
		//Bindless Resources:

		DescriptorSetLayoutConfiguration SceneResourcesDescLayoutConf;

		//Primitive Info
		SceneResourcesDescLayoutConf.AddBinding(
			DescriptorBinding{
				.Binding = 0,
				.Count   = 1,
				.Stage   = HYD_SHADER_STAGE_VERTEX_BIT,
				.Type    = HYD_DESCRIPTOR_TYPE_STORAGE_BUFFER 
			}
		);

		/*
		//Materials
		SceneResourcesDescLayoutConf.AddBinding(
			DescriptorBinding{
				.Binding = 1, 
				.Count   = pMaterialCount,
				.Stage   = HYD_SHADER_STAGE_FRAGMENT_BIT,
				.Type    = HYD_DESCRIPTOR_TYPE_STORAGE_BUFFER 
			}
		);

		//Images Info
		SceneResourcesDescLayoutConf.AddBinding(
			DescriptorBinding{
				.Binding = 2, 
				.Count   = pImageCount,
				.Stage   = HYD_SHADER_STAGE_FRAGMENT_BIT,
				.Type    = HYD_DESCRIPTOR_TYPE_SAMPLED_IMAGE 
			}
		);

		//Samplers Info
		SceneResourcesDescLayoutConf.AddBinding(
			DescriptorBinding{
				.Binding = 3,
				.Count   = pSamplerCount,
				.Stage   = HYD_SHADER_STAGE_FRAGMENT_BIT,
				.Type    = HYD_DESCRIPTOR_TYPE_SAMPLER  
			}
		);
		*/
		m_SceneDataDescLayoutID  = Renderer::Self().CreateDescriptorSetLayout(SceneResourcesDescLayoutConf);

		//Camera Descriptor Set Layout:

		DescriptorSetLayoutConfiguration CameraDescLayoutConf;
		CameraDescLayoutConf.AddBinding(
			DescriptorBinding{
				.Binding = 0,
				.Count   = 1,
				.Stage   = HYD_SHADER_STAGE_VERTEX_BIT,
				.Type    = HYD_DESCRIPTOR_TYPE_UNIFORM_BUFFER 
			}
		);

		m_CameraDataDescLayoutID = Renderer::Self().CreateDescriptorSetLayout(CameraDescLayoutConf);


		m_SceneDataDescSetID  = Renderer::Self().AllocateDescriptorSet(m_SceneDataDescLayoutID);
		m_CameraDataDescSetID = Renderer::Self().AllocateDescriptorSet(m_CameraDataDescLayoutID);

		PipelineLayoutConfiguration LayoutConf;
		LayoutConf.AttachDescriptorLayout(m_SceneDataDescLayoutID);
		LayoutConf.AttachDescriptorLayout(m_CameraDataDescLayoutID);

		LayoutConf.SetPushConstant(0, sizeof(uint32)*2, HYD_SHADER_STAGE_VERTEX_BIT);

		m_PipelineLayout = Renderer::Self().CreatePipelineLayout(LayoutConf);
	}


	void Scene::AllocateDescriptors(
		uint32 pPrimitiveCount,
		uint32 pMaterialCount,
		uint32 pImageCount,
		uint32 pSamplerCount
	) noexcept
	{

		const uint32 Frames = Renderer::Self().FramesInFlight();

		//Bindless
		for (uint32 frame = 0 ; frame < Frames ; ++frame)
		{
			auto& Descriptor = Renderer::Self().AccessDescriptorSets(m_SceneDataDescSetID);
			
			Descriptor.at(frame).AttachStorageBuffer(
				0, *m_PrimitiveList, sizeof(MeshData)*pPrimitiveCount, frame*(sizeof(MeshData)*pPrimitiveCount)  
			);

			Descriptor.at(frame).UpdateDescriptorSet();
		}

		//Camera Data:
		for (uint32 frame = 0 ; frame < Frames ; ++frame)
		{
			auto& Descriptor = Renderer::Self().AccessDescriptorSets(m_CameraDataDescSetID);
			
			Descriptor.at(frame).AttachUniformBuffer(
				0, (*m_CameraData).at(frame) 
			);

			Descriptor.at(frame).UpdateDescriptorSet();
		}
	}


};