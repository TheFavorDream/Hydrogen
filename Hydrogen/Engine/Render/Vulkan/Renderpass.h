/*
	Implemention for Vulkan Renderpass
*/

#pragma once

#include "../../Common.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "VkEnumReDefs.h"
#include "FrameBuffer.h"
#include "Commands.h"
#include "../../VecMath/Vector/Vectors.h"

namespace Hydrogen
{


	struct AttachmentReference
	{
		uint32 	    Index  = 0;
		ImageLayout Layout = HYD_IMAGE_LAYOUT_UNDEFINED;
	};

	struct RenderpassAttachment
	{
		ImageFormat      Format          = HYD_FORMAT_UNDEFINED;
		LoadOperation    LoadOp          = HYD_ATTACHMENT_LOAD_OP_LOAD;
		StoreOperation   StoreOp         = HYD_ATTACHMENT_STORE_OP_STORE;
		LoadOperation    StencilLoadOp   = HYD_ATTACHMENT_LOAD_OP_DONT_CARE;
		StoreOperation   StencilStoreOp  = HYD_ATTACHMENT_STORE_OP_DONT_CARE;
		ImageSampleCount SampleCount     = HYD_SAMPLE_COUNT_1_BIT;
		ImageLayout      InitLayout      = HYD_IMAGE_LAYOUT_UNDEFINED;
		ImageLayout      FinalLayout     = HYD_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	};
	
	struct SubpassConfiguration
	{
	public:
		void AddInputAttachment       (AttachmentReference pInput) 	      noexcept;
		void AddColorAttachment       (AttachmentReference pColor) 	      noexcept;
		void AddDepthStencilAttachment(AttachmentReference pDepthStencil) noexcept;
		void AddResolveAttachment     (AttachmentReference pResolve)      noexcept;
		void AddIPreservedAttachment  (uint32 pPreserved) 			      noexcept;

	private:
		std::vector<AttachmentReference> m_Input;
		std::vector<AttachmentReference> m_Color;
		std::vector<AttachmentReference> m_DepthStencil;
		std::vector<AttachmentReference> m_Resolve;
		std::vector<uint32> 			 m_Preserved;


	private:
		friend class Internal::Vulkan::Renderpass;
	};
	
	struct SubpassDependencyConfiguration
	{
		uint32 				 SourceSubpass;
		uint32 				 DestinationSubpass;
		uint32               SrcStageMask;
		uint32               DstStageMask;
		uint32	             SrcAccessMask;
		uint32	             DstAccessMask;
	};

	//Used to Create Renderpasses
	struct RenderpassConfiguration
	{
	public:
	
		void AddAttachment(RenderpassAttachment pAttachment)  			   noexcept;
		void AddSubpass   (SubpassConfiguration pSubpassConf) 			   noexcept;
		void AddDependency(SubpassDependencyConfiguration pDependencyConf) noexcept;

	private:
		std::vector<RenderpassAttachment>           m_Attachments;
		std::vector<SubpassConfiguration>           m_Subpasses;
		std::vector<SubpassDependencyConfiguration> m_Dependencies;

	private:
		friend class Internal::Vulkan::Renderpass;
	};


namespace Internal
{
namespace Vulkan
{

	struct SubpassRef
	{
		std::vector<VkAttachmentReference> Inputs;
		std::vector<VkAttachmentReference> Colors;
		std::vector<VkAttachmentReference> DepthStencil;
		std::vector<VkAttachmentReference> Resolved;
		std::vector<uint32> 			   Presereved;
	};


	class Renderpass final
	{
	public:

		 Renderpass() noexcept;
		~Renderpass() noexcept;


		Renderpass(const Renderpass& pOther) 			= delete;
		Renderpass& operator=(const Renderpass& pOther) = delete;


		//move
		Renderpass(Renderpass&& pOther) 		   noexcept;
		Renderpass& operator=(Renderpass&& pOther) noexcept;

		//Renderpass Creation:
		uint32 CreateRenderPass  (
			RenderpassConfiguration pConfiguration,
			bool 				    pKeepCache=false
		) noexcept;


		uint32 RecreateRenderPass() noexcept;

		inline uint32 SubpassCount() const {return m_SubpassCount;}

	private: //Accessable by friends only

		void BeginRenderPass(
			const FrameBuffer& 	 pFrameBuffer,
			VkRect2D 			 pRenderArea ,
			VecF4 				 pClearColor = VecF4(0.1f, 0.1f, 0.0f, 1.0f)
		) noexcept;

		void EndRenderPass() noexcept;

		//A Renderpass must be destroyed by the engine itself, not by the user
		uint32 DestroyRenderPass () noexcept;


	private: //Accessable by members only

		uint32 CreateRenderpassObject() noexcept;
		
		void   CleanUp() noexcept;

	private:

		VkRenderPass m_Handle 		= VK_NULL_HANDLE;
		uint32 		 m_SubpassCount = 0;
		//Stores the configuration for current Renderpass object
		std::vector<VkAttachmentDescription> m_AttachmentDescriptions;
		std::vector<SubpassRef>    			 m_SubpassRefs;
		std::vector<VkSubpassDescription>    m_Subpasses;
		std::vector<VkSubpassDependency>     m_SubpassDependencies;
		

	private:
		friend class Hydrogen::Renderer;
		friend class FrameBuffer;
		friend class GraphicsPipeline;
	};
	
};
}
};