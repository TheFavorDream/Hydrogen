#include "Mouse.h"
#include "HydPch.h"


namespace Hydrogen
{
	double		Mouse::m_X = 0.0f;
	double		Mouse::m_Y = 0.0f;
	float		Mouse::m_OffsetY = 0.0f;
	float		Mouse::m_OffsetX = 0.01f;
	float		Mouse::m_LastY = 0.0f;
	float		Mouse::m_LastX = 0.0f;
	bool		Mouse::m_CameraMode = false;
	bool		Mouse::m_RawMotion = false;
	int		    Mouse::m_ScrollY = 0;
	ScrollState Mouse::m_ScrollState = SCROLL_NOCHANGE;
	Event       Mouse::m_LeftBtn;
	Event       Mouse::m_RightBtn;
	GLFWwindow* Mouse::m_WindowRef = NULL;

	

	void Mouse::InitMouse(GLFWwindow* pWindow)
	{
		glfwSetCursorPosCallback(pWindow, Mouse::MouseCursorCallBack);
		glfwSetMouseButtonCallback(pWindow, Mouse::MouseButtonCallBack);
		glfwSetScrollCallback(pWindow, Mouse::MouseScrollCallBack);

		m_WindowRef = pWindow; // We Store a window ref
		m_RawMotion = glfwRawMouseMotionSupported();

		if (m_RawMotion)
			glfwSetInputMode(m_WindowRef, GLFW_RAW_MOUSE_MOTION, GL_TRUE);
	}

	void Mouse::MouseCursorCallBack(GLFWwindow * pWindow, double pMouseX, double pMouseY)
	{

		if (m_CameraMode)
		{
			Mouse::m_OffsetX =  -(m_LastX - (float)pMouseX);
			Mouse::m_OffsetY =  (m_LastY - (float)pMouseY);
			m_LastX = (float)pMouseX;
			m_LastY = (float)pMouseY;
		}
		else 
		{
			Mouse::m_X = pMouseX;
			Mouse::m_Y = pMouseY;
		}

	}

	void Mouse::MouseButtonCallBack(GLFWwindow* pWindow, int pButton, int pAction, int pMods)
	{
		State CurrentState = (pAction == GLFW_RELEASE) ? KEY_UP : KEY_DOWN;
		Action action;
		switch (pButton)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			action = Action(m_LeftBtn.KeyState - CurrentState);

			switch (action)
			{
			case KEY_PRESSED:
				m_LeftBtn.Pressed_Counter += 1;
				break;
			case KEY_RELEASED:
				m_LeftBtn.Released_Counter += 1;
				break;
			}
			m_LeftBtn.KeyState = CurrentState;
			break;

		case GLFW_MOUSE_BUTTON_RIGHT:
			action = Action(m_RightBtn.KeyState - CurrentState);

			switch (action)
			{
			case KEY_PRESSED:
				m_RightBtn.Pressed_Counter += 1;
				break;
			case KEY_RELEASED:
				m_RightBtn.Released_Counter += 1;
				break;
			}
			m_RightBtn.KeyState = CurrentState;
			break;
		}

	}

	void Mouse::MouseScrollCallBack(GLFWwindow * pWindow, double pOffsetX, double pOffsetY)
	{
		m_ScrollState = ScrollState(int(pOffsetY));
		m_ScrollY +=(int32)pOffsetY;
	}

	void Mouse::DisableCursor(bool pDisable)
	{
		m_CameraMode = pDisable;
		glfwSetInputMode(m_WindowRef, GLFW_CURSOR,  (pDisable)? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	double Mouse::GetCursorX(){ return m_X; }

	double Mouse::GetCursorY(){ return m_Y; }

	double Mouse::GetOffsetX(){  
		double Offset = double(m_OffsetX);
		m_OffsetX = 0.0f;
		return Offset;
	}

	double Mouse::GetOffsetY() { 
		double Offset = double(m_OffsetY);
		m_OffsetY = 0.0f;
		return Offset;
	}

	State Mouse::GetRightKeyState() { return m_RightBtn.KeyState; }

	State Mouse::GetLeftKeyState() { return m_LeftBtn.KeyState; }


	bool Mouse::IsRightKeyPressed()
	{
		if (m_RightBtn.Pressed_Counter > 0)
		{
			m_RightBtn.Pressed_Counter -= 1;
			return true;
		}
		return false;
	}

	bool Mouse::IsLeftKeyPressed()
	{
		if (m_LeftBtn.Pressed_Counter > 0)
		{
			m_LeftBtn.Pressed_Counter -= 1;
			return true;
		}
		return false;
	}

	bool Mouse::IsRightKeyReleased()
	{
		if (m_RightBtn.Released_Counter > 0)
		{
			m_RightBtn.Released_Counter -= 1;
			return true;
		}
		return false;
	}

	bool Mouse::IsLeftKeyReleased()
	{
		if (m_LeftBtn.Released_Counter > 0)
		{
			m_LeftBtn.Released_Counter -= 1;
			return true;
		}
		return false;
	}

	int Mouse::GetScrollVerticalOffset() { return m_ScrollY; }

	ScrollState Mouse::GetScrollState() 
	{
		ScrollState state = m_ScrollState;
		m_ScrollState = SCROLL_NOCHANGE;
		return state;
	}

};