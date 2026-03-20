#include "MaterialPool.h"
#include "HydPch.h"


namespace Hydrogen
{


	uint32 MaterialPool::InitPool()
	{
		return HYD_OK;
	}

	uint32 MaterialPool::ShutdownPool()
	{
		m_Materials.Clear();
		return HYD_OK;
	}

	Id MaterialPool::CreateMaterial()
	{
		return 0;
	}

	uint32 MaterialPool::DestroyMaterial(Id* pMaterialID)
	{
		m_Materials.Pop(pMaterialID);
		return HYD_OK;
	}

	uint32 MaterialPool::BindMaterial(Id pMaterial, const Shader& pShader)
	{
		if (pMaterial == 0)
			return HYD_INVALID_VALUE;
		GetMaterial(pMaterial).Bind(pShader);
		return HYD_OK;
	}

	uint32 MaterialPool::UnbindMaterial(Id pMaterial)
	{
		if (pMaterial == 0)
			return HYD_INVALID_VALUE;
		GetMaterial(pMaterial).Unbind();
		return HYD_OK;
	}


	Material& MaterialPool::GetMaterial(Id pMatID)
	{
		if (pMatID == 0)
			__debugbreak();
		return m_Materials.GetResource(pMatID);
	}

};