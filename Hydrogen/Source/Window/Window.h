#pragma once

#include "Common.h"
#include "3rdParty/glfw/include/GLFW/glfw3.h"
#include "Math/Vector/VectorDef.h"
#include "Event/Mouse.h"

namespace Hydrogen
{
	class Window
	{
	public:
		
		Window();
		Window(int32 pWidth, int32 pHeight, const char* pTitle);
		~Window();


		bool MakeWindow(int32 pWidth, int32 pHeight, const char* pTitle);
		bool DestroyWindow();


		int SetViewportSize(int32 pWidth, int32 pHeight, int32 pStartX =0, int32 pStartY=0);
		void ProcessWindow(bool& pRunningFlag);
		int SetViewportRatio(float pWidth, float pHeight);

		bool IsMouseInViewPort();

		inline GLFWwindow* GetWindow() { return m_Window; }
		inline int32 GetWidth() { return m_Width; }
		inline int32 GetHeight() { return m_Height; }
		inline Vec4 GetViewportSize() { return m_ViewportSize; }

	public:
		static Window* const GetCurrentWindow();

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