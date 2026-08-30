#include "Material/Texture.h"
#include "Vulkan/Buffer.h"
#include "Vulkan/Descriptors.h"
#include "Vulkan/Image.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/Swapchain.h"
#include "Vulkan/UniformBuffer.h"
#include "Vulkan/VkEnumReDefs.h"
#include "Window/Window.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <unordered_map>
#include <utility>
#include <vector>
#include <vulkan/vulkan_core.h>
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

		//Vulkan Init
		m_VkInstance = CreateVkInstance();
		if (m_VkInstance == VK_NULL_HANDLE)
		{
			Log::SetError("Unable to Create Vulkan Instance");
			return HYD_FAILED;
		}
		

	

	
	
		//Create Rendering Surface
		m_Window.CreateVulkanSurface(m_VkInstance);
		
		//Creating Logical device and queues
		CHECK_ERROR(m_Device.CreateDevice(
			m_VkInstance,
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
		ImageConfiguration ImageConf{};

		ImageConf.ImageSize     = Vec3<uint32>(SwapchainConf.ImageSize.width, SwapchainConf.ImageSize.height, 1);
		ImageConf.Type     	    = HYD_IMAGE_TYPE_2D;
		ImageConf.Format        = HYD_FORMAT_D32_SFLOAT;
		ImageConf.InitialLayout = HYD_IMAGE_LAYOUT_UNDEFINED;
		ImageConf.MipMapLevels  = 1; 
		ImageConf.SampleCount   = HYD_SAMPLE_COUNT_1_BIT;
		ImageConf.Usage  	    = HYD_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		ImageConf.SharingMode   = QueueSharingMode{
			.SharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
		
		DepthAttachment.CreateImage(ImageConf);

		ImageViewConfiguration ViewConf{};
		ViewConf.Image    = &DepthAttachment;
		ViewConf.ViewType = HYD_IMAGE_VIEW_TYPE_2D;
		ViewConf.Format   = HYD_FORMAT_D32_SFLOAT;
		ViewConf.SubResources.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;// | VK_IMAGE_ASPECT_STENCIL_BIT;

		DepthAttachmentView.CreateImageView(ViewConf);

		for (uint32 I = 0 ; I < m_Swapchain.ImageCount() ; ++I)
		{
			Internal::Vulkan::FrameBuffer framebuffer;
			framebuffer.CreateFrameBuffer(
				m_RenderPass.m_Handle,
				{m_Swapchain.GetImage(I).m_Handle, DepthAttachmentView.m_Handle},
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

	uint32 Renderer::Shutdown() noexcept
	{

		

		vkDeviceWaitIdle(m_Device.m_Handle);

		Log::SetInfo("Shutting Down Renderer");
		
		
		for (auto& Cbuffer : m_RenderCommandBuffers)
			Cbuffer.ResetCommandBuffer();


		for (auto& framebuffer : m_FrameBuffers)
			framebuffer.DestroyFrameBuffer();



		for (auto& descSetLayout : m_DescSetLayouts)
			descSetLayout.second.DestroyDescriptorSetLayout();

		for (auto& pipelineLayout : m_PipelineLayouts)
			pipelineLayout.second.DestroyLayout();


		CHECK_ERROR(m_VertexBuffers.Shutdown());
		CHECK_ERROR(m_IndexBuffers.Shutdown());
		CHECK_ERROR(m_Pipelines.Shutdown());
		//CHECK_ERROR(m_Textures.Shutdown());

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

		m_Window.DestroyWindow(m_VkInstance);
		m_Device.DestroyDevice();
		vkDestroyInstance    (m_VkInstance,				 VULKAN_ALLOCATION_CALLBACK);
		
		return HYD_OK;
	}



/*
	Rendering Related:
*/


	void Renderer::PushPrimitive(Ptr<Primitive> pPrimitive) noexcept
	{
		if (pPrimitive != nullptr)
			m_PrimitiveQueue.push(pPrimitive);
	}


	uint32 Renderer::Render()
	{


		//Wait for previous frame to finish:
		vkWaitForFences(m_Device.m_Handle, 1, &m_FrameFinishSignals[m_FrameIndex], VK_TRUE, UINT64_MAX);

		//Reset the Fence for the next frame:
		vkResetFences(m_Device.m_Handle, 1, &m_FrameFinishSignals[m_FrameIndex]);
		m_RenderCommandBuffers[m_FrameIndex].ResetCommandBuffer();
		

		//Retrive an image from the swapchain:
		uint32 ImageIndex = m_Swapchain.AcquireImage(m_ImageAvailableSemaphors[m_FrameIndex]);


		GlobalRenderCommandBuffer().RecordCommandBuffer();

		m_RenderPass.BeginRenderPass(
			m_FrameBuffers[ImageIndex],
			VkRect2D{VkOffset2D{0, 0}, m_Swapchain.GetImageExtent()},
			VecF4(0.02f, 0.02f, 0.02, 1.0f)
		);
		
		m_Window.UpdateViewport();		

		
		Internal::Vulkan::UniformBuffer& ubo = m_RenderedScene->m_UniformBuffers[m_FrameIndex];
		

		ubo.UploadData(m_DefCam->GetViewPtr(),    sizeof(glm::mat4), sizeof(MatF4));
		ubo.UploadData(m_DefCam->GetProjectionPtr(), sizeof(glm::mat4), 2*sizeof(MatF4));


		while (!m_PrimitiveQueue.empty())
		{
			
			Ptr<Primitive> pri = m_PrimitiveQueue.front();
			m_PrimitiveQueue.pop();

			pri->m_Pipeline->BindPipeline();
			pri->m_VertexBuffer->Bind();
			pri->m_IndexBuffer->Bind();
			

			ubo.UploadData(pri->m_Transform.Transpose().GetPointer(), sizeof(MatF4));
			ubo.Bind(
				AccessPipelineLayout(pri->m_Pipeline->m_PipelineLayout)
			);
	
			
			
			
			
			
	      pri->m_Material.Bind(AccessPipelineLayout(pri->m_Pipeline->m_PipelineLayout));

		 	//Issue a draw call
		 	vkCmdDrawIndexed(
		 		GlobalRenderCommandBuffer().GetHandle(),
				pri->m_IndexBuffer->GetCount(), 1,0, 0, 0);

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


		DepthAttachmentView.DestroyImageView();
		DepthAttachment.DestroyImage();


		m_FrameBuffers.clear();

		Internal::Vulkan::SwapchainRecreateConfiguration ReConf;



		ReConf.NewExtent = m_Swapchain.SelectExtent(
			m_Window.QuarrySurfaceInfo(m_Device.m_PhysicalDevice),
			m_Window
		);

		uint32 Res = m_Swapchain.RecreateSwapchain(
			ReConf
		);


		ImageConfiguration ImageConf{};

		ImageConf.ImageSize     = Vec3<uint32>(ReConf.NewExtent.width, ReConf.NewExtent.height, 1);
		ImageConf.Type     	    = HYD_IMAGE_TYPE_2D;
		ImageConf.Format        = HYD_FORMAT_D32_SFLOAT;
		ImageConf.InitialLayout = HYD_IMAGE_LAYOUT_UNDEFINED;
		ImageConf.MipMapLevels  = 1; 
		ImageConf.SampleCount   = HYD_SAMPLE_COUNT_1_BIT;
		ImageConf.Usage  	    = HYD_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		ImageConf.SharingMode   = QueueSharingMode{
			.SharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
		
		DepthAttachment.CreateImage(ImageConf);

		ImageViewConfiguration ViewConf{};
		ViewConf.Image    = &DepthAttachment;
		ViewConf.ViewType = HYD_IMAGE_VIEW_TYPE_2D;
		ViewConf.Format   = HYD_FORMAT_D32_SFLOAT;
		ViewConf.SubResources.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;// | VK_IMAGE_ASPECT_STENCIL_BIT;
		ViewConf.SubResources.baseMipLevel = 0;
		ViewConf.SubResources.baseArrayLayer = 0;
		ViewConf.SubResources.layerCount = 1;
		ViewConf.SubResources.levelCount = 1;

		DepthAttachmentView.CreateImageView(ViewConf);

		for (uint32 I = 0 ; I < m_Swapchain.ImageCount() ; ++I)
		{
			Internal::Vulkan::FrameBuffer framebuffer;
			framebuffer.CreateFrameBuffer(
				m_RenderPass.m_Handle,
				{m_Swapchain.GetImage(I).m_Handle, DepthAttachmentView.m_Handle},
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

	


	/*
	
		Vulkan Stuff:
	*/

	std::vector<const char*> Renderer::CheckForInstanceExtensions(const std::vector<const char*>& pRequired) noexcept
	{
		
		//Retrive all supported instance level extensions
		uint32 Count = 0;
		std::vector<VkExtensionProperties> SupportedExtensions;
		vkEnumerateInstanceExtensionProperties(nullptr, &Count, nullptr);
		SupportedExtensions.resize(Count);
		vkEnumerateInstanceExtensionProperties(nullptr, &Count, SupportedExtensions.data());

		std::vector<const char*> Available;

		uint32 ReqSize = pRequired.size();

		for (auto& ext : SupportedExtensions)
		{
			for (size_t Iter = 0; Iter < ReqSize; ++Iter)
			{
				if (strcmp(ext.extensionName, pRequired[Iter]) == 0)
					Available.push_back(pRequired[Iter]);
				
				if (Available.size() == ReqSize)
					return std::vector<const char*>(pRequired.begin(), pRequired.end());
			}
		}

		
		return Available;
	}


	VkInstance Renderer::CreateVkInstance(bool pValidationLayers) noexcept
	{
		VkInstance Instance = VK_NULL_HANDLE;

		VkApplicationInfo AppInfo{};
		AppInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		AppInfo.pNext            = nullptr;
		AppInfo.apiVersion       = VK_MAKE_VERSION(1, 0, 0);
		AppInfo.pApplicationName = m_Window.GetTitle();
		AppInfo.pEngineName		 = "Hydrogen";
		AppInfo.engineVersion    = VK_MAKE_VERSION(HYD_VERSION_MAJOR, HYD_VERSION_MINOR, HYD_VERSION_PATCH);
		


		VkInstanceCreateInfo InstanceCInfo{};
		InstanceCInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		InstanceCInfo.pNext = nullptr;
		InstanceCInfo.pApplicationInfo      = &AppInfo;

		//Validation Layers:
		if (pValidationLayers)
		{
			//Enable Validation layers:
			const char* Layers[1] = { "VK_LAYER_KHRONOS_validation" };
			InstanceCInfo.enabledLayerCount   = 1;
			InstanceCInfo.ppEnabledLayerNames = Layers;
		}
		

		uint32 Count = 0;
		const char** extensions = glfwGetRequiredInstanceExtensions(&Count);

		for (uint32 I = 0 ; I < Count ; ++I)
			m_InstanceLevelExtensions.push_back(extensions[I]);
		

		std::vector<const char*> AvailableExtensions = CheckForInstanceExtensions(m_InstanceLevelExtensions);

		InstanceCInfo.enabledExtensionCount   = AvailableExtensions.size();
		InstanceCInfo.ppEnabledExtensionNames = AvailableExtensions.data();

		//Creating the actual instance
		vkCreateInstance(&InstanceCInfo, VULKAN_ALLOCATION_CALLBACK, &Instance);

		return Instance;
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



}; // namespace Hydrogen