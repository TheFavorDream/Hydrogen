/*
	Implemention for Vulkan Renderpass
*/

#pragma once

#include "../../Common.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "FrameBuffer.h"
#include "Commands.h"
#include "Structs.h"
#include "../../VecMath/Vector/Vectors.h"

namespace Hydrogen
{
namespace Vulkan
{

	struct SubpassRefs
	{
		//Attachment References
		HYD_VEC<VkAttachmentReference> Colors       = {};
		HYD_VEC<VkAttachmentReference> Inputs       = {};
		HYD_VEC<VkAttachmentReference> Resolved     = {};
		HYD_VEC<uint32>				   Preserved    = {};
		VkAttachmentReference          DepthStencil = { UINT32_MAX, VK_IMAGE_LAYOUT_UNDEFINED};
	};


	class RenderPass final
	{
	public:

		 RenderPass()  = default;
		~RenderPass() noexcept;


		RenderPass(const RenderPass& pOther) = delete;

		//move
		RenderPass(RenderPass&& pOther) noexcept;
		RenderPass& operator=(RenderPass&& pOther) noexcept;



		//Attachment info
		/*
			Adds an attachment to the renderpass
			returns the index of the attachment reference in the array, used to attach resources to the subpasses
		*/
		VkAttachmentReference AddAttachment(
			VkImageLayout		    pImageLayout,
			VkAttachmentDescription pDescription
		) noexcept;

		VkAttachmentReference AddAttachment(
			VkImageLayout		         pImageLayout,
			VkFormat					 pFormat,
			VkImageLayout				 pFinalLayout,
			VkImageLayout				 pInitialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
			VkSampleCountFlagBits		 pSamples		 = VK_SAMPLE_COUNT_1_BIT,
			VkAttachmentLoadOp			 pLoadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,   
			VkAttachmentStoreOp			 pStoreOp        = VK_ATTACHMENT_STORE_OP_STORE,
			VkAttachmentLoadOp			 pStencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VkAttachmentStoreOp			 pStencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VkAttachmentDescriptionFlags pFlags			 = 0
		) noexcept;

		//Subpass Info

		uint32 AddSubpass(
			VkPipelineBindPoint						 pPipelineBindPoint,
			const HYD_VEC<VkAttachmentReference>&	 pColorAttachments       = {},
			const HYD_VEC<VkAttachmentReference>&    pInputAttachments		 = {},
			VkAttachmentReference				     pDepthStencilAttachment = {UINT32_MAX, VK_IMAGE_LAYOUT_UNDEFINED},
			const HYD_VEC<VkAttachmentReference>&    pResolveAttachments     = {},
			const HYD_VEC<uint32>&					 pPreservedAttachments   = {},
			VkSubpassDescriptionFlags pFlags								 = 0
		) noexcept;

		uint32 AddSubpassDependency(
			uint32 				 pSrcSubpass,
			uint32 				 pDstSubpass,
			VkPipelineStageFlags pSrcStageMask,
			VkPipelineStageFlags pDstStageMask,
			VkAccessFlags 		 pSrcAccessMask,
			VkAccessFlags 		 pDstAccessMask,
			uint32 	             pDependencyFlags = 0
		) noexcept;

		uint32 BeginRenderPass(
			const CommandBuffer& pCommandBuffer,
			const FrameBuffer& 	 pFrameBuffer,
			VkRect2D 			 pRenderArea ,
			VecF4 				 pClearColor = VecF4(0.0f, 0.0f, 0.0f, 1.0f)
		) noexcept;

		uint32 EndRenderPass(
			const CommandBuffer& pCommandBuffer
		) noexcept;

		uint32 CreateRenderPass  (VkRenderPassCreateFlags pFlags=0) noexcept;
		uint32 RecreateRenderPass() noexcept;

		uint32 DestroyRenderPass (bool pKeepData=false) noexcept;
		void   FreeCache();

		inline const VkRenderPass					   GetHandle()		      const { return m_Handle; }
		inline const HYD_VEC<VkAttachmentDescription>& GetAttachments()       const { return m_Attachments; }
		inline const HYD_VEC<SubpassRefs>&			   GetSubpassReferences() const { return m_SubpassRefs; }
		inline const HYD_VEC<VkSubpassDescription>&    GetSubpasses()		  const { return m_Subpasses; }

	private:

	private:
		VkRenderPass m_Handle = VK_NULL_HANDLE;

		HYD_VEC<VkAttachmentDescription> m_Attachments;
		HYD_VEC<SubpassRefs>			 m_SubpassRefs;
		HYD_VEC<VkSubpassDescription>	 m_Subpasses;
		HYD_VEC<VkSubpassDependency>     m_Dependencies;

	};
	
};
};