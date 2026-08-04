#include "Renderpass.h"
#include "../Renderer.h" //for logical device
#include <vulkan/vulkan_core.h>
namespace Hydrogen
{

	Vulkan::RenderPass::~RenderPass() noexcept
	{
		if (m_Handle != VK_NULL_HANDLE)
			DestroyRenderPass();
	}


	//move
	Vulkan::RenderPass::RenderPass(Vulkan::RenderPass&& pOther) noexcept
	{
		m_Handle	  = pOther.m_Handle;
		m_Attachments = std::move(pOther.m_Attachments);
		m_Subpasses   = std::move(pOther.m_Subpasses);

		pOther.m_Handle = nullptr;
	}

	Vulkan::RenderPass& Vulkan::RenderPass::operator=(Vulkan::RenderPass&& pOther) noexcept
	{
		m_Handle	  = pOther.m_Handle;
		m_Attachments = std::move(pOther.m_Attachments);
		m_Subpasses	  = std::move(pOther.m_Subpasses);

		pOther.m_Handle = nullptr;

		return *this;
	}



	//Attachment info

	VkAttachmentReference Vulkan::RenderPass::AddAttachment(
		VkImageLayout		    pImageLayout,
		VkAttachmentDescription pDescription
	) noexcept
	{
		m_Attachments.push_back( pDescription );

		VkAttachmentReference Reference{};
		Reference.attachment = m_Attachments.size() - 1;
		Reference.layout	 = pImageLayout;

		return Reference;
	}

	VkAttachmentReference Vulkan::RenderPass::AddAttachment(
		VkImageLayout		         pImageLayout,
		VkFormat					 pFormat,
		VkImageLayout				 pFinalLayout,
		VkImageLayout				 pInitialLayout,
		VkSampleCountFlagBits		 pSamples,
		VkAttachmentLoadOp			 pLoadOp,
		VkAttachmentStoreOp			 pStoreOp,
		VkAttachmentLoadOp			 pStencilLoadOp,
		VkAttachmentStoreOp			 pStencilStoreOp,
		VkAttachmentDescriptionFlags pFlags
	) noexcept
	{

		VkAttachmentDescription Description{};

		Description.flags			= pFlags;
		Description.format			= pFormat;
		Description.initialLayout	= pInitialLayout;
		Description.finalLayout		= pFinalLayout;
		Description.samples			= pSamples;
		Description.loadOp			= pLoadOp;
		Description.storeOp			= pStoreOp;
		Description.stencilLoadOp	= pStencilLoadOp;
		Description.stencilStoreOp	= pStencilStoreOp;

		m_Attachments.push_back( Description );

		VkAttachmentReference Reference{};
		Reference.attachment = m_Attachments.size() - 1;
		Reference.layout	 = pImageLayout;

		return Reference;
	}




	uint32 Vulkan::RenderPass::AddSubpass(
		VkPipelineBindPoint						  pPipelineBindPoint,
		const HYD_VEC<VkAttachmentReference>&     pColorAttachments,
		const HYD_VEC<VkAttachmentReference>&     pInputAttachments,
		VkAttachmentReference					  pDepthStencilAttachment,
		const HYD_VEC<VkAttachmentReference>&     pResolveAttachments,
		const HYD_VEC<uint32>&					  pPreservedAttachments,
		VkSubpassDescriptionFlags				  pFlags
	) noexcept	
	{

		SubpassRefs Refs;

		VkSubpassDescription Description{};
		Description.flags					= pFlags;
		Description.pipelineBindPoint		= pPipelineBindPoint;
		
		if (pInputAttachments.size())
			Refs.Inputs = pInputAttachments;
		

		if (pColorAttachments.size())
			Refs.Colors = pColorAttachments;



		if (pPreservedAttachments.size())
			Refs.Preserved = pPreservedAttachments;


		if (pResolveAttachments.size())
			Refs.Resolved = pResolveAttachments;

		
		
		Refs.DepthStencil = pDepthStencilAttachment;



		m_Subpasses.push_back(Description);
		m_SubpassRefs.push_back(Refs);

		return m_Subpasses.size() - 1; //return the index of the subpass.
	}



	uint32 Vulkan::RenderPass::AddSubpassDependency(
		uint32 				 pSrcSubpass,
		uint32 				 pDstSubpass,
		VkPipelineStageFlags pSrcStageMask,
		VkPipelineStageFlags pDstStageMask,
		VkAccessFlags 		 pSrcAccessMask,
		VkAccessFlags 		 pDstAccessMask,
		uint32 	             pDependencyFlags// = 0
	) noexcept
	{
		VkSubpassDependency DInfo{};
		DInfo.dependencyFlags = pDependencyFlags;
		DInfo.srcSubpass 	  = pSrcSubpass;
		DInfo.dstSubpass 	  = pDstSubpass;
		DInfo.srcAccessMask   = pSrcAccessMask;
		DInfo.dstAccessMask   = pDstStageMask;
		DInfo.srcStageMask    = pSrcStageMask;
		DInfo.dstStageMask    = pDstStageMask;
		
		m_Dependencies.push_back(DInfo);
		return HYD_OK;
	}



	uint32 Vulkan::RenderPass::CreateRenderPass(VkRenderPassCreateFlags pFlags) noexcept
	{
		VkRenderPassCreateInfo CInfo{};
		CInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		CInfo.pNext = nullptr;
		CInfo.flags = pFlags;

		CInfo.attachmentCount = m_Attachments.size();
		CInfo.pAttachments    = (m_Attachments.size()) ? m_Attachments.data() : nullptr;
	


		CInfo.subpassCount = m_Subpasses.size();
		for (uint64 Iter = 0; Iter < CInfo.subpassCount; ++Iter)
		{
			VkSubpassDescription& Subpass   = m_Subpasses.at(Iter);
			SubpassRefs&		  Refs      = m_SubpassRefs.at(Iter);

			Subpass.colorAttachmentCount    = Refs.Colors.size();
			Subpass.pColorAttachments	    = Refs.Colors.data();

			Subpass.inputAttachmentCount    = Refs.Inputs.size();
			Subpass.pInputAttachments       = Refs.Inputs.data();

			Subpass.preserveAttachmentCount = Refs.Preserved.size();
			Subpass.pPreserveAttachments    = Refs.Preserved.data();

			Subpass.pResolveAttachments     = Refs.Resolved.data();

			if (Refs.DepthStencil.attachment != UINT32_MAX)
				Subpass.pDepthStencilAttachment = &Refs.DepthStencil;
		}

		CInfo.pSubpasses   = m_Subpasses.data();
	
		CInfo.dependencyCount = m_Dependencies.size();
		CInfo.pDependencies   = (m_Dependencies.size()) ? m_Dependencies.data() : nullptr;

		VkResult Result = vkCreateRenderPass(Renderer::Self().GetDevice(), &CInfo, VULKAN_ALLOCATION_CALLBACK, &m_Handle);
		if (Result != VK_SUCCESS)
			return HYD_FAILED;
		

		return HYD_OK;
	}

	uint32 Vulkan::RenderPass::RecreateRenderPass() noexcept
	{
		DestroyRenderPass(true);
		return CreateRenderPass();
	}

	uint32 Vulkan::RenderPass::DestroyRenderPass(bool pKeepData) noexcept
	{
		vkDestroyRenderPass(Renderer::Self().GetDevice(), m_Handle, VULKAN_ALLOCATION_CALLBACK);

		if (pKeepData)
			return HYD_OK;

		m_Attachments.clear();
		m_SubpassRefs.clear();
		m_Subpasses.clear();
		m_Handle = VK_NULL_HANDLE;

		return HYD_OK;
	}




	uint32 Vulkan::RenderPass::BeginRenderPass(
		const Vulkan::CommandBuffer& pCommandBuffer,
		const Vulkan::FrameBuffer& 	 pFrameBuffer,
		VkRect2D 					 pRenderArea ,
		VecF4 						 pClearColor //= VecF4(0.0f, 0.0f, 0.0f, 1.0f)
	) noexcept
	{

		VkClearValue ClearVal{{pClearColor.R, pClearColor.G, pClearColor.B, pClearColor.A}};

		VkRenderPassBeginInfo BeginInfo{};
		BeginInfo.sType 	  	  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		BeginInfo.pNext 	  	  = nullptr;
		BeginInfo.renderPass  	  = m_Handle;
		BeginInfo.framebuffer 	  = pFrameBuffer.GetHandle();
		BeginInfo.clearValueCount = 1;
		BeginInfo.pClearValues    = &ClearVal;
		BeginInfo.renderArea 	  = pRenderArea;


		vkCmdBeginRenderPass(pCommandBuffer.GetHandle(), &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		return HYD_OK;
	}


		uint32 Vulkan::RenderPass::EndRenderPass(
			const Vulkan::CommandBuffer& pCommandBuffer
		) noexcept
		{
			vkCmdEndRenderPass(pCommandBuffer.GetHandle());
			return HYD_OK;
		}

	void Vulkan::RenderPass::FreeCache()
	{
		m_Attachments.clear();
		m_SubpassRefs.clear();
		m_Subpasses.clear();
	}
	

}; 