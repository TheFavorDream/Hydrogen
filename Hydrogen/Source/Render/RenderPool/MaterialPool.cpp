#include "MaterialPool.h"
#include "HydPch.h"

#include "Render/Renderer.h"

namespace Hydrogen
{


	uint32 MaterialPool::InitPool()
	{
		Wraper<Material> DefMat = new Material();


		DefMat.Ptr->SetBaseColorTexture(TexturePool::CreateTexture2D(std::move(TexturePool::GenerateMagneta(512, 512, 64))));

		m_DefaultMaterial = m_Materials.Push(std::move(DefMat));
		
		Renderer::SetDefaultMaterial(m_DefaultMaterial);
		return HYD_OK;
	}

	uint32 MaterialPool::ShutdownPool()
	{
		m_Materials.Clear();
		return HYD_OK;
	}

	Id MaterialPool::Push(Wraper<Material>& pNewMaterial)
	{
		return m_Materials.Push(pNewMaterial);
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


	HYD Id MaterialPool::GetDefaultMaterial()
	{
		ASSERT(m_DefaultMaterial == 0, "No Default Material has been setted");
		return m_DefaultMaterial;
	}

	Material& MaterialPool::GetMaterial(Id pMatID)
	{
		if (pMatID == 0)
			__debugbreak();
		return m_Materials.GetResource(pMatID);
	}

};