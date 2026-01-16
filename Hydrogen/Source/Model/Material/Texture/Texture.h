#pragma once
#include "Common.h"
#include "Glew/glew.h"
#include "3rdParty/stb_image/stb_image.h"

namespace Hydrogen
{

	class Texture
	{
	public:

		Texture() = default;
		Texture(int32 pWidth, int32 pHeight, int32 pChannels=3, uint8* pData=nullptr);
		~Texture();

		Texture(const Texture& pOther); // Copy Constructor
		Texture(Texture&& pOther);//Move Constructor

		int CreateTexture(int32 pWidth, int32 pHeight, int32 pChannels = 3, uint8* pData = nullptr);
		int CreateTexture(std::string pPath);
		int DestroyTexture();

		void Bind(uint32 pSlot=0);
		void Unbind();
		
		inline int32 GetTextureID() { return m_TextureID; }
		inline int32 GetWidth() { return m_Width; }
		inline int32 GetHeight() { return m_Height; }

	private:

		GLenum RetiveChannels(int32 pChannelNumber);

	private:
		uint32 m_TextureID = 0;
		int32 m_Width = 0;
		int32 m_Height = 0;
		int32 m_Channels = 0;
	};

};
