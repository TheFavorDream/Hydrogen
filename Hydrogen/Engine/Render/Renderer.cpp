#include "Attachment.h"
#include "Vulkan/VkEnumReDefs.h"
#include <GLFW/glfw3.h>
#define RENDERER_H
#include "Renderer.h"
#include "../Core/Core.h"
#include "HydPch.h"


namespace Hydrogen
{
	Ptr<Renderer> Renderer::s_Self = nullptr;
	Renderer& Renderer::Self() { return *(Renderer::s_Self); }


	uint32 Renderer::Init(WindowInfo pWindow) noexcept

	{
		Log::SetInfo("Renderer:Init");


		if (m_Window.MakeWindow(pWindow.Width, pWindow.Height, pWindow.Title) != HYD_OK)
		{
			Log::SetError("Unable to Create Window");
			return HYD_FAILED;
		}


		CHECK_ERROR(InitVulkan());


		m_DefaultCamera.SetupCamera(60.0f, glm::vec3(0.0f), 2.0f, 0.001f, 10000.0f);

		return HYD_OK;
	}

	uint32 Renderer::Shutdown() noexcept
	{

		

		vkDeviceWaitIdle(m_Device.m_Handle);

		Log::SetInfo("Shutting Down Renderer");
		
		
		for (auto& Cbuffer : m_RenderCommandBuffers)
			Cbuffer.ResetCommandBuffer();


		for (auto& framebuffer : m_FrameBuffers)
			framebuffer.DestroyFrameBuffer();


		for (auto& descSetPool : m_DescriptorPools)
			descSetPool.DestroyDescriptorPool();

		for (auto& descSetLayout : m_DescSetLayouts)
			descSetLayout.second.DestroyDescriptorSetLayout();

		for (auto& pipelineLayout : m_PipelineLayouts)
			pipelineLayout.second.DestroyLayout();



		m_DescriptorSets.clear();
		m_DescriptorPools.clear();
		m_DescSetLayouts.clear();
		m_PipelineLayouts.clear();


		for (auto& vbo : m_VertexBuffers)
			vbo.Object.DestroyBuffer();
		for (auto& ebo : m_IndexBuffers)
			ebo.Object.DestroyBuffer();
		for (auto& tex : m_Textures)
			tex.Object.DestroyTexture();
		for (auto& pipeline : m_Pipelines)
			pipeline.Object.DestroyPipeline();

		for (auto& Uniforms : m_UniformBuffers)
		{
			for(auto& Uniform : Uniforms.Object)
				Uniform.DestroyBuffer();
		}

		DepthAttachment.DestroyAttachment();
		NormalAttachment.DestroyAttachment();


		CHECK_ERROR(m_UniformBuffers.Shutdown());

		CHECK_ERROR(m_VertexBuffers.Shutdown());
		CHECK_ERROR(m_IndexBuffers.Shutdown());

		CHECK_ERROR(m_Pipelines.Shutdown());
		CHECK_ERROR(m_Textures.Shutdown());

		CHECK_ERROR(m_TransferCommandPool.DestroyPool());
		CHECK_ERROR(m_RenderCommandPool.DestroyPool());


		m_RenderPass.DestroyRenderPass();
		m_Swapchain.DestroySwapchain();

		for (auto& semaphore : m_RenderSignalSemaphors)
			vkDestroySemaphore(m_Device.m_Handle, semaphore, VULKAN_ALLOCATION_CALLBACK);
		
		for (uint32 I = 0 ; I < m_FramesInFlights ; ++I)
		{
			vkDestroySemaphore(m_Device.m_Handle, m_ImageAvailableSemaphors[I], VULKAN_ALLOCATION_CALLBACK);
			vkDestroyFence(m_Device.m_Handle,     m_FrameFinishSignals[I],      VULKAN_ALLOCATION_CALLBACK);
		}

		m_Device.DestroyDevice();
		m_Window.DestroyWindow(m_Instance.GetInstance());
		m_Instance.DestroyInstance();
		
		return HYD_OK;
	}



/*
	Rendering Related:
*/


	void Renderer::PushInstruction(
		Instruction pIns
	) noexcept
	{
		m_InstructionQueue.push(pIns);
	}


	uint32 Renderer::Render()
	{


		//Wait for previous frame to finish:
		vkWaitForFences(m_Device.m_Handle, 1, &m_FrameFinishSignals[m_FrameIndex], VK_TRUE, UINT64_MAX);

		//Reset the Fence for the next frame:
		vkResetFences(m_Device.m_Handle, 1, &m_FrameFinishSignals[m_FrameIndex]);
		
		
		//Retrive an image from the swapchain:
		uint32 ImageIndex = m_Swapchain.AcquireImage(m_ImageAvailableSemaphors[m_FrameIndex]);
		
		GlobalRenderCommandBuffer().ResetCommandBuffer();
		GlobalRenderCommandBuffer().RecordCommandBuffer();
		
		m_RenderPass.BeginRenderPass(
			m_FrameBuffers[ImageIndex],
			VkRect2D{VkOffset2D{0, 0}, m_Swapchain.GetImageExtent()},
			VecF4(0.02f, 0.02f, 0.02, 1.0f)
		);
		
		
		m_Window.UpdateViewport();		
		
		while (!m_InstructionQueue.empty())
		{
			
			//Retrive the instruction:
			Instruction instruction = m_InstructionQueue.front();
			m_InstructionQueue.pop();
			
			//Bind 
			instruction.Pipeline->BindPipeline();
			instruction.Vertices->Bind();
			instruction.Indices->Bind();
			
			
			Internal::Vulkan::UniformBuffer& ubo = instruction.Uniform->at(m_FrameIndex);
			
			ubo.UploadData(instruction.ModelMatrix.GetPointer(),sizeof(MatF4));
			ubo.UploadData(instruction.CameraPtr->GetViewPtr(),sizeof(glm::mat4),sizeof(MatF4));
			ubo.UploadData(instruction.CameraPtr->GetProjectionPtr(),sizeof(glm::mat4),2*sizeof(MatF4));
			ubo.Bind(
				AccessPipelineLayout(instruction.Pipeline->m_PipelineLayout)
			);
	
			
	      	instruction.MaterialPtr->Bind(
				AccessPipelineLayout(instruction.Pipeline->m_PipelineLayout)
			);

			uint32 IndexCount = instruction.Indices->GetCount();
		 	//Issue a draw call
		 	vkCmdDrawIndexed(
		 		GlobalRenderCommandBuffer().GetHandle(),
				IndexCount, 1,0, 0, 0
			);

		}

		m_RenderPass.EndRenderPass();
		GlobalRenderCommandBuffer().EndRecordingCommandBuffer();



		ExecuteCommandBuffers(
			m_Device.m_Queues.Graphics,
			{m_RenderCommandBuffers[m_FrameIndex].GetHandle()},
			std::vector<VkPipelineStageFlags>{VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
			std::vector<VkSemaphore>{m_ImageAvailableSemaphors[m_FrameIndex]},
			std::vector<VkSemaphore>{m_RenderSignalSemaphors[ImageIndex]},
			m_FrameFinishSignals[m_FrameIndex]
		);

		//Present the renderer image
		m_Swapchain.PresentImage(ImageIndex, m_Device.m_Queues.Present, {m_RenderSignalSemaphors[ImageIndex]});


		m_FrameIndex = (m_FrameIndex + 1) % m_FramesInFlights;
		Core::s_Self->m_Running = !m_Window.ShouldWindowClose();
		return HYD_OK;
	}


	uint32 Renderer::RecreateSwapchain() noexcept
	{

		vkDeviceWaitIdle(m_Device.m_Handle);

		for (auto& framebuffer : m_FrameBuffers)
			framebuffer.DestroyFrameBuffer();


		DepthAttachment.DestroyAttachment();
		NormalAttachment.DestroyAttachment();

		m_FrameBuffers.clear();

		Internal::Vulkan::SwapchainRecreateConfiguration ReConf;



		ReConf.NewExtent = m_Swapchain.SelectExtent(
			m_Window.QuarrySurfaceInfo(m_Device.m_PhysicalDevice),
			m_Window
		);

		uint32 Res = m_Swapchain.RecreateSwapchain(
			ReConf
		);


		DepthAttachment.CreateAttachment(
			HYD_ATTACHMENT_TYPE_DEPTH,
			m_Swapchain.GetImageExtent().width,
			m_Swapchain.GetImageExtent().height,
			HYD_SAMPLE_COUNT_1_BIT
		);

		NormalAttachment.CreateAttachment(
			HYD_ATTACHMENT_TYPE_COLOR,
			m_Swapchain.GetImageExtent().width,
			m_Swapchain.GetImageExtent().height,
			HYD_SAMPLE_COUNT_1_BIT
		);

		for (uint32 I = 0 ; I < m_Swapchain.ImageCount() ; ++I)
		{
			Internal::Vulkan::FrameBuffer framebuffer;
			framebuffer.CreateFrameBuffer(
				m_RenderPass,
				{m_Swapchain.GetAttachment(I), &NormalAttachment, &DepthAttachment},
				m_Swapchain.GetImageExtent().width,
				m_Swapchain.GetImageExtent().height
			);
			m_FrameBuffers.push_back(std::move(framebuffer));
		}

		return HYD_OK;
	}




	void Renderer::SetCamera(
		Ptr<Camera> pCamera
	) noexcept
	{
		m_DefCam = pCamera;
	}


	uint32 Renderer::ExecuteCommandBuffers(
		VkQueue 										 pQueue,
		const std::vector<VkCommandBuffer>&      	     pCommandBuffers,
		std::optional<std::vector<VkPipelineStageFlags>> pWaitStages,
		std::optional<std::vector<VkSemaphore>> 		 pWaitSemaphores,
		std::optional<std::vector<VkSemaphore>> 		 pSignalSemaphores,
		VkFence 								       	 pSignalFence      //= VK_NULL_HANDLE
	) noexcept
	{

		//Submmiting work to the queue:
		VkSubmitInfo SubmitInfo{};
		SubmitInfo.sType 			    = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		SubmitInfo.pNext 			    = nullptr;
		SubmitInfo.commandBufferCount   = pCommandBuffers.size();
		SubmitInfo.pCommandBuffers      = pCommandBuffers.data();

		if (pWaitSemaphores.has_value())
		{
			SubmitInfo.waitSemaphoreCount   = pWaitSemaphores->size();
			SubmitInfo.pWaitSemaphores      = pWaitSemaphores->data();
		}

		if (pWaitStages.has_value())
			SubmitInfo.pWaitDstStageMask    = pWaitStages->data();
		
		if (pSignalSemaphores.has_value())
		{
			SubmitInfo.signalSemaphoreCount = pSignalSemaphores->size();
			SubmitInfo.pSignalSemaphores    = pSignalSemaphores->data();
		}

		if (vkQueueSubmit(pQueue, 1, &SubmitInfo, pSignalFence) != VK_SUCCESS)
		{
			Log::SetError("Unable to summit work to queue");
			return HYD_FAILED;
		}
		return HYD_OK;
	}
	


	Instance<Internal::Vulkan::VertexBuffer> Renderer::InstanceVertexBuffer() noexcept
	{
		Instance<Internal::Vulkan::VertexBuffer> instance = m_VertexBuffers.Resource();
		return std::move(instance);
	}

	Instance<Internal::Vulkan::IndexBuffer>  Renderer::InstanceIndexBuffer()  noexcept
	{
		Instance<Internal::Vulkan::IndexBuffer> instance = m_IndexBuffers.Resource();
		return std::move(instance);
	}

	Instance<Internal::Vulkan::GraphicsPipeline> Renderer::CreatePipeline(
		const GraphicsPipelineConfiguration& pConf
	) noexcept
	{
		Instance<Internal::Vulkan::GraphicsPipeline> Pipeline = m_Pipelines.Resource();
		Pipeline.GetPtr()->CreatePipeline(pConf);
		return Pipeline;
	}

	Shader  Renderer::CreateShader(
		const ShaderConfiguration& pConf
	) noexcept
	{
		Internal::Vulkan::Shader Shader;
		Shader.CreateShader(pConf);
		return Shader;
	}


	uint32 Renderer::CreateSyncObjects() noexcept
	{


		
		m_ImageAvailableSemaphors.resize(m_FramesInFlights);
		m_FrameFinishSignals.resize(m_FramesInFlights);
		m_RenderSignalSemaphors.resize(m_Swapchain.ImageCount());

		VkSemaphoreCreateInfo SemCInfo{};

		SemCInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		SemCInfo.pNext = nullptr;



		for (uint32 I = 0 ; I < m_Swapchain.ImageCount() ; ++I)
			vkCreateSemaphore(m_Device.m_Handle, &SemCInfo, VULKAN_ALLOCATION_CALLBACK, &m_RenderSignalSemaphors[I]);
		
		for (uint32 I = 0 ; I < m_FramesInFlights ; ++I)
			vkCreateSemaphore(m_Device.m_Handle, &SemCInfo, VULKAN_ALLOCATION_CALLBACK, &m_ImageAvailableSemaphors[I]);
		
		
		
		
		VkFenceCreateInfo FenceCInfo{};
		FenceCInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceCInfo.pNext = nullptr;
		FenceCInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (uint32 I = 0 ; I < m_FramesInFlights ; ++I)
			vkCreateFence(m_Device.m_Handle, &FenceCInfo, VULKAN_ALLOCATION_CALLBACK, &m_FrameFinishSignals[I]);


		return HYD_OK;
	}

	uint32 Renderer::CreateRenderPass() noexcept
	{
		//Attachment configurations:
		Hydrogen::RenderpassAttachment   ColorOut;
		ColorOut.Format      = Renderer::Self().Swapchain().GetImageFormat();
		ColorOut.InitLayout  = Hydrogen::HYD_IMAGE_LAYOUT_UNDEFINED;
		ColorOut.FinalLayout = Hydrogen::HYD_IMAGE_LAYOUT_PRESENT_SRC_KHR; 
		ColorOut.LoadOp      = Hydrogen::HYD_ATTACHMENT_LOAD_OP_CLEAR;
		ColorOut.StoreOp     = Hydrogen::HYD_ATTACHMENT_STORE_OP_STORE;
		ColorOut.SampleCount = Hydrogen::HYD_SAMPLE_COUNT_1_BIT;


		Hydrogen::RenderpassAttachment   NormalOut;
		NormalOut.Format      = NormalAttachment.GetAttachmentFormat();
		NormalOut.SampleCount = NormalAttachment.GetAttachmentSampleCount();
		NormalOut.InitLayout  = Hydrogen::HYD_IMAGE_LAYOUT_UNDEFINED;
		NormalOut.FinalLayout = Hydrogen::HYD_IMAGE_LAYOUT_PRESENT_SRC_KHR; 
		NormalOut.LoadOp      = Hydrogen::HYD_ATTACHMENT_LOAD_OP_CLEAR;
		NormalOut.StoreOp     = Hydrogen::HYD_ATTACHMENT_STORE_OP_STORE;

		Hydrogen::RenderpassAttachment DepthOut;
		DepthOut.Format      = DepthAttachment.GetAttachmentFormat();
		DepthOut.SampleCount = DepthAttachment.GetAttachmentSampleCount();
		DepthOut.InitLayout  = Hydrogen::HYD_IMAGE_LAYOUT_UNDEFINED;
		DepthOut.FinalLayout = Hydrogen::HYD_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; 
		DepthOut.LoadOp      = Hydrogen::HYD_ATTACHMENT_LOAD_OP_CLEAR;
		DepthOut.StoreOp     = Hydrogen::HYD_ATTACHMENT_STORE_OP_STORE;

		//Subpass Configurations:
		Hydrogen::SubpassConfiguration SubpassConf;
		SubpassConf.AddColorAttachment({.Index=0, .Layout=HYD_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}); 
		SubpassConf.AddColorAttachment({.Index=1, .Layout=HYD_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}); 
		SubpassConf.AddDepthStencilAttachment({.Index=2, .Layout=HYD_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL});
		

		//Subpass Dependencies Configurations:
		Hydrogen::SubpassDependencyConfiguration DependencyConf;
		DependencyConf.SourceSubpass      = UINT32_MAX; //Extenal source
		DependencyConf.DestinationSubpass = 0;
		DependencyConf.SrcStageMask       = Hydrogen::HYD_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		DependencyConf.SrcAccessMask 	  = 0;
		DependencyConf.DstStageMask       = Hydrogen::HYD_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;   
		DependencyConf.DstAccessMask      = Hydrogen::HYD_ACCESS_COLOR_ATTACHMENT_WRITE;    

		Hydrogen::SubpassDependencyConfiguration DepthDependency;

		DepthDependency.SourceSubpass      = UINT32_MAX;
		DepthDependency.DestinationSubpass = 0;
		DepthDependency.SrcStageMask       = HYD_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS | HYD_PIPELINE_STAGE_LATE_FRAGMENT_TESTS;
		DepthDependency.SrcAccessMask      = 0;
		DepthDependency.DstStageMask       = HYD_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS | HYD_PIPELINE_STAGE_LATE_FRAGMENT_TESTS;
		DepthDependency.DstAccessMask      = HYD_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE;

		Hydrogen::RenderpassConfiguration RenderPassConf;
		RenderPassConf.AddAttachment(ColorOut);
		RenderPassConf.AddAttachment(NormalOut);
		RenderPassConf.AddAttachment(DepthOut);
		RenderPassConf.AddSubpass(SubpassConf);
		RenderPassConf.AddDependency(DependencyConf);
		RenderPassConf.AddDependency(DepthDependency);
		return m_RenderPass.CreateRenderPass(RenderPassConf);
	}




	uint32 Renderer::InitVulkan() noexcept
	{

		//Vulkan Init


		VkApplicationInfo AppInfo{
			.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext            = nullptr,
			.pApplicationName = m_Window.GetTitle(),
			.pEngineName      = "Hydrogen",
			.engineVersion    = VK_MAKE_VERSION(HYD_VERSION_MAJOR, HYD_VERSION_MINOR, HYD_VERSION_PATCH),
			.apiVersion       = VK_MAKE_VERSION(1, 0, 0),
		};

		uint32 ReqCount   = 0;
		const char** exts = glfwGetRequiredInstanceExtensions(&ReqCount);
	
		for (uint32 Iter = 0 ; Iter < ReqCount; ++Iter)
			m_Instance.PushExtension(exts[Iter]);

		CHECK_ERROR(m_Instance.CreateVkInstance(AppInfo, true));

	
	
		//Create Rendering Surface
		m_Window.CreateVulkanSurface(m_Instance.GetInstance());
		
		//Creating Logical device and queues
		CHECK_ERROR(m_Device.CreateDevice(
			m_Instance.GetInstance(),
			m_DeviceLevelExtensions
		));


		//Swapchain creation:

		Internal::Vulkan::SwapchainConfiguration SwapchainConf;

		SwapchainConf.Surface             = m_Window.GetSurface();
		SwapchainConf.SurfaceCapabilities = m_Window.QuarrySurfaceInfo(m_Device.m_PhysicalDevice);
		SwapchainConf.ImageSize           = m_Swapchain.SelectExtent(SwapchainConf.SurfaceCapabilities, m_Window);
		SwapchainConf.ImageCount          = 3;
		SwapchainConf.DesiredFormats      = {VK_FORMAT_R8G8B8A8_SRGB};
		SwapchainConf.DesiredPresentMode  = {VK_PRESENT_MODE_FIFO_KHR};

		CHECK_ERROR(m_Swapchain.CreateSwapchain(
		SwapchainConf
		));

		//Creating the Depth Buffer:

		DepthAttachment.CreateAttachment(
			HYD_ATTACHMENT_TYPE_DEPTH,
			SwapchainConf.ImageSize.width,
			SwapchainConf.ImageSize.height,
			HYD_SAMPLE_COUNT_1_BIT
		);

		NormalAttachment.CreateAttachment(
			HYD_ATTACHMENT_TYPE_COLOR,
			SwapchainConf.ImageSize.width,
			SwapchainConf.ImageSize.height,
			HYD_SAMPLE_COUNT_1_BIT
		);

		//Renderpass:
		CHECK_ERROR(CreateRenderPass());


		for (uint32 I = 0 ; I < m_Swapchain.ImageCount() ; ++I)
		{
			Internal::Vulkan::FrameBuffer framebuffer;
			framebuffer.CreateFrameBuffer(
				m_RenderPass,
				{m_Swapchain.GetAttachment(I),&NormalAttachment, &DepthAttachment},
				m_Swapchain.GetImageExtent().width,
				m_Swapchain.GetImageExtent().height
			);
			m_FrameBuffers.push_back(std::move(framebuffer));
		}




		//Command Buffer Creation:
		CHECK_ERROR(m_RenderCommandPool.CreatePool(m_Device.m_QueueFamily.Graphics.value(),
		 VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
		);

		m_RenderCommandBuffers.resize(m_FramesInFlights);
		for (uint32 I = 0 ; I < m_FramesInFlights ; I++)
		{
			m_RenderCommandBuffers[I] = m_RenderCommandPool.AllocateCommandBuffer(); 
		}


		CHECK_ERROR(
				m_TransferCommandPool.CreatePool(m_Device.m_QueueFamily.Transfer.value(), 
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT)
		);

		m_TransferCommandBuffer = m_TransferCommandPool.AllocateCommandBuffer();

		//Create synchronization objects:
		CHECK_ERROR(CreateSyncObjects());


		return HYD_OK;
	}



	
	//Creates a Pipeline Layout and returns the handle
	HYD_ID_SPACE Renderer::CreatePipelineLayout(
		PipelineLayoutConfiguration pConf
	) noexcept
	{
		Internal::Vulkan::PipelineLayout Layout;
		Layout.CreateLayout(pConf);
		HYD_ID_SPACE Id = m_PipelineLayoutsIDGen++;
		m_PipelineLayouts.emplace(
			Id, std::move(Layout)
		);


		return Id;
	}




	Internal::Vulkan::PipelineLayout& Renderer::AccessPipelineLayout(
		HYD_ID_SPACE pID
	) noexcept
	{
		ASSERT((m_PipelineLayouts.find(pID) != m_PipelineLayouts.end()), "Requested Pipeline Layout does not exist");
		return m_PipelineLayouts.at(pID);
	}

	//Creates a Descriptor Set Layout and returns the handle
	HYD_ID_SPACE Renderer::CreateDescriptorSetLayout(
		DescriptorSetLayoutConfiguration pConf
	) noexcept
	{
		Internal::Vulkan::DescriptorSetLayout Layout;
		Layout.CreateDescriptorSetLayout(pConf);
		HYD_ID_SPACE Id = m_DescSetLayoutsIDGen++; 
		m_DescSetLayouts.emplace(Id, std::move(Layout));
		return Id;
	}

	Internal::Vulkan::DescriptorSetLayout& Renderer::AccessDescriptorSetLayout(
		HYD_ID_SPACE pID
	) noexcept
	{
		ASSERT((m_DescSetLayouts.find(pID) != m_DescSetLayouts.end()), "Requested DescSet Layout does not exist");
		return m_DescSetLayouts.at(pID);
	}


	HYD_ID_SPACE Renderer::AllocateDescriptorSet(
		HYD_ID_SPACE pSetLayoutID
	) noexcept
	{
		Internal::Vulkan::DescriptorSetLayout& SetLayout = AccessDescriptorSetLayout(pSetLayoutID);

		//Create DescriptorPool 

		std::vector<VkDescriptorPoolSize> PoolSizes;
		for (auto& binding : SetLayout.m_Bindings)
		{
			PoolSizes.push_back(
				VkDescriptorPoolSize{
					.type 			 = binding.descriptorType,
					.descriptorCount = m_FramesInFlights
				}
			);
		}

		Internal::Vulkan::DescriptorPool Pool;
		Pool.CreateDescriptorPool(std::move(PoolSizes), m_FramesInFlights*PoolSizes.size());
		HYD_ID_SPACE PoolID = m_DescriptorPoolIDGen++;

		//Allocate Descriptor Set
		std::vector<Internal::Vulkan::DescriptorSet> Sets; Sets.resize(m_FramesInFlights);
		for (uint32 Iter = 0 ; Iter < m_FramesInFlights ; ++Iter)
		{
			Sets.at(Iter) 			  = Pool.AllocateDescriptorSet(SetLayout);
			Sets.at(Iter).m_ParentPool = PoolID;
		}

		m_DescriptorPools.push_back(std::move(Pool));


		HYD_ID_SPACE SetID = m_DescriptorSetIDGen++;
		m_DescriptorSets.emplace(
			SetID,
			std::move(Sets)
		);

		return SetID;
	}

	Internal::Vulkan::DescriptorSet& Renderer::AccessDescriptorSet(
		HYD_ID_SPACE pID
	) noexcept
	{
		ASSERT((m_DescriptorSets.find(pID) != m_DescriptorSets.end()), "Requested Descriptor Set  does not exist");
		return m_DescriptorSets.at(pID).at(m_FrameIndex);
	}


	HYD_ID_SPACE Renderer::CreateSampler(
		SamplerConfiguration pConf
	) noexcept
	{
		HYD_ID_SPACE ID = m_SamplerIDGen; 
		m_Samplers.at(m_SamplerIDGen++).CreateSampler(pConf);
		return ID;
	}

	Internal::Vulkan::Sampler& Renderer::AccessSampler(
		HYD_ID_SPACE pID
	) noexcept
	{
		ASSERT((m_Samplers.find(pID) != m_Samplers.end()), "Requested  Sampler does not exist");
		return m_Samplers.at(pID);
	}


	std::vector<Instance<Texture2D>> Renderer::CreateTextures(
		std::vector<TextureConfiguration>& pConfs
	) noexcept
	{
		std::vector<Instance<Texture2D>> TextureInstances;
		TextureInstances.resize(pConfs.size());

		for (uint32 TexIndex = 0 ; TexIndex < TextureInstances.size() ; ++TexIndex)
		{
			Instance<Texture2D> texture = m_Textures.Resource();
			texture->CreateTexture(std::move(pConfs.at(TexIndex)));
			TextureInstances.push_back(std::move(texture));
		}

		return std::move(TextureInstances);
	}

	Instance<Texture2D> Renderer::CreateTexture(
		TextureConfiguration&& pConf
	) noexcept
	{
		Instance<Texture2D> texture = m_Textures.Resource();
		texture->CreateTexture(std::move(pConf));
		return std::move(texture);
	}

	UniformRef Renderer::CreateUniformBuffer(
		uint64 		 pSize,
		uint32 		 pBinding,
		HYD_ID_SPACE pUniformBufferSetID
	) noexcept
	{
		UniformRef NewUniform = m_UniformBuffers.Resource();
		NewUniform->resize(m_FramesInFlights);


		auto& DescriptorSets = m_DescriptorSets.at(pUniformBufferSetID);

		for (uint32 FrameIndex = 0 ; FrameIndex < m_FramesInFlights ; ++FrameIndex)
		{
			NewUniform->at(FrameIndex).CreateUniformBuffer(pSize);
			NewUniform->at(FrameIndex).SetDescriptorSet(pUniformBufferSetID);

			DescriptorSets.at(FrameIndex).AttachUniformBuffer(
				pBinding, 
				NewUniform->at(FrameIndex)
			);

			DescriptorSets.at(FrameIndex).UpdateDescriptorSet();
		}

		return std::move(NewUniform);
	}


}; // namespace Hydrogen