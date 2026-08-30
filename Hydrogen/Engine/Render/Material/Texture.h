#pragma once

#include "../../Common.h"
#include "../Vulkan/Image.h"
#include "../Vulkan/Descriptors.h"

namespace Hydrogen
{



	enum Channels {CH_RED=1, CH_RG, CH_RGB, CH_RGBA};

	// Image Object
	struct ImageData
	{
		uint8*	 Data    = nullptr;
		int32	 Width   = 0;
		int32	 Height  = 0;
		Channels ImageChannel = CH_RGBA;


		 ImageData() {};
		 ImageData(void* pData, int32 pWidth, int32 pHeight);
		~ImageData();

		ImageData(ImageData&& pOther);
		ImageData(const ImageData& pOther);

		uint32 LoadImageFromDisk(const char* pSource);
		uint32 LoadImageFromMemory(Buffer pImageData, int32 pDesiredChannels=0);
		uint32 FreeImage();




	};



	struct TextureConfiguration
	{

		TextureConfiguration() = default;

		TextureConfiguration(TextureConfiguration&& pOther) 		   noexcept;
		TextureConfiguration& operator=(TextureConfiguration&& pOther) noexcept;


		Buffer 	         	 Data;
		SamplerConfiguration ImageSampler;
		uint32 				 TexCoordSet = 0;
		ShaderUniformBinding BindingLocation;
	};

	
	class Texture2D final
	{
	public:
	
		HYD  Texture2D() noexcept;
		HYD ~Texture2D() noexcept;

		//Copy Constructor
		HYD Texture2D(const Texture2D& pOther) noexcept;
		HYD Texture2D(Texture2D&& pOther) 	   noexcept;

		HYD Texture2D& operator=(const Texture2D& pOther)  noexcept;
		HYD Texture2D& operator=(Texture2D&& pOther) 	   noexcept;


		HYD uint32 CreateTexture(
			TextureConfiguration pConf
		)  noexcept;

		HYD void   DestroyTexture() noexcept;

		HYD void Bind();

	private:
		Internal::Vulkan::Image     m_ImageHandle;
		Internal::Vulkan::ImageView m_View;
		Internal::Vulkan::Sampler   m_Sampler;
		uint32 				        m_TexCoordSet = 0;
		ShaderUniformBinding        m_BindingLocation;

	private:
		friend class Material;
	};

};