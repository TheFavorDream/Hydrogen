#pragma once

#include "Common.h"
#include "Misc/LinkedList.h"
#include "Model/Material/Texture/Texture.h"

namespace Hydrogen
{

	class TextureHandler
	{
	public:

		//Init and Shutdown:
		HYD static uint32 InitHandler();
		HYD static uint32 ShutdownHandler();

		//Adding Texture:

		HYD static Id     PushTexture(Texture&& pTextureObj);
		HYD static Id	  Create2DTexture(const Image& pImage, Sampler pSampler = Sampler());
		HYD static Id	  Create2DTexture(std::string& pPath);
		HYD static uint32 DestroyTexture(Id* pTextureID);


		//Binding:
		HYD static uint32 BindTexture(const Id pTexture, uint32 Slot=0);
		HYD static uint32 UnbindTexture(const Id pTexture);


		HYD static const Texture& GetTexture(const Id pTextureId);

	private:
		static LinkedList<Texture> s_Textures;
		static Id s_CurrentTexture;
	};


};
