#include "TextureHandler.h"

namespace Hydrogen
{

	Id TextureHandler::s_CurrentTexture;
	LinkedList<Texture> TextureHandler::s_Textures;


	uint32 TextureHandler::InitHandler()
	{
		return HYD_OK;
	}

	uint32 TextureHandler::ShutdownHandler()
	{
		s_Textures.Clear();
		return HYD_OK;
	}


	Id TextureHandler::PushTexture(Texture && pTextureObj)
	{
		return s_Textures.AddTail(std::move(pTextureObj));
	}

	Id TextureHandler::Create2DTexture(const Image & pImage, Sampler pSampler)
	{
		Texture tex;
		tex.CreateTexture(pImage, pSampler);
		return s_Textures.AddTail(std::move(tex));

	}

	Id TextureHandler::Create2DTexture(std::string & pPath)
	{
		Texture tex;
		tex.CreateTexture(pPath);
		return s_Textures.AddTail(std::move(tex));
	}

	uint32 TextureHandler::DestroyTexture(Id * pTextureID)
	{
		s_Textures.Remove(pTextureID);
		return HYD_OK;
	}


	uint32 TextureHandler::BindTexture(const Id pTexture, uint32 Slot)
	{
		GetTexture(pTexture).Bind(Slot);
		return HYD_OK;
	}
	uint32 TextureHandler::UnbindTexture(const Id pTexture)
	{
		GetTexture(pTexture).Unbind();
		return HYD_OK;
	}


	const Texture & TextureHandler::GetTexture(const Id pTextureId)
	{
		return s_Textures.GetDataByID(pTextureId);
	}
};