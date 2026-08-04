#include "Shader.h"
#include "../Renderer.h"
#include <vulkan/vulkan_core.h>
#include "../../Core/FileSystem.h"

namespace Hydrogen
{


    Vulkan::Shader::~Shader() noexcept
    {
        if (m_Handle != VK_NULL_HANDLE)
            DestroyShader();
    
        m_EntryPoint = "main";
        m_Stage      = HYD_STAGE_UNSPECIFIED;
    }

    //Move
    Vulkan::Shader::Shader(Shader&& pOther) noexcept
    {
        m_Handle        = pOther.m_Handle;
        m_EntryPoint    = pOther.m_EntryPoint;
        m_Stage         = pOther.m_Stage;
        
        pOther.m_Handle = VK_NULL_HANDLE;
        pOther.m_Stage  = HYD_STAGE_UNSPECIFIED;
    }

    Vulkan::Shader& Vulkan::Shader::operator=(Shader&& pOther) noexcept
    {
        m_Handle        = pOther.m_Handle;
        m_EntryPoint    = pOther.m_EntryPoint;
        m_Stage         = pOther.m_Stage;
        
        pOther.m_Handle = VK_NULL_HANDLE;
        pOther.m_Stage  = HYD_STAGE_UNSPECIFIED;

        return *this;
    }


    //Create shader module from SpirV code
    uint32 Vulkan::Shader::CreateShaderFromSpirV(
        ShaderType        pShaderType,
        const Buffer&     pCode,
        const char*       pEntryPoint
    ) noexcept
    {
        VkShaderModuleCreateInfo CInfo{};
        CInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        CInfo.pNext    = nullptr;
        CInfo.codeSize = pCode.Length();
        CInfo.pCode    = static_cast<uint32_t*>((void*)pCode.GetPtr());
        

        VkResult Res = vkCreateShaderModule(
            Renderer::Self().GetDevice(),
       &CInfo,
        VULKAN_ALLOCATION_CALLBACK,
     &m_Handle);


        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Unable to Create Shader Module. VkCode:%i", uint32(Res))
            );
            return HYD_FAILED;
        }

        m_Stage = pShaderType;

        return HYD_OK;
    }

    //Load a SpirV binary, then create the shader
    uint32 Vulkan::Shader::CreateShaderFromFile(
        ShaderType         pShaderType,
        const std::string& pPath,
        const char*        pEntryPoint
    ) noexcept
    {
        Buffer Code = FileSys::ReadFile(pPath);

        if (!Code.GetPtr())
            return HYD_FAILED;

        return CreateShaderFromSpirV(pShaderType, Code, pEntryPoint);
    }

    //Destroys the shader
    uint32 Vulkan::Shader::DestroyShader() noexcept
    {
        vkDestroyShaderModule(
                  Renderer::Self().GetDevice(),
            m_Handle,
              VULKAN_ALLOCATION_CALLBACK
            );
 
        m_Handle = VK_NULL_HANDLE;    
        return HYD_OK;
    }

};
