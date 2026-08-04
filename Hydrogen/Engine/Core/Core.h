/*
	
	The Core of Hydroge.
	Every Client must inherit a class from Core

*/

#pragma once
#include "../Common.h"
#include "../Log/Log.h"
#include "../Render/Renderer.h"
#include "Scene.h"
#include "Timer/Timer.h"

#include "Layer.h"

namespace Hydrogen
{


	class Core
	{
	public:
		
		HYD  Core(); //Default Constructor
		HYD  Core(WindowInfo pWindowInfo); //Constructor with a window
		HYD ~Core();


		HYD uint32 PushLayer(Ptr<Layer> pLayer);



	protected:
		HYD void Loop();
	
	private:
		HYD void Event();
		HYD void Update();

	public:
		HYD static Instance<Scene>   GetCurrentScene();
		HYD inline static float		 GetDeltaTime() { return s_DeltaTime; }

		HYD static Instance<Scene> LoadSceneGLTF(const std::string& pPath);

	private:
		HYD static void SetDeltaTime(float pDelta);

	private:

		std::vector<Ptr<Layer>> m_Layers;
		
		bool  m_Running = false;

	private:
		static float		       s_DeltaTime;
		static ResourcePool<Scene> s_Scenes;
		static Instance<Scene>	   s_CurrentScene;

		friend class Renderer;
	};

};
