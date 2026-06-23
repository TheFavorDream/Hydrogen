#pragma once
#include "Common.h"
#include "Glew/glew.h"
#include "Log/Log.h"
#include "Render/Material/Image.h"

namespace Hydrogen
{

	static GLenum RetriveChannel(int32 pChannel);

	class GLTexture2D 
	{
	public:

		HYD GLTexture2D() = default;
		HYD GLTexture2D(Image& pImage, Sampler pSampler = Sampler());
		HYD ~GLTexture2D();
		
		HYD GLTexture2D(const GLTexture2D& pOther) = delete;
		HYD GLTexture2D(GLTexture2D&& pOther);//Move Constructor

		HYD GLTexture2D& operator=(const GLTexture2D& other) = delete;
		HYD GLTexture2D& operator=(GLTexture2D&& pOther) noexcept;


		HYD uint32 CreateTexture(const Image& pImage, Sampler pSampler = Sampler());
		HYD uint32 DestroyTexture();


		//Bind & Unbind:
		HYD void Bind(uint32 pSlot = 0) const;
		HYD void Unbind(uint32 pSlot)   const;

		HYD inline uint32 GetTextureID()  noexcept {return m_TextureID;}
		HYD inline uint32 GetWidth()	  noexcept {return m_Width;}
		HYD inline uint32 GetHeight()	  noexcept {return m_Height;}
		HYD inline uint32 GetChannels()   noexcept {return m_Channels;}

	private:
		uint32 m_TextureID = 0;
		uint32 m_Width     = 0;
		uint32 m_Height    = 0;
		uint32 m_Channels  = 0;
	};


	class GLTextureCube
	{
	public:

		//Constructors:
		HYD  GLTextureCube() = default;
		HYD ~GLTextureCube();

		HYD GLTextureCube(const GLTextureCube& pOther) = delete;
		HYD GLTextureCube(GLTextureCube&& pOther);//Move Constructor

		HYD GLTextureCube& operator=(const GLTextureCube& other) = delete;
		HYD GLTextureCube& operator=(GLTextureCube&& pOther);

		HYD uint32 CreateCubeMap(const std::array<Image, 6>& pImages, const std::array<Sampler, 6>& pSamplers = { Sampler() });
		HYD uint32 DestroyTexture();

		HYD void Bind(uint32 pSlot = 0) const;
		HYD void Unbind(uint32 pSlot)   const;

		//Getters:
		HYD inline uint32 GetTextureID()  noexcept { return m_TextureID; }
		HYD inline uint32 GetWidth()	  noexcept { return m_Width; }
		HYD inline uint32 GetHeight()	  noexcept { return m_Height; }
		HYD inline uint32 GetChannels()   noexcept { return m_Channels; }

	private:
		uint32 m_TextureID = 0;
		uint32 m_Width     = 0;
		uint32 m_Height    = 0;
		uint32 m_Channels  = 0;
	};


};
