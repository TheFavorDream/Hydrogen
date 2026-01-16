#pragma once


#include "../Common.h"
#include "../3rdParty/glfw/include/GLFW/glfw3.h"
#include <unordered_map>
#include <queue>

namespace Hydrogen
{

	enum State {KEY_DOWN, KEY_UP};
	enum Action {KEY_RELEASED=-1, KEY_NOACTION=0 ,KEY_PRESSED=1};
	enum KeyMode {PRESS_ONLY, RELEASE_ONLY, PRESS_RELEASE};

	struct Event
	{
		State KeyState=KEY_UP;
		KeyMode Mode=PRESS_ONLY;

		int Pressed_Counter = 0;
		int Released_Counter = 0;
	};

	class Keyboard
	{
	public:

		static void InitKeyboard(GLFWwindow* pWindow);
		static void KeyCallBack(GLFWwindow* pWindow, int pKey, int pScancode, int pAction, int pMods);

		static bool IsKeyPressed(int pScanCode);
		static bool IsKeyReleased(int pScanCode);

		static State GetKeyState(int pScanCode);

		static void SetKeyEventHint(int pScanCode, KeyMode pMode);

	private:
		static std::unordered_map<int, Event> m_Mapping;
	};
};
