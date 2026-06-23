#include "World.h"
#include "HydPch.h"

#include "Xenon/include/Loader.h"

namespace Hydrogen
{

/*

	World implementation:

*/

	World::World()
	{
	}

	World::~World()
	{
		m_Scenes.Shutdown();
	}



	uint32 World::Update(double pDeltaTime)
	{
		return HYD_OK;
	}

	uint32 World::Process()
	{
		return HYD_OK;
	}

	uint32 World::Render()
	{
		if (!m_CurrentScene.IsNull())
			m_CurrentScene->Render();
		return HYD_OK;
	}



	uint32 World::LoadGltf(const std::string& pPath)
	{
		Xenon::Model model = Xenon::Loader::Load(pPath, Xenon::LF_NO_MATERIAL);


		//Constructing the Scene Graph


		for (auto& scene : model)
		{
			Instance<Scene> NewScene = m_Scenes.Resource();

			//Set the name for the scene:
			NewScene->m_Name = scene->GetName();


			std::stack<Xenon::Node> Travers;

			for (auto& node : *scene)
			{
				Travers.push(node);
			}

			Ptr<HYD_VEC<Node>> ParentNode = &NewScene->m_Nodes;

			while (!Travers.empty())
			{
				Xenon::Node Current = Travers.top();
				Travers.pop();

				

				for (auto& node : Current)
				{
					Travers.push(node);
				}
			}

		}

		return HYD_OK;
	}
	
	Ptr<Scene> World::GetCurrentScene()
	{
		return m_CurrentScene.GetPtr();
	}


};