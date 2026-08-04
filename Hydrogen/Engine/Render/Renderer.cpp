#include "Shader.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <vk_video/vulkan_video_codec_h264std.h>
#include <vulkan/vulkan_core.h>
#define RENDERER_H
#include "Renderer.h"
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

		m_VkPhysicalDevice = SelectPhysicalDevice();
		if (m_VkPhysicalDevice == VK_NULL_HANDLE)
		{
			Log::SetError("Unable to Select a physial Device");
			return HYD_FAILED;
		}

		//Print the Selected Device Info:
		Log::SetInfo(Log::FmtStr("GPU:%s Selected as the main device", m_DeviceInfo.Name.c_str()));


		//Create Rendering Surface

		m_VkSurface = CreateSurface();
		if (m_VkSurface == VK_NULL_HANDLE)
		{
			Log::SetError("Unable to Create Surface");
			return HYD_FAILED;
		}

		//Creating Logical device and queues
		m_Device = CreateDevice();
		if (m_Device == VK_NULL_HANDLE)
		{
			Log::SetError("Unable to Create Logical Device");
			return HYD_FAILED;
		}

		//Swapchain creation:

		SurfaceInfo _Surfaceinfo          = m_Swapchain.QuarrySurfaceInfo(m_VkPhysicalDevice, m_VkSurface);

		VkSurfaceFormatKHR _SurfaceFormat = m_Swapchain.SelectSurfaceFormat   (_Surfaceinfo, { VK_FORMAT_R8G8B8A8_SRGB });
		VkPresentModeKHR   _PresentMode   = m_Swapchain.SelectPresentationMode(_Surfaceinfo, { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_KHR});
		VkExtent2D         _SurfaceExtent = m_Swapchain.SelectExtent          (_Surfaceinfo, m_Window);


		uint32 Res = m_Swapchain.CreateSwapchain(
			_Surfaceinfo,
			m_Device,
			m_VkSurface,
			_Surfaceinfo.Capabilities.minImageCount+1,
			_SurfaceExtent, 
			_SurfaceFormat,
			_PresentMode
		);


		if (Res != HYD_OK)
		{
			Log::SetError("Unable to Create Swapchain");
			return HYD_FAILED;
		}



		//Command Buffer Creation:
		if (m_CommandPool.CreatePool(m_DeviceInfo.Queues.Graphics.value(),
		 VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) != HYD_OK)
		 {
			return HYD_FAILED;
		 }


		m_CommandBuffer = m_CommandPool.AllocateCommandBuffer(); 


		//Renderpass Setup:

		VkAttachmentReference ColorAttachment = m_RenderPass.AddAttachment(
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			m_Swapchain.GetImageFormat(),
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

		m_RenderPass.AddSubpass(VK_PIPELINE_BIND_POINT_GRAPHICS, { ColorAttachment });

		m_RenderPass.AddSubpassDependency(
			VK_SUBPASS_EXTERNAL,
			0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0,VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
		);

		Res = m_RenderPass.CreateRenderPass();
		if (Res != HYD_OK)
		{
			Log::SetError("RenderPass Creation Failed");
			return HYD_FAILED;
		}


		//Framebuffer setup:

		for (uint32 I = 0 ; I < m_Swapchain.ImageCount() ; ++I)
		{
			Vulkan::FrameBuffer framebuffer;
			framebuffer.CreateFrameBuffer(
				m_RenderPass.GetHandle(),
				m_Swapchain.GetImage(I),
				m_Swapchain.GetImageExtent().width,
				m_Swapchain.GetImageExtent().height
			);
			m_FrameBuffers.push_back(std::move(framebuffer));
		}



		//Shader Compliation:

		Res = m_VertexShader.CreateShaderFromFile(
HYD_STAGE_VERTEX_SHADER,
 	  "/home/Volta/Desktop/Dev/Hydrogen/Hydrogen/Engine/Render/Shaders/VertexTest.spv"
	);
		
		if (Res != HYD_OK)
			return HYD_FAILED;
		
		Res = m_FragmentShader.CreateShaderFromFile(
HYD_STAGE_FRAGMENT_SHADER,
	 "/home/Volta/Desktop/Dev/Hydrogen/Hydrogen/Engine/Render/Shaders/FragmentTest.spv"
	);

		if (Res != HYD_OK)
			return HYD_FAILED;





		//Setting up the pipeline:

		m_BasicPipeline.SetInputAssembler(
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VK_FALSE
		);


		m_BasicPipeline.SetViewport(
			m_Swapchain.GetImageExtent().width,
			m_Swapchain.GetImageExtent().height,
			0, 0
		);

		m_BasicPipeline.SetRasterizer();

		//Stencil & Depth
		m_BasicPipeline.SetDepthStencil(VK_FALSE, VK_FALSE);

		m_BasicPipeline.CreatePipelineLayout();


		VkPipelineColorBlendAttachmentState AttachmentColorBlend;

		AttachmentColorBlend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		AttachmentColorBlend.blendEnable 		 = VK_FALSE;
		AttachmentColorBlend.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		AttachmentColorBlend.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		AttachmentColorBlend.colorBlendOp 		 = VK_BLEND_OP_ADD; // Optional
		AttachmentColorBlend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		AttachmentColorBlend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		AttachmentColorBlend.alphaBlendOp 		 = VK_BLEND_OP_ADD; // Optional


		m_BasicPipeline.SetColorBlend(
			{AttachmentColorBlend}
		);

		m_BasicPipeline.SetDynamicStates(
			{VK_DYNAMIC_STATE_VIEWPORT}
		);

		if (m_BasicPipeline.CreatePipeline(
			{&m_VertexShader, &m_FragmentShader},
			m_RenderPass.GetHandle(),
			0
		) != HYD_OK)
		{
			return HYD_FAILED;
		}



		//Create synchronization objects:

		VkSemaphoreCreateInfo SemCInfo{};

		SemCInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		SemCInfo.pNext = nullptr;

		vkCreateSemaphore(m_Device, &SemCInfo, VULKAN_ALLOCATION_CALLBACK, &m_ImageAvailable);
		vkCreateSemaphore(m_Device, &SemCInfo, VULKAN_ALLOCATION_CALLBACK, &m_RenderingFinished);
		
		VkFenceCreateInfo FenceCInfo{};
		FenceCInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceCInfo.pNext = nullptr;
		FenceCInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		vkCreateFence(m_Device, &FenceCInfo, VULKAN_ALLOCATION_CALLBACK, &m_FrameFinished);

		return HYD_OK;
	}

	uint32 Renderer::Shutdown() noexcept
	{

		vkDeviceWaitIdle(m_Device);

		Log::SetInfo("Shutting Down Renderer");
		
		m_CommandBuffer.ResetCommandBuffer();
		
		m_BasicPipeline.DestroyPipeline();
		m_VertexShader.DestroyShader();
		m_FragmentShader.DestroyShader();

		for (auto& framebuffer : m_FrameBuffers)
			framebuffer.DestroyFrameBuffer();


		m_CommandPool.DestroyPool();

		m_RenderPass.DestroyRenderPass();
		m_Swapchain.DestroySwapchain();

		vkDestroySemaphore(m_Device, m_ImageAvailable, VULKAN_ALLOCATION_CALLBACK);
		vkDestroySemaphore(m_Device, m_RenderingFinished, VULKAN_ALLOCATION_CALLBACK);
		vkDestroyFence(m_Device, m_FrameFinished, VULKAN_ALLOCATION_CALLBACK);
		vkDestroyDevice      (m_Device,                  VULKAN_ALLOCATION_CALLBACK);
		vkDestroySurfaceKHR  (m_VkInstance, m_VkSurface, VULKAN_ALLOCATION_CALLBACK);
		vkDestroyInstance    (m_VkInstance,				 VULKAN_ALLOCATION_CALLBACK);
		m_Window.DestroyWindow();

		return HYD_OK;
	}



	void Renderer::PushPrimitive(Primitive* pPrimitive)
	{
		if (pPrimitive != nullptr)
			m_PrimitiveQueue.push(pPrimitive);
	}

	uint32 Renderer::Render()
	{

		//Wait for previous frame to finish:
		vkWaitForFences(m_Device, 1, &m_FrameFinished, VK_TRUE, UINT64_MAX);

		//Reset the Fence for the next frame:
		vkResetFences(m_Device, 1, &m_FrameFinished);
		
		m_CommandBuffer.ResetCommandBuffer();
		

		//Retrive an image from the swapchain:
		uint32 ImageIndex = m_Swapchain.AcquireImage(m_ImageAvailable);


		m_CommandBuffer.RecordCommandBuffer();	

		m_RenderPass.BeginRenderPass(
			m_CommandBuffer,
			m_FrameBuffers[ImageIndex],
			VkRect2D{VkOffset2D{0, 0}, m_Swapchain.GetImageExtent()}
		);
		

		VkViewport CurrantViewport{0.0f, 0.0f, m_Swapchain.GetImageExtent().width, m_Swapchain.GetImageExtent().height};
		vkCmdSetViewport(m_CommandBuffer.GetHandle(), 0, 1, &CurrantViewport);
		m_BasicPipeline.BindPipeline(m_CommandBuffer);

		//Issue a draw call
		vkCmdDraw(m_CommandBuffer.GetHandle(), 3, 1, 0, 0);


		m_RenderPass.EndRenderPass(m_CommandBuffer);
		m_CommandBuffer.EndRecordingCommandBuffer();

		VkSemaphore 		 waitSemaphors[] 	= {m_ImageAvailable};
		VkPipelineStageFlags waitStages[] 		= {/*VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT*/VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

		//Submmiting work to the queue:
		VkSubmitInfo SubmitInfo{};
		SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		SubmitInfo.pNext = nullptr;
		SubmitInfo.commandBufferCount = 1;
		SubmitInfo.pCommandBuffers    = m_CommandBuffer.GetHandlePtr();
		SubmitInfo.waitSemaphoreCount = 1;
		SubmitInfo.pWaitSemaphores    = waitSemaphors;
		SubmitInfo.pWaitDstStageMask  = waitStages;

		VkSemaphore signalSemaphores[] = {m_RenderingFinished};
		SubmitInfo.signalSemaphoreCount = 1;
		SubmitInfo.pSignalSemaphores    = signalSemaphores;

		if (vkQueueSubmit(m_Queues.Graphics, 1, &SubmitInfo, m_FrameFinished) != VK_SUCCESS)
			Log::SetError("Unable to summit work to queue");



		//Present the renderer image
		m_Swapchain.PresentImage(ImageIndex, m_Queues.Present, {m_RenderingFinished});


		return HYD_OK;
	}





	uint32 Renderer::ReCreateSwapchain() noexcept
	{

		vkDeviceWaitIdle(m_Device);

		for (auto& framebuffer : m_FrameBuffers)
			framebuffer.DestroyFrameBuffer();

		m_Swapchain.DestroySwapchain();
		

		m_FrameBuffers.clear();

		SurfaceInfo _Surfaceinfo          = m_Swapchain.QuarrySurfaceInfo(m_VkPhysicalDevice, m_VkSurface);

		VkSurfaceFormatKHR _SurfaceFormat = m_Swapchain.SelectSurfaceFormat   (_Surfaceinfo, { VK_FORMAT_R8G8B8A8_SRGB });
		VkPresentModeKHR   _PresentMode   = m_Swapchain.SelectPresentationMode(_Surfaceinfo, { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_KHR});
		VkExtent2D         _SurfaceExtent = m_Swapchain.SelectExtent          (_Surfaceinfo, m_Window);


		uint32 Res = m_Swapchain.CreateSwapchain(
			_Surfaceinfo,
			m_Device,
			m_VkSurface,
			_Surfaceinfo.Capabilities.minImageCount+1,
			_SurfaceExtent, 
			_SurfaceFormat,
			_PresentMode
		);

		for (uint32 I = 0 ; I < m_Swapchain.ImageCount() ; ++I)
		{
			Vulkan::FrameBuffer framebuffer;
			framebuffer.CreateFrameBuffer(
				m_RenderPass.GetHandle(),
				m_Swapchain.GetImage(I),
				m_Swapchain.GetImageExtent().width,
				m_Swapchain.GetImageExtent().height
			);
			m_FrameBuffers.push_back(std::move(framebuffer));
		}

		return HYD_OK;
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

	std::vector<const char*> Renderer::CheckForDeviceExtensions(const std::vector<const char*>& pRequired) noexcept
	{
		uint32 ExtensionCount = 0;
		std::vector<VkExtensionProperties> Extensions;
		vkEnumerateDeviceExtensionProperties(m_VkPhysicalDevice, nullptr, &ExtensionCount, nullptr);
		Extensions.resize(ExtensionCount);
		vkEnumerateDeviceExtensionProperties(m_VkPhysicalDevice, nullptr, &ExtensionCount, Extensions.data());

		std::vector<const char*> Available;

		for (auto& extension : Extensions)
		{
			for (auto& required : pRequired)
			{
				if (pRequired.size() == Available.size())
					return std::move(Available);

				if (!strcmp(extension.extensionName, required))
				{
					Available.push_back(required);
				}
			}
		}

		return std::move(Available);

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

	VkPhysicalDevice Renderer::SelectPhysicalDevice() noexcept
	{

		//Quarry all devices:
		uint32 DeviceCount = 0;
		std::vector<VkPhysicalDevice> Devices;

		vkEnumeratePhysicalDevices(m_VkInstance, &DeviceCount, nullptr);
		Devices.resize(DeviceCount);
		vkEnumeratePhysicalDevices(m_VkInstance, &DeviceCount, Devices.data());

		//Quarry Devices Properties & features
		std::vector<VkPhysicalDeviceProperties> Properties; Properties.resize(DeviceCount);
		std::vector<VkPhysicalDeviceFeatures>   Features;   Features.resize(DeviceCount);

		std::vector<VkPhysicalDeviceMemoryProperties> MemProperties; MemProperties.resize(DeviceCount);
		
		for (size_t Iter = 0; Iter < DeviceCount; ++Iter)
		{
			vkGetPhysicalDeviceProperties(Devices[Iter], &Properties[Iter]);
			vkGetPhysicalDeviceFeatures(Devices[Iter],   &Features[Iter]);
			vkGetPhysicalDeviceMemoryProperties(Devices[Iter], &MemProperties[Iter]);
		}

		//Select the best device possible:

		//if only a single device exists, choose that
		if (DeviceCount == 1)
		{

			m_DeviceInfo.Name		      = Properties[0].deviceName;
			m_DeviceInfo.DriverVersion    = Properties[0].driverVersion;
			m_DeviceInfo.ApiVersion       = Properties[0].apiVersion;
			m_DeviceInfo.Type		      = Properties[0].deviceType;
			m_DeviceInfo.Limits           = Properties[0].limits;
			m_DeviceInfo.Features         = Features[0];
			m_DeviceInfo.MemoryProperties = MemProperties[0];

			return Devices[0];
		}


		ASSERT(false, "Multiple Device Selection not implemented yet!");

		return VK_NULL_HANDLE;
	}

	VkSurfaceKHR Renderer::	CreateSurface() noexcept
	{
		VkSurfaceKHR Surface = VK_NULL_HANDLE;
		glfwCreateWindowSurface(m_VkInstance, m_Window.GetWindow(), nullptr, &Surface);
		
		if (Surface == VK_NULL_HANDLE)
		{
			const char* LogMessage;
			glfwGetError(&LogMessage);
			Log::SetError(Log::FmtStr("Cannot Create Surface. glfw Says:%s", LogMessage));
		}

		return Surface;
	}

	QueueFamily Renderer::EnumerateQueueFamilies(VkPhysicalDevice pPhysicalDevice) noexcept
	{

		uint32 QueueFamilyCount = 0;
		std::vector<VkQueueFamilyProperties> QueueProperties;
		vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &QueueFamilyCount, nullptr);
		QueueProperties.resize(QueueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &QueueFamilyCount, QueueProperties.data());

		QueueFamily Family;

		uint32 Index = 0;
		for (auto& family : QueueProperties)
		{
			if (Family.IsComplete())
				return Family;

			if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{

				VkBool32 IsPresentSupported = VK_FALSE;
				vkGetPhysicalDeviceSurfaceSupportKHR(m_VkPhysicalDevice, Index, m_VkSurface, &IsPresentSupported);
				if (IsPresentSupported)
				{
					Family.Present  = Index;
					Family.Graphics = Index;
				}

			}

			if (family.queueFlags & VK_QUEUE_COMPUTE_BIT)
				Family.Compute = Index;
			if (family.queueFlags & VK_QUEUE_TRANSFER_BIT)
				Family.Transfer = Index;
			
			++Index;
		}

		return Family;
	}

	VkDevice Renderer::CreateDevice() noexcept
	{
		//Get the available queue familes
		QueueFamily Family = EnumerateQueueFamilies(m_VkPhysicalDevice);


		std::vector<VkDeviceQueueCreateInfo> QueuesCInfo;
		std::set<uint32> QueueFamilyIndices = {Family.Graphics.value(), Family.Present.value() , Family.Transfer.value() , Family.Compute.value() };
		float Priority = 1.0f;
		for (auto queueIndex : QueueFamilyIndices)
		{
			VkDeviceQueueCreateInfo QueueCInfo{};
			QueueCInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			QueueCInfo.pNext            = nullptr;
			QueueCInfo.pQueuePriorities = &Priority;
			QueueCInfo.queueFamilyIndex = queueIndex;
			QueueCInfo.queueCount		= 1;

			QueuesCInfo.push_back(QueueCInfo);
		}



		//Checking for available extensions:
		uint32 RequiredCount    = m_DeviceLevelExtensions.size();
		auto AvailbaleDeviceLevelExtensions = CheckForDeviceExtensions(m_DeviceLevelExtensions);

		if (RequiredCount != m_DeviceLevelExtensions.size())
		{
			Log::SetWarning("Not all required extensions are supported");
		}

		//Device Creation:
		VkDeviceCreateInfo DeviceCInfo{};
		DeviceCInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		DeviceCInfo.pNext = nullptr;
		DeviceCInfo.flags = 0;
		DeviceCInfo.queueCreateInfoCount = QueuesCInfo.size();
		DeviceCInfo.pQueueCreateInfos    = QueuesCInfo.data();
		//Device Level extensions.
		DeviceCInfo.enabledExtensionCount   = m_DeviceLevelExtensions.size();
		DeviceCInfo.ppEnabledExtensionNames = m_DeviceLevelExtensions.data();


		VkDevice Device = VK_NULL_HANDLE;
		VkResult Result = vkCreateDevice(m_VkPhysicalDevice, &DeviceCInfo, VULKAN_ALLOCATION_CALLBACK, &Device);

		vkGetDeviceQueue(Device, Family.Graphics.value(), 0, &m_Queues.Graphics);
		vkGetDeviceQueue(Device, Family.Present.value(),  0, &m_Queues.Present);
		vkGetDeviceQueue(Device, Family.Transfer.value(), 0, &m_Queues.Transfer);
		vkGetDeviceQueue(Device, Family.Compute.value(),  0, &m_Queues.Compute);

		m_DeviceInfo.Queues = Family;

		return Device;
	}
	
};