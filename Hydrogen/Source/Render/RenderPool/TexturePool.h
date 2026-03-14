#pragma once

#include "Common.h"
#include "Core/ResourcePool.h"
#include "Render/Platform/OpenGL/GLTexture.h"

namespace Hydrogen
{

	class TexturePool
	{
	public:

		//Init & Shutdown
		HYD uint32 InitPool();
		HYD uint32 ShutdownPool();

		//Adding Texture:

		HYD Id     PushTexture(Texture** pTexture);

		HYD Id	   Create2DTexture(const Image& pImage, Sampler pSampler = Sampler());
		HYD Id	   Create2DTexture(std::string& pPath);
		HYD uint32 DestroyTexture(Id* pTextureID);


		//Binding:
		HYD uint32 BindTexture(  const Id pTexture, uint32 pSlot=0);
		HYD uint32 UnbindTexture(const Id pTexture, uint32 pSlot=0);


		HYD const Texture& GetTexture(const Id pTextureId);

	private:
		ResourcePool<Texture> m_Textures;
	};


};