#pragma once

#include "Common.h"
#include "ResourceHandler.h"
#include "Model/Material/Material.h"
#include "Model/Material/Texture/Texture.h"


namespace Hydrogen
{

	class MaterialHandler
	{
	public:


		HYD static uint32 InitHandler();
		HYD static uint32 ShutdownHandler();

		HYD static Id	  PushMaterial(Material* pMaterialObj);
		HYD static uint32 DestroyMaterial(Id* pMaterialID);

		HYD static Id CreateMaterial();

		HYD static const Material& GetMaterial(Id pMatID);


		//Adding Texture:

		HYD static Id     PushTexture(Texture* pTextureObj);
		HYD static Id	  Create2DTexture(const Image& pImage, Sampler pSampler = Sampler());
		HYD static Id	  Create2DTexture(std::string& pPath);
		HYD static uint32 DestroyTexture(Id* pTextureID);


		//Binding:
		HYD static uint32 BindTexture(const Id pTexture, uint32 Slot = 0);
		HYD static uint32 UnbindTexture(const Id pTexture);


		HYD static const Texture& GetTexture(const Id pTextureId);

	private:
		static Id s_CurrentBindedMaterial;
		HYD static std::vector<Id> s_MaterialIds;
		HYD static std::vector<Id> s_TextureIds;
	};

};