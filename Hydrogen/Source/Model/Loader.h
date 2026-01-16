#pragma once

#include "Common.h"
#include <string>
#include "Model.h"
#include "Glew/glew.h"
#include <vector>

namespace Hydrogen
{

	enum SupportedAPIs {NONE, OPENGL};

	class Loader
	{
	public:

		static int SetUpHydrogen(uint32 pGraphicalAPI);
		static Model* Load(std::string pModelPath, uint32 pFlag = 0, uint32* pErrorCode=nullptr);

		static int FreeModelPool();

		static const std::vector<Model*>& GetModels();

	private:
		static std::vector<Model*> m_Models;
		static SupportedAPIs m_CurrentAPI;
		friend class Model;
	};
};