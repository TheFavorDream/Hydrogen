#include "Event.h"
#include <utility>

namespace Hydrogen
{
//------------------------------------Mouse Events-----------------------------------------

	bool 			Mouse::m_CameraMode;
	bool 			Mouse::m_RawMotion;
	Ptr<GLFWwindow> Mouse::m_WindowRef = nullptr;

    VecD2 	  Mouse::MousePosition;
    KeyState  Mouse::MouseLeftKey  = KEY_UP;
    KeyState  Mouse::MouseRightKey = KEY_UP;

	void Mouse::InitMouse(Ptr<GLFWwindow> pWindow)
	{
		glfwSetCursorPosCallback(pWindow, Mouse::MouseCursorCallBack);
		glfwSetMouseButtonCallback(pWindow, Mouse::MouseButtonCallBack);
		glfwSetScrollCallback(pWindow, Mouse::MouseScrollCallBack);

		m_WindowRef = pWindow; // We Store a window ref
		m_RawMotion = glfwRawMouseMotionSupported();

		if (m_RawMotion)
			glfwSetInputMode(m_WindowRef, GLFW_RAW_MOUSE_MOTION, GL_TRUE);
	}

	void Mouse::MouseCursorCallBack(
		Ptr<GLFWwindow> pWindow,
		double 			pMouseX, 
		double 			pMouseY
	)
	{

		FrameEvent::Self.PositionOffset = VecD2(
			(pMouseX-MousePosition.X),
			(pMouseY-MousePosition.Y)
		);

		MousePosition = VecD2(
			pMouseX,
			pMouseY
		);
	}

	void Mouse::MouseButtonCallBack(
		Ptr<GLFWwindow> pWindow,
		int 			pButton,
		int 			pAction,
		int 			pMods
	)
	{
		KeyState  CurrentState  = (pAction == GLFW_RELEASE) ? KEY_UP : KEY_DOWN;

		switch (pButton)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			FrameEvent::Self.MouseLeftKeyAction = KeyAction((uint32)CurrentState - (uint32)MouseLeftKey);
			MouseLeftKey       				    = CurrentState;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			FrameEvent::Self.MouseRightKeyAction = KeyAction((uint32)CurrentState - (uint32)MouseRightKey);
			MouseRightKey       				 = CurrentState;
			break;
		}

	}

	void Mouse::MouseScrollCallBack(
		Ptr<GLFWwindow> pWindow,
		double 			pOffsetX,
		double 			pOffsetY
	)
	{
		FrameEvent::Self.ScrollOffset.X = pOffsetX;
		FrameEvent::Self.ScrollOffset.Y = pOffsetY;
	}

	void Mouse::DisableCursor(bool pDisable)
	{
		m_CameraMode = pDisable;
		glfwSetInputMode(m_WindowRef, GLFW_CURSOR,  (pDisable)? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

//-------------------------------------------Key Board-------------------------------------------------------------

	std::unordered_map<uint32, KeyState> Keyboard::KeyStates;

	void Keyboard::InitKeyboard(Ptr<GLFWwindow> pWindow)
	{
		glfwSetKeyCallback(pWindow, Keyboard::KeyCallBack);
	}

	void Keyboard::KeyCallBack(
		Ptr<GLFWwindow> pWindow,
		int 			pKey,
		int 			pScancode,
		int 			pAction,
		int 			pMods
	)
	{
		KeyState  CurrentState 				 = (pAction == GLFW_RELEASE) ? KEY_UP : KEY_DOWN;
		KeyAction CurrentAction 			 = KeyAction((uint32)CurrentState - (uint32)KeyStates[pKey]); 
		KeyStates[pKey] 					 = CurrentState;

		FrameEvent::Self.KeyEvents.push_back(
			std::pair<uint32, KeyAction>(pKey, CurrentAction)
		);

	}


//-----------------------------------------------------------------------------
	FrameEvent FrameEvent::Self = FrameEvent();



	bool FrameEvent::CheckKeyState(
		uint32 		pKeyNum,
	    KeyAction   pAction
	) noexcept
	{
		//Iter over This Frames Events:
		for (auto& key : KeyEvents)
		{
			//Find the desired Key
			if (key.first == pKeyNum)
			{
				KeyAction action = key.second;
				//If action was the desired one, reset it back to ACTION_NONE and return true
				if (action == pAction)
				{
					key.second = ACTION_NONE;
					return true;
				}
			}
		}
		return false;
	}


/*
	Purpose: Retrive the Mouse Key Press State & Reset to ACTION_NONE
*/

    bool FrameEvent::IsMouseRightPressed() noexcept
	{
		if (MouseRightKeyAction == ACTION_PRESS)
		{
			MouseRightKeyAction = ACTION_NONE;
			return true;
		}
		return false;
	}
    bool FrameEvent::IsMouseLeftPressed()  noexcept
	{
		if (MouseLeftKeyAction == ACTION_PRESS)
		{
			MouseLeftKeyAction = ACTION_NONE;
			return true;
		}
		return false;
	}

/*
	Purpose: Retrive the Mouse Key Release State & Reset to ACTION_NONE
*/

    bool FrameEvent::IsMouseRightReleased() noexcept
	{
		if (MouseRightKeyAction == ACTION_RELEASE)
		{
			MouseRightKeyAction = ACTION_NONE;
			return true;
		}
		return false;
	}
    bool FrameEvent::IsMouseLeftReleased()        noexcept
	{
		if (MouseLeftKeyAction == ACTION_RELEASE)
		{
			MouseLeftKeyAction = ACTION_NONE;
			return true;
		}
		return false;
	}

/*
	Purpose: Retrive the Mouse Key Held State & Reset to ACTION_NONE
*/
	bool FrameEvent::IsMouseRightHeld() 		  noexcept
	{
		if (MouseRightKeyAction == ACTION_HELD)
		{
			MouseRightKeyAction = ACTION_NONE;
			return true;
		}
		return false;
	}
    bool FrameEvent::IsMouseLeftHeld()  		  noexcept
	{
		if (MouseLeftKeyAction == ACTION_HELD)
		{
			MouseLeftKeyAction = ACTION_NONE;
			return true;
		}
		return false;
	}


/*
	Purpose: Retrives the Current InFrame Key State
*/

	KeyState FrameEvent::QueueKeyState(
		uint32 pKeyNum
	) noexcept
	{
		if (Keyboard::KeyStates.find(pKeyNum) != Keyboard::KeyStates.end())
			return Keyboard::KeyStates[pKeyNum];
		return KEY_INVALID;
	}

/*
	Purpose: Reset the Events to the default state to be ready for new frame
*/
	void FrameEvent::Reset()
	{
		KeyEvents.clear();

		MouseLeftKeyAction  = ACTION_NONE;
		MouseRightKeyAction = ACTION_NONE;

		PositionOffset = VecD2(0.0);
		ScrollOffset   = VecD2(0.0);
	}


};