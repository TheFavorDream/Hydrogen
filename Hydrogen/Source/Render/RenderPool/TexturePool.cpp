#include "TexturePool.h"

namespace Hydrogen
{
	uint32 TexturePool::InitPool()
	{
		return HYD_OK;
	}


	uint32 TexturePool::ShutdownPool()
	{
		return HYD_OK;
	}


	Id TexturePool::PushTexture(Texture** pTexture)
	{
		return m_Textures.Push(*pTexture);
		pTexture = nullptr;
	}

	Id TexturePool::Create2DTexture(const Image& pImage, Sampler pSampler)
	{
		Texture* texture = ResourcePool<Texture>::New();
		texture->CreateTexture(pImage, pSampler);

		return m_Textures.Push(texture);

	}

	Id TexturePool::Create2DTexture(std::string & pPath)
	{
		Texture* texture = ResourcePool<Texture>::New();
		texture->CreateTexture(pPath);

		return m_Textures.Push(texture);
	}

	uint32 TexturePool::DestroyTexture(Id* pTextureID)
	{
		return m_Textures.Pop(pTextureID);
	}


	uint32 TexturePool::BindTexture(const Id pTexture, uint32 pSlot)
	{
		if (pTexture == 0)
			return HYD_INVALID_VALUE;
		GetTexture(pTexture).Bind(pSlot);
		return HYD_OK;
	}
	uint32 TexturePool::UnbindTexture(const Id pTexture, uint32 pSlot)
	{
		if (pTexture == 0)
			return HYD_INVALID_VALUE;
		GetTexture(pTexture).Unbind(pSlot);
		return HYD_OK;
	}


	const Texture& TexturePool::GetTexture(const Id pTextureId)
	{
		return m_Textures.GetResource(pTextureId);
	}

};
