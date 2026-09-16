#include "Light.h"
#include "../Render/Renderer.h"

namespace Hydrogen
{

    Light::Light(
        std::string pName,
        VecF3       pPosition,
        VecF3       pColor,
        VecF3       pAmbient,  //= VecF3(0.05f),
        VecF3       pDiffuse , //= VecF3(1.0f),
        VecF3       pSpecular  //= VecF3(1.0f)  
    ) noexcept
        : m_Name(pName),
          m_Position(pPosition),
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

        UploadData();
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

        if (m_IsDirty)
        {
            //Reupload Light data:
            UploadData();
        }


       Renderer::Self().AccessUniformBuffer(m_UniBuffer).Bind(
            pPipelineLayout
       );
    }

    const Light& LightCollection::AccessLight(
        uint32 pIndex
    ) const noexcept
    {
        ASSERT(pIndex < m_Lights.size(), "Light Collection: Out of Range");
        return m_Lights.at(pIndex);
    } 

    Light& LightCollection::EditLight(
        uint32 pIndex
    ) noexcept
    {
        ASSERT(pIndex < m_Lights.size(), "Light Collection: Out of Range");
        m_IsDirty = true;
        return m_Lights.at(pIndex);
    }

    /*
        Purpose: Uploads Light data in uniform buffer:
    */

    void LightCollection::UploadData() noexcept
    {
        struct Data
        {
            alignas(16) VecF3        Position;
            alignas(16) VecF3        Color;
            alignas(16) VecF3        Ambient;
            alignas(16) VecF3        Diffuse;
            alignas(16) VecF3        Specular;
        }; 

    
        std::vector<Data> Cache; Cache.resize(m_Lights.size());
        for (uint32 LightIndex = 0 ; LightIndex < m_LightCount ; ++LightIndex)
        {
            Cache.at(LightIndex) = Data
                                   {
                                        .Position = m_Lights.at(LightIndex).m_Position,
                                        .Color    = m_Lights.at(LightIndex).m_Color,
                                        .Ambient  = m_Lights.at(LightIndex).m_Ambient,
                                        .Diffuse  = m_Lights.at(LightIndex).m_Diffuse,
                                        .Specular = m_Lights.at(LightIndex).m_Specular
                                    };
            
        }

        for (uint32 FrameIndex = 0 ; FrameIndex < Renderer::Self().FramesInFlight() ; ++FrameIndex)
        {
            m_UniBuffer->at(FrameIndex).UploadData(
                Cache.data(),
                Cache.size()*sizeof(Data)
            );
        }

        m_IsDirty = false;
    }

};