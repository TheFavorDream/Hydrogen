/*

	Contains the information to present an image
*/

#pragma once

#include "Common.h"
#include "3rdParty/stb_image/stb_image.h"
#include "Timer/Timer.h"

namespace Hydrogen
{



	enum Filter {
		NEAREST			        = GL_NEAREST,
		LINEAR				    = GL_LINEAR,
		NEAREST_MIPMAP_NEAREST  = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST   = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR   = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR    = GL_LINEAR_MIPMAP_LINEAR
	};

	enum Wrap {
		CLAMP_TO_EDGE    = GL_CLAMP_TO_EDGE,
		MIRRORED_REPEAT  = GL_MIRRORED_REPEAT,
		REPEAT			 = GL_REPEAT
	};

	// Texture Sampler structure
	struct Sampler
	{
		Sampler() {}

		Filter Mag=LINEAR;
		Filter Min=LINEAR;

		Wrap WrapS= CLAMP_TO_EDGE;
		Wrap WrapT= CLAMP_TO_EDGE;
	};
	

	enum Channels {CH_RED=1, CH_RG, CH_RGB, CH_RGBA};

	// Image Object
	struct Image
	{
		uint8*	 Data    = nullptr;
		int32	 Width   = 0;
		int32	 Height  = 0;
		Channels ImageChannel = CH_RGBA;


		Image() {};
		Image(Image&& pOther) {
			Data   = pOther.Data;
			Width  = pOther.Width;
			Height = pOther.Height;
			ImageChannel = pOther.ImageChannel;

			pOther.Data = nullptr;
		}

		Image(const Image& pOther) = delete;

		uint32 LoadImage(const char* pSource)
		{
			Data = stbi_load(pSource, &Width, &Height, (int*)&ImageChannel, NULL);
			if (Data == nullptr)
				return HYD_IMAGE_FAILED;
			return HYD_OK;
		}

		uint32 LoadImage(uint8* pBuffer, const uint32 pLength)
		{
			//Data = stbi_loadf_from_memory(pBuffer, pLength, &Width, &Height, (int*)&ImageChannel, NULL);
			if (Data == nullptr)
				return HYD_IMAGE_FAILED;
			return HYD_OK;
		}

		uint32 FreeImage()
		{
			if (Data != nullptr)
				stbi_image_free(Data);

			Data = nullptr;
			return HYD_OK;
		}

		~Image()
		{
			FreeImage();

		}
	};

};
