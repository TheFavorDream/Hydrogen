#include "Image.h"

namespace Hydrogen
{

	Image::Image(Image&& pOther)
	{
		Data		 = pOther.Data;
		Width		 = pOther.Width;
		Height		 = pOther.Height;
		ImageChannel = pOther.ImageChannel;

		pOther.Data = nullptr;
	}

	uint32 Image::LoadImage(const char* pSource)
	{
		Data = stbi_load(pSource, &Width, &Height, (int*)&ImageChannel, NULL);
		if (Data == nullptr)
			return HYD_IMAGE_FAILED;
		return HYD_OK;
	}

	uint32 Image::LoadImage(uint8* pBuffer, const uint32 pLength)
	{
		//Data = stbi_loadf_from_memory(pBuffer, pLength, &Width, &Height, (int*)&ImageChannel, NULL);
		if (Data == nullptr)
			return HYD_IMAGE_FAILED;
		return HYD_OK;
	}

	uint32 Image::FreeImage()
	{
		if (Data != nullptr)
			stbi_image_free(Data);

		Data = nullptr;
		return HYD_OK;
	}

	Image::~Image()
	{
		FreeImage();
	}

};