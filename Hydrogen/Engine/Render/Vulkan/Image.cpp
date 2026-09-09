#include "Image.h"
#include "../Renderer.h"
#include "Commands.h"
#include "VkEnumReDefs.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>


namespace Hydrogen
{
namespace Internal
{

    Vulkan::Image:: Image() noexcept
    {

    }
    Vulkan::Image::~Image() noexcept
    {

    }


    //Vulkan::Image::Image(const Vulkan::Image& pOther) noexcept
    //{
    //    ASSERT(false, "not implemented yet");
    //}

    Vulkan::Image::Image(Vulkan::Image&& pOther)      noexcept
    {

        m_Handle      = pOther.m_Handle;
        m_Format      = pOther.m_Format;
        m_Type        = pOther.m_Type;
        m_SampleCount = pOther.m_SampleCount;
        m_Width       = pOther.m_Width;
        m_Heihgt      = pOther.m_Heihgt;
        m_Depth       = pOther.m_Depth;
        m_Level       = pOther.m_Level;
        m_Layer       = pOther.m_Layer;
        m_SharingMode = pOther.m_SharingMode;

        pOther.m_Handle      = VK_NULL_HANDLE;
        pOther.m_Format      = VK_FORMAT_UNDEFINED;
        pOther.m_Type        = VK_IMAGE_TYPE_MAX_ENUM;
        pOther.m_SampleCount = VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
        pOther.m_Width       = 0;
        pOther.m_Heihgt      = 0;
        pOther.m_Depth       = 0;
        pOther.m_Level       = 0;
        pOther.m_Layer       = 0;
    
    }

   // Vulkan::Image& Vulkan::Image::operator=(const Vulkan::Image& pOther) noexcept
   // {
   //     ASSERT(false, "not implemented yet");
   //     return *this;
   // }
    Vulkan::Image& Vulkan::Image::operator=(Vulkan::Image&& pOther)      noexcept
    {
        m_Format      = pOther.m_Format;
        m_Type        = pOther.m_Type;
        m_SampleCount = pOther.m_SampleCount;
        m_Width       = pOther.m_Width;
        m_Heihgt      = pOther.m_Heihgt;
        m_Depth       = pOther.m_Depth;
        m_Level       = pOther.m_Level;
        m_Layer       = pOther.m_Layer;
        m_SharingMode = pOther.m_SharingMode;

        pOther.m_Handle      = VK_NULL_HANDLE;
        pOther.m_Format      = VK_FORMAT_UNDEFINED;
        pOther.m_Type        = VK_IMAGE_TYPE_MAX_ENUM;
        pOther.m_SampleCount = VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
        pOther.m_Width       = 0;
        pOther.m_Heihgt      = 0;
        pOther.m_Depth       = 0;
        pOther.m_Level       = 0;
        pOther.m_Layer       = 0;
        return *this;
    }

        
    uint32 Vulkan::Image::CreateImage(
        const ImageConfiguration& pImageConf
    ) noexcept
    {
        
        
        m_Format       = VkFormat(pImageConf.Format);
        m_Type         = VkImageType(pImageConf.Type);
        m_SampleCount  = VkSampleCountFlagBits(pImageConf.SampleCount);
        m_Width        = pImageConf.ImageSize.X;      
        m_Heihgt       = pImageConf.ImageSize.Y;
        m_Depth        = pImageConf.ImageSize.Z;
        m_Level        = pImageConf.MipMapLevels;
        m_Layer        = pImageConf.ArrayLayers;
        m_Usage        = VkImageUsageFlags(pImageConf.Usage);
        m_SharingMode  = pImageConf.SharingMode;
        m_Layout       = VkImageLayout(pImageConf.InitialLayout);
        


        VkImageCreateInfo CInfo{};
        CInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        CInfo.pNext                 = nullptr;
        CInfo.flags                 = 0;
        CInfo.format                = m_Format;
        CInfo.imageType             = m_Type; 
        CInfo.arrayLayers           = m_Layer;
        CInfo.mipLevels             = m_Level;
        CInfo.samples               = m_SampleCount;
        CInfo.tiling                = VK_IMAGE_TILING_OPTIMAL;
        CInfo.sharingMode           = m_SharingMode.SharingMode;
        CInfo.queueFamilyIndexCount = m_SharingMode.QueueFamilyIndexCount;
        CInfo.pQueueFamilyIndices   = m_SharingMode.QueueFamilyIndices;
        CInfo.initialLayout         = m_Layout;
        CInfo.usage                 = VkImageUsageFlags(m_Usage);
        CInfo.extent = VkExtent3D{
            .width  = m_Width,
            .height = m_Heihgt,
            .depth  = m_Depth
        };

        VkResult Res = vkCreateImage(
            Renderer::Self().GetDevice(),
            &CInfo,
            VULKAN_ALLOCATION_CALLBACK,
            &m_Handle
        );

        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Unable to Create Image. VkError:%i", int32(Res))
            );
            return HYD_FAILED;
        }


        //Allocating Actual memory:

        VkMemoryRequirements MemReq{};
        vkGetImageMemoryRequirements(
            Renderer::Self().GetDevice(),
            m_Handle,
            &MemReq
        );

        m_Memory = Renderer::Self().m_Device.AllocateMemory(
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MemReq
        );

        vkBindImageMemory(
            Renderer::Self().GetDevice(),
            m_Handle,
            m_Memory,
            0
        );

        return HYD_OK;
    }

    void Vulkan::Image::DestroyImage() noexcept
    {
        vkDestroyImage(
            Renderer::Self().GetDevice(),
            m_Handle,
            VULKAN_ALLOCATION_CALLBACK
        );

        Renderer::Self().m_Device.FreeMemory(&m_Memory);
        m_Handle = VK_NULL_HANDLE;
    }



    void Vulkan::Image::TransitionLayout(
        ImageLayout        pNewLayout,
        PipelineStageFlags pSrcStage,
        PipelineStageFlags pDstStage
    ) noexcept
    {


        VkImageMemoryBarrier MemBarrier{};

        MemBarrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        MemBarrier.pNext                           = nullptr;
        MemBarrier.image                           = m_Handle;
        MemBarrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        MemBarrier.subresourceRange.levelCount     = m_Level;
        MemBarrier.subresourceRange.baseMipLevel   = 0;
        MemBarrier.subresourceRange.layerCount     = 1;
        MemBarrier.subresourceRange.baseArrayLayer = 0;
        MemBarrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        MemBarrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        

        if (
            m_Layout == VK_IMAGE_LAYOUT_UNDEFINED
                &&
            VkImageLayout(pNewLayout) == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        )
        {
            MemBarrier.oldLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
            MemBarrier.newLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            MemBarrier.srcAccessMask = 0;
            MemBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        }

        else if (
            m_Layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL 
                &&
            VkImageLayout(pNewLayout) == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        )
        {
            MemBarrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            MemBarrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            MemBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            MemBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        }


        CommandBuffer& commandBuffer = Renderer::Self().GlobalTransferCommandBuffer(); 

        commandBuffer.RecordCommandBuffer();

        vkCmdPipelineBarrier(
            commandBuffer.GetHandle(),
            VkShaderStageFlags(pSrcStage),
            VkShaderStageFlags(pDstStage),
            0,
            0, nullptr,
            0, nullptr,
            1, &MemBarrier
        );

        m_Layout = VkImageLayout(pNewLayout);
        commandBuffer.EndRecordingCommandBuffer();

        Renderer::Self().ExecuteCommandBuffers(
            Renderer::Self().GetQueues().Transfer,
            {commandBuffer.GetHandle()}
        );

        vkQueueWaitIdle(Renderer::Self().GetQueues().Transfer);
    }

    void Vulkan::Image::GenerateMipMaps() noexcept
    {

        VkImageMemoryBarrier ImgBarrier;
        ImgBarrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        ImgBarrier.pNext               = nullptr;
        ImgBarrier.image               = m_Handle;
        ImgBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        ImgBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        ImgBarrier.subresourceRange    = VkImageSubresourceRange{
            .aspectMask       = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel     = 0,
            .levelCount       = 1,
            .baseArrayLayer   = 0,
            .layerCount       = 1
        };

        CommandBuffer& Transfer = Renderer::Self().GlobalTransferCommandBuffer();
        
        
        int32 MipWidth  = static_cast<int32>(m_Width);
        int32 MipHeight = static_cast<int32>(m_Heihgt);
        
        for (uint32 level = 1 ; level < m_Level ; ++level)
        {
            Transfer.RecordCommandBuffer();

            ImgBarrier.subresourceRange.baseMipLevel = level - 1;
            ImgBarrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            ImgBarrier.newLayout                     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            ImgBarrier.dstAccessMask                 = VK_ACCESS_TRANSFER_READ_BIT;
            ImgBarrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;

            vkCmdPipelineBarrier(
                Transfer.GetHandle(),
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0, nullptr, 0, nullptr, 1, &ImgBarrier);

            VkImageBlit Blit;

            Blit.srcSubresource = VkImageSubresourceLayers{
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel       = level - 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            };
            Blit.srcOffsets[0] = VkOffset3D{0, 0, 0};
            Blit.srcOffsets[1] = VkOffset3D{MipWidth, MipHeight, 1};

            Blit.dstSubresource = VkImageSubresourceLayers{
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel       = level,
                .baseArrayLayer = 0,
                .layerCount     = 1
            };
            Blit.dstOffsets[0] = VkOffset3D{0, 0, 0};
            Blit.dstOffsets[1] = VkOffset3D{MipWidth > 1? MipWidth/2 : 1, MipHeight > 1? MipHeight/2 : 1, 1};

            MipWidth  /= 2;
            MipHeight /= 2;


            vkCmdBlitImage(
                Transfer.GetHandle(),
                m_Handle, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                m_Handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &Blit,
                VK_FILTER_LINEAR
            );


            ImgBarrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            ImgBarrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            ImgBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            ImgBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(Transfer.GetHandle(),
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &ImgBarrier
            );


            Transfer.EndRecordingCommandBuffer();
            Renderer::Self().ExecuteCommandBuffers(
                Renderer::Self().GetQueues().Transfer,
                {Transfer.GetHandle()}
            );
            vkQueueWaitIdle(Renderer::Self().GetQueues().Transfer);
        }


        
        Transfer.RecordCommandBuffer();

        ImgBarrier.subresourceRange.baseMipLevel = m_Level - 1; 
        ImgBarrier.oldLayout        = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        ImgBarrier.newLayout        = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        ImgBarrier.srcAccessMask    = VK_ACCESS_TRANSFER_READ_BIT;
        ImgBarrier.dstAccessMask    = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(Transfer.GetHandle(),
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &ImgBarrier);
        
        Transfer.EndRecordingCommandBuffer();
        Renderer::Self().ExecuteCommandBuffers(
            Renderer::Self().GetQueues().Transfer,
            {Transfer.GetHandle()}
        );

        vkQueueWaitIdle(Renderer::Self().GetQueues().Transfer);
    }



	ImageFormat Vulkan::Image::ChannelToImageFormat(uint32 pChannels)
	{
		switch (pChannels)
		{
			case 1:
				return HYD_FORMAT_R8_SRGB;
			case 2:
				return HYD_FORMAT_R8G8_SRGB;
			case 3:
				return HYD_FORMAT_R8G8B8A8_SRGB;
			case 4:
				return HYD_FORMAT_R8G8B8A8_SRGB;
		}
		return HYD_FORMAT_UNDEFINED;
	}



    //Image View Impl:


    Vulkan::ImageView:: ImageView() noexcept
     : m_Handle(VK_NULL_HANDLE)
    {
    }

    Vulkan::ImageView::~ImageView() noexcept
    {
    }

    Vulkan::ImageView::ImageView(const Vulkan::ImageView& pOther) noexcept
        : m_Handle(pOther.m_Handle)
    {
        
    }

    Vulkan::ImageView::ImageView(Vulkan::ImageView&&      pOther) noexcept
        : m_Handle(pOther.m_Handle)
    {
        pOther.m_Handle = VK_NULL_HANDLE;
    }

    Vulkan::ImageView& Vulkan::ImageView::operator=(const Vulkan::ImageView& pOther) noexcept
    {
       m_Handle = pOther.m_Handle;

        return *this;
    }

    Vulkan::ImageView& Vulkan::ImageView::operator=(Vulkan::ImageView&&      pOther) noexcept
    {
        m_Handle = pOther.m_Handle;
        pOther.m_Handle = VK_NULL_HANDLE;
        return *this;
    }

        

    uint32 Vulkan::ImageView::CreateImageView(
        ImageViewConfiguration pViewConf
    ) noexcept
    {


        VkImageViewCreateInfo CInfo{};
        CInfo.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        CInfo.pNext            = nullptr;
        CInfo.image            = (pViewConf.Image)->m_Handle;
        CInfo.format           = (pViewConf.Format != HYD_FORMAT_UNDEFINED)? VkFormat(pViewConf.Format) : (pViewConf.Image)->m_Format; 
        CInfo.viewType         = VkImageViewType(pViewConf.ViewType);
        CInfo.components       = pViewConf.CompMapping;

        CInfo.subresourceRange.levelCount     = (pViewConf.SubResources.levelCount == UINT32_MAX)? pViewConf.Image->m_Level : pViewConf.SubResources.levelCount;
        CInfo.subresourceRange.layerCount     = (pViewConf.SubResources.layerCount == UINT32_MAX)? pViewConf.Image->m_Level : pViewConf.SubResources.layerCount;
        CInfo.subresourceRange.baseMipLevel   =  pViewConf.SubResources.baseMipLevel;
        CInfo.subresourceRange.baseArrayLayer =  pViewConf.SubResources.baseArrayLayer;
        CInfo.subresourceRange.aspectMask     =  pViewConf.SubResources.aspectMask;

        
        VkResult Res = vkCreateImageView(
            Renderer::Self().GetDevice(),
            &CInfo,
            VULKAN_ALLOCATION_CALLBACK,
            &m_Handle
        );
        

        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Unable to create Image View. vkError:%i", int32(Res))
            );
            return HYD_FAILED;
        }

        return HYD_OK;
    }



    void Vulkan::ImageView::DestroyImageView() noexcept
    {
        if (m_Handle == VK_NULL_HANDLE)
            return;

        vkDestroyImageView(
            Renderer::Self().GetDevice(),
            m_Handle,
            VULKAN_ALLOCATION_CALLBACK
        );

        m_Handle = VK_NULL_HANDLE;
    }


    
/*

    Sampler Implementation:

*/


    Vulkan::Sampler:: Sampler() noexcept
    {

    }

    Vulkan::Sampler::~Sampler() noexcept
    {

    }


    Vulkan::Sampler::Sampler(Sampler&& pOther)  noexcept
        : m_Handle(pOther.m_Handle)
    {
        pOther.m_Handle = VK_NULL_HANDLE;
    }

    Vulkan::Sampler& Vulkan::Sampler::operator=(Sampler&& pOther) noexcept
    {
        m_Handle        = pOther.m_Handle;
        pOther.m_Handle = VK_NULL_HANDLE;
        return *this;
    }


    uint32 Vulkan::Sampler::CreateSampler(
        SamplerConfiguration pConf
    ) noexcept
    {
        VkSamplerCreateInfo CInfo{};
        CInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        CInfo.pNext                   = nullptr;
        CInfo.flags                   = 0;
        CInfo.minFilter               = VkFilter(pConf.Minification); 
        CInfo.magFilter               = VkFilter(pConf.Magnification);
        CInfo.mipmapMode              = VkSamplerMipmapMode(pConf.MipMapFilter);  
        CInfo.addressModeU            = VkSamplerAddressMode(pConf.WrapU);
        CInfo.addressModeV            = VkSamplerAddressMode(pConf.WrapV);
        CInfo.addressModeW            = VkSamplerAddressMode(pConf.WrapW);
        CInfo.anisotropyEnable        = VkBool32(pConf.AnisotropyEnable);
        CInfo.maxAnisotropy           = pConf.MaxAnisotropy;
        CInfo.mipLodBias              = 0.0f; 
        CInfo.compareEnable           = VK_FALSE;
        CInfo.maxLod                  = VK_LOD_CLAMP_NONE;
        CInfo.unnormalizedCoordinates = VK_FALSE;
        CInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;


        VkResult Res = vkCreateSampler(
            Renderer::Self().GetDevice(),
            &CInfo,
            VULKAN_ALLOCATION_CALLBACK,
            &m_Handle
        );

        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Unable to Create Sampler. VkError:%i", int32(Res))
            );
            return HYD_FAILED;
        }
        return HYD_OK;
    }


    void Vulkan::Sampler::DestroySampler() noexcept
    {
        if (m_Handle == VK_NULL_HANDLE)
            return;

        vkDestroySampler(
            Renderer::Self().GetDevice(),
            m_Handle,
            VULKAN_ALLOCATION_CALLBACK    
        );
        m_Handle = VK_NULL_HANDLE;
    }

};
};