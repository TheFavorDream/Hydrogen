#pragma once

#include "Common.h"
#include "Misc/LinkedList.h"
#include "Model/Material/Material.h"
#include "Model/Material/Texture/Texture.h"


namespace Hydrogen
{

	class MaterialHandler
	{
	public:


		HYD static uint32 InitHandler();
		HYD static uint32 ShutdownHandler();

		HYD static Id	  PushMaterial(Material&& pMaterialObj);
		HYD static uint32 DestroyMaterial(Id* pMaterialID);

		HYD static Id CreateMaterial();

		HYD static const Material& GetMaterial(Id pMatID);

	private:
		static Id s_CurrentBindedMaterial;

		static LinkedList<Material> s_Materials;
	};

};