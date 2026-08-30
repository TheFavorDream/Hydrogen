#pragma once

#include "../../Common.h"
#include "Buffer.h"
#include "Descriptors.h"
#include "Pipeline.h"
#include <vulkan/vulkan.h>


namespace Hydrogen
{

namespace Internal
{
namespace Vulkan
{

    class UniformBuffer final : public BasicBuffer
    {
    public:

        uint32 CreateUniformBuffer(
            uint64 pSize
        ) noexcept;
        

        uint32 UploadData(
            void*  pData,
            uint64 pSize,
            uint64 pOffset = 0
        ) noexcept override;

        void Bind(
            const PipelineLayout& pPipelineLayout 
        ) noexcept;

        void SetDescriptorSet(HYD_ID_SPACE pSetID) noexcept;

    private:

        HYD_ID_SPACE m_DescriptorSetID = 0;
    
    private:
        friend class Hydrogen::Renderer;
        friend class DescriptorSetLayout;
        friend class DescriptorSet;
    };

};
};
};

