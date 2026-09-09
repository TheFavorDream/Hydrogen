#pragma  once
#include "../../Common.h"
#include <cstdint>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "VkEnumReDefs.h"
#include "Device.h"
#include "../../Core/ResourcePool.h"

namespace Hydrogen
{


    typedef VkComponentMapping ComponentMapping;  

    struct ImageConfiguration
    {

        ImageType         Type;
        Vec3<uint32>      ImageSize;
        ImageFormat       Format;
        uint32            MipMapLevels  = 1;
        uint32            ArrayLayers   = 1;
        ImageSampleCount  SampleCount;
        uint32            Usage;
        QueueSharingMode  SharingMode;
        ImageLayout       InitialLayout;
    };

    struct ImageViewConfiguration
    {
        Ptr<Internal::Vulkan::Image>    Image;
        ImageViewType                   ViewType;
        ImageFormat                     Format      = HYD_FORMAT_UNDEFINED;
        ComponentMapping                CompMapping = ComponentMapping{
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY
            };
        VkImageSubresourceRange         SubResources = VkImageSubresourceRange{
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel   = 0,
                .levelCount     = UINT32_MAX,
                .baseArrayLayer = 0,
                .layerCount     = UINT32_MAX
            };
    };


    struct SamplerConfiguration
    {
        FilterMode       Minification;
        FilterMode       Magnification;
        MipMapFilterMode MipMapFilter;
        WrapMode         WrapU;
        WrapMode         WrapV;
        WrapMode         WrapW;
        bool             AnisotropyEnable;
        float            MaxAnisotropy;
    };

namespace Internal
{
namespace Vulkan
{
    class Image
    {
    public:
         Image() noexcept;
        ~Image() noexcept;


        Image(const Image& pOther) = delete;
        Image(Image&& pOther)      noexcept;

        Image& operator=(const Image& pOther) = delete;
        Image& operator=(Image&& pOther)      noexcept;

        
        uint32 CreateImage(
            const ImageConfiguration& pImageConf
        ) noexcept;

        void DestroyImage() noexcept;


        void TransitionLayout(
            ImageLayout        pNewLayout,
            PipelineStageFlags pSrcStage,
            PipelineStageFlags pDstStage
        ) noexcept;

        void GenerateMipMaps() noexcept;

        inline VkExtent3D GetImageExtent() const {return VkExtent3D{.width=m_Width, .height=m_Heihgt, .depth=1 };}

    public:

		static ImageFormat ChannelToImageFormat(uint32 pChannels);

    private:
        VkImage                m_Handle = VK_NULL_HANDLE;
        VkDeviceMemory         m_Memory = VK_NULL_HANDLE;
        VkFormat               m_Format = VK_FORMAT_UNDEFINED;
        VkImageType            m_Type;
        VkSampleCountFlagBits  m_SampleCount;
        VkImageUsageFlags      m_Usage;
        VkImageLayout          m_Layout;
        uint32                 m_Width  = 0;
        uint32                 m_Heihgt = 0;
        uint32                 m_Depth  = 0;
        uint32                 m_Level  = 0;
        uint32                 m_Layer  = 0; 
        QueueSharingMode       m_SharingMode;

    private:
        friend class Hydrogen::Renderer;
        friend class StagingBuffer;
        friend class Swapchain;
        friend class ImageView;
    };



    class ImageView
    {

    public:

         ImageView() noexcept;
        ~ImageView() noexcept;

        ImageView(const ImageView& pOther) noexcept;
        ImageView(ImageView&&      pOther) noexcept;

        ImageView& operator=(const ImageView& pOther) noexcept;
        ImageView& operator=(ImageView&&      pOther) noexcept;

        

        uint32 CreateImageView(
            ImageViewConfiguration pViewConf
        ) noexcept;



        void DestroyImageView() noexcept;
        
    private:
        VkImageView m_Handle      = 0;

    private:
        friend class Hydrogen::Renderer;
        friend class DescriptorSet;
        friend class FrameBuffer;
    };



    class Sampler
    {
    public:

         Sampler() noexcept;
        ~Sampler() noexcept;


        Sampler(const Sampler& pOther)             = delete;
        Sampler& operator=(const Sampler& pOther)  = delete;

        Sampler(Sampler&& pOther)            noexcept;
        Sampler& operator=(Sampler&& pOther) noexcept;

        uint32 CreateSampler(
            SamplerConfiguration pConf
        ) noexcept;


        void DestroySampler() noexcept;

    private:
        VkSampler m_Handle = VK_NULL_HANDLE;

    private:
        friend class Hydrogen::Renderer; 
        friend class DescriptorSet;
    };


};
};

};
