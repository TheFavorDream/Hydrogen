#pragma once
#include "Common.h"
#include "Glew/glew.h"
#include "3rdParty/stb_image/stb_image.h"
#include "Log/Log.h"
#include "Image.h"
#include "ResourceHandler/ResourceHandler.h"

namespace Hydrogen
{

	class Texture : public Resource
	{
	public:

		HYD Texture() = default;
		HYD Texture(Image& pImage, Sampler pSampler = Sampler());
		HYD ~Texture();
		
		HYD Texture(const Texture& pOther) = delete;
		HYD Texture(Texture&& pOther);//Move Constructor

		HYD Texture& operator=(const Texture& other) = delete;
		HYD Texture& operator=(Texture&& pOther) noexcept;

		HYD int CreateTexture(const Image& pImage, Sampler pSampler = Sampler());
		HYD int CreateTexture(std::string pPath);
		HYD int DestroyTexture();
		
		HYD void Bind(uint32 pSlot=0) const;
		HYD void Unbind() const;
		 
		HYD inline int32 GetTextureID() { return m_TextureID; }
		HYD inline int32 GetWidth() { return m_Width; }
		HYD inline int32 GetHeight() { return m_Height; }

	private:

		HYD GLenum RetriveChannel(int32 pChannel);

	private:
		uint32 m_TextureID = 0;
		int32 m_Width = 0;
		int32 m_Height = 0;
		int32 m_Channels = 0;
	};

};
