#pragma once
#include "Common.h"
#include "Glew/glew.h"
#include "Log/Log.h"
#include "Render/Material/Image.h"
#include "Render/Material/Textures/TextureBase.h"

namespace Hydrogen
{

	static GLenum RetriveChannel(int32 pChannel);

	class GLTexture2D : public TextureBase
	{
	public:

		HYD GLTexture2D() = default;
		HYD GLTexture2D(Image& pImage, Sampler pSampler = Sampler());
		HYD ~GLTexture2D();
		
		HYD GLTexture2D(const GLTexture2D& pOther) = delete;
		HYD GLTexture2D(GLTexture2D&& pOther);//Move Constructor

		HYD GLTexture2D& operator=(const GLTexture2D& other) = delete;
		HYD GLTexture2D& operator=(GLTexture2D&& pOther) noexcept;


		HYD uint32 CreateTexture(const Image& pImage, Sampler pSampler = Sampler()) override;
		HYD uint32 DestroyTexture() override;


		//Bind & Unbind:
		HYD void Bind(uint32 pSlot = 0) const override;
		HYD void Unbind(uint32 pSlot)   const override;
	};


	class GLTextureCube : public TextureCube
	{
	public:

		//Constructors:
		HYD  GLTextureCube() = default;
		HYD ~GLTextureCube();

		HYD GLTextureCube(const GLTextureCube& pOther) = delete;
		HYD GLTextureCube(GLTextureCube&& pOther);//Move Constructor

		HYD GLTextureCube& operator=(const GLTextureCube& other) = delete;
		HYD GLTextureCube& operator=(GLTextureCube&& pOther);

		HYD uint32 CreateCubeMap(const std::array<Image, 6>& pImages, const std::array<Sampler, 6>& pSamplers = { Sampler() }) override;
		HYD uint32 DestroyTexture() override;

		HYD void Bind(uint32 pSlot = 0) const  override;
		HYD void Unbind(uint32 pSlot)   const  override;

	};


};
