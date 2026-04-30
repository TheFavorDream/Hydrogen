#pragma once

#include "Common.h"
#include "Render/Material/Image.h"


namespace Hydrogen
{

	class TextureBase
	{
	public:

		//Constructors
		HYD TextureBase();

		HYD virtual ~TextureBase() = default;

		HYD TextureBase(const TextureBase& pOther) = delete;
		HYD TextureBase(TextureBase&& pOther);

		HYD TextureBase& operator=(const TextureBase& pOther) = delete;
		HYD TextureBase& operator=(TextureBase&& pOther);


		//Creators:
		HYD virtual uint32 CreateTexture(const Image& pImage, Sampler pSampler = Sampler()) = 0;
		HYD virtual uint32 DestroyTexture() = 0;

		//Read & Copy Texture:


		//Bind & Unbind:
		HYD virtual void Bind(uint32 pSlot = 0) const = 0;
		HYD virtual void Unbind(uint32 pSlot)   const = 0;

		//Getters:
		HYD inline uint32 GetTextureID() noexcept { return m_TextureID; }
		HYD inline uint32 GetWidth()	 noexcept { return m_Width; }
		HYD inline uint32 GetHeight()	 noexcept { return m_Height; }
		HYD inline uint32 GetChannels()	 noexcept { return m_Channels; }

	protected:
		uint32 m_TextureID = 0;
		uint32 m_Width     = 0;
		uint32 m_Height    = 0;
		uint32 m_Channels  = 0;
	};


	class TextureCube : public TextureBase
	{
	public:

		HYD  TextureCube();
		HYD ~TextureCube() = default;

		HYD TextureCube(const TextureCube& pOther) = delete;
		HYD TextureCube(TextureCube&& pOther);

		HYD TextureCube& operator=(const TextureCube& pOther) = delete;
		HYD TextureCube& operator=(TextureCube&& pOther);

		//Creators:

		//Creates an cube map with images and samplers
		HYD virtual uint32 CreateCubeMap(const std::array<Image, 6>& pImages, const std::array<Sampler, 6>& pSamplers = {Sampler()}) = 0;
		HYD virtual uint32 DestroyTexture() override = 0;

		//Read & Copy Texture:


		//Bind & Unbind:
		HYD virtual void Bind(uint32 pSlot = 0) const override = 0;
		HYD virtual void Unbind(uint32 pSlot)   const override = 0;

	protected:
	};

};