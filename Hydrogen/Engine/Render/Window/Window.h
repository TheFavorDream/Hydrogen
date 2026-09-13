#pragma once

#include "../../Common.h"
#include "../../Log/Log.h"
#include "GLFW/glfw3.h"
#include "../../VecMath/Vector/Vectors.h"
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{


	struct SurfaceInfo
	{
		VkSurfaceCapabilitiesKHR    Capabilities;
		HYD_VEC<VkSurfaceFormatKHR> Formats;
		HYD_VEC<VkPresentModeKHR>   PresentModes;
	};

	struct WindowInfo
	{
		HYD WindowInfo() {}
		HYD WindowInfo(uint32 pWidth, uint32 pHeight, const char* pTitle) noexcept
			: Width(pWidth), Height(pHeight), Title(pTitle)
		{}
		uint32 Width  = 0;
		uint32 Height = 0;
		const char* Title = "";
	};

	class Window
	{
	public:

		//Window Callback functions:
		static void ResizeCallback  (GLFWwindow* pWindow, int32 pWidth, int32 pHeight);
		static void MaximizeCallback(GLFWwindow* pWindow, int32 pMaximized);
		static void FocusCallback   (GLFWwindow* pWindow, int32 pFocus);

	public:
		
		HYD Window();
		HYD Window(int32 pWidth, int32 pHeight, const char* pTitle);
		HYD Window(WindowInfo pCInfo);
		HYD ~Window();

		HYD uint32 MakeWindow(int32 pWidth, int32 pHeight, const char* pTitle);
		HYD uint32 CreateVulkanSurface(VkInstance pVkInstance) noexcept;
		void 	   DestroyWindow(VkInstance pVkInstance)       noexcept;


		HYD void UpdateViewport() noexcept;

		HYD int  SetViewportSize(int32 pWidth, int32 pHeight, int32 pStartX =0, int32 pStartY=0);
		HYD bool ShouldWindowClose();
		HYD int  SetViewportRatio(float pWidth, float pHeight);
		 
		HYD bool IsMouseInViewPort();
		 


		SurfaceInfo QuarrySurfaceInfo(
			VkPhysicalDevice pDevice
		) noexcept;

		HYD inline GLFWwindow* GetHandle()   const { return m_Window; }
		HYD inline const char* GetTitle()    const { return m_Title; }
		HYD inline int32 GetWidth()		     const { return m_Width; }
		HYD inline int32 GetHeight()         const { return m_Height; }
		HYD inline uint32 GetFrameBufferWidth()		     const { return m_ViewportSize.X; }
		HYD inline uint32 GetFrameBufferHeight()         const { return m_ViewportSize.Y; }
		HYD inline VkSurfaceKHR GetSurface() const {return m_Surface;}

	public:
		HYD static Window* const GetCurrentWindow();

	private:

		VecF2 m_ViewportRatios;

		Vec4<int32> m_ViewportSize;

		int32 m_Width, m_Height;
		const char*  m_Title;
		GLFWwindow*  m_Window;
		VkSurfaceKHR m_Surface;


		friend class Renderer;
	private:
		static Window* s_CurrentWindow;
	};
};