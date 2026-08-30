#include "Renderpass.h"
#include "../Renderer.h" //for logical device
#include <vulkan/vulkan_core.h>



namespace Hydrogen
{

	//Config Interface Implementation:


	void SubpassConfiguration::AddInputAttachment       (AttachmentReference pInput) 	      noexcept
	{
		m_Input.push_back(pInput);
	}

	void SubpassConfiguration::AddColorAttachment       (AttachmentReference pColor) 	      noexcept
	{
		m_Color.push_back(pColor);
	}

	void SubpassConfiguration::AddDepthStencilAttachment(AttachmentReference pDepthStencil)   noexcept
	{
		m_DepthStencil.push_back(pDepthStencil);
	}
	void SubpassConfiguration::AddResolveAttachment     (AttachmentReference pResolve)        noexcept
	{
		m_Resolve.push_back(pResolve);
	}
	void SubpassConfiguration::AddIPreservedAttachment  (uint32 pPreserved) 			      noexcept
	{
		m_Preserved.push_back(pPreserved);
	}



	void RenderpassConfiguration::AddAttachment(RenderpassAttachment pAttachment)  			   noexcept 
	{
		m_Attachments.push_back(pAttachment);
	}
	void RenderpassConfiguration::AddSubpass(SubpassConfiguration pSubpassConf) 			   noexcept
	{
		m_Subpasses.push_back(pSubpassConf);
	}
	void RenderpassConfiguration::AddDependency(SubpassDependencyConfiguration pDependencyConf) noexcept
	{
		m_Dependencies.push_back(pDependencyConf);
	}

namespace Internal
{
	//Renderpass Implementation:
	Vulkan::Renderpass::Renderpass() noexcept
	{

	}

	Vulkan::Renderpass::~Renderpass() noexcept
	{
		if (m_Handle != VK_NULL_HANDLE)
			DestroyRenderPass();
	}


	//move
	Vulkan::Renderpass::Renderpass(Vulkan::Renderpass&& pOther) noexcept
	{
		m_Handle	    = pOther.m_Handle;
		m_SubpassCount  = pOther.m_SubpassCount;

		pOther.m_Handle 	  = nullptr;
		pOther.m_SubpassCount = 0;

		m_AttachmentDescriptions = std::move(m_AttachmentDescriptions);
		m_SubpassRefs 			 = std::move(m_SubpassRefs);
		m_Subpasses   			 = std::move(m_Subpasses);
		m_SubpassDependencies 	 = std::move(m_SubpassDependencies);
	}

	Vulkan::Renderpass& Vulkan::Renderpass::operator=(Vulkan::Renderpass&& pOther) noexcept
	{
		m_Handle	    = pOther.m_Handle;
		m_SubpassCount  = pOther.m_SubpassCount;

		pOther.m_Handle 	  = nullptr;
		pOther.m_SubpassCount = 0;

		m_AttachmentDescriptions = std::move(m_AttachmentDescriptions);
		m_SubpassRefs 			 = std::move(m_SubpassRefs);
		m_Subpasses   			 = std::move(m_Subpasses);
		m_SubpassDependencies 	 = std::move(m_SubpassDependencies);

		return *this;
	}



	uint32 Vulkan::Renderpass::CreateRenderPass(
		RenderpassConfiguration pConfiguration,
		bool 				    pKeepCache//=false
	) noexcept
	{


		//Attachments
		m_AttachmentDescriptions.resize(pConfiguration.m_Attachments.size());
		for (uint32 Iter = 0  ; Iter < pConfiguration.m_Attachments.size() ; ++Iter)
		{
			VkAttachmentDescription& VkDesc = m_AttachmentDescriptions.at(Iter);
			RenderpassAttachment&    AttachmentInfo = pConfiguration.m_Attachments.at(Iter);

			VkDesc.format  		    = VkFormat(AttachmentInfo.Format);
			VkDesc.loadOp  		    = VkAttachmentLoadOp(AttachmentInfo.LoadOp);
			VkDesc.storeOp 		    = VkAttachmentStoreOp(AttachmentInfo.StoreOp);
			VkDesc.samples		    = VkSampleCountFlagBits(AttachmentInfo.SampleCount);
			VkDesc.stencilLoadOp    = VkAttachmentLoadOp(AttachmentInfo.StencilLoadOp);
			VkDesc.stencilStoreOp   = VkAttachmentStoreOp(AttachmentInfo.StencilStoreOp);
			VkDesc.initialLayout    = VkImageLayout(AttachmentInfo.InitLayout);
			VkDesc.finalLayout      = VkImageLayout(AttachmentInfo.FinalLayout);
			VkDesc.flags 			  = 0;
		}

		//Subpass Dependencies:
		m_SubpassDependencies.resize(pConfiguration.m_Dependencies.size());
		for (uint32 Iter = 0  ; Iter < pConfiguration.m_Dependencies.size() ; ++Iter)
		{
			VkSubpassDependency&   			  VkRef = m_SubpassDependencies.at(Iter);
			SubpassDependencyConfiguration    Ref   = pConfiguration.m_Dependencies.at(Iter); 
			
			VkRef.srcSubpass    = (Ref.SourceSubpass != UINT32_MAX)?      Ref.SourceSubpass      : VK_SUBPASS_EXTERNAL;
			VkRef.dstSubpass    = (Ref.DestinationSubpass != UINT32_MAX)? Ref.DestinationSubpass : VK_SUBPASS_EXTERNAL;
			VkRef.srcAccessMask = VkAccessFlags(Ref.SrcAccessMask);
			VkRef.dstAccessMask = VkAccessFlags(Ref.DstAccessMask);
			VkRef.srcStageMask  = VkPipelineStageFlags(Ref.SrcStageMask);
			VkRef.dstStageMask  = VkPipelineStageFlags(Ref.DstStageMask);
			VkRef.dependencyFlags = 0;
		}



		auto VkAttachmentRefs=[](const std::vector<AttachmentReference>& Refs)->std::vector<VkAttachmentReference>{
			std::vector<VkAttachmentReference> VkRefs;
			VkRefs.resize(Refs.size());

			for (uint32 Iter = 0 ; Iter < Refs.size() ; ++Iter)
			{
				VkAttachmentReference& VkRef = VkRefs.at(Iter);
				VkRef.attachment = Refs[Iter].Index;
				VkRef.layout     = VkImageLayout(Refs[Iter].Layout);
			}

			return VkRefs;
		};

		//Subpasses:
		m_SubpassRefs.resize(pConfiguration.m_Subpasses.size());
		m_Subpasses.resize(pConfiguration.m_Subpasses.size());

		for (uint32 Iter = 0 ; Iter < pConfiguration.m_Subpasses.size() ; ++Iter)
		{
			SubpassRef&           VkSubpassRef  = m_SubpassRefs.at(Iter);
			VkSubpassDescription& VkSubpassDesc = m_Subpasses.at(Iter);
			SubpassConfiguration& Ref   		= pConfiguration.m_Subpasses.at(Iter);

			VkSubpassRef.Inputs       = VkAttachmentRefs(Ref.m_Input);
			VkSubpassRef.Colors       = VkAttachmentRefs(Ref.m_Color);
			VkSubpassRef.DepthStencil = VkAttachmentRefs(Ref.m_DepthStencil);
			VkSubpassRef.Resolved     = VkAttachmentRefs(Ref.m_Resolve);
			VkSubpassRef.Presereved   = Ref.m_Preserved;
			
			VkSubpassDesc.colorAttachmentCount    = VkSubpassRef.Colors.size();
			VkSubpassDesc.pColorAttachments       = VkSubpassRef.Colors.data();
			VkSubpassDesc.inputAttachmentCount    = VkSubpassRef.Inputs.size();
			VkSubpassDesc.pInputAttachments       = VkSubpassRef.Inputs.data();
			VkSubpassDesc.pDepthStencilAttachment = VkSubpassRef.DepthStencil.data();
			VkSubpassDesc.pResolveAttachments     = VkSubpassRef.Resolved.data();
			VkSubpassDesc.preserveAttachmentCount = VkSubpassRef.Presereved.size();
			VkSubpassDesc.pPreserveAttachments    = VkSubpassRef.Presereved.data();
			VkSubpassDesc.pipelineBindPoint 	  = VK_PIPELINE_BIND_POINT_GRAPHICS;

			m_SubpassCount += 1;
		}


		uint32 Res = CreateRenderpassObject();

		//Clean Up 
		if (!pKeepCache)
			CleanUp();

		return Res; 
	}

	uint32 Vulkan::Renderpass::RecreateRenderPass() noexcept
	{

		if (!m_Subpasses.size())
			return HYD_FAILED; //Resources has been cleaned up

		DestroyRenderPass();
		return CreateRenderpassObject();
	}

	uint32 Vulkan::Renderpass::DestroyRenderPass() noexcept
	{
		vkDestroyRenderPass(Renderer::Self().GetDevice(), m_Handle, VULKAN_ALLOCATION_CALLBACK);
		m_Handle = VK_NULL_HANDLE;
		CleanUp();
		return HYD_OK;
	}




	void Vulkan::Renderpass::BeginRenderPass(
		const Vulkan::FrameBuffer& 	 pFrameBuffer,
		VkRect2D 					 pRenderArea ,
		VecF4 						 pClearColor //= VecF4(0.0f, 0.0f, 0.0f, 1.0f)
	) noexcept
	{

		VkClearValue ClearVals[] = {
			{pClearColor.R, pClearColor.G, pClearColor.B, pClearColor.A},
			{0.0f}
		};

		VkRenderPassBeginInfo BeginInfo{};
		BeginInfo.sType 	  	  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		BeginInfo.pNext 	  	  = nullptr;
		BeginInfo.renderPass  	  = m_Handle;
		BeginInfo.framebuffer 	  = pFrameBuffer.GetHandle();
		BeginInfo.clearValueCount = 2;
		BeginInfo.pClearValues    = ClearVals;
		BeginInfo.renderArea 	  = pRenderArea;


		vkCmdBeginRenderPass(Renderer::Self().GlobalRenderCommandBuffer().GetHandle(), &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}


	void Vulkan::Renderpass::EndRenderPass() noexcept
	{
		vkCmdEndRenderPass(Renderer::Self().GlobalRenderCommandBuffer().GetHandle());
	}

	

	uint32 Vulkan::Renderpass::CreateRenderpassObject() noexcept
	{
		//Actual Renderpass Create Info:

		VkRenderPassCreateInfo CInfo{};
		CInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		CInfo.pNext = nullptr;
		CInfo.attachmentCount = m_AttachmentDescriptions.size();
		CInfo.pAttachments    = m_AttachmentDescriptions.data();

		CInfo.dependencyCount = m_SubpassDependencies.size();
		CInfo.pDependencies   = m_SubpassDependencies.data();

		CInfo.subpassCount    = m_Subpasses.size();
		CInfo.pSubpasses      = m_Subpasses.data();

		CInfo.flags = 0;

		VkResult Res = vkCreateRenderPass(
			Renderer::Self().GetDevice(),
			&CInfo,
			VULKAN_ALLOCATION_CALLBACK, &m_Handle);
		
		if (Res != VK_SUCCESS)
		{
			Log::SetError(
				Log::FmtStr("Unable to Create Render Pass. VkError:%i", int32(Res))
			);
			return HYD_FAILED;
		}
		return HYD_OK;
	}


	void   Vulkan::Renderpass::CleanUp() noexcept
	{
		m_AttachmentDescriptions.clear();
		m_SubpassDependencies.clear();
		m_SubpassRefs.clear();
		m_Subpasses.clear();
	}

};
}; 