#pragma once

#include "Common.h"
#include "Hydrogen.h"
#include "Window.h"
#include "Event/Keyboard.h"
#include "Event/Mouse.h"
#include "Camera/Camera.h"
#include "../glm/common.hpp"
#include "../glm/gtc/type_ptr.hpp"

//ImGui (temp)
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace HydrogenViewer
{
	class Application
	{
	public:

		 Application();
		~Application();

		void Run();

	private:
		void Setup();
		void Event();
		void Update();
		void Render();
		void RenderUI();

	private:

		Window m_Window;

		Camera m_Camera;
		Hydrogen::Shader m_Shader;
		
		std::vector<std::string> ModelNames;
		std::vector<Hydrogen::Model*> m_Models;
		Hydrogen::Model* m_CurrentModel = NULL;

		float m_deltaTime = 0.0f;

		bool m_IsRunning;
	};
};