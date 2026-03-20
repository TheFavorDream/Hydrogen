#pragma once

#include "Common.h"
#include "Log/Log.h"
#include "GLFW/glfw3.h"
#include "VecMath/Vector/VectorDef.h"
#include "Event/Mouse.h"

namespace Hydrogen
{
	class Window
	{
	public:
		
		HYD Window();
		HYD Window(int32 pWidth, int32 pHeight, const char* pTitle);
		HYD ~Window();

		HYD uint32 MakeWindow(int32 pWidth, int32 pHeight, const char* pTitle);
		HYD bool DestroyWindow();

		HYD int SetViewportSize(int32 pWidth, int32 pHeight, int32 pStartX =0, int32 pStartY=0);
		HYD void ProcessWindow(bool& pRunningFlag);
		HYD int SetViewportRatio(float pWidth, float pHeight);
		 
		HYD bool IsMouseInViewPort();
		 
		HYD inline GLFWwindow* GetWindow() { return m_Window; }
		HYD inline int32 GetWidth() { return m_Width; }
		HYD inline int32 GetHeight() { return m_Height; }
		HYD inline Vec4 GetViewportSize() { return m_ViewportSize; }

	public:
		HYD static Window* const GetCurrentWindow();

	private:

		Vec2 m_ViewportRatios;
		Vec4 m_ViewportSize;

		int32 m_Width, m_Height;
		const char* m_Title;
		GLFWwindow* m_Window;
	private:
		static Window* s_CurrentWindow;
	};
};