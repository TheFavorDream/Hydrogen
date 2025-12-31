#include "Loader.h"

namespace Hydrogen
{

	SupportedAPIs Loader::m_CurrentAPI;

	int Loader::SetUpHydrogen(uint32 pGraphicalAPI)
	{
		if (pGraphicalAPI < 0 || pGraphicalAPI > OPENGL)
			return HYD_INVALID_API;

		m_CurrentAPI = (SupportedAPIs)pGraphicalAPI;

		switch (m_CurrentAPI)
		{
		case OPENGL:
			if (glewInit() != 0)
				return HYD_GLEW_FAILED;
			break;
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		return HYD_OK;
	}

	Model* Loader::Load(std::string pModelPath, int pFlag)
	{
		Model *model = new Model();
		uint32 Err = model->LoadModel(pModelPath);
		if (Err != HYD_OK)
		{
			model->Free();
			delete model;
			return nullptr;
		}
		return  model;
	}

	int Loader::Free(Model ** pModel)
	{
		(*pModel)->Free();
		delete *pModel;
		*pModel = 0;
		return HYD_OK;
	}
};