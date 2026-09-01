#include "Attachment.h"
#include "Vulkan/Image.h"
#include "Vulkan/VkEnumReDefs.h"

namespace Hydrogen
{

    Attachment:: Attachment() noexcept
    {

    }
    Attachment::~Attachment() noexcept
    {

    }
    
    Attachment::Attachment(const Attachment& pOther) noexcept
    {

    }

    Attachment::Attachment(Attachment&& pOther)      noexcept
    {

    }

    Attachment& Attachment::operator=(const Attachment& pOther) noexcept
    {
        
        return *this;
    }
    Attachment& Attachment::operator=(Attachment&& pOther)      noexcept
    {
        
        return *this;
    }


    uint32 Attachment::CreateAttachment(
        AttachmentType   pType,
        uint32           pWidth,
        uint32           pHeight,
        ImageSampleCount pSampleCount
    ) noexcept
    {

        ImageConfiguration ImgConf{};
        
        ImgConf.ImageSize.X   = pWidth;
        ImgConf.ImageSize.Y   = pHeight;
        ImgConf.ImageSize.Z   = 1;
        ImgConf.ArrayLayers   = 1;
        ImgConf.InitialLayout = HYD_IMAGE_LAYOUT_UNDEFINED;
        ImgConf.MipMapLevels  = 1;
        ImgConf.SampleCount   = pSampleCount;
        ImgConf.Type          = HYD_IMAGE_TYPE_2D;
        


        ImageViewConfiguration ViewConf{};
        ViewConf.ViewType = HYD_IMAGE_VIEW_TYPE_2D;
 

        switch(pType)
        {
        case HYD_ATTACHMENT_TYPE_COLOR:
            ImgConf.Format  = HYD_FORMAT_R8G8B8A8_SRGB;
            ImgConf.Usage   = HYD_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            ViewConf.Format = HYD_FORMAT_R8G8B8A8_SRGB;
            ViewConf.SubResources = VkImageSubresourceRange{
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            };
            break;
        case HYD_ATTACHMENT_TYPE_DEPTH:
            ImgConf.Format  = HYD_FORMAT_D32_SFLOAT;
            ImgConf.Usage   = HYD_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            ViewConf.Format = HYD_FORMAT_D32_SFLOAT;
            ViewConf.SubResources = VkImageSubresourceRange{
                .aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            };
            break;

        case HYD_ATTACHMENT_TYPE_STENCIL:
            ImgConf.Format  = HYD_FORMAT_S8_UINT;
            ImgConf.Usage   = HYD_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            ViewConf.Format = HYD_FORMAT_S8_UINT;
            ViewConf.SubResources = VkImageSubresourceRange{
                .aspectMask     = VK_IMAGE_ASPECT_STENCIL_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            };
            break;
        }

        CHECK_ERROR(m_Image.CreateImage(ImgConf));

        ViewConf.Image = &m_Image;
        CHECK_ERROR(m_View.CreateImageView(ViewConf));

        return HYD_OK;
    }


    void Attachment::DestroyAttachment() noexcept
    {
        m_View.DestroyImageView();
        m_Image.DestroyImage();
    }

};