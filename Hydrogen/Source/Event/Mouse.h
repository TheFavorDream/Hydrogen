#pragma once

#include "Common.h"
#include "GLFW/glfw3.h"
#include "Keyboard.h"

namespace Hydrogen
{

	enum ScrollState { SCROLL_DOWN=-1, SCROLL_NOCHANGE, SCROLL_UP };

	class Mouse
	{
	public:

		//Sets All the needed Call backs
		HYD static void InitMouse(GLFWwindow* pWindow);

		//Call Backs
		HYD static void MouseCursorCallBack(GLFWwindow* pWindow, double pMouseX, double pMouseY);
		HYD static void MouseButtonCallBack(GLFWwindow* pWindow, int pButton, int pAction, int pMods);
		HYD static void MouseScrollCallBack(GLFWwindow* pWindow, double pOffsetX, double pOffsetY);

		HYD static void DisableCursor(bool pDisable);

		//Cursor State:
		HYD static double GetCursorX();
		HYD static double GetCursorY();

		HYD static double GetOffsetX();
		HYD static double GetOffsetY();

		//Button State:
		HYD static State  GetRightKeyState();
		HYD static State  GetLeftKeyState();
		 
		HYD static bool   IsRightKeyPressed();
		HYD static bool   IsLeftKeyPressed();
		 
		HYD static bool   IsRightKeyReleased();
		HYD static bool   IsLeftKeyReleased();


		//Scroll State:
		HYD static int GetScrollVerticalOffset();
		HYD static ScrollState GetScrollState();
	private:

		//Mouse State:
		static bool m_CameraMode;
		static bool m_RawMotion;
		//Cursor State:
		static double m_X, m_Y;
		static float m_OffsetX, m_OffsetY;
		static float m_LastX, m_LastY;

		//Button State:
		static Event m_RightBtn;
		static Event m_LeftBtn;

		//Scroll State
		static int m_ScrollY;
		static ScrollState m_ScrollState;

		static GLFWwindow* m_WindowRef;
	};
};
