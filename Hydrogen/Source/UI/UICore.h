#pragma once

#include "Common.h"
#include "GuiWindow.h"

#include "3rdParty/imgui/imgui.h"
#include "3rdParty/imgui/imgui_impl_opengl3.h"
#include "3rdParty/imgui/imgui_impl_glfw.h"

#include "Glew/glew.h"
#include "Window/Window.h"
#include "Math/Vector/VectorDef.h"

#include <unordered_map>
#include <string>
#include "Log/Log.h"
namespace Hydrogen
{


	enum HYD_WindowConf {
		HYD_NORESIZE = 1 << 0,
		HYD_NOREPOS = 1 << 1
	};


	class UICore
	{
	public:
		HYD static UICore* Self();

	public:

		HYD int InitUI();
		HYD int ShutdownUI();
		 
		HYD int CreateUIWindow(std::string pTitle, Vec2 pSize, Vec2 pPos=Vec2(0.0f), RefernceOrigin pOrigin = UP_LEFT,std::string pID="");
		HYD void Render();
		 
		HYD GuiWindow* GetWindow(std::string pID);

	private:
		std::unordered_map<std::string, GuiWindow> m_GUIs;

		Vec4 m_BackgroundColor = Vec4(0.035f, 0, 0.082f, 1.0f);
		Vec4 m_ForeGroundColor = Vec4(1.0f, 0.238f, 0.895f, 1.0f);


	private:
		static UICore* s_Self;
	};

};

