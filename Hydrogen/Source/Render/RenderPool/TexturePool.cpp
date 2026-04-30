#include "TexturePool.h"
#include "HydPch.h"
#include "Render/Renderer.h"

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


	Id TexturePool::PushTexture(Wraper<TextureBase>& pTexture)
	{
		return m_Textures.Push(std::move(pTexture));
		pTexture = nullptr;
	}


	uint32 TexturePool::DestroyTexture(Id* pTextureID)
	{
		return m_Textures.Pop(pTextureID);
	}


	uint32 TexturePool::BindTexture(const Id pTexture, uint32 pSlot)
	{
		if (pTexture == 0)
			return HYD_INVALID_VALUE;

		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			dynamic_cast<GLTexture2D*>(&m_Textures.GetResource(pTexture))->Bind();
			return HYD_OK;
			
		}
		return HYD_INVALID_API;
	}

	uint32 TexturePool::UnbindTexture(const Id pTexture, uint32 pSlot)
	{
		if (pTexture == 0)
			return HYD_INVALID_VALUE;
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			dynamic_cast<GLTexture2D*>(&m_Textures.GetResource(pTexture))->Unbind(pSlot);
			return HYD_OK;
		}
		return HYD_INVALID_API;
	}



	Image TexturePool::GenerateMagneta(uint32 pWidth, uint32 pHeight, uint32 pNumPerRow)
	{

		Vec3<uint8>* Data = Memory::AllocateArray<Vec3<uint8>>(pWidth*pHeight); //(Vec3<uint8>*)Memory::AllocateMemory(pWidth * pHeight * sizeof(Vec3<uint8>));

		bool Pass = false;
		for (int CounterH = 0, i = 0; i < pWidth; i++)
		{
			CounterH += 1;
			if (CounterH >= (pWidth / pNumPerRow))
			{
				CounterH = 0;
				Pass = !Pass;
			}
		
			for (int CounterV = 0, j = 0; j < pHeight; j++)
			{
				CounterV += 1;
				if (CounterV >= (pHeight / pNumPerRow))
				{
					CounterV = 0;
					Pass = !Pass;
				}
		
				if (Pass)
					continue;
		
				Data[(i*pWidth) + j] = Vec3<uint8>(255, 0, 220);
			}
		}

		return std::move(Image(Data, pWidth, pHeight));
	}

	Wraper<TextureBase> TexturePool::CreateTexture2D(const Image& pImage, Sampler pSampler)
	{
		Wraper<TextureBase> texture;
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			texture.Set(new GLTexture2D());
			texture.Ptr->CreateTexture(pImage, pSampler);
			break;
		}
		return texture;
	}

};
