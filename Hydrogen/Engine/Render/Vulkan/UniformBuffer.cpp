#include "UniformBuffer.h"
#include "../Renderer.h"
#include "Buffer.h"
#include "Descriptors.h"
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{
namespace Internal
{

    uint32 Vulkan::UniformBuffer::CreateUniformBuffer(
        uint64 pSize
    ) noexcept
    {
        uint32 Res = CreateGeneralBuffer(
            pSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        vkMapMemory(
            Renderer::Self().GetDevice(),
            m_MemoryHandle, 0, pSize, 0, &m_VirtualAddress
        );

        return Res;
    }
        
    uint32 Vulkan::UniformBuffer::UploadData(
        void*  pData,
        uint64 pSize,
        uint64 pOffset //=0
    ) noexcept
    {
        memcpy((void*)((int8*)m_VirtualAddress+pOffset), pData, pSize);
        return HYD_OK;
    }


    void Vulkan::UniformBuffer::Bind(
        const PipelineLayout& pPipelineLayout 
    ) noexcept
    {

        if (!m_DescriptorSetID)
            return;

        Renderer::Self().AccessDescriptorSet(m_DescriptorSetID).Bind(pPipelineLayout, m_Binding.Set);
    }


    void Vulkan::UniformBuffer::SetDescriptorSet(HYD_ID_SPACE pSetID) noexcept
    {   
        m_DescriptorSetID = pSetID;
    }

};
};