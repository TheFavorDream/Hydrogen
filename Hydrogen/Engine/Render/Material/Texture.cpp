#include "Texture.h"
#include "HydPch.h"
#include "../Vulkan/Image.h"
#include "../Vulkan/Buffer.h"
#include "../Renderer.h"
#include "../../../3rdParty/stb_image/stb_image.h"


namespace Hydrogen
{


	ImageData::ImageData(void* pData, int32 pWidth, int32 pHeight):
		Data((uint8*)pData), Width(pWidth), Height(pHeight), ImageChannel(CH_RGB)
	{

	}


	ImageData::ImageData(const ImageData& pOther)
	{

		Width		 = pOther.Width;
		Height       = pOther.Height;
		ImageChannel = pOther.ImageChannel;

		//TODO: Replace this with memory allocator:
		Data = (uint8*)Memory::AllocateMemory(Width*Height*ImageChannel);
		memcpy((void*)&Data[0], (void*)&pOther.Data[0], Width*Height*(int)ImageChannel);
	}

	ImageData::ImageData(ImageData&& pOther)
	{
		Data		 = pOther.Data;
		Width		 = pOther.Width;
		Height		 = pOther.Height;
		ImageChannel = pOther.ImageChannel;

		pOther.Data = nullptr;
	}

	uint32 ImageData::LoadImageFromDisk(const char* pSource)
	{
		Data = stbi_load(pSource, &Width, &Height, (int*)&ImageChannel, 0);
		if (Data == nullptr)
			return HYD_IMAGE_FAILED;
		return HYD_OK;
	}

	uint32 ImageData::LoadImageFromMemory(Buffer pImageData, int32 pDesiredChannels)
	{
		Data = stbi_load_from_memory(pImageData.GetPtr(), pImageData.Length(), &Width, &Height, (int*)&ImageChannel, pDesiredChannels);
		if (Data == nullptr)
			return HYD_IMAGE_FAILED;
		return HYD_OK;
	}

	uint32 ImageData::FreeImage()
	{
		Memory::FreeMemory(Data);
		Data = nullptr;
		return HYD_OK;
	}


	ImageData::~ImageData()
	{
		FreeImage();
	}




	TextureConfiguration::TextureConfiguration(
		TextureConfiguration&& pOther
	) noexcept
	 : Data(std::move(pOther.Data)), ImageSampler(pOther.ImageSampler), BindingLocation(pOther.BindingLocation)
	{
		pOther.ImageSampler    = SamplerConfiguration{};
		pOther.BindingLocation = ShaderUniformBinding{};
	}

	TextureConfiguration& TextureConfiguration::operator=(TextureConfiguration&& pOther) noexcept
	{

		Data 		    = std::move(pOther.Data);
		ImageSampler 	= pOther.ImageSampler;
		BindingLocation = pOther.BindingLocation;

		pOther.ImageSampler    = SamplerConfiguration{};
		pOther.BindingLocation = ShaderUniformBinding{};
	}




/*

	Texture2D Implementation:

*/

	Texture2D::Texture2D() noexcept
	{
	}
	Texture2D::~Texture2D() noexcept
	{
		DestroyTexture();
	}

	//Copy Constructor
	Texture2D::Texture2D(const Texture2D& pOther)  noexcept
	{
		ASSERT(false, "Not Implemented");
	}
	Texture2D::Texture2D(Texture2D&& pOther) 	   noexcept
	{
		ASSERT(false, "Not Implemented");

	}

	Texture2D& Texture2D::operator=(const Texture2D& pOther)   noexcept
	{
		ASSERT(false, "Not Implemented");

		return *this;
	}
	Texture2D& Texture2D::operator=(Texture2D&& pOther) 	   noexcept
	{
		ASSERT(false, "Not Implemented");

		return *this;
	}


	uint32 Texture2D::CreateTexture(
		TextureConfiguration pConf
	)  noexcept
	{

		ImageData ImgData; ImgData.LoadImageFromMemory(pConf.Data, 4);

		uint32 ImageLength = ImgData.Width*ImgData.Height*4	;

		ImageConfiguration ImageConf;
		ImageConf.ImageSize               = Vec3<uint32>(ImgData.Width, ImgData.Height, 1);
		ImageConf.ArrayLayers             = 1;
		ImageConf.MipMapLevels            = 1;
		ImageConf.InitialLayout           = HYD_IMAGE_LAYOUT_UNDEFINED;
		ImageConf.Format                  = Internal::Vulkan::Image::ChannelToImageFormat(ImgData.ImageChannel);
		ImageConf.SampleCount             = HYD_SAMPLE_COUNT_1_BIT;
		ImageConf.Usage                   = HYD_IMAGE_USAGE_SAMPLED_BIT | HYD_IMAGE_USAGE_TRANSFER_DST_BIT; 
		ImageConf.Type 			          = HYD_IMAGE_TYPE_2D;
		ImageConf.SharingMode.SharingMode = VK_SHARING_MODE_EXCLUSIVE;

		CHECK_ERROR(m_ImageHandle.CreateImage(ImageConf));

		//Transition Layout to Transfer Dst optimal

		m_ImageHandle.TransitionLayout(
			HYD_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			HYD_PIPELINE_STAGE_TOP_OF_PIPE,
			HYD_PIPELINE_STAGE_TRANSFER
		);



		//Coping the image from stage buffer to actual image
		Internal::Vulkan::StagingBuffer CopyBuffer;
		CopyBuffer.CreateBuffer(ImageLength);
		CopyBuffer.UploadData(ImgData.Data, ImageLength);
		CopyBuffer.CopyImage(m_ImageHandle);
		CopyBuffer.DestroyBuffer();


		//Transition Layout to Shader read optimal

		m_ImageHandle.TransitionLayout(
			HYD_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			HYD_PIPELINE_STAGE_TRANSFER,
			HYD_PIPELINE_STAGE_FRAGMENT_SHADER
		);


		ImageViewConfiguration ViewConf;
		ViewConf.Image 	      = &m_ImageHandle;
		ViewConf.ViewType     = HYD_IMAGE_VIEW_TYPE_2D;
		ViewConf.Format       = ImageConf.Format;
		ViewConf.SubResources = VkImageSubresourceRange{
			.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel   = 0,
			.levelCount 	= 1,
			.baseArrayLayer = 0,
			.layerCount     = 1
		};

		CHECK_ERROR(m_View.CreateImageView(ViewConf));

		//Create the Sampler:
		CHECK_ERROR(m_Sampler.CreateSampler(pConf.ImageSampler));

		return HYD_OK;
	}

	void  Texture2D::DestroyTexture() noexcept
	{
		m_ImageHandle.DestroyImage();
		m_View.DestroyImageView();
		m_Sampler.DestroySampler();
	}

};