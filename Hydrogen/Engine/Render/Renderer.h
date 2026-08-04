/*

	Main Renderer of Hydrogen.
	Rendering happens via a Rendering Request:

	-----------			------------	   ---------------
	|	Data  |  ---->  | Renderer | ----> | FrameBuffer |
	-----------         ------------       ---------------
*/

#pragma once


#include "../Common.h"
#include "../Log/Log.h"
#include "Vulkan/Structs.h"
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "Window/Window.h"
#include "../Geometry/Mesh.h"
#include "Core/Core.h"
#include "Material/Texture.h"
#include "Camera/Camera.h"

#include "Vulkan/Swapchain.h"
#include "Vulkan/Renderpass.h"
#include "Vulkan/FrameBuffer.h"
#include "Vulkan/Shader.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/Commands.h"

namespace Hydrogen
{
	
	using namespace std;



	class Renderer final
	{
	public:
		HYD static Renderer& Self();


	public:

		//Init and shutdown
		HYD uint32 Init(WindowInfo pWindow=WindowInfo()) noexcept;
		HYD uint32 Shutdown() noexcept;

		//Adds Renderable Primitive to the Rendering queue
		HYD void PushPrimitive(Ptr<Primitive> pPrimitive);

		//Main Rendering 
		HYD uint32 Render();

		HYD uint32 ReCreateSwapchain() noexcept;

		HYD inline Ptr<GLFWwindow> GetWindowHandle() const { return m_Window.GetWindow(); }
		HYD inline Window& GetWindow() { return m_Window; }





		HYD inline const GPUInfo& GetGPUInfo() const { return m_DeviceInfo; }
		HYD inline const Queues&  GetQueues()  const { return m_Queues; }
		HYD inline const VkDevice GetDevice()  const { return m_Device; }


	private:

		//Enumerates all available extensions and checks if the pRequired extensions are supported, if so they will be returned.
		std::vector<const char*> CheckForInstanceExtensions(const std::vector<const char*>& pRequired) noexcept;
		std::vector<const char*> CheckForDeviceExtensions(  const std::vector<const char*>& pRequired) noexcept;
		//Vulkan Initialization:
		VkInstance CreateVkInstance(bool pValidationLayers=true) noexcept;

		//Physical Device Selection:
		VkPhysicalDevice SelectPhysicalDevice() noexcept;

		//Create Surface:
		VkSurfaceKHR CreateSurface() noexcept;

		//Create Logical Device & Queues
		QueueFamily EnumerateQueueFamilies(VkPhysicalDevice pPhysicalDevice) noexcept;
		VkDevice    CreateDevice() noexcept;

	private:

		queue<Ptr<Primitive>> m_PrimitiveQueue;

		Window m_Window;

	private:
		
		std::vector<const char*> m_DeviceLevelExtensions        = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		std::vector<const char*> m_InstanceLevelExtensions		= {};
		GPUInfo m_DeviceInfo;


		//Vulkan State:
		VkInstance       m_VkInstance       = VK_NULL_HANDLE;
		VkPhysicalDevice m_VkPhysicalDevice = VK_NULL_HANDLE;
		VkSurfaceKHR	 m_VkSurface		= VK_NULL_HANDLE;
		VkDevice         m_Device			= VK_NULL_HANDLE;

		Queues              			 m_Queues;
		Vulkan::Swapchain		         m_Swapchain;
		Vulkan::RenderPass               m_RenderPass;
		Vulkan::CommandPool 			 m_CommandPool;
		Vulkan::CommandBuffer		     m_CommandBuffer;
		std::vector<Vulkan::FrameBuffer> m_FrameBuffers;


		//synchronization Objects:
		VkSemaphore m_ImageAvailable;
		VkSemaphore m_RenderingFinished;
		VkFence 	m_FrameFinished;

		Vulkan::Shader m_VertexShader;
		Vulkan::Shader m_FragmentShader;

		Vulkan::Pipeline m_BasicPipeline;

	private:
		static Ptr<Renderer> s_Self;
		friend class Core;
		friend class Window;
	};


};