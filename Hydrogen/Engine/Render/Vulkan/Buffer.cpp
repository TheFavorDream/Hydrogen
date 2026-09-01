#include "Buffer.h"
#include "Commands.h"
#include "../Renderer.h"
#include "Commands.h"
#include <cstring>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{
namespace Internal
{

    Vulkan::BasicBuffer::BasicBuffer() noexcept
    {

    }

    Vulkan::BasicBuffer::~BasicBuffer() noexcept
    {
        if (m_BufferHandle != VK_NULL_HANDLE)
            DestroyBuffer();
    }



    //Move 
    Vulkan::BasicBuffer::BasicBuffer(BasicBuffer&& pOther)    noexcept
    {
        m_BufferHandle        = pOther.m_BufferHandle;
        m_MemoryHandle        = pOther.m_MemoryHandle;
        m_BufferUsage         = pOther.m_BufferUsage;
        m_Size                = pOther.m_Size;
        m_Format              = pOther.m_Format;

        pOther.m_BufferHandle = VK_NULL_HANDLE;
        pOther.m_MemoryHandle = VK_NULL_HANDLE;
        pOther.m_Size         = 0;
        pOther.m_Format       = VK_FORMAT_UNDEFINED;
        
    }

    Vulkan::BasicBuffer& Vulkan::BasicBuffer::operator=(BasicBuffer&& pOther)  noexcept
    {
        m_BufferHandle        = pOther.m_BufferHandle;
        m_MemoryHandle        = pOther.m_MemoryHandle;
        m_BufferUsage         = pOther.m_BufferUsage;
        m_Size                = pOther.m_Size;
        m_Format              = pOther.m_Format;

        pOther.m_BufferHandle = VK_NULL_HANDLE;
        pOther.m_MemoryHandle = VK_NULL_HANDLE;
        pOther.m_Size         = 0;
        pOther.m_Format       = VK_FORMAT_UNDEFINED;

        return *this;
    }



    uint32 Vulkan::BasicBuffer::CreateGeneralBuffer(
        uint32                pSize,
        VkBufferUsageFlags    pUsage,
        VkMemoryPropertyFlags pMemoryProperty,
        VkSharingMode         pSharingMode,        //= VK_SHARING_MODE_EXCLUSIVE,
        std::vector<uint32>   pQueueFamilyIndices //= {}
    ) noexcept
    {

        m_Size           = pSize;
        m_BufferUsage    = pUsage;
        m_MemoryProperty = pMemoryProperty;
        
        VkBufferCreateInfo CInfo{};
        CInfo.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        CInfo.pNext                 = nullptr;
        CInfo.usage                 = pUsage;
        CInfo.size                  = pSize;
        CInfo.sharingMode           = pSharingMode;
        CInfo.queueFamilyIndexCount = pQueueFamilyIndices.size();
        CInfo.pQueueFamilyIndices   = (pQueueFamilyIndices.size())? pQueueFamilyIndices.data() : nullptr;

        VkResult Res = vkCreateBuffer(
            Renderer::Self().GetDevice(),
             &CInfo, VULKAN_ALLOCATION_CALLBACK, &m_BufferHandle);


        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Unable to Create Vertex Buffer.vkError: %i", int32(Res))
            );
            return HYD_FAILED;
        }


        VkMemoryRequirements MemReq{};

        vkGetBufferMemoryRequirements(
        Renderer::Self().GetDevice(),
        m_BufferHandle,
&MemReq);



        m_MemoryHandle = Renderer::Self().m_Device.AllocateMemory(pMemoryProperty,  MemReq);

        if (m_MemoryHandle == VK_NULL_HANDLE)
            return HYD_FAILED;


        Res = vkBindBufferMemory(
            Renderer::Self().GetDevice(),
            m_BufferHandle, m_MemoryHandle, 0
        );

        if (Res != VK_SUCCESS)
        {
            Log::SetError(Log::FmtStr("Unable to attach memory to buffer .vkError: %i", int32(Res)));
            return HYD_FAILED;
        }

        return HYD_OK;
    }


    uint32 Vulkan::BasicBuffer::UploadData(
        void*  pData,
        uint64 pSize,
        uint64 pOffset //=0
    ) noexcept
    {

        uint32 Res = MapMemory(pSize, pOffset);

        if (Res != HYD_OK)
        {
            Log::SetError("Unable to Map Buffer");
            return HYD_FAILED;
        }

        memcpy(m_VirtualAddress, pData, pSize);

        UnmapMemory();

        return HYD_OK;
    }


    uint32 Vulkan::BasicBuffer::MapMemory(
            uint32 pSize,
            uint32 pOffset//=0
    )   noexcept
    {
        VkResult Res = vkMapMemory(
            Renderer::Self().GetDevice(),
             m_MemoryHandle,
             pOffset, pSize,
                  0, &m_VirtualAddress);
        return (Res == VK_SUCCESS)? HYD_OK : HYD_FAILED;
    }

    uint32 Vulkan::BasicBuffer::UnmapMemory() noexcept
    {
        vkUnmapMemory(Renderer::Self().GetDevice(), m_MemoryHandle);
        m_VirtualAddress = nullptr;
        return HYD_OK;
    }

    uint32 Vulkan::BasicBuffer::UploadDataWithoutMapping(
        void*  pData,
        uint64 pSize,
        uint64 pOffset//=0
    ) noexcept
    {

        if (!m_VirtualAddress)
            return HYD_FAILED;

        memcpy((void*)((int8*)m_VirtualAddress+pOffset), pData, pOffset);
        return HYD_OK;
    }

    uint32 Vulkan::BasicBuffer::DestroyBuffer() noexcept
    {


        vkDestroyBuffer(Renderer::Self().GetDevice(), m_BufferHandle, VULKAN_ALLOCATION_CALLBACK);
        Renderer::Self().m_Device.FreeMemory(&m_MemoryHandle);
        m_BufferHandle = VK_NULL_HANDLE;
        m_Size   = 0;
        m_Format = VK_FORMAT_UNDEFINED;
        return HYD_OK;
    }




    //Staging Buffer:


    uint32 Vulkan::StagingBuffer::CreateBuffer(
        uint32              pSize,
        VkSharingMode       pSharingMode,        //= VK_SHARING_MODE_EXCLUSIVE,
        std::vector<uint32> pQueueFamilyIndices  //= {}
    ) noexcept
    {
        return CreateGeneralBuffer(pSize,
             VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        pSharingMode, pQueueFamilyIndices);
    }


    uint32 Internal::Vulkan::StagingBuffer::CopyBuffer(
        BasicBuffer& pDstBuffer,
        uint64       pOffset   //= 0
        
    ) noexcept
    {


        VkBufferCopy CopyRegion{};
        CopyRegion.dstOffset = pOffset;
        CopyRegion.srcOffset = 0;
        CopyRegion.size      = m_Size;

        
        Internal::Vulkan::CommandBuffer& commandBuffer = Renderer::Self().GlobalTransferCommandBuffer();

        commandBuffer.RecordCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        vkCmdCopyBuffer(
         commandBuffer.GetHandle(),
             m_BufferHandle,
             pDstBuffer.GetHandle(),
               1, &CopyRegion);

        commandBuffer.EndRecordingCommandBuffer();


        Renderer::Self().ExecuteCommandBuffers(
            Renderer::Self().GetQueues().Transfer,
            {commandBuffer.GetHandle()}
        );

		vkQueueWaitIdle(Renderer::Self().GetQueues().Transfer);


        return HYD_OK;
    }


    uint32 Vulkan::StagingBuffer::CopyImage(
        Vulkan::Image& pImage
    ) noexcept
    {


        VkBufferImageCopy CopyInfo;

        CopyInfo.bufferOffset      = 0;
        CopyInfo.bufferImageHeight = 0;
        CopyInfo.bufferRowLength   = 0;
        CopyInfo.imageExtent  = pImage.GetImageExtent();
        CopyInfo.imageOffset  = VkOffset3D{.x=0, .y=0, .z=0};
        CopyInfo.imageSubresource.mipLevel       = 0;
        CopyInfo.imageSubresource.layerCount     = 1;
        CopyInfo.imageSubresource.baseArrayLayer = 0;
        CopyInfo.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;


        Internal::Vulkan::CommandBuffer& commandBuffer = Renderer::Self().GlobalTransferCommandBuffer();
        commandBuffer.RecordCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        vkCmdCopyBufferToImage(
            commandBuffer.GetHandle(),
            m_BufferHandle,
            pImage.m_Handle,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &CopyInfo
        );        

        commandBuffer.EndRecordingCommandBuffer();


        Renderer::Self().ExecuteCommandBuffers(
            Renderer::Self().GetQueues().Transfer,
            {commandBuffer.GetHandle()}
        );

		vkQueueWaitIdle(Renderer::Self().GetQueues().Transfer);

        return HYD_OK;
    }

        
    //Vertex Buffer Implementation:

    uint32 Vulkan::VertexBuffer::CreateBuffer(
        uint32              pSize,
        VkSharingMode       pSharingMode,        //= VK_SHARING_MODE_EXCLUSIVE,
        std::vector<uint32> pQueueFamilyIndices //= {}
    ) noexcept
    {
        return CreateGeneralBuffer(
            pSize, 
            (VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT),
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
           pSharingMode, pQueueFamilyIndices);
    }


    uint32 Vulkan::VertexBuffer::UploadData(
        void*  pData,
        uint64 pSize,
        uint64 pOffset //=0
    ) noexcept
    {
        
        StagingBuffer stagingBuffer;
        stagingBuffer.CreateBuffer(pSize);
        stagingBuffer.UploadData(pData, pSize);
        stagingBuffer.CopyBuffer(*this, pOffset);
        stagingBuffer.DestroyBuffer();

        return HYD_OK;
    }


    void Vulkan::VertexBuffer::Bind() noexcept
    {
        VkDeviceSize Offsets[] = {0};
        vkCmdBindVertexBuffers(
            Renderer::Self().GlobalRenderCommandBuffer().GetHandle(),
            0, 1, &m_BufferHandle, Offsets
        );
    }



    //

    uint32 Vulkan::IndexBuffer::CreateBuffer(
        uint32              pSize,
        uint64              pCount,
        VkIndexType         pIndexType,
        VkSharingMode       pSharingMode,        //= VK_SHARING_MODE_EXCLUSIVE,
        std::vector<uint32> pQueueFamilyIndices //= {}
    ) noexcept
    {
        m_Count     = pCount;
        m_IndexType = pIndexType;
        return CreateGeneralBuffer(pSize,
             (VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT),
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
             pSharingMode, pQueueFamilyIndices);
    }


    uint32 Vulkan::IndexBuffer::UploadData(
        void*  pData,
        uint64 pSize,
        uint64 pOffset //=0
    ) noexcept
    {
        
        StagingBuffer stagingBuffer;
        stagingBuffer.CreateBuffer(pSize);
        stagingBuffer.UploadData(pData, pSize);
        stagingBuffer.CopyBuffer(*this, pOffset);
        stagingBuffer.DestroyBuffer();

        return HYD_OK;
    }


    void Vulkan::IndexBuffer::Bind() noexcept
    {
        vkCmdBindIndexBuffer(
            Renderer::Self().GlobalRenderCommandBuffer().GetHandle(),
            m_BufferHandle, 0, m_IndexType
        );
    }


};
};