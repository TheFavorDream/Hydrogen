#pragma once

#include "../../Common.h"
#include "../../Log/Log.h"
#include "GLFW/glfw3.h"
#include "../../VecMath/Vector/Vectors.h"
#include "../../Event/Mouse.h"

namespace Hydrogen
{

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
		HYD bool DestroyWindow();

		HYD int SetViewportSize(int32 pWidth, int32 pHeight, int32 pStartX =0, int32 pStartY=0);
		HYD void ProcessWindow(bool& pRunningFlag);
		HYD int SetViewportRatio(float pWidth, float pHeight);
		 
		HYD bool IsMouseInViewPort();
		 
		HYD inline GLFWwindow* GetWindow() const { return m_Window; }
		HYD inline const char* GetTitle()  const { return m_Title; }
		HYD inline int32 GetWidth()		   const { return m_Width; }
		HYD inline int32 GetHeight()       const { return m_Height; }
		HYD inline VecF4 GetViewportSize() const { return m_ViewportSize; }

	public:
		HYD static Window* const GetCurrentWindow();

	private:

		VecF2 m_ViewportRatios;
		VecF4 m_ViewportSize;

		int32 m_Width, m_Height;
		const char* m_Title;
		GLFWwindow* m_Window;
	private:
		static Window* s_CurrentWindow;
	};
};