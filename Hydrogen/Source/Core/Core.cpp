#include "HydPch.h"
#include "Core.h"


namespace Hydrogen
{


	Scene* Core::s_CurrentScene = nullptr;

	Core::Core()
	{

	}

	Core::Core(int32 pWidth, int32 pHeight, const char* pTitle)
	{
		Log::SetLevel(LV3);
		Log::EnableFile();

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
	}

	uint32 Core::PushLayer(Layer* pLayer)
	{

		if (pLayer == nullptr)
		{
			return HYD_INVALID_VALUE;
		}

		pLayer->Setup();
		m_Layers.push_back(pLayer);
		return HYD_OK;
	}

	HYD uint32 Core::PushScene(Scene* pScene)
	{
		if (pScene == nullptr)
			return HYD_INVALID_VALUE;
		s_CurrentScene = pScene;
		return HYD_OK;
	}

	void Core::Loop()
	{
		static double LastTime = 0;

		//Main Loop
		while (m_Running)
		{
			Event();
			Update();
			Renderer::Render();

			m_Window.ProcessWindow(m_Running);

			//Calculate delta Time:
			double Current = glfwGetTime();
			m_deltaTime = float(Current - LastTime) * 1000.0f;
			LastTime = Current;
		}
	}




	void Core::Event()
	{
		glfwPollEvents();
		for (auto& i : m_Layers)
		{
			i->Event(m_deltaTime);
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