/*
	
	The Core of Hydroge.
	Every Client must inherit a class from Core

*/

#pragma once
#include "Common.h"
#include "Log/Log.h"
#include "Render/Renderer.h"
#include "Window/Window.h"
#include "World.h"
#include "Timer/Timer.h"

#include "Layer.h"

namespace Hydrogen
{


	class Core
	{
	public:
		
		HYD  Core(); //Default Constructor
		HYD  Core(int32 pWidth, int32 pHeight, const char* pTitle); //Constructor with a window
		HYD ~Core();


		HYD uint32 PushLayer(Ptr<Layer> pLayer);



	protected:
		HYD void Loop();
	
	private:
		HYD void Event();
		HYD void Update();

	public:
		HYD static uint32		PushWorld(Ptr<World> pScene);
		HYD static Ptr<World>   GetCurrentWorld();
		HYD inline static float GetDeltaTime() { return s_DeltaTime; }
	private:
		HYD static void SetDeltaTime(float pDelta);

	private:

		std::vector<Ptr<Layer>> m_Layers;

		bool  m_Running		= false;

	private:
		static float s_DeltaTime;
		static Ptr<World> s_CurrentWorld;

	protected:
		Window m_Window;
		
	};

};
