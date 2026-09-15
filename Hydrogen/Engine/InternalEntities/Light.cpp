#include "Light.h"
#include "../Render/Renderer.h"

namespace Hydrogen
{

    Light::Light(
        VecF3 pPosition,
        VecF3 pColor,
        VecF3 pAmbient,  //= VecF3(0.05f),
        VecF3 pDiffuse , //= VecF3(1.0f),
        VecF3 pSpecular  //= VecF3(1.0f)  
    ) noexcept
        : m_Position(pPosition),
          m_Color(pColor),
          m_Ambient(pAmbient),
          m_Diffuse(pDiffuse),
          m_Specular(pSpecular)
    {
        
    }

//-----------------------Light Collection------------------------------------

    LightCollection:: LightCollection() noexcept
        : m_LightCount(0), m_DescriptorSetID(0), m_DescriptorPool(0)
    {

    }
    LightCollection::~LightCollection() noexcept
    {
        m_Lights.clear();
        m_DescriptorSetID = 0;
        m_LightCount      = 0;
        m_DescriptorPool  = 0;
    }

    LightCollection::LightCollection(const LightCollection& pOther) noexcept
    {
        ASSERT(false, "Not Implemented");
    }
    LightCollection::LightCollection(LightCollection&&      pOther) noexcept
        : m_Lights(std::move(pOther.m_Lights)),
          m_UniBuffer(std::move(pOther.m_UniBuffer)),
          m_LightCount(pOther.m_LightCount),
          m_DescriptorSetID(pOther.m_DescriptorSetID),
          m_DescriptorPool(pOther.m_DescriptorPool)

    {
        pOther.m_LightCount      = 0;
        pOther.m_DescriptorSetID = 0;
        pOther.m_DescriptorPool  = 0;
    }


    //Asign 
    LightCollection& LightCollection::operator=(const LightCollection& pOther) noexcept
    {
        ASSERT(false, "Not Implemented");
        return *this;
    }
    LightCollection& LightCollection::operator=(LightCollection&&      pOther) noexcept
    {
        m_Lights          = std::move(pOther.m_Lights);
        m_UniBuffer       = std::move(pOther.m_UniBuffer);
        m_LightCount      = pOther.m_LightCount;
        m_DescriptorSetID = pOther.m_DescriptorSetID; 
    
        pOther.m_LightCount      = 0;
        pOther.m_DescriptorSetID = 0;
        pOther.m_DescriptorPool  = 0;
        return *this;
    }
        
    uint32 LightCollection::CreateCollection(
        uint32 pSet,
        uint32 pDescSetID
    ) noexcept
    {


        //Allocating the Descriptor Set:
        m_DescriptorSetID = Renderer::Self().AllocateDescriptorSet(pDescSetID);



        //Allocating Uniform Buffers:
        uint32 BuffSize = sizeof(Light) * m_Lights.size();

        m_UniBuffer = Renderer::Self().CreateUniformBuffer(
            BuffSize,
            ShaderUniformBinding{
                .Binding     = 0,
                .Set         = int32(pSet), 
                .ShaderStage = HYD_SHADER_STAGE_FRAGMENT_BIT,
                .Type        = HYD_DESCRIPTOR_TYPE_UNIFORM_BUFFER
            },
            m_DescriptorSetID
        );

        for (uint32 FrameIndex = 0 ; FrameIndex < Renderer::Self().FramesInFlight() ; ++FrameIndex)
        {
            m_UniBuffer->at(FrameIndex).UploadData(
                m_Lights.data(),
                BuffSize
            );
        }
        return HYD_OK;
    }

    uint32 LightCollection::CreateLight(
        Light&& pNewLight
    ) noexcept
    {
        m_Lights.emplace_back(
            pNewLight
        );
        m_LightCount += 1;
        return HYD_OK;
    }

    /*
        Purpose: Bind the Entire Light Collection to the specified Descriptor Set
    */
    HYD void LightCollection::BindCollection(
        const Internal::Vulkan::PipelineLayout& pPipelineLayout 
    ) noexcept
    {
       Renderer::Self().AccessUniformBuffer(m_UniBuffer).Bind(
            pPipelineLayout
       );
    }

    Light& LightCollection::AccessLight(
        uint32 pIndex
    ) noexcept
    {
        ASSERT(pIndex < m_Lights.size(), "Light Collection: Out of Range");
        return m_Lights.at(pIndex);
    } 

};