#include "Shader.h"
#include "../Renderer.h"
#include <vulkan/vulkan_core.h>
#include "../../Core/FileSystem.h"

namespace Hydrogen
{
namespace Internal
{


    Vulkan::Shader::Shader() noexcept
    {
        
    }



    Vulkan::Shader::~Shader() noexcept
    {
        if (m_Handle != VK_NULL_HANDLE)
            DestroyShader();
    
        m_EntryPoint = "main";

    }

    //Move
    Vulkan::Shader::Shader(Shader&& pOther) noexcept
    {
        m_Handle        = pOther.m_Handle;
        m_EntryPoint    = pOther.m_EntryPoint;
        m_Stage         = pOther.m_Stage;
        
        pOther.m_Handle = VK_NULL_HANDLE;
    }

    Vulkan::Shader& Vulkan::Shader::operator=(Shader&& pOther) noexcept
    {
        m_Handle        = pOther.m_Handle;
        m_EntryPoint    = pOther.m_EntryPoint;
        m_Stage         = pOther.m_Stage;
    
        return *this;
    }




    //Load a SpirV binary, then create the shader
    uint32 Vulkan::Shader::CreateShader(
        const ShaderConfiguration& pConf
    ) noexcept
    {
        
        VkShaderModuleCreateInfo CInfo{};
        CInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        CInfo.pNext    = nullptr;


        Buffer Code;
        if (!pConf.Path.empty())
        {
            Code = Hydrogen::Internal::FileSys::ReadFile(pConf.Path);

            if (!Code.GetPtr())
                return HYD_FAILED;

            CInfo.codeSize = Code.Length();
            CInfo.pCode    = reinterpret_cast<uint32_t*>(Code.GetPtr());
        }
        
        else if (pConf.Binary.GetPtr())
        {
            CInfo.codeSize = pConf.Binary.Length();
            CInfo.pCode    = reinterpret_cast<uint32_t*>(pConf.Binary.GetPtr());
        }

        

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

        m_Stage = VkShaderStageFlagBits(pConf.Type);

        return HYD_OK;
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
};
