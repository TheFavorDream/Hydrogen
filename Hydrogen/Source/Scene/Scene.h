#pragma once

#include "../Common.h"

#include <vector>
#include <string>
#include "../Shader/Shader.h"
#include "Node.h"
#include "../Model/Model.h"

namespace Hydrogen
{

	class Scene
	{
	public:

		Scene(std::string pName);
		~Scene();

		void RenderScene(Shader& pShader, Model* pModel);

	private:

		//Name of the Scene
		std::string m_Name;

		//Refrence to Nodes:
		std::vector<Node*> m_Nodes;

		friend class Model;
	};

};