/*
    Attachments are special textures that can be attached to framebuffers as rendering targets 
*/


#pragma once

#include "../Common.h"
#include "Vulkan/Image.h"
#include "Vulkan/VkEnumReDefs.h"
#include "Vulkan/Renderpass.h"

namespace Hydrogen
{


    enum AttachmentType
    {
        HYD_ATTACHMENT_TYPE_COLOR   = 1,
        HYD_ATTACHMENT_TYPE_DEPTH   = 2,
        HYD_ATTACHMENT_TYPE_STENCIL = 3
    };

 

    class Attachment final
    {
    public:
        
        HYD  Attachment() noexcept;
        HYD ~Attachment() noexcept;

        HYD Attachment(const Attachment& pOther) noexcept;
        HYD Attachment(Attachment&& pOther)      noexcept;

        HYD Attachment& operator=(const Attachment& pOther) noexcept;
        HYD Attachment& operator=(Attachment&& pOther)      noexcept;

        HYD uint32 CreateAttachment(
            AttachmentType   pType,
            uint32           pWidth,
            uint32           pHeight,
            ImageSampleCount pSampleCount
        ) noexcept;
        

        HYD void DestroyAttachment() noexcept;

        inline ImageFormat      GetAttachmentFormat()      noexcept {return m_Format;}
        inline ImageSampleCount GetAttachmentSampleCount() noexcept {return m_SampleCount;}
         

    private:
        Internal::Vulkan::Image     m_Image;
        Internal::Vulkan::ImageView m_View;

        ImageFormat      m_Format        = HYD_FORMAT_UNDEFINED;
        ImageSampleCount m_SampleCount   = HYD_SAMPLE_COUNT_1_BIT;

    private: //friend classes
        friend class Internal::Vulkan::FrameBuffer;
        friend class Internal::Vulkan::Swapchain;
    };

};