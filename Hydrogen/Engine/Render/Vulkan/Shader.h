/*
    Implementation for Vulkan Shader
*/

#pragma once

#include "../../Common.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "VkEnumReDefs.h"

namespace Hydrogen
{


    struct ShaderConfiguration
    {
        ShaderType  Type;
        Buffer      Binary;
        std::string Path;
        std::string EntryName = "main";
    };


namespace Internal
{
namespace Vulkan
{

    //Vulkan Shader class:
    class Shader
    {
    public:

        Shader() noexcept;
        ~Shader() noexcept;

        //Copy 
        Shader(const Shader& pOther) = delete;
        
        //Move
        Shader(Shader&& pOther)             noexcept;
        Shader& operator=(Shader&& pOther)  noexcept;
        
        
    private:

        //create shader
        uint32 CreateShader(
            const ShaderConfiguration& pConf
        ) noexcept; 
 

        //Destroys the shader
        uint32 DestroyShader() noexcept;
        

    private:
        VkShaderModule             m_Handle     = VK_NULL_HANDLE;
        VkShaderStageFlagBits      m_Stage;
        const char*    m_EntryPoint = "main"; //Default


    private:
        friend class Hydrogen::Renderer;
        friend class GraphicsPipeline; 
    };


};
};

    typedef Internal::Vulkan::Shader Shader;
};