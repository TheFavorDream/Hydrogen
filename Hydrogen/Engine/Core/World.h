#pragma once

#include "Common.h"
#include "Scene.h"
#include "ResourcePool.h"

#include "Xenon/include/Xenon.h"

namespace Hydrogen
{

	class World
	{
	public:

		HYD  World();
		HYD ~World();


		//World object shouldn't have copy and move constructors.
		//it's constructed once in heap and passed around by a pointer
		HYD  World(const World& pOther)			   = delete;
		HYD  World(World&& pOther)				   = delete;
		HYD  World& operator=(const World& pOther) = delete;
		HYD  World& operator=(World&& pOther)	   = delete;


		//Loads a gltf asset and adds the scenes to the World:
		HYD uint32 LoadGltf(const std::string& pPath);

		HYD Ptr<Scene> GetCurrentScene();

		HYD uint32 Update(double pDeltaTime=1.0f);
		HYD uint32 Process();
		HYD uint32 Render();

	private:
		ResourcePool<Scene> m_Scenes;
		Instance<Scene>		m_CurrentScene;
	};

};