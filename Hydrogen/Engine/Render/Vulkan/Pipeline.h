/*
    Implementation for Vulkan pipeline
*/

#pragma once

#include "../../Common.h"
#include "Renderpass.h"
#include "Commands.h"
#include "Shader.h"
#include "Descriptors.h"
#include "VertexAttribute.h"
#include <cstdint>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "../../VecMath/Vector/Vectors.h"
#include "VkEnumReDefs.h"
#include "../../Core/ResourcePool.h"

namespace Hydrogen{
    
    
    typedef VkPipelineColorBlendAttachmentState ColorBlendState;
    typedef VkStencilOpState                    StencilOpState;
    



    struct PipelineLayoutConfiguration
    {
    public:
        HYD void AttachDescriptorLayout(
            HYD_ID_SPACE pDescSetLayoutID
        ) noexcept;
    
        HYD void SetPushConstant(
            uint32       pOffset,
            uint32       pSize,
            ShaderStages pShaderStage
        ) noexcept;

    private:
        std::vector<VkPushConstantRange>       m_PushConstants;
        std::vector<HYD_ID_SPACE>              m_Layouts;

    private:
        friend class Renderer;
        friend class Internal::Vulkan::PipelineLayout;
    };

    struct GraphicsPipelineConfiguration
    {
    public:

        HYD void AddVertexBufferLayout(
            Hydrogen::Internal::Vulkan::VertexAttribute pAttribs
        ) noexcept;

        //Shaders:
        HYD void AttachShader(
            const ShaderConfiguration& pShader
        ) noexcept;

        HYD void AttachShaders(
            const std::vector<ShaderConfiguration>& pShaders
        ) noexcept;

        //Subpass :
        HYD void SetSubpass(
            uint32 pSubpassIndex
        ) noexcept;


        //Viewport/Scissoring:

        HYD void SetViewport(
            Vec4<uint32> pViewport,
            Vec4<uint32> pScissor = Vec4<uint32>(uint32(UINT32_MAX))
        ) noexcept;

        //Primitive Assembly:
        HYD void SetPrimitiveTogology(
            PrimitiveTopology pTogology=HYD_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
        ) noexcept;

        //Rasterizer:
        HYD void SetRasterizer(
            PolygonMode pPolyMode    = HYD_POLYGON_MODE_FILL,
            CullModes   pCullingMode = HYD_CULL_MODE_BACK,
            FrontFace   pFrontFace   = HYD_FRONT_FACE_CLOCKWISE,
            float       pLineWidth   = 1.0f,
            bool        pDepthClamp  = false
        ) noexcept;


        //Depth & Stencil:
        HYD void SetDepthStencil(
            bool             pEnableDepthWrite = false,
            bool             pEnableDepthTest  = false,
            CompareOperation pCompOp           = HYD_COMPARE_OP_LESS,
            bool             pStencilTest      = false,
            bool             pEnableBoundTest  = false,
            VecF2            pBounds           = VecF2(0.0f, 1.0f)
        ) noexcept;

        //Color Blending:
        HYD void ColorBlending(
            std::vector<ColorBlendState> pBlendStates   = {},
            bool                         pEnableLogicOp = false,
            LogicOperations              pOperation     = HYD_LOGIC_OP_COPY
        ) noexcept;


        HYD void SetPipelineLayout(
            HYD_ID_SPACE pLayoutID
        ) noexcept;

        //Set the parent
        HYD void SetParent(
            Ptr<Internal::Vulkan::GraphicsPipeline> pParent
        ) noexcept;

    private:

        HYD_ID_SPACE                          m_LayoutID;

        std::vector<ShaderConfiguration>      m_Shaders;
        uint32                                m_Subpass;
        Internal::Vulkan::VertexAttribute     m_BufferLayout;
        std::vector<DynamicState>             m_States;

        Vec4<uint32>                          m_Viewport;
        Vec4<uint32>                          m_Scissor;
        
        PrimitiveTopology                     m_Topology;
        
        PolygonMode                           m_PolyMode        = HYD_POLYGON_MODE_FILL;
        CullModes                             m_CullMode        = HYD_CULL_MODE_BACK;
        FrontFace                             m_FrontFace       = HYD_FRONT_FACE_CLOCKWISE;
        bool                                  m_DepthClamp      = false;
        float                                 m_LineWidth       = 1.0f;
        
        bool                                  m_EnableDepthTest = false;
        bool                                  m_EnableDepthWrite= false;
        CompareOperation                      m_CompareOperator = HYD_COMPARE_OP_LESS;
        bool                                  m_DepthBoundTest  = false;
        float                                 m_MinDepthBounds  = 0.0f;
        float                                 m_MaxDepthBounds  = 1.0f;
        bool                                  m_StencilTest     = false;
        StencilOpState                        m_StencilFront    = {};
        StencilOpState                        m_StencilBack     = {};
        
        std::vector<ColorBlendState>          m_AttachmentStates;
        bool                                  m_BlendLogicEnable  = false;
        LogicOperations                       m_ColorBlendLogicOp = HYD_LOGIC_OP_COPY;
        std::array<float, 4>                  m_BlendConstants    = {0.0f, 0.0f, 0.0,0.0f};
    
        Ptr<Internal::Vulkan::GraphicsPipeline> m_ParentPipeline = nullptr;
    
    private:
        friend class Internal::Vulkan::GraphicsPipeline;
        friend class Renderer;
    };
    
    


namespace Internal
{
namespace Vulkan{


    class PipelineLayout final
    {
    public:

         PipelineLayout() noexcept;
        ~PipelineLayout() noexcept;


        //Copy
        PipelineLayout(const PipelineLayout& pOther)            = delete;
        PipelineLayout& operator=(const PipelineLayout& pOther) = delete;

        //Move
        PipelineLayout(PipelineLayout&&)            noexcept;
        PipelineLayout& operator=(PipelineLayout&&) noexcept;

    private:

        uint32 CreateLayout(
            const PipelineLayoutConfiguration& pConf
        ) noexcept;


        void UpdatePushConstant(
            VkShaderStageFlags pStageFlags,
            uint32             pOffset,
            uint32             pSize,
            const void*        pValue
        ) const noexcept;

        void DestroyLayout() noexcept;




    private:
        VkPipelineLayout                  m_Handle         = VK_NULL_HANDLE;
        std::vector<VkPushConstantRange>  m_PushConstants;
        std::vector<HYD_ID_SPACE>         m_DescSetLayoutIDs;
    
    private:
        friend class Hydrogen::Renderer;
        friend class DescriptorSetLayout;
        friend class DescriptorSet;
        friend class GraphicsPipeline;
        friend class UniformBuffer;
    };



    class GraphicsPipeline final
    {
    public:

         GraphicsPipeline() noexcept;
        ~GraphicsPipeline() noexcept;

        GraphicsPipeline(const GraphicsPipeline& pOther) = delete;

        //Move
        GraphicsPipeline(GraphicsPipeline&&            pOther) noexcept;
        GraphicsPipeline& operator=(GraphicsPipeline&& pOther) noexcept;

    private: //friend accessable 
        //inline const VkPipelineLayout GetLayoutHandle() const {return m_PipelineLayout;};

        uint32 CreatePipeline(
            const GraphicsPipelineConfiguration& pConfig
        )   noexcept;

        uint32 DestroyPipeline()  noexcept;
        
        //Bind this pipeline
        void BindPipeline() noexcept;


    private: //Member Accessable
        uint32 CreatePipelineObject() noexcept;

    private:

        VkPipeline m_Handle = VK_NULL_HANDLE;

        HYD_ID_SPACE m_PipelineLayout;

        uint32 m_Subpass;
        std::vector<VkPipelineShaderStageCreateInfo> m_Stages;
        //Dynamic States:
        VkPipelineDynamicStateCreateInfo     m_DynamicStatesInfo;
        std::vector<VkDynamicState>          m_DynamicStates;
        //Vertex Input State
        VkPipelineVertexInputStateCreateInfo m_VertexInput{};

        //Input Assembler States:
        VkPipelineInputAssemblyStateCreateInfo m_InputAssemblyCInfo;

        //Viewport & Scissor:
        VkPipelineViewportStateCreateInfo m_ViewportStateCInfo;

        //Rasterizer State:
        VkPipelineRasterizationStateCreateInfo  m_Rasterizer;
        //Tessellation Control:
        VkPipelineTessellationStateCreateInfo   m_TesselationControl;
        //Multisampling State:
        VkPipelineMultisampleStateCreateInfo    m_MultiSampleState;
        //Color Blending:
        std::vector<VkPipelineColorBlendAttachmentState> m_Attachments;
        VkPipelineColorBlendStateCreateInfo              m_ColorBlendState;
        //Depth & Stencil State:
        VkPipelineDepthStencilStateCreateInfo   m_DepthStencil;

        Ptr<GraphicsPipeline> m_Parent = nullptr;

    private:
        friend class Hydrogen::Renderer;
    };

};
};

};