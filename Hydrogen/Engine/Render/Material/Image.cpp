#include "Image.h"

namespace Hydrogen
{

	Image::Image(void* pData, int32 pWidth, int32 pHeight):
		Data((uint8*)pData), Width(pWidth), Height(pHeight), ImageChannel(CH_RGB)
	{

	}


	Image::Image(const Image& pOther)
	{

		Width		 = pOther.Width;
		Height       = pOther.Height;
		ImageChannel = pOther.ImageChannel;

		//TODO: Replace this with memory allocator:
		Data = (uint8*)Memory::AllocateMemory(Width*Height*ImageChannel);
		memcpy((void*)&Data[0], (void*)&pOther.Data[0], Width*Height*(int)ImageChannel);
	}

	Image::Image(Image&& pOther)
	{
		Data		 = pOther.Data;
		Width		 = pOther.Width;
		Height		 = pOther.Height;
		ImageChannel = pOther.ImageChannel;

		pOther.Data = nullptr;
	}

	uint32 Image::LoadImageFromDisk(const char* pSource)
	{
		Data = stbi_load(pSource, &Width, &Height, (int*)&ImageChannel, 0);
		if (Data == nullptr)
			return HYD_IMAGE_FAILED;
		return HYD_OK;
	}

	uint32 Image::LoadImageFromMemory(uint8* pBuffer, const uint64 pLength)
	{
		Data = stbi_load_from_memory(pBuffer, (int32)pLength, &Width, &Height, (int*)&ImageChannel, 0);
		if (Data == nullptr)
			return HYD_IMAGE_FAILED;
		return HYD_OK;
	}

	uint32 Image::FreeImage()
	{
		Memory::FreeMemory(Data);
		Data = nullptr;
		return HYD_OK;
	}


	Image::~Image()
	{
		FreeImage();
	}

};