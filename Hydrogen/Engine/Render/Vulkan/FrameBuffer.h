#pragma once


#include "../../Common.h"
#include <vulkan/vulkan.h>

namespace Hydrogen
{

namespace Internal
{
namespace Vulkan
{

    class FrameBuffer final
    {
    public:

        FrameBuffer() = default;
        ~FrameBuffer() noexcept;

        FrameBuffer(const FrameBuffer& pOther) = delete;
        FrameBuffer(FrameBuffer&& pOther)            noexcept;
        FrameBuffer& operator=(FrameBuffer&& pOther) noexcept;
        
        //Creates the framebuffer 
        uint32 CreateFrameBuffer(
            VkRenderPass&                     pRenderPass,
            const std::vector<VkImageView>&   pImageViews,
            uint32                            pWidth, 
            uint32                            pHeight,
            uint32                            pLayers=1,
            VkFramebufferCreateFlagBits       pFlags=VkFramebufferCreateFlagBits(0)
        )  noexcept;
        
        //Creates the framebuffer using a single image
        uint32 CreateFrameBuffer(
            VkRenderPass                pRenderPass,
            VkImageView                 pImageView,
            uint32                      pWidth, 
            uint32                      pHeight,
            uint32                      pLayers=1,
            VkFramebufferCreateFlagBits pFlags=VkFramebufferCreateFlagBits(0)
        )  noexcept;
        
        uint32 DestroyFrameBuffer() noexcept;
        inline const VkFramebuffer GetHandle() const {return m_Handle;}
    private:
        VkFramebuffer m_Handle = VK_NULL_HANDLE;
    };
    
};
};
};
