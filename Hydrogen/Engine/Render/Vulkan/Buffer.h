/*
    Vulkan Buffer Implementation
*/

#pragma once


#include "../../Common.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{
namespace Vulkan
{


    class VertexBuffer final
    {
    public:
        
         VertexBuffer() noexcept;
        ~VertexBuffer() noexcept;

        //Copy
        VertexBuffer(const VertexBuffer& pOther) = delete;

        //Move 
        VertexBuffer(VertexBuffer&& pOther)             noexcept;
        VertexBuffer& operator=(VertexBuffer&& pOther)  noexcept;



        uint32 CreateBuffer(

        ) noexcept;

        uint32 DestroyBuffer(

        ) noexcept;


        uint32 Bind()   noexcept;
        uint32 Unbind() noexcept;



        inline VkBuffer GetHandle() const {return m_Handle;}
        inline uint64   GetSize()   const {return m_Size;}


    private:

        VkBuffer m_Handle = VK_NULL_HANDLE;
        uint64   m_Size   = 0;
        VkFormat m_Format = VK_FORMAT_UNDEFINED;
        
    };



};
};