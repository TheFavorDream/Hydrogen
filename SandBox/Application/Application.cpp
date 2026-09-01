#include "Application.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>



Application::Application()
	: Core(WindowInfo(1400, 700, "Hydrogen"))
{


	//Renderer Setup:
	CreateRenderPass();

	m_AppLayer = new AppLayer();
	PushLayer(m_AppLayer);
}

Application::~Application()								
{
	
}

void Application::Run()
{
	Loop();
}

uint32 Application::CreateRenderPass() noexcept
{


	//Attachment configurations:
	Hydrogen::RenderpassAttachment   ColorOut;
	ColorOut.Format      =  Renderer::Self().Swapchain().GetImageFormat();
	ColorOut.InitLayout  = Hydrogen::HYD_IMAGE_LAYOUT_UNDEFINED;
	ColorOut.FinalLayout = Hydrogen::HYD_IMAGE_LAYOUT_PRESENT_SRC_KHR; 
	ColorOut.LoadOp      = Hydrogen::HYD_ATTACHMENT_LOAD_OP_CLEAR;
	ColorOut.StoreOp     = Hydrogen::HYD_ATTACHMENT_STORE_OP_STORE;
	ColorOut.SampleCount = Hydrogen::HYD_SAMPLE_COUNT_1_BIT;

	Hydrogen::RenderpassAttachment DepthOut;
	DepthOut.Format      = Hydrogen::HYD_FORMAT_D32_SFLOAT;
	DepthOut.InitLayout  = Hydrogen::HYD_IMAGE_LAYOUT_UNDEFINED;
	DepthOut.FinalLayout = Hydrogen::HYD_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; 
	DepthOut.LoadOp      = Hydrogen::HYD_ATTACHMENT_LOAD_OP_CLEAR;
	DepthOut.StoreOp     = Hydrogen::HYD_ATTACHMENT_STORE_OP_STORE;
	DepthOut.SampleCount = Hydrogen::HYD_SAMPLE_COUNT_1_BIT;

	//Subpass Configurations:
	Hydrogen::SubpassConfiguration SubpassConf;
	SubpassConf.AddColorAttachment({.Index=0, .Layout=HYD_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}); 
	SubpassConf.AddDepthStencilAttachment({.Index=1, .Layout=HYD_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL});
	

	//Subpass Dependencies Configurations:
	Hydrogen::SubpassDependencyConfiguration DependencyConf;
	DependencyConf.SourceSubpass      = UINT32_MAX; //Extenal source
	DependencyConf.DestinationSubpass = 0;
	DependencyConf.SrcStageMask       = Hydrogen::HYD_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
	DependencyConf.SrcAccessMask 	  = 0;
	DependencyConf.DstStageMask       = Hydrogen::HYD_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;   
	DependencyConf.DstAccessMask      = Hydrogen::HYD_ACCESS_COLOR_ATTACHMENT_WRITE;    

	Hydrogen::SubpassDependencyConfiguration DepthDependency;

	DepthDependency.SourceSubpass      = UINT32_MAX;
	DepthDependency.DestinationSubpass = 0;
	DepthDependency.SrcStageMask       = HYD_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS | HYD_PIPELINE_STAGE_LATE_FRAGMENT_TESTS;
	DepthDependency.SrcAccessMask      = 0;
	DepthDependency.DstStageMask       = HYD_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS | HYD_PIPELINE_STAGE_LATE_FRAGMENT_TESTS;
	DepthDependency.DstAccessMask      = HYD_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE;

	Hydrogen::RenderpassConfiguration RenderPassConf;
	RenderPassConf.AddAttachment(ColorOut);
	RenderPassConf.AddAttachment(DepthOut);
	RenderPassConf.AddSubpass(SubpassConf);
	RenderPassConf.AddDependency(DependencyConf);
	RenderPassConf.AddDependency(DepthDependency);

	return Renderer::Self().RenderPass().CreateRenderPass(RenderPassConf);
}


		
	

