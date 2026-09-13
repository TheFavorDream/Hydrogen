/*
    Input Events Processing
*/

#pragma once

#include "../Common.h"
#include "../Render/Window/Window.h"
#include "../VecMath/Vector/Vectors.h"
#include <unordered_map>

namespace Hydrogen
{

    enum KeyState
	{
		KEY_UP     = 0,
		KEY_DOWN   = 1,
		KEY_INVALID
	};

    enum KeyAction  
	{
		ACTION_RELEASE = -1,
		ACTION_HELD    =  0,
		ACTION_PRESS   =  1,
		ACTION_NONE	   =  2
	};


	class Mouse
	{
	public:

		//Sets All the needed Call backs
		HYD static void InitMouse(Ptr<GLFWwindow> pWindow);

		//Call Backs
		HYD static void MouseCursorCallBack(
			Ptr<GLFWwindow> pWindow,
			double 			pMouseX,
			double 			pMouseY
		);

		HYD static void MouseButtonCallBack(
			Ptr<GLFWwindow> pWindow,
			int 		    pButton,
			int 		    pAction,
			int 		    pMods
		);

		HYD static void MouseScrollCallBack(
			Ptr<GLFWwindow> pWindow,
			double 			pOffsetX,
			double 			pOffsetY
		);

		HYD static void DisableCursor(bool pDisable);


	public:

        HYD static VecD2 	 MousePosition;

        HYD static KeyState  MouseLeftKey;
        HYD static KeyState  MouseRightKey;
	private:

		//Mouse State:
		static bool m_CameraMode;
		static bool m_RawMotion;

		static GLFWwindow* m_WindowRef;
	};


	class Keyboard
	{
	public:

		HYD static void InitKeyboard(GLFWwindow* pWindow);
		HYD static void KeyCallBack(GLFWwindow* pWindow, int pKey, int pScancode, int pAction, int pMods);


		HYD static std::unordered_map<uint32, KeyState> KeyStates;
	};

    struct FrameEvent
    {

		static FrameEvent Self;

        //Mouse Events
        VecD2 PositionOffset;
        VecD2 ScrollOffset;
    
        KeyAction MouseLeftKeyAction  = ACTION_NONE;
        KeyAction MouseRightKeyAction = ACTION_NONE;

		//Key Events:
		std::vector<std::pair<uint32, KeyAction>> KeyEvents;

        //returns true if the key is pressed and resets it

		bool CheckKeyState(uint32 pKeyNum, KeyAction pAction) noexcept;

		inline bool IsKeyPressed (uint32 pKeyNum) noexcept
		{
			return CheckKeyState(pKeyNum, ACTION_PRESS);
		}

        inline bool IsKeyReleased(uint32 pKeyNum) noexcept
		{
			return CheckKeyState(pKeyNum, ACTION_RELEASE);
		}

        inline bool IsKeyHeld    (uint32 pKeyNum) noexcept
		{
			return CheckKeyState(pKeyNum, ACTION_HELD);
		}


		KeyState QueueKeyState(
			uint32 pKeyNum
		) noexcept;

        bool IsMouseRightPressed() 		  noexcept;
        bool IsMouseLeftPressed()  		  noexcept;
	
        bool IsMouseRightReleased()       noexcept;
        bool IsMouseLeftReleased()        noexcept;

        bool IsMouseRightHeld() 		  noexcept;
        bool IsMouseLeftHeld()  		  noexcept;

	private:
		void Reset();
	private:
		friend class Core;
		friend class Mouse;
		friend class Keyboard;
    };

};
