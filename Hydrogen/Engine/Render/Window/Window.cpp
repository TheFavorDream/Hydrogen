#include "Window.h"
#include "HydPch.h"

#include "../Renderer.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{


	Window* Window::s_CurrentWindow = nullptr;

	void Window::ResizeCallback(GLFWwindow* pWindow, int32 pWidth, int32 pHeight)
	{

		// glfwGetFramebufferSize(pWindow,
		// 	 &Renderer::Self().GetWindow().m_ViewportSize.X,
		// 	&Renderer::Self().GetWindow().m_ViewportSize.Y
		// );

		Renderer::Self().RecreateSwapchain();
	}

	void Window::MaximizeCallback(GLFWwindow* pWindow, int32 pMaximized)
	{
	}

	void Window::FocusCallback(GLFWwindow * pWindow, int32 pFocus)
	{
	}





	Window::Window() noexcept :
		m_Window(NULL), m_WindowSize(VecI2(0, 0)), m_Title("Test")
	{
		s_CurrentWindow = this;
	}
	
	Window::Window(
		int32 		 pWidth,
		int32 		 pHeight,
		const char * pTitle
	) noexcept:
		m_Window(NULL),
		m_WindowSize(VecI2(pWidth, pHeight)),
		m_Title(pTitle)
	{
		s_CurrentWindow = this;
		MakeWindow(m_WindowSize.X, m_WindowSize.Y, m_Title);
	}
	
	Window::Window(
		WindowInfo pCInfo
	) noexcept
	{
		MakeWindow(pCInfo.Width, pCInfo.Height, pCInfo.Title);
	}


	Window::~Window() noexcept
	{
		//DestroyWindow(VK_NULL_HANDLE);
	}
	
	uint32 Window::MakeWindow(
		int32 		pWidth,
		int32 		pHeight,
		const char* pTitle
	) noexcept
	{

		
		//glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND | GLFW_PLATFORM_X11);

		if (glfwInit() == GLFW_FALSE)
		{
			const char* ErrorMessage;
			glfwGetError(&ErrorMessage);
			Log::SetError(Log::FmtStr("Failed to Init glfw. GLFW Says:%s", ErrorMessage));
			return HYD_FAILED;
		}

		if (m_Window != NULL)
		{
			Log::SetWarning("Window Already exists");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


		m_Window = glfwCreateWindow(pWidth, pHeight, pTitle, NULL, NULL);
			
		if (m_Window == NULL)
		{
			Log::SetError("", HYD_WINDOW_FAILD, __FILE__, __LINE__);
			return HYD_WINDOW_FAILD;
		}
		
		m_ViewportRatios.X = 100.0f;
		m_ViewportRatios.Y = 100.0f;


		glfwSetWindowSizeCallback    ( m_Window, Window::ResizeCallback);
		glfwSetWindowMaximizeCallback( m_Window, Window::MaximizeCallback);
		glfwSetWindowFocusCallback   ( m_Window, Window::FocusCallback);

		return HYD_OK;
	}
	
	uint32 Window::CreateVulkanSurface(
		VkInstance pVkInstance
	) noexcept
	{
		glfwCreateWindowSurface(pVkInstance, m_Window, VULKAN_ALLOCATION_CALLBACK, &m_Surface);
		
		if (m_Surface == VK_NULL_HANDLE)
		{
			const char* LogMessage;
			glfwGetError(&LogMessage);
			Log::SetError(Log::FmtStr("Cannot Create Surface. glfw Says:%s", LogMessage));
			return HYD_FAILED;
		}
		return HYD_OK;
	}


	void Window::DestroyWindow(
		VkInstance pVkInstance
	) noexcept
	{
		vkDestroySurfaceKHR  (pVkInstance, m_Surface, VULKAN_ALLOCATION_CALLBACK);
		glfwDestroyWindow(m_Window);
		m_Window = NULL;
	}


	bool Window::ShouldWindowClose()
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::SetViewportRatio(
		float pWidth,
		float pHeight,
		float pX, //= 0.0f,
		float pY  //= 0.0f
	) noexcept
	{
		if (pWidth < 0.0f || pHeight < 0.0f)
		{
			Log::SetError("Can't use nagative value", HYD_INVALID_VALUE, __FILE__, __LINE__);
			return;
		}
		m_ViewportRatios.X = pX;
		m_ViewportRatios.Y = pY;
		m_ViewportRatios.Z = pWidth;
		m_ViewportRatios.W = pHeight;
	}

	VkViewport Window::GetViewportSize()    noexcept
	{
		glfwGetWindowSize(
			m_Window,
			reinterpret_cast<int*>(&m_WindowSize.X),
			reinterpret_cast<int*>(&m_WindowSize.Y)
		);

		m_ViewportSize.x        = m_WindowSize.X * (m_ViewportRatios.X / 100.0f);
		m_ViewportSize.y        = m_WindowSize.Y * (m_ViewportRatios.Y / 100.0f);
		m_ViewportSize.width    = m_WindowSize.X * (m_ViewportRatios.Z / 100.0f);
		m_ViewportSize.height   = m_WindowSize.Y * (m_ViewportRatios.W / 100.0f);
		m_ViewportSize.minDepth = 0.0f;
		m_ViewportSize.maxDepth = 1.0f;
		
		return m_ViewportSize;
	}

	VkRect2D   Window::GetViewportScissor() noexcept
	{
		m_Scissor = VkRect2D
		{
			.offset = VkOffset2D
			{
				.x = static_cast<int32>(m_ViewportSize.x),
				.y = static_cast<int32>(m_ViewportSize.y)
			},
			.extent = VkExtent2D
			{
				.width  = static_cast<uint32>(m_ViewportSize.width),
				.height = static_cast<uint32>(m_ViewportSize.height)
			}
		};
		return m_Scissor;
	}

	bool Window::IsMouseInViewPort() noexcept
	{
		double X = Mouse::MousePosition.X;
		double Y = Mouse::MousePosition.Y;

		//if (X < m_ViewportSize.X && X > m_ViewportSize.Z)
		//{
		//	if (Y < m_ViewportSize.Y && Y > m_ViewportSize.W)
		//		return true;
		//}

		return false;
	}

	Window* const Window::GetCurrentWindow()
	{
		return s_CurrentWindow;
	}


	SurfaceInfo Window::QuarrySurfaceInfo(
		VkPhysicalDevice pDevice
	) noexcept
	{
		SurfaceInfo surfaceDetail;
		//Capabilities:
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, m_Surface, &surfaceDetail.Capabilities);

		//Surface formats:
		uint32_t FormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, m_Surface, &FormatCount, nullptr);
		surfaceDetail.Formats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, m_Surface, &FormatCount, surfaceDetail.Formats.data());


		//Presentation Modes:

		uint32_t PresentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, m_Surface, &PresentModeCount, nullptr);
		surfaceDetail.PresentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, m_Surface, &PresentModeCount, surfaceDetail.PresentModes.data());

		return surfaceDetail;
	}

};

	