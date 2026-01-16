/*
	
	The Core of Hydroge.
	Every Client must inherit a class from Core

*/

#pragma once

#include "Common.h"
#include "Model/Loader.h"
#include "Window/Window.h"
#include "Event/Mouse.h"
#include "Event/Keyboard.h"
#include "Layer.h"

#include <vector>


namespace Hydrogen
{

	class Core
	{
	public:
		
		Core(); //Default Constructor
		Core(int32 pWidth, int32 pHeight, const char* pTitle); //Constructor with a window
		~Core();


		int PushLayer(Layer* pLayer);

	protected:

		void Loop();
	
	private:

		void Event();
		void Update();
		void Render();

	private:

		float m_deltaTime =  0.0f;
		std::vector<Layer*> m_Layers;
		bool m_Running;

	protected:
		Window m_Window;
		
	};

};
