#include "Window.h"

namespace Hydrogen
{


	Window* Window::s_CurrentWindow = nullptr;

	Window::Window():
		m_Window(NULL), m_Width(1000), m_Height(800), m_Title("Test")
	{
		s_CurrentWindow = this;
	}
	
	Window::Window(int32 pWidth, int32 pHeight, const char * pTitle):
		m_Window(NULL), m_Width(pWidth), m_Height(pHeight), m_Title(pTitle)
	{
		s_CurrentWindow = this;
		MakeWindow(m_Width, m_Height, m_Title);
	}
	
	Window::~Window()
	{
		DestroyWindow();
	}
	
	bool Window::MakeWindow(int32 pWidth, int32 pHeight, const char * pTitle)
	{

		if (glfwInit() == GLFW_FALSE)
		{
			std::cout << "Unable to Init GLFW";
		}

		if (m_Window != NULL)
		{
			Log::SetError(HYD_WINDOW_FAILD, __FILE__, __LINE__);
			return HYD_WINDOW_FAILD;
		}

		glfwWindowHint(GLFW_SAMPLES, 6);

		m_Window = glfwCreateWindow(pWidth, pHeight, pTitle, NULL, NULL);
			
		if (m_Window == NULL)
		{
			Log::SetError(HYD_WINDOW_FAILD, __FILE__, __LINE__);
			return HYD_WINDOW_FAILD;
		}

		glfwMakeContextCurrent(m_Window);
		
		m_ViewportRatios.X = 100.0f;
		m_ViewportRatios.Y = 100.0f;

		return HYD_OK;
	}
	
	bool Window::DestroyWindow()
	{
		glfwDestroyWindow(m_Window);
		m_Window = NULL;
		return HYD_OK;
	}

	int Window::SetViewportSize(int32 pWidth, int32 pHeight, int32 pStartX, int32 pStartY)
	{
		GL_CALL(glViewport(pStartX, pStartY, pWidth, pHeight));
		return HYD_OK;
	}

	void Window::ProcessWindow(bool& pRunningFlag)
	{
		glfwGetWindowSize(m_Window, &m_Width, &m_Height);

		//Calculate Viewport Size based on Ratio:
		m_ViewportSize.X = (m_ViewportRatios.X/100.0f)*float(m_Width);
		m_ViewportSize.Y = (m_ViewportRatios.Y/100.0f)*float(m_Height);
		m_ViewportSize.Z = 0.0f;
		m_ViewportSize.W = (m_Height - m_ViewportSize.Y);

		pRunningFlag = !glfwWindowShouldClose(m_Window);
		SetViewportSize((int32)m_ViewportSize.X, (int32)m_ViewportSize.Y, (int32)m_ViewportSize.Z, (int32)m_ViewportSize.W);
		glfwSwapBuffers(m_Window);
	}

	int Window::SetViewportRatio(float pWidth, float pHeight)
	{
		if (pWidth < 0.0f || pHeight < 0.0f)
		{
			Log::SetError(HYD_INVALID_VALUE, __FILE__, __LINE__);
			return HYD_INVALID_VALUE;
		}
		m_ViewportRatios.X = pWidth;
		m_ViewportRatios.Y = pHeight;

		m_ViewportSize.X = (m_ViewportRatios.X / 100.0f)*float(m_Width);
		m_ViewportSize.Y = (m_ViewportRatios.Y / 100.0f)*float(m_Height);
		m_ViewportSize.Z = 0.0f;
		m_ViewportSize.W = (m_Height - m_ViewportSize.Y);

		return HYD_OK;
	}

	bool Window::IsMouseInViewPort()
	{
		double X = Mouse::GetCursorX();
		double Y = Mouse::GetCursorY();

		if (X < m_ViewportSize.X && X > m_ViewportSize.Z)
		{
			if (Y < m_ViewportSize.Y && Y > m_ViewportSize.W)
				return true;
		}

		return false;
	}

	Window* const Window::GetCurrentWindow()
	{
		return s_CurrentWindow;
	}

};

	