/*
	
	The Core of Hydroge.
	Every Client must inherit a class from Core

*/

#pragma once

#include "Common.h"
#include "Log/Log.h"
#include "Window/Window.h"
#include "Event/Mouse.h"
#include "Event/Keyboard.h"
#include "Glew/glew.h"
#include "Scene.h"
#include "Render/Renderer.h"

#include "Layer.h"

namespace Hydrogen
{


	class Core
	{
	public:
		
		HYD  Core(); //Default Constructor
		HYD  Core(int32 pWidth, int32 pHeight, const char* pTitle); //Constructor with a window
		HYD ~Core();


		HYD uint32 PushLayer(Layer* pLayer);


	public:
		HYD static uint32 PushScene(Scene* pScene);
		HYD inline static Scene* GetCurrentScene() { return s_CurrentScene; }
	protected:

		HYD void Loop();
	
	private:
		HYD void Event();
		HYD void Update();

	private:

		std::vector<Layer*> m_Layers;

		float m_deltaTime	= 0.0f;
		bool  m_Running		= false;

	private:
		static Scene* s_CurrentScene;

	protected:
		Window m_Window;
		
	};

};
