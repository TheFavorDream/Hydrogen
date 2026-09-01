/*
    Attachments are special textures that can be attached to framebuffers as rendering targets 
*/


#pragma once

#include "../Common.h"
#include "Vulkan/Image.h"
#include "Vulkan/VkEnumReDefs.h"

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

    private:
        Internal::Vulkan::Image     m_Image;
        Internal::Vulkan::ImageView m_View;

    private: //friend classes
        friend class Internal::Vulkan::FrameBuffer;
        friend class Internal::Vulkan::Swapchain;
    };

};