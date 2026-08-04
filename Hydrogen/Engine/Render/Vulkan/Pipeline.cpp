#include "Pipeline.h"
#include "../Renderer.h"
#include <vulkan/vulkan_core.h>


namespace Hydrogen
{


    Vulkan::Pipeline::Pipeline() noexcept
    {

        m_PrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        m_PrimitiveRestart  = VK_FALSE;

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


        m_DepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        m_DepthStencil.pNext = nullptr;


        m_ColorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        m_ColorBlendState.pNext = nullptr;
        m_Attachments.clear();

        
        m_DynamicStatesInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        m_DynamicStatesInfo.pNext = nullptr;
        m_DynamicStatesInfo.flags = 0; 

    }

    Vulkan::Pipeline::~Pipeline() noexcept
    {
        if (m_Handle != VK_NULL_HANDLE)
            DestroyPipeline();
    }

    Vulkan::Pipeline::Pipeline(Vulkan::Pipeline&& pOther) noexcept
    {
        m_Handle        = pOther.m_Handle;
        pOther.m_Handle = VK_NULL_HANDLE;
        
    }

    Vulkan::Pipeline& Vulkan::Pipeline::operator=(Vulkan::Pipeline&& pOther) noexcept
    {
        m_Handle        = pOther.m_Handle;
        pOther.m_Handle = VK_NULL_HANDLE;
        return *this;
    }

        
    //Vertex input
    void Vulkan::Pipeline::SetVertexInput(
    //empty for now
    ) noexcept
    {
        m_VertexInput.flags = 0;

        m_VertexInput.vertexAttributeDescriptionCount = 0;
        m_VertexInput.pVertexAttributeDescriptions    = nullptr;

        m_VertexInput.vertexBindingDescriptionCount   = 0;
        m_VertexInput.pVertexBindingDescriptions      = nullptr;
    }


    //Input Assembly:
    void Vulkan::Pipeline::SetInputAssembler(
        VkPrimitiveTopology pTopology,
        VkBool32            pPrimitiveRestart
    ) noexcept
    {
        m_PrimitiveTopology = pTopology;
        m_PrimitiveRestart  = pPrimitiveRestart;
    }



    void Vulkan::Pipeline::SetViewport(
        uint32 pWidth,
        uint32 pHeight,
        uint32 pX         ,//= 0
        uint32 pY         ,//= 0
        float  pMinDepth  ,//= 0.0f
        float  pMaxDepth   //= 0.0f
    ) noexcept
    {
        m_Viewport.width    = pWidth;
        m_Viewport.height   = pHeight;
        m_Viewport.x        = pX;
        m_Viewport.y        = pY;
        m_Viewport.minDepth = pMinDepth;
        m_Viewport.maxDepth = pMaxDepth;

        //default scissoring:
        m_Scissor.extent = VkExtent2D{pWidth, pHeight};
        m_Scissor.offset = VkOffset2D{0, 0};
    }


    void Vulkan::Pipeline::SetScissor(
        uint32 pWidth,
        uint32 pHeight,
        int32 pX,         //= 0,
        int32 pY          //= 0
    ) noexcept
    {
        m_Scissor.extent = VkExtent2D{pWidth, pHeight};
        m_Scissor.offset = VkOffset2D{pX, pY};
    }



    //Rasterizer State:

    void Vulkan::Pipeline::SetRasterizer(
        VkPolygonMode   pPolyMode                ,//= VK_POLYGON_MODE_FILL,
        VkCullModeFlags pCullMode                ,//= VK_CULL_MODE_BACK_BIT,
        VkFrontFace     pFrontFace               ,//= VK_FRONT_FACE_CLOCKWISE,
        VkBool32        pDepthClamp              ,//= VK_FALSE,
        VkBool32        pPrimitiveDiscard        ,//= VK_FALSE,
        float           pLineWidth               ,//= 1.0f,
        VkBool32        pDepthBiasEnable         ,//= VK_FALSE,
        float           pDepthBiasConstantFactor ,//= 0.0f,
        float           pDepthBiasClamp          ,//= 0.0f,
        float           pDepthBiasSlopeFactor     //= 0.0f
    ) noexcept
    {

        m_Rasterizer.polygonMode             = pPolyMode;
        m_Rasterizer.cullMode                = pCullMode;
        m_Rasterizer.frontFace               = pFrontFace;
        m_Rasterizer.depthClampEnable        = pDepthClamp;
        m_Rasterizer.rasterizerDiscardEnable = pPrimitiveDiscard;
        m_Rasterizer.lineWidth               = pLineWidth;
        m_Rasterizer.depthBiasEnable         = pDepthBiasEnable;
        m_Rasterizer.depthBiasConstantFactor = pDepthBiasConstantFactor;
        m_Rasterizer.depthBiasSlopeFactor    = pDepthBiasSlopeFactor;
        m_Rasterizer.depthBiasClamp          = pDepthBiasClamp;
    }


    //Depth & Stencil State

    void Vulkan::Pipeline::SetDepthStencil(
        VkBool32            pEnableTest      ,
        VkBool32            pEnableWrite     ,
        VkCompareOp         pCompareOperator ,//= VK_COMPARE_OP_LESS,
        VkBool32            pDepthBoundTest  ,//= VK_FALSE,
        float               pMinDepthBounds  ,//= 0.0f,
        float               pMaxDepthBounds  ,//= 1.0f,
        VkBool32            pStencilTest     ,//= VK_FALSE,
        VkStencilOpState    pStencilFront    ,//= {},
        VkStencilOpState    pStencilBack      //= {}
    ) noexcept
    {


        m_DepthStencil.depthTestEnable         = pEnableTest;
        m_DepthStencil.depthWriteEnable        = pEnableWrite;
        m_DepthStencil.depthCompareOp          = pCompareOperator;

        m_DepthStencil.depthBoundsTestEnable   = pDepthBoundTest;
        m_DepthStencil.maxDepthBounds          = pMaxDepthBounds;
        m_DepthStencil.minDepthBounds          = pMinDepthBounds;

        m_DepthStencil.stencilTestEnable       = pStencilTest;
        m_DepthStencil.front                   = pStencilFront;
        m_DepthStencil.back                    = pStencilBack;
    }


    //Dynamic States:

    void Vulkan::Pipeline::SetDynamicStates(
        const std::vector<VkDynamicState>& pStates,
        VkPipelineDynamicStateCreateFlags  //pFlags  = 0
    ) noexcept
    {
        m_DynamicStates = pStates;
        
        m_DynamicStatesInfo.dynamicStateCount = m_DynamicStates.size();
        m_DynamicStatesInfo.pDynamicStates    = m_DynamicStates.data();
    }

    //Color Blending:

    void Vulkan::Pipeline::SetColorBlend(
        std::vector<VkPipelineColorBlendAttachmentState> pAttachments     ,
        VkBool32                                         pLogicEnable     ,//= VK_FALSE,
        VkLogicOp                                        pLogicOp         ,//= VK_LOGIC_OP_COPY,
        std::array<float, 4>                             pBlendConstants  ,//= {0.0f, 0.0f, 0.0f, 0.0f},
        VkPipelineColorBlendStateCreateFlags             pFlags           //= 0
    ) noexcept
    {
        //Attachments
        m_Attachments = pAttachments;

        m_ColorBlendState.flags           = pFlags;
        m_ColorBlendState.attachmentCount = m_Attachments.size();
        m_ColorBlendState.pAttachments    = m_Attachments.data();
        m_ColorBlendState.logicOpEnable   = pLogicEnable;
        m_ColorBlendState.logicOp         = pLogicOp;

        m_ColorBlendState.blendConstants[0] = pBlendConstants[0];
        m_ColorBlendState.blendConstants[1] = pBlendConstants[1];
        m_ColorBlendState.blendConstants[2] = pBlendConstants[2];
        m_ColorBlendState.blendConstants[3] = pBlendConstants[3];

    }


            
    void Vulkan::Pipeline::CreatePipelineLayout(
    ) noexcept
    {
        VkPipelineLayoutCreateInfo CInfo{};

        CInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        CInfo.pNext = nullptr;
        CInfo.flags = 0;

        CInfo.setLayoutCount = 0;
        CInfo.pSetLayouts    = nullptr;

        CInfo.pushConstantRangeCount = 0;
        CInfo.pPushConstantRanges    = nullptr;

        VkResult Res = vkCreatePipelineLayout(Renderer::Self().GetDevice()
        , &CInfo, VULKAN_ALLOCATION_CALLBACK,
        &m_PipelineLayout);


        if (Res != VK_SUCCESS)
        {
            Log::SetError(Log::FmtStr("Pipeline Creation Failed. VkError:%i", int32(Res)));
        }

    }


    //Pipeline operations:


    uint32 Vulkan::Pipeline::CreatePipeline(
        const std::vector<Ptr<Vulkan::Shader>>& pShaders,
        VkRenderPass                            pRenderPass,
        uint32                                  pSubpass
    )   noexcept
    {

        if (m_Handle != VK_NULL_HANDLE)
            return HYD_FAILED;


        if (m_PipelineLayout == VK_NULL_HANDLE)
            return HYD_INVALID_VALUE;

        VkGraphicsPipelineCreateInfo CInfo{};
        CInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        CInfo.pNext = nullptr;
        CInfo.flags = 0;

        //Shader Settings:
        
        
        std::vector<VkPipelineShaderStageCreateInfo> Stages; Stages.resize(pShaders.size());
        for (uint32 I = 0 ; I < pShaders.size() ; ++I)
        {
            VkPipelineShaderStageCreateInfo StageCInfo{};
            StageCInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            StageCInfo.pNext  = nullptr;
            StageCInfo.flags  = 0;
            StageCInfo.pName  = pShaders[I]->GetEntryName();
            StageCInfo.module = pShaders[I]->GetHandle();
            StageCInfo.stage  = pShaders[I]->GetShaderType();
            Stages[I] = (StageCInfo);
        } 
        
        CInfo.stageCount = Stages.size();
        CInfo.pStages    = Stages.data();


        //Vertex Input:
        CInfo.pVertexInputState = &m_VertexInput;
        

        //Input Assembly:
        VkPipelineInputAssemblyStateCreateInfo InputAssemblyCInfo{};
        InputAssemblyCInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        InputAssemblyCInfo.pNext                  = nullptr;
        InputAssemblyCInfo.flags                  = 0;
        InputAssemblyCInfo.primitiveRestartEnable = m_PrimitiveRestart;
        InputAssemblyCInfo.topology               = m_PrimitiveTopology;

        CInfo.pInputAssemblyState = &InputAssemblyCInfo;


        //Tessellation Control:
        CInfo.pTessellationState = &m_TesselationControl;

        //Viewport States:

        VkPipelineViewportStateCreateInfo ViewportStateCInfo{};
        ViewportStateCInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        ViewportStateCInfo.pNext = nullptr;
        ViewportStateCInfo.flags = 0;

        ViewportStateCInfo.viewportCount = 1;
        ViewportStateCInfo.scissorCount  = 1;
        ViewportStateCInfo.pViewports    = &m_Viewport;
        ViewportStateCInfo.pScissors     = &m_Scissor;
        
        CInfo.pViewportState = &ViewportStateCInfo;
        

        //Rasterization State:
        CInfo.pRasterizationState = &m_Rasterizer;

        //Multisampling State:
        CInfo.pMultisampleState   = &m_MultiSampleState;

        //Depth Stencil:
        CInfo.pDepthStencilState  = &m_DepthStencil;

        //Color Blending State:
        CInfo.pColorBlendState    = &m_ColorBlendState;

        //Pipeline Layout:
        CInfo.layout              = m_PipelineLayout;


        CInfo.pDynamicState       = &m_DynamicStatesInfo;


        CInfo.renderPass = pRenderPass;
        CInfo.subpass    = pSubpass;

        CInfo.basePipelineHandle = VK_NULL_HANDLE;
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



    //Bind this pipeline
    uint32 Vulkan::Pipeline::BindPipeline(
        const Vulkan::CommandBuffer& pCommandBuffer,
        VkPipelineBindPoint          pBindPoint      //= VK_PIPELINE_BIND_POINT_GRAPHICS 
    ) noexcept
    {
        vkCmdBindPipeline(
            pCommandBuffer.GetHandle(),
             pBindPoint,
              m_Handle);
        return HYD_OK;
    }


    //Destroy

    uint32 Vulkan::Pipeline::DestroyPipeline()  noexcept
    {
        vkDestroyPipeline(Renderer::Self().GetDevice(), m_Handle, VULKAN_ALLOCATION_CALLBACK);
        vkDestroyPipelineLayout(Renderer::Self().GetDevice(), m_PipelineLayout, VULKAN_ALLOCATION_CALLBACK);

        m_Handle         = VK_NULL_HANDLE;
        m_PipelineLayout = VK_NULL_HANDLE;

        return HYD_OK;
    }

};