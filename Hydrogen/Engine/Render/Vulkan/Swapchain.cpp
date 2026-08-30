#include "Swapchain.h"
#include "../Renderer.h"
#include "Image.h"
#include "VkEnumReDefs.h"
#include <cstddef>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{
namespace Internal
{
	Vulkan::Swapchain::Swapchain() noexcept
	{
		m_PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		m_PresentInfo.pNext = nullptr;
	}


	Vulkan::Swapchain::~Swapchain() noexcept
	{
		DestroySwapchain();
	}

	Vulkan::Swapchain::Swapchain(Vulkan::Swapchain&& pOther) noexcept
		: m_Handle(pOther.m_Handle), m_ImageCount(pOther.m_ImageCount), m_SurfaceFormat(pOther.m_SurfaceFormat), m_ImageExtent(pOther.m_ImageExtent), m_Images(std::move(pOther.m_Images)), m_ImageViews(pOther.m_ImageViews)
	{
		pOther.m_Handle                   = VK_NULL_HANDLE;
		pOther.m_SurfaceFormat.format     = VK_FORMAT_UNDEFINED;
		pOther.m_SurfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		pOther.m_ImageCount               = 0;
		pOther.m_ImageExtent.height       = 0;
		pOther.m_ImageExtent.width        = 0;
	}

	uint32 Vulkan::Swapchain::CreateSwapchain(
		const SwapchainConfiguration& pConf
	)  noexcept
	{


		m_SurfaceFormat = SelectSurfaceFormat(pConf.SurfaceCapabilities, pConf.DesiredFormats);
		m_ImageExtent   = pConf.ImageSize;
		m_ImageCount    = pConf.ImageCount;
		VkPresentModeKHR PresentMode = SelectPresentationMode(pConf.SurfaceCapabilities, pConf.DesiredPresentMode);

		m_SwapchainCInfo.sType		     = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		m_SwapchainCInfo.pNext		     = nullptr;
		m_SwapchainCInfo.flags		     = 0;
		m_SwapchainCInfo.oldSwapchain     = VK_NULL_HANDLE;
		m_SwapchainCInfo.surface          = pConf.Surface;
		m_SwapchainCInfo.minImageCount    = m_ImageCount;
		m_SwapchainCInfo.imageFormat      = m_SurfaceFormat.format;
		m_SwapchainCInfo.imageColorSpace  = m_SurfaceFormat.colorSpace;
		m_SwapchainCInfo.presentMode      = PresentMode;
		m_SwapchainCInfo.imageArrayLayers = 1;
		m_SwapchainCInfo.clipped		  = VK_TRUE;
		m_SwapchainCInfo.imageExtent	  = m_ImageExtent;
		m_SwapchainCInfo.imageUsage		  = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		m_SwapchainCInfo.compositeAlpha	  = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		m_SwapchainCInfo.preTransform	  = pConf.SurfaceCapabilities.Capabilities.currentTransform;

		if (Renderer::Self().GetQueues().Graphics == Renderer::Self().GetQueues().Present)
		{
			m_SwapchainCInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
			m_SwapchainCInfo.queueFamilyIndexCount = 0;
			m_SwapchainCInfo.pQueueFamilyIndices   = nullptr;
		}

		else
		{
			std::array<uint32, 2> Indices = { Renderer::Self().m_Device.m_QueueFamily.Graphics.value(), Renderer::Self().m_Device.m_QueueFamily.Present.value() };
			m_SwapchainCInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
			m_SwapchainCInfo.queueFamilyIndexCount = 2;
			m_SwapchainCInfo.pQueueFamilyIndices   = Indices.data();
		}


		VkResult Result = vkCreateSwapchainKHR(
			Renderer::Self().GetDevice()
			, &m_SwapchainCInfo, VULKAN_ALLOCATION_CALLBACK, &m_Handle);

		
		//Retriving the images:
		vkGetSwapchainImagesKHR(
			Renderer::Self().GetDevice(), m_Handle, &m_ImageCount, nullptr);
		std::vector<VkImage> Handles;Handles.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(Renderer::Self().GetDevice(), m_Handle, &m_ImageCount, Handles.data());
		
		m_ImageViews.resize(m_ImageCount);
		m_Images.resize(m_ImageCount);

		for (size_t Iter = 0; Iter < m_ImageCount; ++Iter)
		{
			//Image
			Image& image = m_Images[Iter];

			image.m_Handle  = Handles[Iter];
			image.m_Width   = m_ImageExtent.width;
			image.m_Heihgt  = m_ImageExtent.height;
			image.m_Format  = m_SurfaceFormat.format;
			image.m_Depth   = 1;
			image.m_Layer   = 1;
			image.m_Level   = 1;

			//Image Views:
			ImageViewConfiguration ViewConf;
			ViewConf.Image    = &image,
			ViewConf.ViewType = HYD_IMAGE_VIEW_TYPE_2D,
			ViewConf.Format   = (ImageFormat)m_SurfaceFormat.format;

			CHECK_ERROR(m_ImageViews[Iter].CreateImageView(
				ViewConf
			));

		}


		Log::SetInfo(
			Log::FmtStr("Swapchain Created with %i Images.(Width:%i, Height%i)",
		 m_ImageCount, m_ImageExtent.width, m_ImageExtent.height));

		return HYD_OK;
	}


	uint32 Vulkan::Swapchain::AcquireImage(
		VkSemaphore pWaitSemaphore ,//= VK_NULL_HANDLE,
		VkFence     pWaitFence	   //= VK_NULL_HANDLE
	) noexcept
	{
		uint32 ImageIndex = 0;
		VkResult Res = vkAcquireNextImageKHR(
				Renderer::Self().GetDevice(),
			 m_Handle,     UINT64_MAX,
			 pWaitSemaphore, pWaitFence, &ImageIndex
			);


		return ImageIndex;
	}

	uint32 Vulkan::Swapchain::PresentImage(
		uint32 					  pImageIndex,
		VkQueue 				  pPresentQueue,
	 	std::vector<VkSemaphore>  pWaitSemaphores//={}
	) noexcept
	{
		m_PresentInfo.sType 		 	 = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		m_PresentInfo.pNext 		 	 = nullptr;
		m_PresentInfo.pImageIndices  	 = &pImageIndex;
		m_PresentInfo.swapchainCount 	 = 1;
		m_PresentInfo.pSwapchains    	 = &m_Handle;
		m_PresentInfo.waitSemaphoreCount = pWaitSemaphores.size();
		m_PresentInfo.pWaitSemaphores    = pWaitSemaphores.data();

		return vkQueuePresentKHR(pPresentQueue, &m_PresentInfo);
	}

	uint32 Vulkan::Swapchain::DestroySwapchain() noexcept
	{
		if (m_Handle == VK_NULL_HANDLE)
			return HYD_OK;

		for (auto& imageview : m_ImageViews)
			imageview.DestroyImageView();

		vkDestroySwapchainKHR(Renderer::Self().GetDevice(), m_Handle, VULKAN_ALLOCATION_CALLBACK);

		m_Handle	               = VK_NULL_HANDLE;
		m_SurfaceFormat.format     = VK_FORMAT_UNDEFINED;
		m_SurfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		m_ImageCount			   = 0;
		m_ImageExtent.height	   = 0;
		m_ImageExtent.width		   = 0;
		m_Images.clear();
		m_ImageViews.clear();		
		return HYD_OK;
	}

	uint32 Vulkan::Swapchain::RecreateSwapchain(
		Vulkan::SwapchainRecreateConfiguration pConf
	) noexcept
	{	
 
		VkSwapchainKHR OldSwapchain = m_Handle;
		m_Handle = VK_NULL_HANDLE;
		
		m_SwapchainCInfo.oldSwapchain 	  = OldSwapchain;
		m_SwapchainCInfo.imageExtent      = pConf.NewExtent;
		m_ImageExtent 				      = pConf.NewExtent;
		
		VkResult Res = vkCreateSwapchainKHR(
			Renderer::Self().GetDevice(),
			&m_SwapchainCInfo, VULKAN_ALLOCATION_CALLBACK, &m_Handle);
			
		if (Res != VK_SUCCESS)
		{
			Log::SetError(
				Log::FmtStr("Unable to Recreate Swapchain. VkError:%i", int32(Res))
			);
			return HYD_FAILED;
		}

		vkDestroySwapchainKHR(
			Renderer::Self().GetDevice(), OldSwapchain, VULKAN_ALLOCATION_CALLBACK);


		for (auto& imageview : m_ImageViews)
			imageview.DestroyImageView();
		
		m_ImageViews.clear();
		m_Images.clear();

		//Retriving the images:
		vkGetSwapchainImagesKHR(
			Renderer::Self().GetDevice(), m_Handle, &m_ImageCount, nullptr);
		std::vector<VkImage> Handles;Handles.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(Renderer::Self().GetDevice(), m_Handle, &m_ImageCount, Handles.data());
		
		m_ImageViews.resize(m_ImageCount);
		m_Images.resize(m_ImageCount);

		for (size_t Iter = 0; Iter < m_ImageCount; ++Iter)
		{
			//Image
			Image& image = m_Images[Iter];

			image.m_Handle  = Handles[Iter];
			image.m_Width   = m_ImageExtent.width;
			image.m_Heihgt  = m_ImageExtent.height;
			image.m_Format  = m_SurfaceFormat.format;
			image.m_Depth   = 1;
			image.m_Layer   = 1;
			image.m_Level   = 1;

			//Image Views:
			ImageViewConfiguration ViewConf;
			ViewConf.Image    = &image,
			ViewConf.ViewType = HYD_IMAGE_VIEW_TYPE_2D,
			ViewConf.Format   = (ImageFormat)m_SurfaceFormat.format;

			CHECK_ERROR(m_ImageViews[Iter].CreateImageView(
				ViewConf
			));

		}


		Log::SetInfo(
			Log::FmtStr("Swapchain Recreated with %i Images.(Width:%i, Height%i)",
		 m_ImageCount, m_ImageExtent.width, m_ImageExtent.height));

		return HYD_OK;
	}

	VkSurfaceFormatKHR Vulkan::Swapchain::SelectSurfaceFormat(
		const SurfaceInfo&		 pSurfaceInfo,
		const HYD_VEC<VkFormat>& pDesiredFormats
	) noexcept
	{
		for (size_t Iter = 0; Iter < pDesiredFormats.size(); ++Iter)
		{
			for (auto& format : pSurfaceInfo.Formats)
			{
				if (format.format == pDesiredFormats[Iter])
					return format;
			}
		}

		Log::SetWarning(Log::FmtStr("None of Desired Image formats are supported. Selected:%i", pSurfaceInfo.Formats[0]));

		//Select the first format
		return  pSurfaceInfo.Formats[0];
	}

	VkPresentModeKHR Vulkan::Swapchain::SelectPresentationMode(
		const SurfaceInfo&				 pSurfaceInfo,
		const HYD_VEC<VkPresentModeKHR>& pDesiredModes
	) noexcept
	{
		for (auto& desired : pDesiredModes)
		{
			for (auto& available : pSurfaceInfo.PresentModes)
			{
				if (available == desired)
					return desired;
			}
		}
	
		return VK_PRESENT_MODE_FIFO_KHR;
	}


	VkExtent2D Vulkan::Swapchain::SelectExtent(
		const SurfaceInfo& pSurfaceInfo,
		const Window&	   pWindow
	) noexcept
	{
		if (pSurfaceInfo.Capabilities.currentExtent.width != std::numeric_limits<uint32>::max())
			return pSurfaceInfo.Capabilities.currentExtent;


		uint32 Width, Height;
		glfwGetFramebufferSize(pWindow.GetHandle(), (int32*)&Width, (int32*)&Height);

		VkExtent2D extend;

		extend.width  = std::clamp(Width,  pSurfaceInfo.Capabilities.minImageExtent.width,  pSurfaceInfo.Capabilities.maxImageExtent.width);
		extend.height = std::clamp(Height, pSurfaceInfo.Capabilities.minImageExtent.height, pSurfaceInfo.Capabilities.maxImageExtent.height);

		return extend;
	}


	Vulkan::ImageView Vulkan::Swapchain::GetImage(uint32 pIndex) const noexcept
	{
		XE_ASSERT(pIndex < m_ImageViews.size(), "Out of imageView bound");
		return m_ImageViews[pIndex];
	}

};
};


