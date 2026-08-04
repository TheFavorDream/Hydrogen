#include "Keyboard.h"
#include "HydPch.h"


namespace Hydrogen
{

	std::unordered_map<int, Event> Keyboard::m_Mapping;


	void Keyboard::InitKeyboard(GLFWwindow* pWindow)
	{
		glfwSetKeyCallback(pWindow, Keyboard::KeyCallBack);
	}

	void Keyboard::KeyCallBack(GLFWwindow * pWindow, int pKey, int pScancode, int pAction, int pMods)
	{
		State CurrentState = (pAction == GLFW_RELEASE) ? KEY_UP : KEY_DOWN;
		State PrevState = GetKeyState(pKey);

		Action action = Action(PrevState - CurrentState);
		m_Mapping[pKey].KeyState = CurrentState;

		if (action != 0)
		{
			//Apply filtering
			if (m_Mapping[pKey].Mode == PRESS_ONLY && action == KEY_PRESSED)
				m_Mapping[pKey].Pressed_Counter += 1;
			

			if (m_Mapping[pKey].Mode == RELEASE_ONLY && action == KEY_RELEASED)
				m_Mapping[pKey].Released_Counter += 1;

			
			
			if (m_Mapping[pKey].Mode == PRESS_RELEASE)
			{
				switch (action)
				{
				case KEY_PRESSED:
					m_Mapping[pKey].Pressed_Counter += 1;
					break;
				case KEY_RELEASED:
					m_Mapping[pKey].Released_Counter += 1;
					break;
				}

			}

		}

	}

	bool Keyboard::IsKeyPressed(int pScanCode)
	{
		if (m_Mapping.find(pScanCode) == m_Mapping.end())
		{
			return false;
		}

		if (m_Mapping[pScanCode].Pressed_Counter > 0)
		{
			m_Mapping[pScanCode].Pressed_Counter -= 1;
			return true;
		}
		return false;
	}

	bool Keyboard::IsKeyReleased(int pScanCode)
	{
		if (m_Mapping.find(pScanCode) == m_Mapping.end())
		{
			return false;
		}
		if (m_Mapping[pScanCode].Released_Counter > 0)
		{
			m_Mapping[pScanCode].Released_Counter -= 1;
			return true;
		}
		return false;
	}

	State Keyboard::GetKeyState(int pScanCode)
	{
		State state = (m_Mapping.find(pScanCode) != m_Mapping.end())? m_Mapping[pScanCode].KeyState : KEY_UP;
		return state;
	}

	void Keyboard::SetKeyEventHint(int pScanCode, KeyMode pMode)
	{
		m_Mapping[pScanCode].Mode = pMode;
	}

};