#pragma once

#include "Common.h"
#include "Render/Material/Material.h"
#include "Core/ResourcePool.h"

namespace Hydrogen
{

	class MaterialPool
	{
	public:


		HYD uint32 InitPool();
		HYD uint32 ShutdownPool();

		HYD Id	   Push(Wraper<Material>& pNewMaterial);
		HYD Id     CreateMaterial();
		HYD uint32 DestroyMaterial(Id* pMaterialID);

		HYD uint32 BindMaterial(Id pMaterial, const Shader& pShader);
		HYD uint32 UnbindMaterial(Id pMaterial);


		HYD Id GetDefaultMaterial();

		HYD Material& GetMaterial(Id pMatID);

	private:
		Id m_CurrentBindedMaterial = 0;
		Id m_DefaultMaterial	   = 0;
		ResourcePool<Material> m_Materials;
	};

};