#include "HydPch.h"
#include "Core.h"
#include "Xenon/include/Xenon.h"
#include "Xenon/include/Loader.h"
#include "Render/Shader.h"

#include "Timer/Timer.h"

using namespace std::chrono_literals;

namespace Hydrogen
{

	ResourcePool<Scene> Core::s_Scenes;
	Instance<Scene>		Core::s_CurrentScene;
	float				Core::s_DeltaTime = 0.0f;

	Core::Core()
	{

	}

	Core::Core(WindowInfo pWindowInfo)
	{
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


		Mouse::InitMouse(Renderer::Self().GetWindowHandle());
		Keyboard::InitKeyboard(Renderer::Self().GetWindowHandle());

		Xenon::Log::SetLogCallBack(Log::CheckXenonErrors);

		//ShaderPool::s_Self = Memory::AllocateRaw<ShaderPool>();

		m_Running = true;
	}

	Core::~Core()
	{

		for (int i = 0; i < m_Layers.size(); i++)
		{
			m_Layers[i]->Shutdown();
			delete m_Layers[i];
		}

		s_CurrentScene.Reset();

		//Scene Shutdown:
		for (auto& scene : s_Scenes)
		{
			scene.Object.FreeScene();
		}
		s_Scenes.Shutdown();

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



	Instance<Scene> Core::GetCurrentScene()
	{
		ASSERT(!s_CurrentScene.IsNull(), "No Scene is Current!")
		return s_CurrentScene;
	}


	//TODO: Opmtimize this half-retarded pieace of shit
	Instance<Scene> Core::LoadSceneGLTF(const std::string & pPath)
	{

		//PROFILE_START("Model Loading");

		Xenon::Model model = Xenon::Loader::Load(pPath, Xenon::LF_BASE_NORMAL_ONLY);
		//Constructing the Scene Graph


		std::vector<Instance<Mesh>>		 MeshIns;
		std::vector<Instance<Scene>>     SceneIns;
		std::unordered_map<uint64, Instance<Texture2D>> TexIns;

		for (auto& scene : model)
		{
			Instance<Scene> CurrentScene = s_Scenes.Resource();

			//Set the name for the scene:
			CurrentScene->m_Name = scene->GetName();

			//Texture Loading:

			for (auto& texture : scene->GetTextures())
			{
				Log::SetInfo("Loading Texture");
				Xenon::BinaryData Img = texture.second.RetriveImageData();
				Image image; image.LoadImageFromMemory(Img.Ptr, Img.ByteLength);
				//TexIns[texture.first] = Renderer::CreateTexture2D(image , Sampler((uint32)texture.second.Sampler.Mag, (uint32)texture.second.Sampler.Mag, (uint32)texture.second.Sampler.WrapS, (uint32)texture.second.Sampler.WrapT));
			}

			//Mesh Creation:
			for (auto& mesh : scene->GetMeshes())
			{
				Mesh NewMesh = Mesh::CreateGLTFMesh(mesh);
				
				uint64 Index = 0;
				for (auto& pri : mesh)
				{
					NewMesh.GetPrimitve(Index++).m_Material = Material::CreateMaterialGLTF(pri.GetMatrial(), TexIns);
				}

				MeshIns.push_back(CurrentScene->m_Meshes.PushObject(std::move(NewMesh)));
			}


			//Node-Tree:

			using NODE = std::pair<Xenon::Node, Ptr<Node>>;

			std::stack<NODE> Travers;

			for (auto& node : *scene)
			{
				Travers.push(NODE(node, CurrentScene.GetPtr()));
			}

			Ptr<Node> Parent = nullptr;
			while (!Travers.empty())
			{
				NODE node = std::move(Travers.top());
				Travers.pop();

				//Create and Configure the node:
				Node NewNode;
				NewNode.GetName()		  = node.first.GetName();
				NewNode.m_PointerToParent = node.second;
				if (!node.first.IsMeshEmpty())
				{
					NewNode.GetMesh() = MeshIns.at(node.first.GetMeshIndex());
				}
				NewNode.m_Transform.t_Scale      = node.first.Scale();
				NewNode.m_Transform.t_Rotate     = node.first.Rotation();
				NewNode.m_Transform.t_Translate  = node.first.Translation();

				

				//Dumb shit
				//Push it to the parent node: (starting with scene as parent)
				NewNode.m_Children.reserve(node.first.ChildCount());
				node.second->m_Children.push_back(std::move(NewNode));
				Parent = &node.second->m_Children.at(node.second->m_Children.size() - 1);

				for (auto& Child : node.first)
				{
					Travers.push(NODE(Child, Parent));
				}

			}

			SceneIns.push_back(std::move(CurrentScene));
		}

		s_CurrentScene = SceneIns.at(model.DefaultScene());
		
		SceneIns.clear();
		MeshIns.clear();

		//PROFILE_STOP
		//std::cout << Profiler::

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
			Renderer::Self().Render();

			Renderer::Self().GetWindow().ProcessWindow(m_Running);
			timer.StopTimer();
			//Calculate delta Time:

			SetDeltaTime((float)timer.GetElapsedInMillis());
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