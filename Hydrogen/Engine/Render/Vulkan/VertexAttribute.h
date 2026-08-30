


#pragma once

#include "../../Common.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{
namespace Internal
{
namespace Vulkan
{

    struct VertexAttribute
    {
        std::vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;
        std::vector<VkVertexInputBindingDescription  > m_BindingDescriptions;
    };

};
};
};