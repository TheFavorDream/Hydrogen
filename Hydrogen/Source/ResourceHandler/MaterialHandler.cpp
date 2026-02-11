#include "MaterialHandler.h"

namespace Hydrogen
{

	Id					 MaterialHandler::s_CurrentBindedMaterial = 0;
	LinkedList<Material> MaterialHandler::s_Materials;

	uint32 MaterialHandler::InitHandler()
	{
		return HYD_OK;
	}

	uint32 MaterialHandler::ShutdownHandler()
	{
		s_Materials.Clear();
		return HYD_OK;
	}

	Id MaterialHandler::PushMaterial(Material&& pMaterialObj)
	{
		return s_Materials.AddTail(std::move(pMaterialObj));
	}

	uint32 MaterialHandler::DestroyMaterial(Id * pMaterialID)
	{
		s_Materials.Remove(pMaterialID);
		return HYD_OK;
	}

	Id MaterialHandler::CreateMaterial()
	{
		return 0;
	}

	const Material & MaterialHandler::GetMaterial(Id pMatID)
	{
		if (pMatID == 0)
			__debugbreak();
		return s_Materials.GetDataByID(pMatID);
	}

};