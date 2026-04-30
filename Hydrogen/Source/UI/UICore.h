#pragma once

#include "Common.h"
#include "Log/Log.h"
#include "GuiWindow.h"
#include "HydPch.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Glew/glew.h"
#include "Window/Window.h"
#include "VecMath/Vector/Vectors.h"


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
		 
		HYD int CreateUIWindow(const std::string& pTitle, VecF2 pSize, VecF2 pPos=VecF2(0.0f), RefernceOrigin pOrigin = UP_LEFT,std::string pID="");
		//HYD int CreateUIWindow(const std::string& pTitle);
		HYD void Render();
		 
		HYD inline uint32 GetFPS() { return (uint32)ImGui::GetIO().Framerate; }

		HYD inline bool WantToCapture() { return (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse); }

		HYD GuiWindow* GetWindow(const std::string& pID);

	private:

		std::unordered_map<std::string, GuiWindow> m_GUIs;

		VecF4 m_BackgroundColor = VecF4(0.035f, 0, 0.082f, 1.0f);
		VecF4 m_ForeGroundColor = VecF4(1.0f, 0.238f, 0.895f, 1.0f);


	private:
		static UICore* s_Self;
	};

};

