#pragma once

#include "../../Common.h"
#include "Shader.h"
#include <cstdint>
#include <vulkan/vulkan.h>
#include "VkEnumReDefs.h"

namespace Hydrogen
{
    
    struct ShaderUniformBinding
    {
        uint32       Binding;
        int32        Set            = -1; //Disable
        ShaderStages ShaderStage;
        UniformType  Type;
    };
    
    struct ShaderUniformLayout
    {
        public:
        HYD void AddUniformBinding(
            ShaderUniformBinding pBinding
        ) noexcept;
        private:
        std::vector<ShaderUniformBinding> m_Bindings;             
        
    };
    
    
    struct DescriptorBinding
    {
        uint32       Binding;
        uint32       Count;
        ShaderStages Stage;
        UniformType  Type;
    };
};

#include "UniformBuffer.h"

namespace Hydrogen
{

    struct DescriptorSetLayoutConfiguration
    {
    public:
        HYD void AddBinding(
            DescriptorBinding pBinding
        ) noexcept;
    private:
        std::vector<DescriptorBinding> m_Bindings;
    private:
        friend class Internal::Vulkan::DescriptorSetLayout;  
        friend class Hydrogen::Renderer;

    };

    

namespace Internal
{
namespace Vulkan
{
    struct DescriptorWriteBufferInfo
    {
        uint32   Binding    = UINT32_MAX;
        uint32   ArrayIndex = 0;
        uint32   Offset     = 0;
        uint32   Range      = UINT32_MAX;
        VkBuffer Buffer     = VK_NULL_HANDLE;
    }; 


    struct DescriptorWriteImageInfo
    {
        uint32        Binding     = UINT32_MAX;
        uint32        ArrayIndex  = 0;
        VkSampler     Sampler     = VK_NULL_HANDLE;
        VkImageView   View        = VK_NULL_HANDLE;
        VkImageLayout Layout      = VK_IMAGE_LAYOUT_UNDEFINED;
    }; 

    //Descriptor Pool Implementation
    class DescriptorPool final
    {
    public:

         DescriptorPool() noexcept;
        ~DescriptorPool() noexcept;

        DescriptorPool(const DescriptorPool&)             = delete;
        DescriptorPool& operator=(const DescriptorPool& ) = delete;

        DescriptorPool(DescriptorPool&& pOther)            noexcept;
        DescriptorPool& operator=(DescriptorPool&& pOther) noexcept;


    private: // Accessable by Friends only

        uint32 CreateDescriptorPool(
            std::vector<VkDescriptorPoolSize> pPoolSizes,
            uint32                            pMaxDescriptors,
            VkDescriptorPoolCreateFlags       pFlags = 0
        ) noexcept;

        void DestroyDescriptorPool() noexcept;

        DescriptorSet AllocateDescriptorSet(
            const DescriptorSetLayout&  pDescriptorSetLayout
        ) noexcept;


        std::vector<DescriptorSet> AllocateDescriptorSets(
            uint32                                     pCount,
            const std::vector<VkDescriptorSetLayout>&  pDescriptorSetLayouts
        ) noexcept;


        
    private:
        VkDescriptorPool m_Handle            = VK_NULL_HANDLE;
        uint32           m_ActiveDescriptors = 0;

    private:
        friend class Hydrogen::Renderer;
        friend class DescriptorSet;
    };

    class DescriptorSetLayout final
    {
    public:

         DescriptorSetLayout() noexcept;
        ~DescriptorSetLayout() noexcept;

        DescriptorSetLayout(const DescriptorSetLayout&)             = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&)  = delete;
        
        DescriptorSetLayout(DescriptorSetLayout&& pOther)            noexcept;
        DescriptorSetLayout& operator=(DescriptorSetLayout&& pOther) noexcept;


    private:

        uint32 CreateDescriptorSetLayout(
            DescriptorSetLayoutConfiguration pConf,
            VkDescriptorSetLayoutCreateFlags pFlags = 0
        )  noexcept;

        void DestroyDescriptorSetLayout() noexcept;


    private:
        VkDescriptorSetLayout                     m_Layout    = VK_NULL_HANDLE;
        std::vector<VkDescriptorSetLayoutBinding> m_Bindings;
    private:
        friend class Hydrogen::Renderer;
        friend class DescriptorPool;
        friend class PipelineLayout;
    };

    
    class DescriptorSet final
    {
    public:
    
        DescriptorSet(
            VkDescriptorSet pHandle = VK_NULL_HANDLE
        )  noexcept;

        ~DescriptorSet() noexcept;


        void AttachUniformBuffer(
            uint32         pBinding,
            UniformBuffer& pBuffer,
            uint32         pRange  = UINT32_MAX,
            uint32         pOffset = 0
        ) noexcept;


        void AttachTextureSampler(
            uint32      pBinding,
            Sampler&    pSampler,
            ImageView&  pView,
            ImageLayout pLayout
        ) noexcept;


        void UpdateDescriptorSet() noexcept;

        void Bind(
            const PipelineLayout& pLayout,
            uint32                pFirstSet
        ) noexcept;

    private:
        VkDescriptorSet                        m_Handle = VK_NULL_HANDLE;
        std::vector<DescriptorWriteBufferInfo> m_BufferInfos;
        std::vector<DescriptorWriteImageInfo>  m_ImageInfos;
        HYD_ID_SPACE                           m_ParentPool = 0;

    private:
        friend class Hydrogen::Renderer;
        friend class DescriptorPool;
        friend class DescriptorSetLayout;
        friend class UniformBuffer;
    };


};
};
};
