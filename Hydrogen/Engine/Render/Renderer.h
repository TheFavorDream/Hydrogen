/*
	Main Renderer of Hydrogen.
*/

#pragma once


#include "../Common.h"
#include "../Log/Log.h"
#include "Material/Material.h"
#include "Material/Texture.h"
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "Vulkan/Image.h"
#include "Window/Window.h"
#include "../Geometry/Mesh.h"
#include "../Geometry/Primitive.h"
#include "Core/Core.h"
#include "../InternalEntities/Light.h"
#include "../Camera/Camera.h"
#include "UI/UI.h"
#include "Vulkan/Swapchain.h"
#include "Vulkan/Renderpass.h"
#include "Vulkan/FrameBuffer.h"
#include "Vulkan/Shader.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/Commands.h"
#include "Vulkan/Buffer.h"
#include "Vulkan/UniformBuffer.h"
#include "Vulkan/VertexAttribute.h"
#include "Vulkan/Descriptors.h"
#include "Vulkan/Device.h"


namespace Hydrogen
{



	struct Instruction
	{
		VertexBufferRef       Vertices;
		IndexBufferRef        Indices;
		GraphicsPipelineRef   Pipeline;
		UniformRef			  Uniform;
		Ptr<Material>		  MaterialPtr;
		Ptr<LightCollection>  Light = nullptr;

		 Instruction() = default;
		~Instruction() = default; 
	
		Instruction(const Instruction& ) 			= default;
		Instruction& operator=(const Instruction& ) = default;


		Instruction(Instruction&& pOther)
			: Vertices(std::move(pOther.Vertices)),
		  	  Indices(std::move(pOther.Indices)),
		  	  Pipeline(std::move(pOther.Pipeline)),
		  	  Uniform(std::move(pOther.Uniform)),
		  	  MaterialPtr(pOther.MaterialPtr),
			  Light(pOther.Light)
			{}
		
	};


	struct FrameRenderConfig
	{
	public:
	
		void PushInstruction(
			Instruction 			 pInstruction
		) noexcept;

		void PushInstruction(
			std::vector<Instruction> pInstruction
		) noexcept;


		void Reset() noexcept;

	private:
		std::vector<Instruction> m_Instructions;
	private:
		friend class Renderer;
	};


	class Renderer final
	{
	public:
		HYD static Renderer& Self();
	
	public:

		HYD inline uint32 FramesInFlight() const {return m_FramesInFlights;}

//----------------------------Pushes a Render Instruction to the Instruction Queue-------------------------------
		HYD void PushInstruction(Instruction pIns) noexcept;

//--------------------------Sets the Engines Current Camera-----------------------------------
		HYD void SetCamera(Ptr<Camera> pCamera) noexcept;
			
//---------------------------------------Creates New Vertex & Index Buffers---------------------------------
		HYD Instance<Internal::Vulkan::VertexBuffer>     InstanceVertexBuffer() noexcept;
		HYD Instance<Internal::Vulkan::IndexBuffer>      InstanceIndexBuffer()  noexcept;
		
//-------------------------Creates a Graphics Pipeline Object------------------------------------------
		HYD Instance<Internal::Vulkan::GraphicsPipeline> CreatePipeline(
			const GraphicsPipelineConfiguration& pConf
		) noexcept;

//--------------------------Create A Shader Module---------------------------------------
		HYD Shader  CreateShader(
			const ShaderConfiguration& pConf
		) noexcept;
	
//-----------------------Initializes the UI Core------------------------------------

		HYD uint32 InitUICore(
			UI::CoreConfiguration pConf = UI::CoreConfiguration{} 
		) noexcept;

//----------------------Accessor Methods----------------------------------------
		HYD inline  Internal::Vulkan::Swapchain&     Swapchain()  		{return m_Swapchain;}
		HYD inline  Internal::Vulkan::Renderpass&    RenderPass() 		{return m_RenderPass;}
		HYD inline  Window& 						 GetWindow()  		{return m_Window;}
		HYD inline 	Ptr<Camera>					     GetCurrentCamera() {return m_DefCam;}							 
//--------------------------------------Creates a Pipeline Layout---------------------------------------------
		HYD_ID_SPACE CreatePipelineLayout(
			PipelineLayoutConfiguration pConf
		) noexcept;
	
		Internal::Vulkan::PipelineLayout& AccessPipelineLayout(
			HYD_ID_SPACE pID
		) noexcept;
	
//---------------------------------Creates a Descriptor Set Layout-------------------------------------------
		HYD_ID_SPACE CreateDescriptorSetLayout(
			DescriptorSetLayoutConfiguration pConf
		) noexcept;
	
		Internal::Vulkan::DescriptorSetLayout& AccessDescriptorSetLayout(
			HYD_ID_SPACE pID
		) noexcept;
	
//--------------------------------Descriptor Pool Creation---------------------------------------------------:
		HYD HYD_ID_SPACE CreateDescriptorPool(
			std::vector<DescriptorPoolSize>   pPoolSizes,
			uint32							  pMaxSets   = UINT32_MAX, //if pMaxSets = UINT32_MAX, the Max size will be calculated by the engine
			uint32							  pFlags     = 0	
		) noexcept;
		
		Internal::Vulkan::DescriptorPool& AccessDescriptorPool(
			HYD_ID_SPACE pPoolID
		) noexcept;

//-------------------------------------Descriptor Set Allocation-----------------------------------------------
		HYD_ID_SPACE AllocateDescriptorSet(
			HYD_ID_SPACE pSetLayoutID
		) noexcept;
		
		Internal::Vulkan::DescriptorSet& AccessDescriptorSet(
			HYD_ID_SPACE pID
		) noexcept;
	
//------------------------------------------Sampler Creation--------------------------------------------------

		HYD_ID_SPACE CreateSampler(
			SamplerConfiguration pConf
		) noexcept;
	
		Internal::Vulkan::Sampler& AccessSampler(
			HYD_ID_SPACE pID
		) noexcept; 
	
//-------------------------------------------Texture Creation---------------------------------------------------

		HYD std::vector<Instance<Texture2D>> CreateTextures(
			std::vector<TextureConfiguration>& pConfs
		) noexcept;
	
		HYD Instance<Texture2D> CreateTexture(
			TextureConfiguration&& pConf
		) noexcept;

//-----------------------------------------Shader Uniform Buffer-----------------------------------------------	
		HYD UniformRef CreateUniformBuffer(
			uint64 		 			pSize,
			ShaderUniformBinding    pBinding,
			HYD_ID_SPACE 		    pUniformBufferSetID
		) noexcept;

		HYD Internal::Vulkan::UniformBuffer& AccessUniformBuffer(
			UniformRef pUniformBuffer
		) noexcept;



	private: //friend only access

		//Init and shutdown
		uint32 Init(WindowInfo pWindow=WindowInfo()) noexcept;
		uint32 Shutdown() noexcept;


		//Main Rendering Happens here 
		void Render(
			const std::vector<FrameRenderConfig>& pConfs = {}
		) noexcept;

		//Takes care of Resizing
		uint32 RecreateSwapchain() noexcept;

		//Engine Internal Accessors:
		inline const Internal::Vulkan::Queues&   GetQueues()  const { return m_Device.m_Queues; }
		inline const VkDevice 	 		   	     GetDevice()  const { return m_Device.m_Handle; }

		inline  Internal::Vulkan::CommandBuffer& GlobalRenderCommandBuffer()   {return m_RenderCommandBuffers[m_FrameIndex];};
		inline  Internal::Vulkan::CommandBuffer& GlobalTransferCommandBuffer() {return m_TransferCommandBuffer;}

		//Submits work to be executed in a queue
		uint32 ExecuteCommandBuffers(
			VkQueue 										 pQueue,
			const std::vector<VkCommandBuffer>&  	    	 pCommandBuffers,
			std::optional<std::vector<VkPipelineStageFlags>> pWaitStages       = std::optional<std::vector<VkPipelineStageFlags>>(),
			std::optional<std::vector<VkSemaphore>> 	     pWaitSemaphores   = std::optional<std::vector<VkSemaphore>>(), 
			std::optional<std::vector<VkSemaphore>> 		 pSignalSemaphores = std::optional<std::vector<VkSemaphore>>(),
			VkFence 								       	 pSignalFence      = VK_NULL_HANDLE
		) noexcept;

		void WaitOnDeviceCompletion() noexcept;

	private: //Private members:


		uint32 CreateSyncObjects() noexcept;
		uint32 CreateRenderPass()  noexcept;

		uint32 InitVulkan() noexcept;

	private:
		std::vector<const char*> m_DeviceLevelExtensions        = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };


		const uint32 m_FramesInFlights = 2; //by default
		uint32       m_FrameIndex      = 0;

		
		Internal::Vulkan::Instance					   m_Instance;
		Internal::Vulkan::Device 					   m_Device;
		Internal::Vulkan::Swapchain		         	   m_Swapchain;
		Internal::Vulkan::Renderpass               	   m_RenderPass;
		Internal::Vulkan::CommandPool 			       m_RenderCommandPool;
		Internal::Vulkan::CommandPool 			       m_TransferCommandPool;
		std::vector<Internal::Vulkan::CommandBuffer>   m_RenderCommandBuffers;
		Internal::Vulkan::CommandBuffer		     	   m_TransferCommandBuffer;
		std::vector<Internal::Vulkan::FrameBuffer> 	   m_FrameBuffers;
		

		//synchronization Objects:
		std::vector<VkSemaphore> m_ImageAvailableSemaphors;
		std::vector<VkSemaphore> m_RenderSignalSemaphors;
		std::vector<VkFence> 	 m_FrameFinishSignals;
		


		HYD_ID_SPACE m_PipelineLayoutsIDGen = 1;
		HYD_ID_SPACE m_DescSetLayoutsIDGen  = 1;
		HYD_ID_SPACE m_DescriptorSetIDGen   = 1;
		HYD_ID_SPACE m_DescriptorPoolIDGen  = 0;
		HYD_ID_SPACE m_SamplerIDGen 	    = 1;

		std::unordered_map<HYD_ID_SPACE, Internal::Vulkan::DescriptorPool>   		   m_DescriptorPools;
		std::unordered_map<HYD_ID_SPACE, Internal::Vulkan::PipelineLayout>             m_PipelineLayouts;
		std::unordered_map<HYD_ID_SPACE, Internal::Vulkan::DescriptorSetLayout>        m_DescSetLayouts;
		std::unordered_map<HYD_ID_SPACE, std::vector<Internal::Vulkan::DescriptorSet>> m_DescriptorSets;
		std::unordered_map<HYD_ID_SPACE, Internal::Vulkan::Sampler> 				   m_Samplers;


		ResourcePool<Texture2D>              			   			   m_Textures;
		ResourcePool<Internal::Vulkan::GraphicsPipeline>   			   m_Pipelines;
		ResourcePool<Internal::Vulkan::VertexBuffer> 	   			   m_VertexBuffers;
		ResourcePool<Internal::Vulkan::IndexBuffer>  	   			   m_IndexBuffers;
		ResourcePool<std::vector<Internal::Vulkan::UniformBuffer>>	   m_UniformBuffers;

		Attachment DepthAttachment;

		Instance<Scene> m_RenderedScene;

		Window 		m_Window;
		Ptr<Camera> m_DefCam = nullptr;

		Camera m_DefaultCamera;

	private:
		static Ptr<Renderer> s_Self;

	private: //Friend Decl
		friend class Scene;
		friend class Core;
		friend class Window;

		friend class Internal::Vulkan::FrameBuffer;
		friend class Internal::Vulkan::Shader;
		friend class Internal::Vulkan::Renderpass;
		friend class Internal::Vulkan::Swapchain;
		friend class Internal::Vulkan::CommandBuffer;
		friend class Internal::Vulkan::CommandPool;
		friend class Internal::Vulkan::GraphicsPipeline;
		friend class Internal::Vulkan::PipelineLayout;
		friend class Internal::Vulkan::BasicBuffer;
		friend class Internal::Vulkan::VertexBuffer;
		friend class Internal::Vulkan::StagingBuffer;
		friend class Internal::Vulkan::IndexBuffer;
		friend class Internal::Vulkan::UniformBuffer;
		friend class Internal::Vulkan::DescriptorPool;
		friend class Internal::Vulkan::DescriptorSet; 
		friend class Internal::Vulkan::DescriptorSetLayout;
		friend class Internal::Vulkan::Image;
		friend class Internal::Vulkan::ImageView;
		friend class Internal::Vulkan::Sampler;
		friend class LightCollection; 
		friend class UI::Core;
		//Temp:
		friend class Material;
	};


};