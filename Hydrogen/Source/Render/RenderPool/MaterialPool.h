#pragma once

#include "Common.h"
#include "Render/Platform/OpenGL/GLTexture.h"
#include "Render/Material/Material.h"
#include "Core/ResourcePool.h"

namespace Hydrogen
{

	class MaterialPool
	{
	public:


		HYD uint32 InitPool();
		HYD uint32 ShutdownPool();

		HYD Id     CreateMaterial();
		//HYD Id     CreateMaterial();
		HYD uint32 DestroyMaterial(Id* pMaterialID);

		HYD uint32 BindMaterial(Id pMaterial, const Shader& pShader);
		HYD uint32 UnbindMaterial(Id pMaterial);

		HYD Material& GetMaterial(Id pMatID);

	private:
		Id s_CurrentBindedMaterial;
		ResourcePool<Material> m_Materials;
	};

};