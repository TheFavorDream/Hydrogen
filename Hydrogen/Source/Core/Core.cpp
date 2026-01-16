#include "Core.h"


namespace Hydrogen
{
	Core::Core()
	{

	}

	Core::Core(int32 pWidth, int32 pHeight, const char* pTitle)
	{
		m_Window.MakeWindow(pWidth, pHeight, pTitle);
		Loader::SetUpHydrogen(OPENGL);

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
		m_Running = false;

		Loader::FreeModelPool();

		Log::EngineDump("Dump.txt");
	}

	int Core::PushLayer(Layer * pLayer)
	{

		if (pLayer == nullptr)
		{
			return HYD_INVALID_VALUE;
		}

		pLayer->Setup();
		m_Layers.push_back(pLayer);
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
			Render();

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

	void Core::Render()
	{
		GL_CALL(glClearColor(0.0f, 0.0f, 0.0, 1.0f));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		for (auto& i : m_Layers)
		{
			i->Render();
		}

		m_Window.ProcessWindow(m_Running);
	}

}