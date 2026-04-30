#include "HydPch.h"
#include "Core.h"

using namespace std::chrono_literals;

namespace Hydrogen
{


	Scene* Core::s_CurrentScene = nullptr;
	float Core::s_DeltaTime = 0.0f;

	Core::Core()
	{

	}

	Core::Core(int32 pWidth, int32 pHeight, const char* pTitle)
	{
#ifndef DIST
		Log::SetLevel(LV3);
		Log::EnableFile();
#else
		Log::SetLevel(LV0);
		Log::DisableFile();
#endif

		m_Window.MakeWindow(pWidth, pHeight, pTitle);
		Renderer::Init(API_OPENGL);


		Mouse::InitMouse(m_Window.GetWindow());
		Keyboard::InitKeyboard(m_Window.GetWindow());

		m_Window.SetViewportRatio(100.0f, 100.0f);


		m_Running = true;
	}

	Core::~Core()
	{

		for (int i = 0; i < m_Layers.size(); i++)
		{
			m_Layers[i]->Shutdown();
			delete m_Layers[i];
		}


		Renderer::Shutdown();

		m_Window.DestroyWindow();
		glfwTerminate();


		m_Running = false;

		Memory::CheckAllocation();
	}

	uint32 Core::PushLayer(Ptr<Layer> pLayer)
	{

		if (pLayer == nullptr)
		{
			return HYD_INVALID_VALUE;
		}

		pLayer->Setup();
		m_Layers.push_back(pLayer);
		return HYD_OK;
	}

	HYD uint32 Core::PushScene(Ptr<Scene> pScene)
	{
		if (pScene == nullptr)
			return HYD_INVALID_VALUE;
		s_CurrentScene = pScene;
		return HYD_OK;
	}

	Ptr<Scene> Core::GetCurrentScene()
	{
		ASSERT(s_CurrentScene==nullptr, "No Scene is Current!")
		return s_CurrentScene;
	}

	void Core::SetDeltaTime(float pDelta)
	{
		s_DeltaTime = pDelta;
	}

	void Core::Loop()
	{
		Timer timer;
		//Main Loop
		while (m_Running)
		{
			timer.StartTimer();
			Event();
			Update();
			Renderer::Render();

			m_Window.ProcessWindow(m_Running);


			timer.StopTimer();
			//Calculate delta Time:

			SetDeltaTime(timer.GetElapsedInMillis());
			timer.ResetTimer();


			
		}
	}




	void Core::Event()
	{
		glfwPollEvents();
		for (auto& i : m_Layers)
		{
			i->Event();
		}
	}

	void Core::Update()
	{
		for (auto& i : m_Layers)
		{
			i->Update();
		}
	}

}