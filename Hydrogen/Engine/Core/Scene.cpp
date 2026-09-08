#include "Scene.h"
#include "../HydPch.h"
#include "../Render/Renderer.h"
#include <cstddef>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{

	Scene::Scene()
	{
		m_Camera.SetupCamera(60.0f, glm::vec3(0.0f), 2.0f, 0.001f, 10000.0f);

		Renderer::Self().SetCamera(&m_Camera);
	}

	Scene::~Scene()
	{
		FreeScene();
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
		for (auto& node : m_Children)
		{
			node.Destroy();
		}

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


			//Upload Camera Data:
			Renderer::Self().AccessUniformBuffer(m_Uniforms).UploadData(m_Camera.GetViewPtr(),sizeof(glm::mat4),sizeof(MatF4));
			Renderer::Self().AccessUniformBuffer(m_Uniforms).UploadData(m_Camera.GetProjectionPtr(),sizeof(glm::mat4),2*sizeof(MatF4));
			
			if (!Current.GetMesh().IsNull())
				Current.GetMesh()->Render(
					m_Uniforms,
					Current.GetTransform()
				);

			for (auto node : Current)
			{
				node.GetTransform() = Current.GetTransform() * node.GetTransform();
				Travers.push(node);
			}
		
		}

		return HYD_OK;
	}


	uint32 Scene::LoadScene(
		const Xenon::Scene& pSourceScene
	) noexcept
	{


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

		m_PipelineConf.AddVertexBufferLayout(Attrib);
		m_PipelineConf.SetPrimitiveTogology();
		m_Pipeline = Renderer::Self().CreatePipeline(m_PipelineConf);



		std::vector<Instance<Mesh>>		 MeshIns;
		std::vector<Instance<Scene>>     SceneIns;
		std::unordered_map<uint64, Instance<Texture2D>> TexIns;

		//Set the name for the scene:
		m_Name = pSourceScene.GetName();

		//Texture Loading:
		for (auto& texture : pSourceScene.GetTextures())
		{
			TextureConfiguration TextureConfiguration;

			Log::SetInfo("Loading Texture");


			Xenon::BinaryData Img = texture.second.RetriveImageData();
			TextureConfiguration.Data      = Buffer(Img.Ptr, Img.ByteLength);
			Img.Ptr 	   = nullptr;
			Img.ByteLength = 0;

			TextureConfiguration.ImageSampler = SamplerConfiguration{
				.Minification     = FilterMode(Xenon::Sampler::VulkanFilterEnum(texture.second.TextureSample.Min)),
				.Magnification    = FilterMode(Xenon::Sampler::VulkanFilterEnum(texture.second.TextureSample.Mag)),
				.MipMapFilter     = MipMapFilterMode(Xenon::Sampler::VulkanMipmapFilterEnum(texture.second.TextureSample.Min)),
				.WrapU            = WrapMode(Xenon::Sampler::VulkanWrapEnum(texture.second.TextureSample.WrapS)),
				.WrapV            = WrapMode(Xenon::Sampler::VulkanWrapEnum(texture.second.TextureSample.WrapT)),
				.WrapW            = HYD_SAMPLER_WRAP_MODE_CLAMP_TO_EDGE,
				.AnisotropyEnable = false,
				.MaxAnisotropy    = 0,
			};
			
			TextureConfiguration.TexCoordSet = texture.second.TexCoordSet;
			TexIns.emplace(texture.first, Renderer::Self().CreateTexture(std::move(TextureConfiguration)));
		}



		//Mesh Creation:
		for (auto& mesh : pSourceScene.GetMeshes())
		{
			Mesh NewMesh = Mesh::CreateGLTFMesh(mesh, m_Pipeline);
			
			uint64 Index = 0;
			for (auto& pri : mesh)
			{
				NewMesh.GetPrimitve(Index++).m_Material = Material::CreateMaterialGLTF(
					pri.GetMatrial(),
					TexIns,
					m_MaterialLayoutID,
					m_ShaderBinding.Material
				);
			}
			MeshIns.push_back(m_Meshes.PushObject(std::move(NewMesh)));
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
				NewNode.GetMesh() = MeshIns.at(node.first.GetMeshIndex());
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




		return HYD_OK;
	}

	uint32 Scene::ConfigurePipeline(
		const GraphicsPipelineConfiguration& pConf,
		const SceneShaderLayout&			 pShaderLayout
	) noexcept
	{


		std::vector<DescriptorSetLayoutConfiguration> DescriptorLayoutConfs;
		DescriptorLayoutConfs.resize(2);
		

		//MVP DescriptorSet Layout
		DescriptorLayoutConfs.at(0).AddBinding(DescriptorBinding{
			.Binding = pShaderLayout.MVP.Binding,
			.Count   = 1,
			.Stage   = pShaderLayout.MVP.ShaderStage,
			.Type    = pShaderLayout.MVP.Type
		});


		//Material DescriptorSet Layout
		DescriptorLayoutConfs.at(1).AddBinding(DescriptorBinding{
			.Binding = pShaderLayout.Material.BaseColorBinding,
			.Count   = 1,
			.Stage   = pShaderLayout.Material.ShaderStage,
			.Type    = pShaderLayout.Material.Type
		});
		DescriptorLayoutConfs.at(1).AddBinding(DescriptorBinding{
			.Binding = pShaderLayout.Material.NormalMapBinding,
			.Count   = 1,
			.Stage   = pShaderLayout.Material.ShaderStage,
			.Type    = pShaderLayout.Material.Type
		});
		DescriptorLayoutConfs.at(1).AddBinding(DescriptorBinding{
			.Binding = pShaderLayout.Material.MettallicRoughnessBinding,
			.Count   = 1,
			.Stage   = pShaderLayout.Material.ShaderStage,
			.Type    = pShaderLayout.Material.Type
		});
		DescriptorLayoutConfs.at(1).AddBinding(DescriptorBinding{
			.Binding = pShaderLayout.Material.EmissiveBinding,
			.Count   = 1,
			.Stage   = pShaderLayout.Material.ShaderStage,
			.Type    = pShaderLayout.Material.Type
		});
		DescriptorLayoutConfs.at(1).AddBinding(DescriptorBinding{
			.Binding = pShaderLayout.Material.OcolusionBinding,
			.Count   = 1,
			.Stage   = pShaderLayout.Material.ShaderStage,
			.Type    = pShaderLayout.Material.Type
		});
		


		PipelineLayoutConfiguration PipelineLayoutConf;
		

		HYD_ID_SPACE MVPDescLayoutID      = Renderer::Self().CreateDescriptorSetLayout(DescriptorLayoutConfs[0]);
		HYD_ID_SPACE MaterialDescLayoutID = Renderer::Self().CreateDescriptorSetLayout(DescriptorLayoutConfs[1]);


		m_MaterialLayoutID = MaterialDescLayoutID;
		m_MVPLayoutID      = MVPDescLayoutID;

		Internal::Vulkan::DescriptorSetLayout& MVPDescSetLayout      = Renderer::Self().AccessDescriptorSetLayout(MVPDescLayoutID);
		Internal::Vulkan::DescriptorSetLayout& MaterialDescSetLayout = Renderer::Self().AccessDescriptorSetLayout(MaterialDescLayoutID);

		PipelineLayoutConf.AttachDescriptorLayout(
			MVPDescLayoutID
		);

		PipelineLayoutConf.AttachDescriptorLayout(
			MaterialDescLayoutID
		);
		

		m_PipelineLayoutID = Renderer::Self().CreatePipelineLayout(PipelineLayoutConf);
	

		const uint32 FramesInFlight = Renderer::Self().m_FramesInFlights;
		
		//MVP uniform buffers:

		HYD_ID_SPACE UniformBufferSetID = Renderer::Self().AllocateDescriptorSet(MVPDescLayoutID);

		m_Uniforms = Renderer::Self().CreateUniformBuffer(
			sizeof(MatF4)*3,
			pShaderLayout.MVP,
			UniformBufferSetID
		);

		
		m_ShaderBinding = pShaderLayout;
		m_ShaderBinding.Material.DescriptorSetLayoutID = MaterialDescLayoutID;
		
		m_PipelineConf   = pConf;
		m_PipelineConf.SetPipelineLayout(m_PipelineLayoutID);

		
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


	void Node::Destroy() noexcept
	{
		m_Mesh.Reset();
		for (auto& node : m_Children)
			node.Destroy();
	}


};