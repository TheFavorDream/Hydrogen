#include "Commands.h"
#include "../Renderer.h"
#include <utility>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{
namespace Internal
{
    Vulkan::CommandPool::CommandPool() noexcept
    {
        m_Handle           = VK_NULL_HANDLE;
        m_AllocatedBuffers = 0;
    }

    Vulkan::CommandPool::~CommandPool() noexcept
    {
        if (m_Handle != VK_NULL_HANDLE)
            DestroyPool();
    }


    //Move
    Vulkan::CommandPool::CommandPool(CommandPool&& pOther) noexcept
    {
        m_Handle           = pOther.m_Handle;
        m_AllocatedBuffers = pOther.m_AllocatedBuffers;
        
        pOther.m_Handle           = VK_NULL_HANDLE;
        pOther.m_AllocatedBuffers = 0;
        
    }

    Vulkan::CommandPool& Vulkan::CommandPool::operator=(CommandPool&& pOther) noexcept
    {
        m_Handle           = pOther.m_Handle;
        m_AllocatedBuffers = pOther.m_AllocatedBuffers;
        
        pOther.m_Handle           = VK_NULL_HANDLE;
        pOther.m_AllocatedBuffers = 0;
        
        return *this;
    }


    uint32 Vulkan::CommandPool::CreatePool(
        uint32                   pQueueFamily,
        VkCommandPoolCreateFlags pFlags
    )  noexcept
    {
        VkCommandPoolCreateInfo CInfo{};
        CInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        CInfo.pNext            = nullptr;
        CInfo.flags            = pFlags;
        CInfo.queueFamilyIndex = pQueueFamily;


        VkResult Res = vkCreateCommandPool(
            Renderer::Self().GetDevice(),
       &CInfo, VULKAN_ALLOCATION_CALLBACK,
      &m_Handle);



        if (Res != VK_SUCCESS)
        {
            Log::SetError(Log::FmtStr("Unable to Create Command Pool. VkError:%i", int32(Res)));
            return HYD_FAILED;
        }

        return HYD_OK;
    }

    uint32 Vulkan::CommandPool::DestroyPool() noexcept
    {
        if (m_Handle == VK_NULL_HANDLE)
            return HYD_FAILED;

        vkDestroyCommandPool(
            Renderer::Self().GetDevice(),
            m_Handle,
            VULKAN_ALLOCATION_CALLBACK);
        m_Handle           = VK_NULL_HANDLE;
        m_AllocatedBuffers = 0;

        return HYD_OK;
    }



    //Command buffer
    Vulkan::CommandBuffer Vulkan::CommandPool::AllocateCommandBuffer(
        VkCommandBufferLevel pLevel// = VK_COMMAND_BUFFER_LEVEL_PRIMARY
    ) noexcept
    {
        CommandBuffer buffer;
        buffer.CreateCommandBuffer(m_Handle,  pLevel);
        m_AllocatedBuffers += 1;
        return std::move(buffer);
    }



    std::vector<Vulkan::CommandBuffer> Vulkan::CommandPool::AllocateCommandBuffers(
        uint32               pCount,
        VkCommandBufferLevel pLevel // = VK_COMMAND_BUFFER_LEVEL_PRIMARY
    ) noexcept
    {
        std::vector<Vulkan::CommandBuffer> buffers; buffers.resize(pCount);
        m_AllocatedBuffers += pCount;

        for (uint32 I = 0 ; I < pCount ; ++I)
            buffers[I].CreateCommandBuffer(m_Handle,  pLevel);
        
        return std::move(buffers);
    }




    //Command buffer:

    Vulkan::CommandBuffer::CommandBuffer()  noexcept
    {
        m_Handle = VK_NULL_HANDLE;
    }

    Vulkan::CommandBuffer::~CommandBuffer() noexcept
    {
    
    }

        

    //Move
    Vulkan::CommandBuffer::CommandBuffer(CommandBuffer&& pOther)  noexcept
    {
        m_Handle        = pOther.m_Handle;
        pOther.m_Handle = VK_NULL_HANDLE;
    }

    Vulkan::CommandBuffer& Internal::Vulkan::CommandBuffer::operator=(CommandBuffer&& pOther) noexcept
    {
        m_Handle        = pOther.m_Handle;
        pOther.m_Handle = VK_NULL_HANDLE;
        return *this;
    }


    uint32 Vulkan::CommandBuffer::CreateCommandBuffer(
        VkCommandPool        pPool,
        VkCommandBufferLevel pLevel
    ) noexcept
    {

        VkCommandBufferAllocateInfo AInfo{};
        AInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        AInfo.pNext              = nullptr;
        AInfo.commandPool        = pPool;
        AInfo.level              = pLevel;
        AInfo.commandBufferCount = 1;


        VkResult Res = vkAllocateCommandBuffers(
            Renderer::Self().GetDevice(),
            &AInfo,    
            &m_Handle);

        if (Res != VK_SUCCESS)
        {
            Log::SetError(Log::FmtStr("Unable to allocate command buffer. vkError:%i", int32(Res)));
            m_Handle = VK_NULL_HANDLE;
            return HYD_FAILED;
        }
        return HYD_OK;
    }




    uint32 Vulkan::CommandBuffer::RecordCommandBuffer(
            VkCommandBufferUsageFlags pFlags
    ) noexcept
    {
        VkCommandBufferBeginInfo BeginInfo{};

        BeginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        BeginInfo.flags            = pFlags;
        BeginInfo.pNext            = nullptr;
        BeginInfo.pInheritanceInfo = nullptr;

        VkResult Res;
        if ((Res = vkBeginCommandBuffer(m_Handle, &BeginInfo)) != VK_SUCCESS)
        {
            Log::SetError(Log::FmtStr("Unable to begin recording. VkError:%i", int32(Res)));
            return HYD_FAILED;
        }

        return HYD_OK;
    }

    uint32 Vulkan::CommandBuffer::EndRecordingCommandBuffer() noexcept
    {
        vkEndCommandBuffer(m_Handle);
        return HYD_OK;
    }

    uint32 Vulkan::CommandBuffer::ResetCommandBuffer(
    ) noexcept
    {
        VkResult Res = vkResetCommandBuffer(m_Handle, 0);
		if ( Res != VK_SUCCESS){
            Log::SetError(Log::FmtStr("Unable to Reset Command buffer. VkError:%i", int32(Res)));
            return HYD_FAILED;
        }
        return HYD_OK;
    }

};
};