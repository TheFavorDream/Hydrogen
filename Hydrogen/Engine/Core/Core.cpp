#include "HydPch.h"
#include "Core.h"
#include "../../Xenon/include/Xenon.h"
#include "../../Xenon/include/Loader.h"

#include "Timer/Timer.h"

using namespace std::chrono_literals;


namespace Hydrogen
{
	




	ResourcePool<Scene> Core::s_Scenes;
	float				Core::s_DeltaTime = 0.0f;
	Ptr<Core>			Core::s_Self      = nullptr;

	Core::Core()
	{
		s_Self = this;
	}

	Core::Core(WindowInfo pWindowInfo)
	{

		s_Self = this;

#ifndef DIST
		Log::SetLevel(LV3);
		Log::EnableFile();
#else
		Log::SetLevel(LV0);
		Log::DisableFile();
#endif


		//Renderer Init
		Renderer::s_Self = Memory::AllocateRaw<Renderer>();
		Renderer::s_Self->Init(pWindowInfo);


		Mouse::InitMouse(Renderer::Self().GetWindow().GetHandle());
		Keyboard::InitKeyboard(Renderer::Self().GetWindow().GetHandle());

		Xenon::Log::SetLogCallBack(Log::CheckXenonErrors);

		//ShaderPool::s_Self = Memory::AllocateRaw<ShaderPool>();

		m_Running = true;

		//m_Grid.GenerateGrid();
	}

	Core::~Core()
	{

		Renderer::Self().WaitOnDeviceCompletion();

		//Scene Shutdown:
		for (auto& scene : s_Scenes)
		{
			scene.Object.FreeScene();
		}
		s_Scenes.Shutdown();

		
		for (int i = 0; i < m_Layers.size(); i++)
		{
			m_Layers[i]->Shutdown();
			delete m_Layers[i];
		}
		
		Renderer::s_Self->Shutdown();
		delete Renderer::s_Self;
	
		

		//delete ShaderPool::s_Self;

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



	Xenon::Model Core::Load(const std::string& pPath, uint32 pFlags)
	{
		return Xenon::Loader::Load(pPath, pFlags);
	}

	Instance<Scene> Core::CreateScene() noexcept
	{
		Instance<Scene> scene = Core::s_Scenes.Resource();
		return scene;
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
			Renderer::Self().Render();
			timer.StopTimer();
			//Calculate delta Time:

			SetDeltaTime((float)timer.GetElapsedInMillis());
			timer.ResetTimer();
		}
	}




	void Core::Event() noexcept
	{
		glfwPollEvents();
		for (int32 Iter = m_Layers.size()-1 ; Iter >= 0 ; --Iter)
		{
			m_Layers.at(Iter)->Event(FrameEvent::Self);
		}
		FrameEvent::Self.Reset();
	}

	void Core::Update() noexcept
	{
		for (auto& i : m_Layers)
		{
			i->Update();
		}
	}


	void Core::Render() noexcept
	{


		////Grid:
		//Renderer::Self().PushPrimitive(dynamic_cast<Primitive*>(&m_Grid));

		Renderer::Self().Render();
	}

}