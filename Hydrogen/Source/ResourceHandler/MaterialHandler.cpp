#include "MaterialHandler.h"

namespace Hydrogen
{

	Id					 MaterialHandler::s_CurrentBindedMaterial = 0;
	std::vector<Id> MaterialHandler::s_MaterialIds;
	std::vector<Id> MaterialHandler::s_TextureIds;

	uint32 MaterialHandler::InitHandler()
	{
		return HYD_OK;
	}

	uint32 MaterialHandler::ShutdownHandler()
	{
		
		for (auto& id : s_TextureIds)
		{
			ResourceHandler::PopResource(&id);
		}

		for (auto& id : s_MaterialIds)
		{
			ResourceHandler::PopResource(&id);
		}

		return HYD_OK;
	}

	Id MaterialHandler::PushMaterial(Material* pMaterialObj)
	{
		Id ID = ResourceHandler::PushResource(pMaterialObj);
		s_MaterialIds.push_back(ID);
		return ID;
	}

	uint32 MaterialHandler::DestroyMaterial(Id* pMaterialID)
	{
		ResourceHandler::PopResource(pMaterialID);
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
		return *dynamic_cast<Material*>(ResourceHandler::GetResource(pMatID));
	}



	Id MaterialHandler::PushTexture(Texture* pTextureObj)
	{
		Id ID = ResourceHandler::PushResource(pTextureObj);
		s_TextureIds.push_back(ID);
		return ID;
	}

	Id MaterialHandler::Create2DTexture(const Image & pImage, Sampler pSampler)
	{
		Texture* texture = new Texture();
		texture->CreateTexture(pImage, pSampler);

		return PushTexture(texture);

	}

	Id MaterialHandler::Create2DTexture(std::string & pPath)
	{
		Texture* texture = new Texture();
		texture->CreateTexture(pPath);

		return PushTexture(texture);
	}

	uint32 MaterialHandler::DestroyTexture(Id * pTextureID)
	{
		ResourceHandler::PopResource(pTextureID);
		return HYD_OK;
	}


	uint32 MaterialHandler::BindTexture(const Id pTexture, uint32 Slot)
	{
		GetTexture(pTexture).Bind(Slot);
		return HYD_OK;
	}
	uint32 MaterialHandler::UnbindTexture(const Id pTexture)
	{
		GetTexture(pTexture).Unbind();
		return HYD_OK;
	}


	const Texture& MaterialHandler::GetTexture(const Id pTextureId)
	{
		return* dynamic_cast<Texture*>(ResourceHandler::GetResource(pTextureId));
	}

};