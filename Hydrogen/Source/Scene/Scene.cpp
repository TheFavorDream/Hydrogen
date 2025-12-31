#include "Scene.h"

namespace Hydrogen
{
	Scene::Scene(std::string pName)
		: m_Name(pName)
	{

	}

	Scene::~Scene()
	{
		m_Nodes.clear();
	}


	void Scene::RenderScene(Shader& pShader, Model* pModel)
	{
		for (auto& i : m_Nodes)
		{
			i->RenderMesh(pShader, pModel);
		}
	}
};