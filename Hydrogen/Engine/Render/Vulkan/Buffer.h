/*
    Vulkan Buffer Implementation
*/

#pragma once


#include "../../Common.h"
#include <vulkan/vulkan.h>


namespace Hydrogen
{


namespace Internal
{
namespace Vulkan
{



    class BasicBuffer
    {
    public:


                 BasicBuffer() noexcept;
        virtual ~BasicBuffer() noexcept;


        BasicBuffer(const BasicBuffer& pOther) = delete;


        BasicBuffer(BasicBuffer&& pOther)            noexcept;
        BasicBuffer& operator=(BasicBuffer&& pOther) noexcept;
        
        virtual uint32 UploadData(
            void*  pData,
            uint64 pSize,
            uint64 pOffset=0
        ) noexcept;

        
        virtual uint32 UploadDataWithoutMapping(
            void*  pData,
            uint64 pSize,
            uint64 pOffset=0
        ) noexcept;
        
        virtual uint32 DestroyBuffer() noexcept;

        uint32 MapMemory(
            uint32 pSize,
            uint32 pOffset=0
        )  noexcept;

        uint32 UnmapMemory() noexcept;

        inline VkBuffer  GetHandle() const {return m_BufferHandle;}
        inline VkBuffer* GetHandlePtr()    {return &m_BufferHandle;}
        inline uint64    GetSize()   const {return m_Size;}

        

    protected:
        uint32 CreateGeneralBuffer(
            uint32                pSize,
            VkBufferUsageFlags    pBufferUsage,
            VkMemoryPropertyFlags pMemoryProperty,
            VkSharingMode         pSharingMode        = VK_SHARING_MODE_EXCLUSIVE,
            std::vector<uint32>   pQueueFamilyIndices = {}
        ) noexcept;


    protected:
        VkBuffer              m_BufferHandle  = VK_NULL_HANDLE;
        VkDeviceMemory        m_MemoryHandle  = VK_NULL_HANDLE;
        uint64                m_Size          = 0;
        VkFormat              m_Format        = VK_FORMAT_UNDEFINED;
        VkBufferUsageFlags    m_BufferUsage;
        VkMemoryPropertyFlags m_MemoryProperty;
        void*                 m_VirtualAddress = nullptr;
    };



    //Staging Buffer:


    class StagingBuffer final : public BasicBuffer
    {
    public:

         StagingBuffer() = default;
        ~StagingBuffer() = default;       

        

        uint32 CreateBuffer(
            uint32              pSize,
            VkSharingMode       pSharingMode        = VK_SHARING_MODE_EXCLUSIVE,
            std::vector<uint32> pQueueFamilyIndices = {}
        ) noexcept;


        uint32 CopyBuffer(
            BasicBuffer& pDstBuffer,
            uint64       pOffset = 0
        ) noexcept;
    

        uint32 CopyImage(
            Image& pImage
        ) noexcept;

    };



    //Vertex Buffer:
    class VertexBuffer final : public BasicBuffer
    {
    public:
        
        uint32 CreateBuffer(
            uint32              pSize,
            VkSharingMode       pSharingMode        = VK_SHARING_MODE_EXCLUSIVE,
            std::vector<uint32> pQueueFamilyIndices = {}
        ) noexcept;

        uint32 UploadData(
            void*  pData,
            uint64 pSize,
            uint64 pOffset=0
        ) noexcept override;


        void Bind() const noexcept;

    private:
        StagingBuffer m_StaginBuffer;
    };

    //Index Buffer:


    class IndexBuffer final : public BasicBuffer
    {
    public:

        //Index Buffer Creation:
        uint32 CreateBuffer(
            uint32              pSize,
            uint64              pCount,
            VkIndexType         pIndexType,
            VkSharingMode       pSharingMode        = VK_SHARING_MODE_EXCLUSIVE,
            std::vector<uint32> pQueueFamilyIndices = {}
        ) noexcept;

        uint32 UploadData(
            void*  pData,
            uint64 pSize,
            uint64 pOffset=0
        ) noexcept override;


        void Bind() const  noexcept;

        inline const uint64      GetCount()     const {return m_Count;} 
        inline const VkIndexType GetIndexType() const {return m_IndexType;}

    private:
        uint64        m_Count = 0;
        VkIndexType   m_IndexType;
        StagingBuffer m_StaginBuffer;
    };

};
};
};