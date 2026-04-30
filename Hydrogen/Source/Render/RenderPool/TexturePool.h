#pragma once

#include "Common.h"
#include "Core/ResourcePool.h"
#include "Render/Material/Textures/TextureBase.h"
#include "Render/Platform/OpenGL/GLTextures.h"

namespace Hydrogen
{

	class TexturePool
	{
	public:

		//Init & Shutdown
		HYD uint32 InitPool();
		HYD uint32 ShutdownPool();

		//Adding Texture:

		HYD Id     PushTexture(Wraper<TextureBase>& pTexture);
		HYD uint32 DestroyTexture(Id* pTextureID);


		//Binding:
		HYD uint32 BindTexture(  const Id pTexture, uint32 pSlot=0);
		HYD uint32 UnbindTexture(const Id pTexture, uint32 pSlot=0);

	public:

		HYD static Image GenerateMagneta(uint32 pWidth, uint32 pHeight, uint32 pNumPerRow);
		HYD static Wraper<TextureBase> CreateTexture2D(const Image& pImage, Sampler pSampler = Sampler());

	private:
		ResourcePool<TextureBase> m_Textures;
	};


};