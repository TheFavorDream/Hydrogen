#pragma once

#include "Common.h"
#include "Model/Model.h"
#include "Shader/Shader.h"
#include "Node.h"

#include <vector>
#include <string>

namespace Hydrogen
{

	class Scene
	{
	public:

		Scene(std::string pName);
		~Scene();

		void RenderScene(Shader& pShader, Model* pModel, Matrix* pModelTransformation = nullptr);

	private:

		//Name of the Scene
		std::string m_Name;

		//Refrence to Nodes:
		std::vector<Node*> m_Nodes;

		friend class Model;
	};

};