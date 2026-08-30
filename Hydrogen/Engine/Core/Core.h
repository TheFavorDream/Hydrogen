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


		static Xenon::Model Load(const std::string& pPath, uint32 pFlags=Xenon::LF_DEFAULT);


	protected:
		HYD void Loop();
	
	private:
		HYD void Event();
		HYD void Update();

	public:
		HYD inline static float		 GetDeltaTime() { return s_DeltaTime; }
		HYD static Instance<Scene>   CreateScene() noexcept;

	private:
		HYD static void SetDeltaTime(float pDelta);

	private:

		std::vector<Ptr<Layer>> m_Layers;
		
		bool  m_Running = false;

	public:
		static float		       s_DeltaTime;
		static ResourcePool<Scene> s_Scenes;
		static Ptr<Core>		   s_Self;

		friend class Renderer;
	};

};
