#pragma once

#include "Common.h"
#include "3rdParty/glfw/include/GLFW/glfw3.h"
#include "Keyboard.h"

namespace Hydrogen
{

	enum ScrollState { SCROLL_DOWN=-1, SCROLL_NOCHANGE, SCROLL_UP };

	class Mouse
	{
	public:

		//Sets All the needed Call backs
		static void InitMouse(GLFWwindow* pWindow);

		//Call Backs
		static void MouseCursorCallBack(GLFWwindow* pWindow, double pMouseX, double pMouseY);
		static void MouseButtonCallBack(GLFWwindow* pWindow, int pButton, int pAction, int pMods);
		static void MouseScrollCallBack(GLFWwindow* pWindow, double pOffsetX, double pOffsetY);

		static void DisableCursor(bool pDisable);

		//Cursor State:
		static double GetCursorX();
		static double GetCursorY();

		static double GetOffsetX();
		static double GetOffsetY();

		//Button State:
		static State  GetRightKeyState();
		static State  GetLeftKeyState();

		static bool   IsRightKeyPressed();
		static bool   IsLeftKeyPressed();

		static bool   IsRightKeyReleased();
		static bool   IsLeftKeyReleased();


		//Scroll State:
		static int GetScrollVerticalOffset();
		static ScrollState GetScrollState();
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
