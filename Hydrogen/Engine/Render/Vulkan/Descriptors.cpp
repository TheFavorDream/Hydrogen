#include "Descriptors.h"
#include "../Renderer.h"
#include "Pipeline.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>


namespace Hydrogen
{


    void ShaderUniformLayout::AddUniformBinding(ShaderUniformBinding pBinding) noexcept
    {
        m_Bindings.push_back(pBinding);
    }


    void DescriptorSetLayoutConfiguration::AddBinding(
        DescriptorBinding pBinding
    ) noexcept
    {
        m_Bindings.push_back(pBinding);
    }

namespace Internal
{

    Vulkan::DescriptorPool::DescriptorPool() noexcept
        : m_Handle(VK_NULL_HANDLE), m_ActiveDescriptors(0)
    {
    }

    Vulkan::DescriptorPool::~DescriptorPool() noexcept
    {
        if (m_Handle != VK_NULL_HANDLE)
            DestroyDescriptorPool();
    }


    Vulkan::DescriptorPool::DescriptorPool(DescriptorPool&& pOther)  noexcept 
    {
        m_Handle            = pOther.m_Handle;
        m_ActiveDescriptors = pOther.m_ActiveDescriptors;
    
        pOther.m_Handle             = VK_NULL_HANDLE;
        pOther.m_ActiveDescriptors  = 0;
    }

    Vulkan::DescriptorPool& Vulkan::DescriptorPool::operator=(DescriptorPool&& pOther) noexcept
    {
        m_Handle            = pOther.m_Handle;
        m_ActiveDescriptors = pOther.m_ActiveDescriptors;
    
        pOther.m_Handle             = VK_NULL_HANDLE;
        pOther.m_ActiveDescriptors  = 0;
        
        return *this;
    }


    uint32 Vulkan::DescriptorPool::CreateDescriptorPool(
        std::vector<VkDescriptorPoolSize> pPoolSizes,
        uint32                            pMaxDesciptors,
        VkDescriptorPoolCreateFlags       pFlags //= 0
    ) noexcept
    {

        VkDescriptorPoolCreateInfo CInfo{};
        CInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        CInfo.pNext         = nullptr;
        CInfo.flags         = pFlags;
        CInfo.poolSizeCount = pPoolSizes.size();
        CInfo.pPoolSizes    = pPoolSizes.data();
        CInfo.maxSets       = pMaxDesciptors;
        
        VkResult Res = vkCreateDescriptorPool(
            Renderer::Self().GetDevice(),
            &CInfo, VULKAN_ALLOCATION_CALLBACK,
            &m_Handle
        );


        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Unable to Create Descriptor Pool. VkError:%i", int32(Res))
            );
            return HYD_FAILED;
        }
        return HYD_OK;
    }

    void Vulkan::DescriptorPool::DestroyDescriptorPool() noexcept
    {
        if (m_Handle == VK_NULL_HANDLE)
            return;
        
        vkDestroyDescriptorPool(
            Renderer::Self().GetDevice(),
            m_Handle, VULKAN_ALLOCATION_CALLBACK
        );

        m_Handle            = VK_NULL_HANDLE;
        m_ActiveDescriptors = 0;
    }


    Vulkan::DescriptorSet Vulkan::DescriptorPool::AllocateDescriptorSet(
        const DescriptorSetLayout& pDescriptorSetLayout
    ) noexcept
    {
        VkDescriptorSetAllocateInfo AllocInfo{};
        AllocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        AllocInfo.pNext              = nullptr;
        AllocInfo.descriptorPool     = m_Handle;
        AllocInfo.descriptorSetCount = 1;
        AllocInfo.pSetLayouts        = &pDescriptorSetLayout.m_Layout;


        DescriptorSet descriptorSet;

        VkResult Res = vkAllocateDescriptorSets(
            Renderer::Self().GetDevice(),
            &AllocInfo, &descriptorSet.m_Handle
        );

        m_ActiveDescriptors += 1;
        return std::move(descriptorSet);
    }


    std::vector<Vulkan::DescriptorSet> Vulkan::DescriptorPool::AllocateDescriptorSets(
        uint32                                     pCount,
        const std::vector<VkDescriptorSetLayout>&  pDescriptorSetLayouts

    ) noexcept
    {
        VkDescriptorSetAllocateInfo  AllocInfo{};
        AllocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        AllocInfo.pNext              = nullptr;
        AllocInfo.descriptorPool     = m_Handle;
        AllocInfo.descriptorSetCount = pCount;


        AllocInfo.pSetLayouts        = pDescriptorSetLayouts.data();

        std::vector<VkDescriptorSet> SetHandles;
        SetHandles.resize(pCount);

        VkResult Res = vkAllocateDescriptorSets(
            Renderer::Self().GetDevice(),
            &AllocInfo, SetHandles.data());


        m_ActiveDescriptors += pCount;
        return std::vector<DescriptorSet>(SetHandles.begin(), SetHandles.end());
    }





/*

    DescriptorSetLayout Implementation

*/

    Vulkan::DescriptorSetLayout::DescriptorSetLayout(
    ) noexcept 
        : m_Layout(nullptr)
    {}

    Vulkan::DescriptorSetLayout::~DescriptorSetLayout(
    ) noexcept
    {
        DestroyDescriptorSetLayout();
    }


    Vulkan::DescriptorSetLayout::DescriptorSetLayout(
        DescriptorSetLayout&& pOther
    )      noexcept
    {
        m_Layout           = pOther.m_Layout;
        m_Bindings         = std::move(pOther.m_Bindings);
        pOther.m_Layout    = nullptr;
    }


    Vulkan::DescriptorSetLayout& Vulkan::DescriptorSetLayout::operator=(
        DescriptorSetLayout&& pOther
    ) noexcept
    {
        m_Layout           = pOther.m_Layout;
        m_Bindings         = std::move(pOther.m_Bindings);
        pOther.m_Layout = nullptr;
        return *this;
    }




    uint32 Vulkan::DescriptorSetLayout::CreateDescriptorSetLayout(
        DescriptorSetLayoutConfiguration pConf,
        VkDescriptorSetLayoutCreateFlags pFlags //= 0
    )  noexcept
    {


        m_Bindings.resize(pConf.m_Bindings.size());

        for (uint32 I = 0 ; I < m_Bindings.size() ; ++I)
        {
            m_Bindings[I].binding         = pConf.m_Bindings[I].Binding;
            m_Bindings[I].descriptorCount = pConf.m_Bindings[I].Count;
            m_Bindings[I].descriptorType  = VkDescriptorType(pConf.m_Bindings[I].Type);
            m_Bindings[I].stageFlags      = VkShaderStageFlags(pConf.m_Bindings[I].Stage);
        }


        VkDescriptorSetLayoutCreateInfo CInfo{};

        CInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        CInfo.pNext = nullptr;

        CInfo.bindingCount = m_Bindings.size();
        CInfo.pBindings    = m_Bindings.data();

        CInfo.flags = pFlags;

        VkResult Res = vkCreateDescriptorSetLayout(
            Renderer::Self().GetDevice(),
            &CInfo,
            VULKAN_ALLOCATION_CALLBACK,
            &m_Layout
        );

        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Unable to Create DescriptorSet Layout. VkError:%i", int32(Res))
            );
            return HYD_FAILED;
        }
        return HYD_OK;
    }


    void Vulkan::DescriptorSetLayout::DestroyDescriptorSetLayout() noexcept
    {

        if (m_Layout == VK_NULL_HANDLE)
            return;

        vkDestroyDescriptorSetLayout(
            Renderer::Self().GetDevice(),
            m_Layout,
            VULKAN_ALLOCATION_CALLBACK 
        );

        m_Layout = VK_NULL_HANDLE;
        m_Bindings.clear();
    }


/*

    DescriptorSet Implementation:

*/


    Vulkan::DescriptorSet::DescriptorSet(
        VkDescriptorSet pHandle //= VK_NULL_HANDLE
    )  noexcept
     : m_Handle(pHandle)
    {}

    Vulkan::DescriptorSet::~DescriptorSet() noexcept
    {
        m_Handle = VK_NULL_HANDLE;
    }

    void Vulkan::DescriptorSet::AttachUniformBuffer(
        uint32         pBinding,
        UniformBuffer& pBuffer,
        uint32         pRange,  //= UINT32_MAX,
        uint32         pOffset //= 0
    ) noexcept
    {
        if (pBinding == UINT32_MAX)
            return;

        m_BufferInfos.push_back(
            DescriptorWriteBufferInfo{
                .Binding    = pBinding,
                .ArrayIndex = 0,
                .Offset     = pOffset,
                .Range      = pRange,
                .Buffer     = pBuffer.m_BufferHandle
            }
        );
    }

    void Vulkan::DescriptorSet::AttachTextureSampler(
        uint32      pBinding,
        Sampler&    pSampler,
        ImageView&  pView,
        ImageLayout pLayout
    ) noexcept
    {
        if (pBinding == UINT32_MAX)
            return;

        m_ImageInfos.push_back(
            DescriptorWriteImageInfo{
                .Binding     = pBinding,
                .ArrayIndex  = 0,
                .Sampler     = pSampler.m_Handle,
                .View        = pView.m_Handle,
                .Layout      = VkImageLayout(pLayout)
            }
        );
    }

    void Vulkan::DescriptorSet::UpdateDescriptorSet() noexcept
    {
        std::vector<VkDescriptorBufferInfo> BufferInfos; BufferInfos.resize(m_BufferInfos.size());
        std::vector<VkDescriptorImageInfo>  ImageInfos;  ImageInfos.resize(m_ImageInfos.size());
        std::vector<VkWriteDescriptorSet>   WriteOps;    WriteOps.resize(m_BufferInfos.size() + m_ImageInfos.size());
        uint32 WriteOpsIndex = 0;
        //Buffer Writes
        for (uint32 Iter = 0 ; Iter < m_BufferInfos.size() ; ++Iter)
        {
            DescriptorWriteBufferInfo& BufferInfo = m_BufferInfos.at(Iter);
            BufferInfos.at(Iter) = VkDescriptorBufferInfo{
                .buffer = BufferInfo.Buffer,
                .offset = BufferInfo.Offset,
                .range  = (BufferInfo.Range == UINT32_MAX)? VK_WHOLE_SIZE : BufferInfo.Range
            };

            WriteOps.at(WriteOpsIndex++) = VkWriteDescriptorSet{
                .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext           = nullptr,
                .dstSet          = m_Handle,
                .dstBinding      = BufferInfo.Binding,
                .dstArrayElement = BufferInfo.ArrayIndex,
                .descriptorCount = 1,
                .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .pBufferInfo     = &BufferInfos.at(Iter)
            }; 

        }

        //Image Writes
        for (uint32 Iter = 0 ; Iter < m_ImageInfos.size() ; ++Iter)
        {
            DescriptorWriteImageInfo& ImageInfo = m_ImageInfos.at(Iter);
            ImageInfos.at(Iter) = VkDescriptorImageInfo{
                .sampler      = ImageInfo.Sampler,
                .imageView    = ImageInfo.View,
                .imageLayout  = ImageInfo.Layout
            };

            WriteOps.at(WriteOpsIndex++) = VkWriteDescriptorSet{
                .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext           = nullptr,
                .dstSet          = m_Handle,
                .dstBinding      = ImageInfo.Binding,
                .dstArrayElement = ImageInfo.ArrayIndex,
                .descriptorCount = 1,
                .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .pImageInfo      = &ImageInfos.at(Iter)
            }; 
        }


        vkUpdateDescriptorSets(
            Renderer::Self().GetDevice(),
            WriteOps.size(),
            WriteOps.data(),
            0,
            nullptr
        );

    }


    void Vulkan::DescriptorSet::Bind(
        const Vulkan::PipelineLayout& pLayout,
        uint32                        pFirstSet
    ) noexcept
    {
        vkCmdBindDescriptorSets(
            Renderer::Self().GlobalRenderCommandBuffer().GetHandle(),
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pLayout.m_Handle,
            pFirstSet,1, &m_Handle,
            0, nullptr
        );
    }

};
};