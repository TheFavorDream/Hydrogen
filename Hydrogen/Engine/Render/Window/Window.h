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
		static void ResizeCallback  (Ptr<GLFWwindow> pWindow, int32 pWidth, int32 pHeight);
		static void MaximizeCallback(Ptr<GLFWwindow> pWindow, int32 pMaximized);
		static void FocusCallback   (Ptr<GLFWwindow> pWindow, int32 pFocus);

	public:
		
		HYD Window() noexcept;

		HYD Window(
			int32 	    pWidth,
			int32 	    pHeight,
			const char* pTitle
		) noexcept;
		
		HYD Window(
			WindowInfo pCInfo
		) noexcept;

		HYD ~Window() noexcept;

		/*
			Purpose: Create the window
		*/
		HYD uint32 MakeWindow(
			int32 	    pWidth,
			int32 	    pHeight,
			const char* pTitle
		) noexcept;

		/*
			Purpose: Create Vulkan Surface
		*/
		HYD uint32 CreateVulkanSurface(
			VkInstance pVkInstance
		) noexcept;


		/*
			Purpose: Destroy the Window
		*/
		void DestroyWindow(
			VkInstance pVkInstance
		) noexcept;


		HYD VkViewport GetViewportSize()    noexcept;
		HYD VkRect2D   GetViewportScissor() noexcept;


		HYD int  SetViewportSize(
			int32 pWidth,
			int32 pHeight,
			int32 pStartX = 0,
			int32 pStartY = 0
		) noexcept;
		
		HYD void SetViewportRatio(
			float pWidth,
			float pHeight,
			float pX 		= 0.0f,
			float pY 		= 0.0f
		)  noexcept;
		
		
		HYD bool ShouldWindowClose();
		HYD bool IsMouseInViewPort() noexcept;
		 


		SurfaceInfo QuarrySurfaceInfo(
			VkPhysicalDevice pDevice
		) noexcept;


		HYD inline Ptr<GLFWwindow> GetHandle()   const { return m_Window; }
		HYD inline const char*     GetTitle()    const { return m_Title; }
		HYD inline VkSurfaceKHR    GetSurface()  const {return m_Surface;}


		HYD inline float GetViewportPosX()   const {return m_ViewportSize.x;}
		HYD inline float GetViewportPosY()   const {return m_ViewportSize.y;}
		HYD inline float GetViewportWidth()  const {return m_ViewportSize.width;}
		HYD inline float GetViewportHeight() const {return m_ViewportSize.height;}

		HYD inline uint32 GetWindowWidth () const {return m_WindowSize.X;}
		HYD inline uint32 GetWindowHeight() const {return m_WindowSize.Y;}


	public:
		HYD static Window* const GetCurrentWindow();

	private:

		const char*  	 m_Title;
		Ptr<GLFWwindow>  m_Window;
		VkSurfaceKHR 	 m_Surface;

		VecF4      m_ViewportRatios;
		VkViewport m_ViewportSize;
		VkRect2D   m_Scissor;
		VecI2 m_WindowSize;


	private:
		friend class Renderer;
	
	private:
		static Window* s_CurrentWindow;
	};
};