/*
    Implementation for Vulkan Shader
*/

#pragma once

#include "../../Common.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "../Shader.h"

namespace Hydrogen
{
namespace Vulkan
{

    //Vulkan Shader class:
    class Shader
    {
    public:

         Shader() = default;
        ~Shader() noexcept;

        //Copy 
        Shader(const Shader& pOther);
        
        //Move
        Shader(Shader&& pOther)             noexcept;
        Shader& operator=(Shader&& pOther)  noexcept;
        
        

        //Create shader module from SpirV code
        uint32 CreateShaderFromSpirV(
            ShaderType        pShaderType,
            const Buffer&     pCode,
            const char*       pEntryPoint = "main"
        ) noexcept; 

        //Load a SpirV binary, then create the shader
        uint32 CreateShaderFromFile(
            ShaderType         pShaderType,
            const std::string& pPath,
            const char*        pEntryPoint = "main"
        ) noexcept; 
 

        //Destroys the shader
        uint32 DestroyShader() noexcept;


        inline VkShaderModule        GetHandle()            const {return m_Handle;}
        inline const char*           GetEntryName()         const {return m_EntryPoint;}
        inline VkShaderStageFlagBits GetShaderType()        const {return VkShaderStageFlagBits(m_Stage);}
        

    private:
        VkShaderModule m_Handle     = VK_NULL_HANDLE;
        ShaderType     m_Stage      = HYD_STAGE_UNSPECIFIED;
        const char*    m_EntryPoint = "main"; //Default
    };


};
};