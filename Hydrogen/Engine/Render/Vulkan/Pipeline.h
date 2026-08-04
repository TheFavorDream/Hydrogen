/*
    Implementation for Vulkan pipeline
*/

#pragma once

#include "../../Common.h"
#include "Renderpass.h"
#include "Commands.h"
#include "Shader.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "../../VecMath/Vector/Vectors.h"

namespace Hydrogen{
namespace Vulkan{



    class Pipeline
    {
    public:

         Pipeline() noexcept;
        ~Pipeline() noexcept;

        Pipeline(const Pipeline& pOther) = delete;

        //Move
        Pipeline(Pipeline&& pOther) noexcept;
        Pipeline& operator=(Pipeline&& pOther) noexcept;


        //Vertex Input:

        void SetVertexInput(
            //empty for now
        ) noexcept;

        //Input Assembly:
        void SetInputAssembler(
            VkPrimitiveTopology pTopology,
            VkBool32            pPrimitiveRestart=VK_FALSE
        ) noexcept;

        inline VkPrimitiveTopology GetPrimitiveTopology() const {return m_PrimitiveTopology; } 
        inline VkBool32            GetPrimitiveRestart()  const {return m_PrimitiveRestart;}


        //Viewport & Scissors:

        void SetViewport(
            uint32 pWidth,
            uint32 pHeight,
            uint32 pX         = 0,
            uint32 pY         = 0,
            float  pMinDepth  = 0.0f,
            float  pMaxDepth  = 0.0f
        ) noexcept;


        void SetScissor(
            uint32 pWidth,
            uint32 pHeight,
            int32 pX         = 0,
            int32 pY         = 0
        ) noexcept;


        inline VecI4 GetViewportSize()     const {
            return VecI4(m_Viewport.width, m_Viewport.height, m_Viewport.x, m_Viewport.y);
        }

        inline float GetViewportMaxDepth() const {return m_Viewport.maxDepth;}
        inline float GetViewportMinDepth() const {return m_Viewport.minDepth;}

        inline VecI4 GetViewportScissor()     const {
            return VecI4(m_Scissor.extent.width, m_Scissor.extent.height, m_Scissor.offset.x, m_Scissor.offset.y);
        }


        //Rasterizer State:

        void SetRasterizer(
            VkPolygonMode   pPolyMode                = VK_POLYGON_MODE_FILL,
            VkCullModeFlags pCullMode                = VK_CULL_MODE_BACK_BIT,
            VkFrontFace     pFrontFace               = VK_FRONT_FACE_CLOCKWISE,
            VkBool32        pDepthClamp              = VK_FALSE,
            VkBool32        pPrimitiveDiscard        = VK_FALSE,
            float           pLineWidth               = 1.0f,
            VkBool32        pDepthBiasEnable         = VK_FALSE,
            float           pDepthBiasConstantFactor = 0.0f,
            float           pDepthBiasClamp          = 0.0f,
            float           pDepthBiasSlopeFactor    = 0.0f
        ) noexcept;


        inline VkPipelineRasterizationStateCreateInfo GetRasterizerState() const {return m_Rasterizer;}



        //Dynamic States:

        void SetDynamicStates(
            const std::vector<VkDynamicState>& pStates,
            VkPipelineDynamicStateCreateFlags  pFlags  = 0
        ) noexcept;

        //Multisample:
        //TODO: Implement Multisampling

        //Color Blending:

        void SetColorBlend(
            std::vector<VkPipelineColorBlendAttachmentState> pAttachments,
            VkBool32                                         pLogicEnable     = VK_FALSE,
            VkLogicOp                                        pLogicOp         = VK_LOGIC_OP_COPY,
            std::array<float, 4>                             pBlendConstants  = {0.0f, 0.0f, 0.0f, 0.0f},
            VkPipelineColorBlendStateCreateFlags             pFlags           = 0
        ) noexcept;


        //Depth & Stencil State

        void SetDepthStencil(
            VkBool32            pEnableTest,
            VkBool32            pEnableWrite,
            VkCompareOp         pCompareOperator = VK_COMPARE_OP_LESS,
            VkBool32            pDepthBoundTest  = VK_FALSE,
            float               pMinDepthBounds  = 0.0f,
            float               pMaxDepthBounds  = 1.0f,
            VkBool32            pStencilTest     = VK_FALSE,
            VkStencilOpState    pStencilFront    = {},
            VkStencilOpState    pStencilBack     = {}
        ) noexcept;

        inline VkPipelineDepthStencilStateCreateInfo GetDepthStencilState() const {return m_DepthStencil;}


        //Pipeline layout:
        
        void CreatePipelineLayout(

        ) noexcept;


        uint32 CreatePipeline(
            const std::vector<Ptr<Vulkan::Shader>>& pShaders,
            VkRenderPass                            pRenderPass,
            uint32                                  pSubpass
        )   noexcept;


        //Bind this pipeline
        uint32 BindPipeline(
            const CommandBuffer& pCommandBuffer,
            VkPipelineBindPoint  pBindPoint      = VK_PIPELINE_BIND_POINT_GRAPHICS 
        ) noexcept;


        uint32 RecreatePipeline() noexcept;
        uint32 DestroyPipeline()  noexcept;

    private:

        void Reset() noexcept;

    private:
        VkPipeline m_Handle = VK_NULL_HANDLE;
      

        VkPipelineDynamicStateCreateInfo m_DynamicStatesInfo;
        std::vector<VkDynamicState>      m_DynamicStates;

        //Vertex Input State
        VkPipelineVertexInputStateCreateInfo m_VertexInput{};

        //Input Assembler States:
        VkPrimitiveTopology m_PrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        VkBool32            m_PrimitiveRestart  = VK_FALSE;

        //Viewport & Scissor:
        VkViewport m_Viewport;
        VkRect2D   m_Scissor;

        //Rasterizer State:
        VkPipelineRasterizationStateCreateInfo   m_Rasterizer;


        //Tessellation Control:

        VkPipelineTessellationStateCreateInfo   m_TesselationControl;
        
        //Multisampling State:
        VkPipelineMultisampleStateCreateInfo    m_MultiSampleState;


        //Color Blending:
        std::vector<VkPipelineColorBlendAttachmentState> m_Attachments;
        VkPipelineColorBlendStateCreateInfo              m_ColorBlendState;

        //Depth & Stencil State:
        VkPipelineDepthStencilStateCreateInfo   m_DepthStencil;

        //Pipeline Layout:
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
    };

};
};