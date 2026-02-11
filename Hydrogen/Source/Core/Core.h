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
#include "ResourceHandler/BufferHandler.h"
#include "ResourceHandler/MaterialHandler.h"
#include "ResourceHandler/TextureHandler.h"
#include "Layer.h"

#include <vector>


namespace Hydrogen
{


	class Core
	{
	public:
		
		HYD  Core(); //Default Constructor
		HYD  Core(int32 pWidth, int32 pHeight, const char* pTitle, APIs pAPI=OPENGL); //Constructor with a window
		HYD ~Core();


		HYD int PushLayer(Layer* pLayer);

	protected:

		HYD void Loop();
	
	private:

		HYD uint32 InitAPI();

		HYD void Event();
		HYD void Update();
		HYD void Render();

	private:

		float m_deltaTime =  0.0f;
		std::vector<Layer*> m_Layers;
		bool m_Running;

		APIs m_GraphicAPI;

	protected:
		Window m_Window;
		
	};

};
