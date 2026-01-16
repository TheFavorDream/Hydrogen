#include "Loader.h"

namespace Hydrogen
{

	SupportedAPIs Loader::m_CurrentAPI;
	std::vector<Model*> Loader::m_Models;


	int Loader::SetUpHydrogen(uint32 pGraphicalAPI)
	{
		if (pGraphicalAPI < 0 || pGraphicalAPI > OPENGL)
		{
			Log::SetError(HYD_INVALID_API, __FILE__, __LINE__);
			return HYD_INVALID_API;
		}
		m_CurrentAPI = (SupportedAPIs)pGraphicalAPI;

		switch (m_CurrentAPI)
		{
		case OPENGL:
			if (glewInit() != 0)
			{
				Log::SetError(HYD_GLEW_FAILED, __FILE__, __LINE__);
				return HYD_GLEW_FAILED;
			}

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_MULTISAMPLE);

			break;
		}
		return HYD_OK;
	}



	Model* Loader::Load(std::string pModelPath, uint32 pFlag, uint32* pErrorCode)
	{
		Model *model = new Model();
		m_Models.push_back(model);
		uint32 Err = model->LoadModel(pModelPath);
		if (Err != HYD_OK)
		{
			model->Free();
			if (pErrorCode != nullptr)
				*pErrorCode = Err;
		}
		return  m_Models.at(m_Models.size()-1);
	}

	int Loader::FreeModelPool()
	{
		for (auto &i : m_Models)
		{
			i->Free();
			delete i;
			i = nullptr;
		}
		return HYD_OK;
	}
	const std::vector<Model*>& Loader::GetModels()
	{
		return m_Models;
	}
};