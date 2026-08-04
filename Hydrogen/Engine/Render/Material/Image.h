/*

	Contains the information to present an image
*/

#pragma once

#include "../../Common.h"
#include "stb_image/stb_image.h"

namespace Hydrogen
{



	enum Filter {
		NEAREST			        ,
		LINEAR				    ,
		NEAREST_MIPMAP_NEAREST  ,
		LINEAR_MIPMAP_NEAREST   ,
		NEAREST_MIPMAP_LINEAR   ,
		LINEAR_MIPMAP_LINEAR    
	};

	enum Wrap {
		CLAMP_TO_EDGE    ,
		MIRRORED_REPEAT  ,
		REPEAT			 
	};

	// Texture Sampler structure
	struct Sampler
	{
		Sampler() {}
		Sampler(uint32 pMag, uint32 pMin, uint32 pWrapS, uint32 pWrapT) :
			Mag((Filter)pMag), Min((Filter)pMin), WrapS((Wrap)pWrapS), WrapT((Wrap)pWrapT)
		{}


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
		Image(void* pData, int32 pWidth, int32 pHeight);
		~Image();

		Image(Image&& pOther);
		Image(const Image& pOther);

		uint32 LoadImageFromDisk(const char* pSource);
		uint32 LoadImageFromMemory(uint8* pBuffer, const uint64 pLength);
		uint32 FreeImage();

	};

};
