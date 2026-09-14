#include "Pipeline.h"
#include "../Renderer.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>


namespace Hydrogen
{



    void PipelineLayoutConfiguration::AttachDescriptorLayout(
        HYD_ID_SPACE pDescSetLayoutID
    ) noexcept
    {
        m_Layouts.push_back(pDescSetLayoutID);
    }


    void PipelineLayoutConfiguration::SetPushConstant(
        uint32       pOffset,
        uint32       pSize,
        ShaderStages pShaderStage
    ) noexcept
    {
        m_PushConstants.push_back({
            .stageFlags = VkShaderStageFlags(pShaderStage),
            .offset     = pOffset,
            .size       = pSize
        });
    }

    //Shaders:
    void GraphicsPipelineConfiguration::AttachShader(
        const ShaderConfiguration& pShader
    ) noexcept
    {
        m_Shaders.push_back(pShader);
    }

    void GraphicsPipelineConfiguration::AttachShaders(
        const std::vector<ShaderConfiguration>& pShaders
    ) noexcept
    {
        m_Shaders = pShaders;
    }

    //Subpass :
    void GraphicsPipelineConfiguration::SetSubpass(
        uint32 pSubpassIndex
    ) noexcept
    {
        uint32 SubpassCount = Renderer::Self().RenderPass().SubpassCount();
        ASSERT(pSubpassIndex < SubpassCount, "Invalid subpass");

        m_Subpass = pSubpassIndex;
    }

    //VertexInput:

    void GraphicsPipelineConfiguration::AddVertexBufferLayout(
        Hydrogen::Internal::Vulkan::VertexAttribute pAttribs
    ) noexcept
    {
        m_BufferLayout = pAttribs;
    }

    //Viewport/Scissoring:
    void GraphicsPipelineConfiguration::SetViewport(
        Vec4<uint32> pViewport,
        Vec4<uint32> pScissor //= Vec4<uint32>(UINT32_MAX)
    ) noexcept
    {
        m_Viewport = pViewport;
        m_Scissor  = pScissor;
    }

    //Primitive Assembly:
    void GraphicsPipelineConfiguration::SetPrimitiveTogology(
        PrimitiveTopology pTogology//=HYD_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    ) noexcept
    {
        m_Topology = pTogology;
    }

    //Rasterizer:
    void GraphicsPipelineConfiguration::SetRasterizer(
        PolygonMode pPolyMode    ,//= HYD_POLYGON_MODE_FILL,
        CullModes   pCullingMode ,//= HYD_CULL_MODE_BACK,
        FrontFace   pFrontFace   ,//= HYD_FRONT_FACE_CLOCKWISE,
        float       pLineWidth   ,//= 1.0f,
        bool        pDepthClamp  //= false
    ) noexcept
    {
        m_PolyMode   = pPolyMode;
        m_CullMode   = pCullingMode;
        m_FrontFace  = pFrontFace;
        m_LineWidth  = pLineWidth;
        m_DepthClamp = pDepthClamp;
    }


    //Depth & Stencil:
    void GraphicsPipelineConfiguration::SetDepthStencil(
        bool             pEnableDepthWrite ,//= false,
        bool             pEnableDepthTest  ,//= false,
        CompareOperation pCompOp           ,//= HYD_COMPARE_OP_LESS,
        bool             pStencilTest      ,//= false,
        bool             pEnableBoundTest  ,//= false,
        VecF2            pBounds           //= VecF2(0.0f, 1.0f)
    ) noexcept
    {
        m_EnableDepthTest  = pEnableDepthTest;
        m_EnableDepthWrite = pEnableDepthWrite;
        m_CompareOperator  = pCompOp;
        m_StencilTest      = pStencilTest;
        m_DepthBoundTest   = pEnableBoundTest;
        m_MinDepthBounds   = pBounds.X;
        m_MaxDepthBounds   = pBounds.Y;
    }

    //Color Blending:
    void GraphicsPipelineConfiguration::ColorBlending(
        std::vector<ColorBlendState> pBlendStates   ,//= {},
        bool                         pEnableLogicOp ,//= false,
        LogicOperations              pOperation     //= HYD_LOGIC_OP_COPY
    ) noexcept
    {
        m_AttachmentStates  = std::move(pBlendStates);
        m_BlendLogicEnable  = pEnableLogicOp;
        m_ColorBlendLogicOp = pOperation;
    }

    void GraphicsPipelineConfiguration::SetPipelineLayout(
        HYD_ID_SPACE pLayoutID
    ) noexcept
    {
        m_LayoutID = pLayoutID;
    }


    void GraphicsPipelineConfiguration::SetParent(
        Ptr<Internal::Vulkan::GraphicsPipeline> pParent
    ) noexcept
    {
        m_ParentPipeline = pParent;
    }

namespace Internal
{


//Pipeline Layout Implementation:


    Vulkan::PipelineLayout:: PipelineLayout() noexcept
    {

    }

    Vulkan::PipelineLayout::~PipelineLayout() noexcept
    {
        DestroyLayout();
    }


    Vulkan::PipelineLayout::PipelineLayout(PipelineLayout&& pOther)   noexcept
     : m_Handle(pOther.m_Handle),
       m_DescSetLayoutIDs(std::move(pOther.m_DescSetLayoutIDs))
    {
        pOther.m_Handle = VK_NULL_HANDLE;
    }

    Vulkan::PipelineLayout& Vulkan::PipelineLayout::operator=(PipelineLayout&& pOther) noexcept
    {
       m_Handle           = pOther.m_Handle;
       m_DescSetLayoutIDs = std::move(pOther.m_DescSetLayoutIDs);    
       pOther.m_Handle    = VK_NULL_HANDLE;

        return *this;
    }

    uint32 Vulkan::PipelineLayout::CreateLayout(
        const PipelineLayoutConfiguration& pConf
    ) noexcept
    {

        m_DescSetLayoutIDs = std::vector(pConf.m_Layouts.begin(), pConf.m_Layouts.end());

        std::vector<VkDescriptorSetLayout> LayoutHandles; LayoutHandles.resize(pConf.m_Layouts.size());

        for (uint32 I = 0 ; I < LayoutHandles.size() ; ++I)
        {
            LayoutHandles[I] = Renderer::Self().AccessDescriptorSetLayout(m_DescSetLayoutIDs[I]).m_Layout;
        }


        VkPipelineLayoutCreateInfo CInfo{};
        CInfo.sType       = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        CInfo.pNext       = nullptr;
        CInfo.flags       = 0;

        CInfo.setLayoutCount   = LayoutHandles.size();
        if (LayoutHandles.size())
            CInfo.pSetLayouts  = LayoutHandles.data();

            
        CInfo.pushConstantRangeCount = pConf.m_PushConstants.size();
        if (pConf.m_PushConstants.size())
            CInfo.pPushConstantRanges    = pConf.m_PushConstants.data();
        

        VkResult Res = vkCreatePipelineLayout(
            Renderer::Self().GetDevice(),
            &CInfo,
            VULKAN_ALLOCATION_CALLBACK,
            &m_Handle
        );

        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Unable to Create PipelineLayout. VkError:%i", int32(Res))
            );
            return HYD_FAILED;
        }
        return HYD_OK;
    }


    void Vulkan::PipelineLayout::UpdatePushConstant(
        VkShaderStageFlags pStageFlags,
        uint32             pOffset,
        uint32             pSize,
        const void*        pValue
    ) const noexcept
    {
        vkCmdPushConstants(
            Renderer::Self().GlobalRenderCommandBuffer().GetHandle(),
            m_Handle,
            pStageFlags,
            pOffset,
            pSize,
            pValue
        );
    }



    void Vulkan::PipelineLayout::DestroyLayout() noexcept
    {        
        vkDestroyPipelineLayout(
            Renderer::Self().GetDevice(),
            m_Handle,
            VULKAN_ALLOCATION_CALLBACK);
        m_Handle = VK_NULL_HANDLE;
        m_DescSetLayoutIDs.clear();
    }


//Pipeline Implementation:

    Vulkan::GraphicsPipeline::GraphicsPipeline() noexcept
    {

        m_InputAssemblyCInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        m_InputAssemblyCInfo.pNext                  = nullptr;
        m_InputAssemblyCInfo.flags                  = 0;

        m_ViewportStateCInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        m_ViewportStateCInfo.pNext = nullptr;
        m_ViewportStateCInfo.flags = 0;

        m_MultiSampleState = {};
        m_MultiSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        m_MultiSampleState.pNext = nullptr;
        m_MultiSampleState.sampleShadingEnable   = VK_FALSE;
        m_MultiSampleState.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
        m_MultiSampleState.minSampleShading      = 1.0f; 
        m_MultiSampleState.pSampleMask           = nullptr; 
        m_MultiSampleState.alphaToCoverageEnable = VK_FALSE;
        m_MultiSampleState.alphaToOneEnable      = VK_FALSE;


        m_TesselationControl.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        m_TesselationControl.pNext = nullptr;

        m_VertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        m_VertexInput.pNext = nullptr;
        m_VertexInput.flags = 0;

        m_Rasterizer.sType  = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        m_Rasterizer.pNext  = nullptr;
        m_Rasterizer.flags  = 0;


        m_DepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        m_DepthStencil.pNext = nullptr;


        m_ColorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        m_ColorBlendState.pNext = nullptr;
        m_ColorBlendState.flags = 0;
        
        

	    VkPipelineColorBlendAttachmentState AttachmentColorBlend;
	    AttachmentColorBlend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
	    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	    AttachmentColorBlend.blendEnable 		 = VK_FALSE;
	    AttachmentColorBlend.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	    AttachmentColorBlend.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	    AttachmentColorBlend.colorBlendOp 		 = VK_BLEND_OP_ADD; // Optional
	    AttachmentColorBlend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	    AttachmentColorBlend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	    AttachmentColorBlend.alphaBlendOp 		 = VK_BLEND_OP_ADD; // Optional
        m_Attachments.clear();
        m_Attachments.push_back(AttachmentColorBlend); //default color blending
       // m_Attachments.push_back(AttachmentColorBlend); //default color blending

        
        m_DynamicStatesInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        m_DynamicStatesInfo.pNext = nullptr;
        m_DynamicStatesInfo.flags = 0; 

    }

    Vulkan::GraphicsPipeline::~GraphicsPipeline() noexcept
    {
        if (m_Handle != VK_NULL_HANDLE)
            DestroyPipeline();
    }

    Vulkan::GraphicsPipeline::GraphicsPipeline(Vulkan::GraphicsPipeline&& pOther) noexcept
    {
        m_Handle         = pOther.m_Handle;


        pOther.m_Handle         = VK_NULL_HANDLE;
    }

    Vulkan::GraphicsPipeline& Vulkan::GraphicsPipeline::operator=(Vulkan::GraphicsPipeline&& pOther) noexcept
    {
        m_Handle         = pOther.m_Handle;

        pOther.m_Handle         = VK_NULL_HANDLE;
        return *this;
    }

  


    //Pipeline operations:


    uint32 Vulkan::GraphicsPipeline::CreatePipeline(
        const GraphicsPipelineConfiguration& pConfig
    ) noexcept
    {

        //Shader Settings:
        
        std::vector<Shader> shaders;
        
        shaders.resize(pConfig.m_Shaders.size());
        m_Stages.resize(pConfig.m_Shaders.size());
        
        for (uint32 I = 0 ; I < pConfig.m_Shaders.size() ; ++I)
        {

            shaders.at(I).CreateShader(pConfig.m_Shaders[I]);

            VkPipelineShaderStageCreateInfo StageCInfo{};
            StageCInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            StageCInfo.pNext  = nullptr;
            StageCInfo.flags  = 0;
            StageCInfo.pName  = shaders.at(I).m_EntryPoint;
            StageCInfo.module = shaders.at(I).m_Handle;
            StageCInfo.stage  = shaders.at(I).m_Stage;
            m_Stages[I] = (StageCInfo);
        } 


        //Vertex Input:
        
        
        //Input Assembly:
        m_InputAssemblyCInfo.primitiveRestartEnable = VK_FALSE;
        m_InputAssemblyCInfo.topology               = VkPrimitiveTopology(pConfig.m_Topology);
        
        

        //Viewport States:


        VkViewport Viewport{
            .x        = static_cast<float>(pConfig.m_Viewport.X), 
            .y        = static_cast<float>(pConfig.m_Viewport.Y),
            .width    = static_cast<float>(pConfig.m_Viewport.Z),
            .height   = static_cast<float>(pConfig.m_Viewport.W),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };


        VkRect2D Scissor{
            .offset{
                .x = static_cast<int32>((pConfig.m_Scissor.X == UINT32_MAX)? 0 : pConfig.m_Scissor.X),
                .y = static_cast<int32>((pConfig.m_Scissor.Y == UINT32_MAX)? 0 : pConfig.m_Scissor.Y)
            },
            .extent{
                .width  = static_cast<uint32>((pConfig.m_Scissor.Z == UINT32_MAX)? pConfig.m_Viewport.Z : pConfig.m_Scissor.Z),
                .height = static_cast<uint32>((pConfig.m_Scissor.W == UINT32_MAX)? pConfig.m_Viewport.W : pConfig.m_Scissor.W)
            }
        };

        m_ViewportStateCInfo.viewportCount = 1;
        m_ViewportStateCInfo.scissorCount  = 1;
        m_ViewportStateCInfo.pViewports    = &Viewport;
        m_ViewportStateCInfo.pScissors     = &Scissor;
        
        m_Rasterizer.polygonMode      = (VkPolygonMode)pConfig.m_PolyMode;
        m_Rasterizer.cullMode         = (VkCullModeFlags)pConfig.m_CullMode;
        m_Rasterizer.frontFace        = (VkFrontFace)pConfig.m_FrontFace;
        m_Rasterizer.lineWidth        = pConfig.m_LineWidth;
        m_Rasterizer.depthClampEnable = VkBool32(pConfig.m_DepthClamp);
        m_Rasterizer.depthBiasEnable  = VK_FALSE;
        m_Rasterizer.rasterizerDiscardEnable = VK_FALSE;
        m_Rasterizer.depthBiasClamp          = 0;
    


        m_DepthStencil.depthTestEnable       = (VkBool32)pConfig.m_EnableDepthTest;
        m_DepthStencil.depthWriteEnable      = (VkBool32)pConfig.m_EnableDepthWrite;
        m_DepthStencil.stencilTestEnable     = (VkBool32)pConfig.m_StencilTest;
        m_DepthStencil.depthBoundsTestEnable = (VkBool32)pConfig.m_DepthBoundTest;
        m_DepthStencil.back                  = pConfig.m_StencilBack;
        m_DepthStencil.front                 = pConfig.m_StencilFront;
        m_DepthStencil.minDepthBounds        = pConfig.m_MinDepthBounds;
        m_DepthStencil.maxDepthBounds        = pConfig.m_MaxDepthBounds;
        m_DepthStencil.depthCompareOp        = (VkCompareOp)pConfig.m_CompareOperator;
        m_DepthStencil.flags                 = 0;


        VkDynamicState DynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        m_DynamicStatesInfo.dynamicStateCount = 2;
        m_DynamicStatesInfo.pDynamicStates    = DynamicStates;



        if (pConfig.m_AttachmentStates.size())
        {
            m_Attachments.clear();
            m_Attachments = pConfig.m_AttachmentStates;
        }
        
        m_ColorBlendState.attachmentCount   = m_Attachments.size();
        m_ColorBlendState.pAttachments      = m_Attachments.data();
        m_ColorBlendState.logicOp           = VkLogicOp(pConfig.m_ColorBlendLogicOp);
        m_ColorBlendState.logicOpEnable     = VkBool32(pConfig.m_BlendLogicEnable);
        m_ColorBlendState.blendConstants[0] = pConfig.m_BlendConstants[0];
        m_ColorBlendState.blendConstants[1] = pConfig.m_BlendConstants[1];
        m_ColorBlendState.blendConstants[2] = pConfig.m_BlendConstants[2];
        m_ColorBlendState.blendConstants[3] = pConfig.m_BlendConstants[3];



       
        m_VertexInput.vertexBindingDescriptionCount = pConfig.m_BufferLayout.m_BindingDescriptions.size();
        m_VertexInput.pVertexBindingDescriptions    = pConfig.m_BufferLayout.m_BindingDescriptions.data();

        m_VertexInput.vertexAttributeDescriptionCount = pConfig.m_BufferLayout.m_AttributeDescriptions.size();
        m_VertexInput.pVertexAttributeDescriptions    = pConfig.m_BufferLayout.m_AttributeDescriptions.data();


        m_Subpass        = pConfig.m_Subpass;
        m_PipelineLayout = pConfig.m_LayoutID;

        m_Parent = pConfig.m_ParentPipeline;

        return CreatePipelineObject();
    }
    
    //Bind this pipeline
    void Vulkan::GraphicsPipeline::BindPipeline() const noexcept
    {
        vkCmdBindPipeline(
            Renderer::Self().GlobalRenderCommandBuffer().GetHandle(),
             VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_Handle);
    }


    //Destroy

    uint32 Vulkan::GraphicsPipeline::DestroyPipeline()  noexcept
    {
        vkDestroyPipeline(Renderer::Self().GetDevice(), m_Handle, VULKAN_ALLOCATION_CALLBACK);
        //vkDestroyPipelineLayout(Renderer::Self().GetDevice(), m_PipelineLayout, VULKAN_ALLOCATION_CALLBACK);

        m_Handle         = VK_NULL_HANDLE;
        //m_PipelineLayout = VK_NULL_HANDLE;

        return HYD_OK;
    }




    uint32 Vulkan::GraphicsPipeline::CreatePipelineObject() noexcept
    {
        if (m_Handle != VK_NULL_HANDLE)
            return HYD_FAILED;


        VkGraphicsPipelineCreateInfo CInfo{};
        CInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        CInfo.pNext = nullptr;
        CInfo.flags = (m_Parent != nullptr)?  VK_PIPELINE_CREATE_DERIVATIVE_BIT : 0;

        
        CInfo.stageCount          = m_Stages.size();
        CInfo.pStages             = m_Stages.data();

        CInfo.pVertexInputState   = &m_VertexInput;
        CInfo.pInputAssemblyState = &m_InputAssemblyCInfo;
        //Tessellation Control:
        CInfo.pTessellationState  = &m_TesselationControl;
        CInfo.pViewportState      = &m_ViewportStateCInfo;
        //Rasterization State:
        CInfo.pRasterizationState = &m_Rasterizer;

        //Multisampling State:
        CInfo.pMultisampleState   = &m_MultiSampleState;
        //Depth Stencil:
        CInfo.pDepthStencilState  = &m_DepthStencil;
        //Color Blending State:
        CInfo.pColorBlendState    = &m_ColorBlendState;

        //Pipeline Layout:
        CInfo.pDynamicState       = &m_DynamicStatesInfo;
        CInfo.layout              = Renderer::Self().AccessPipelineLayout(m_PipelineLayout).m_Handle;

        CInfo.renderPass = Renderer::Self().RenderPass().m_Handle;
        CInfo.subpass    = m_Subpass;

        CInfo.basePipelineHandle = (m_Parent != nullptr)? m_Parent->m_Handle : VK_NULL_HANDLE;
        CInfo.basePipelineIndex  = -1;

        //Create Pipeline:

        VkResult Res = vkCreateGraphicsPipelines(
            Renderer::Self().GetDevice(),
            VK_NULL_HANDLE, 
            1, &CInfo,
            VULKAN_ALLOCATION_CALLBACK,
            &m_Handle);


        if (Res != VK_SUCCESS)
        {
            Log::SetError(Log::FmtStr("Unable to Create Graphics Pipeline. VkError:%i", (int32)Res));
            return HYD_FAILED;
        }


        return HYD_OK;
    }



};
};