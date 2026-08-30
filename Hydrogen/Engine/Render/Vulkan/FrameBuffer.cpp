#include "FrameBuffer.h"
#include "../Renderer.h"
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{
namespace Internal
{
    Vulkan::FrameBuffer::~FrameBuffer() noexcept
    {
        if (m_Handle != VK_NULL_HANDLE)
            DestroyFrameBuffer();
    }

    Vulkan::FrameBuffer::FrameBuffer(FrameBuffer&& pOther)  noexcept
        : m_Handle(pOther.m_Handle)
    {
        pOther.m_Handle = VK_NULL_HANDLE;
    }
    
    Vulkan::FrameBuffer& Vulkan::FrameBuffer::operator=(FrameBuffer&& pOther) noexcept
    {
        m_Handle        = pOther.m_Handle;
        pOther.m_Handle = VK_NULL_HANDLE;
    
        return *this;
    }

    //Creates the framebuffer 
    uint32 Vulkan::FrameBuffer::CreateFrameBuffer(
        VkRenderPass&               pRenderPass,
        const std::vector<VkImageView>&   pImageViews,
        uint32                      pWidth, 
        uint32                      pHeight,
        uint32                      pLayers,
        VkFramebufferCreateFlagBits pFlags
    )  noexcept
    {

        if (
            pRenderPass == VK_NULL_HANDLE ||
            (pImageViews.size() == 0 && pFlags != VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT)
        )
        {    return HYD_INVALID_VALUE;}

        //Free the already existed framebuffer
        if (m_Handle != VK_NULL_HANDLE)
            DestroyFrameBuffer();


        VkFramebufferCreateInfo CInfo{};
        CInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        CInfo.pNext           = nullptr;
        CInfo.flags           = pFlags;
        CInfo.renderPass      = pRenderPass;
        CInfo.attachmentCount = pImageViews.size();
        CInfo.pAttachments    = pImageViews.data();
        CInfo.width           = pWidth;
        CInfo.height          = pHeight;
        CInfo.layers          = pLayers;

        VkResult Res = vkCreateFramebuffer(Renderer::Self().GetDevice(), &CInfo, VULKAN_ALLOCATION_CALLBACK, &m_Handle);
    
        return (Res == VK_SUCCESS)? HYD_OK : Res;
    }

    //Creates the framebuffer using a single image
    uint32 Vulkan::FrameBuffer::CreateFrameBuffer(
        VkRenderPass                pRenderPass,
        VkImageView                 pImageView,
        uint32                      pWidth, 
        uint32                      pHeight,
        uint32                      pLayers,
        VkFramebufferCreateFlagBits pFlags
    )  noexcept
    {
        if (
            pRenderPass == VK_NULL_HANDLE ||
            (pImageView == VK_NULL_HANDLE && pFlags != VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT)
        )
        {    return HYD_INVALID_VALUE;}

        //Free the already existed framebuffer
        if (m_Handle != VK_NULL_HANDLE)
            DestroyFrameBuffer();


        VkFramebufferCreateInfo CInfo{};
        CInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        CInfo.pNext           = nullptr;
        CInfo.flags           = pFlags;
        CInfo.renderPass      = pRenderPass;
        CInfo.attachmentCount = 1;
        CInfo.pAttachments    = &pImageView;
        CInfo.width           = pWidth;
        CInfo.height          = pHeight;
        CInfo.layers          = pLayers;

        VkResult Res = vkCreateFramebuffer(Renderer::Self().GetDevice(), &CInfo, VULKAN_ALLOCATION_CALLBACK, &m_Handle);
    
        return (Res == VK_SUCCESS)? HYD_OK : Res;
    }


    uint32 Vulkan::FrameBuffer::DestroyFrameBuffer() noexcept
    {
        if (m_Handle == VK_NULL_HANDLE)
            return HYD_OK;

        vkDestroyFramebuffer(Renderer::Self().GetDevice(), m_Handle, VULKAN_ALLOCATION_CALLBACK);
        m_Handle = VK_NULL_HANDLE;
        return HYD_OK;
    }
};
};