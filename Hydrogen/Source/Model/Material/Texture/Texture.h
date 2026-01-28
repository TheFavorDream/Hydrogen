#pragma once
#include "Common.h"
#include "Glew/glew.h"
#include "3rdParty/stb_image/stb_image.h"
#include "Log/Log.h"

namespace Hydrogen
{

	class Texture
	{
	public:

		HYD Texture() = default;
		HYD Texture(int32 pWidth, int32 pHeight, int32 pChannels=3, uint8* pData=nullptr);
		HYD ~Texture();
		
		HYD Texture(const Texture& pOther); // Copy Constructor
		HYD Texture(Texture&& pOther);//Move Constructor
		
		HYD int CreateTexture(int32 pWidth, int32 pHeight, int32 pChannels = 3, uint8* pData = nullptr);
		HYD int CreateTexture(std::string pPath);
		HYD int DestroyTexture();
		
		HYD void Bind(uint32 pSlot=0);
		HYD void Unbind();
		 
		HYD inline int32 GetTextureID() { return m_TextureID; }
		HYD inline int32 GetWidth() { return m_Width; }
		HYD inline int32 GetHeight() { return m_Height; }

	private:

		GLenum RetiveChannels(int32 pChannelNumber);

	private:
		uint32 m_TextureID = 0;
		int32 m_Width = 0;
		int32 m_Height = 0;
		int32 m_Channels = 0;
	};

};
