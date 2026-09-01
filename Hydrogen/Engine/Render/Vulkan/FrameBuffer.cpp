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
        Renderpass&                         pRenderPass,
        std::vector<Hydrogen::Attachment*>  pAttachments,
        uint32                              pWidth, 
        uint32                              pHeight,
        uint32                              pLayers,
        VkFramebufferCreateFlagBits         pFlags
    )  noexcept
    {


        if (pRenderPass.m_Handle == VK_NULL_HANDLE)
            return HYD_FAILED;

        //Free the already existed framebuffer
        if (m_Handle != VK_NULL_HANDLE)
            DestroyFrameBuffer();

        std::vector<VkImageView> Views; Views.resize(pAttachments.size());
        for (uint32 Iter = 0 ; Iter < Views.size() ; ++Iter)
        {
            Views.at(Iter) = pAttachments.at(Iter)->m_View.m_Handle;
        }

        VkFramebufferCreateInfo CInfo{};
        CInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        CInfo.pNext           = nullptr;
        CInfo.flags           = pFlags;
        CInfo.renderPass      = pRenderPass.m_Handle;
        CInfo.attachmentCount = Views.size();
        CInfo.pAttachments    = Views.data();
        CInfo.width           = pWidth;
        CInfo.height          = pHeight;
        CInfo.layers          = pLayers;

        VkResult Res = vkCreateFramebuffer(Renderer::Self().GetDevice(), &CInfo, VULKAN_ALLOCATION_CALLBACK, &m_Handle);
    
        return (Res == VK_SUCCESS)? HYD_OK : Res;
    }

    //Creates the framebuffer using a single image
    uint32 Vulkan::FrameBuffer::CreateFrameBuffer(
        Renderpass&                 pRenderPass,
        Hydrogen::Attachment&       pAttachment,
        uint32                      pWidth, 
        uint32                      pHeight,
        uint32                      pLayers,
        VkFramebufferCreateFlagBits pFlags
    )  noexcept
    {


        if (pRenderPass.m_Handle == VK_NULL_HANDLE)
            return HYD_FAILED;

        //Free the already existed framebuffer
        if (m_Handle != VK_NULL_HANDLE)
            DestroyFrameBuffer();


        VkFramebufferCreateInfo CInfo{};
        CInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        CInfo.pNext           = nullptr;
        CInfo.flags           = pFlags;
        CInfo.renderPass      = pRenderPass.m_Handle;
        CInfo.attachmentCount = 1;
        CInfo.pAttachments    = &pAttachment.m_View.m_Handle;
        CInfo.width           = pWidth;
        CInfo.height          = pHeight;
        CInfo.layers          = pLayers;

        VkResult Res = vkCreateFramebuffer(
            Renderer::Self().GetDevice(),
            &CInfo, VULKAN_ALLOCATION_CALLBACK, &m_Handle
        );
    
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