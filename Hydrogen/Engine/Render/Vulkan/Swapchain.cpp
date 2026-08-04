#include "Swapchain.h"
#include "../Renderer.h"
#include <cstddef>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
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
		const SurfaceInfo&            pSurfaceInfo,
		VkDevice	                  pDevice,
		VkSurfaceKHR                  pSurface,
		uint32		                  pImageCount,
		VkExtent2D			          pExtent,
		VkSurfaceFormatKHR            pSurfaceFormat,
		VkPresentModeKHR			  pPresentMode
	)  noexcept
	{

		m_ImageCount    = pImageCount;
		m_Device        = pDevice;
		m_ImageExtent   = pExtent;
		m_SurfaceFormat = pSurfaceFormat;

		VkSwapchainCreateInfoKHR ScCInfo{};
		ScCInfo.sType		     = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		ScCInfo.pNext		     = nullptr;
		ScCInfo.flags		     = 0;
		ScCInfo.oldSwapchain     = VK_NULL_HANDLE;
		ScCInfo.surface          = pSurface;
		ScCInfo.minImageCount    = m_ImageCount;
		ScCInfo.imageFormat      = m_SurfaceFormat.format;
		ScCInfo.imageColorSpace  = m_SurfaceFormat.colorSpace;
		ScCInfo.presentMode      = pPresentMode;
		ScCInfo.imageArrayLayers = 1;
		ScCInfo.clipped			 = VK_TRUE;
		ScCInfo.imageExtent		 = m_ImageExtent;
		ScCInfo.imageUsage		 = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		ScCInfo.compositeAlpha	 = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		ScCInfo.preTransform	 = pSurfaceInfo.Capabilities.currentTransform;

		if (Renderer::Self().GetQueues().Graphics == Renderer::Self().GetQueues().Present)
		{
			ScCInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
			ScCInfo.queueFamilyIndexCount = 0;
			ScCInfo.pQueueFamilyIndices   = nullptr;
		}

		else
		{
			std::array<uint32, 2> Indices = { Renderer::Self().GetGPUInfo().Queues.Graphics.value(), Renderer::Self().GetGPUInfo().Queues.Present.value() };
			ScCInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
			ScCInfo.queueFamilyIndexCount = 2;
			ScCInfo.pQueueFamilyIndices   = Indices.data();
		}


		VkResult Result = vkCreateSwapchainKHR(m_Device, &ScCInfo, VULKAN_ALLOCATION_CALLBACK, &m_Handle);


		//Retriving the images:
		vkGetSwapchainImagesKHR(m_Device, m_Handle, &m_ImageCount, nullptr);
		m_Images.resize(m_ImageCount);
		m_ImageViews.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(m_Device, m_Handle, &m_ImageCount, m_Images.data());

		for (size_t Iter = 0; Iter < m_ImageCount; ++Iter)
		{
			VkImageViewCreateInfo ViewCInfo{};

			ViewCInfo.sType						      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ViewCInfo.pNext						      = nullptr;
			ViewCInfo.flags						      = 0;
			ViewCInfo.image						      = m_Images[Iter];
			ViewCInfo.format					      = m_SurfaceFormat.format;
			ViewCInfo.viewType					      = VK_IMAGE_VIEW_TYPE_2D;
			ViewCInfo.components.r				      = VK_COMPONENT_SWIZZLE_IDENTITY;
			ViewCInfo.components.g				      = VK_COMPONENT_SWIZZLE_IDENTITY;
			ViewCInfo.components.b				      = VK_COMPONENT_SWIZZLE_IDENTITY;
			ViewCInfo.components.a				      = VK_COMPONENT_SWIZZLE_IDENTITY;
			ViewCInfo.subresourceRange.layerCount     = 1;
			ViewCInfo.subresourceRange.levelCount     = 1;
			ViewCInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
			ViewCInfo.subresourceRange.baseArrayLayer = 0;
			ViewCInfo.subresourceRange.baseMipLevel	  = 0;
			if (vkCreateImageView(m_Device, &ViewCInfo, VULKAN_ALLOCATION_CALLBACK, &m_ImageViews[Iter]) != VK_SUCCESS)
			{
				Log::SetError("Unable to Create Imageview");
				return HYD_FAILED;
			}
		}


		Log::SetInfo(
			Log::FmtStr("Swapchain Created with %i Images.(Width:%i, Height%i)",
		 pImageCount, m_ImageExtent.width, m_ImageExtent.height));

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
			vkDestroyImageView(m_Device, imageview, VULKAN_ALLOCATION_CALLBACK);

		vkDestroySwapchainKHR(m_Device, m_Handle, VULKAN_ALLOCATION_CALLBACK);

		m_Handle	               = VK_NULL_HANDLE;
		m_SurfaceFormat.format     = VK_FORMAT_UNDEFINED;
		m_SurfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		m_ImageCount			   = 0;
		m_ImageExtent.height	   = 0;
		m_ImageExtent.width		   = 0;
		
		return HYD_OK;
	}

	SurfaceInfo Vulkan::Swapchain::QuarrySurfaceInfo(
		VkPhysicalDevice pDevice,
		VkSurfaceKHR	 pSurface
	) noexcept
	{
		SurfaceInfo surfaceDetail;
		//Capabilities:
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, pSurface, &surfaceDetail.Capabilities);

		//Surface formats:
		uint32_t FormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, pSurface, &FormatCount, nullptr);
		surfaceDetail.Formats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, pSurface, &FormatCount, surfaceDetail.Formats.data());


		//Presentation Modes:

		uint32_t PresentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, pSurface, &PresentModeCount, nullptr);
		surfaceDetail.PresentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, pSurface, &PresentModeCount, surfaceDetail.PresentModes.data());

		return surfaceDetail;
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
		glfwGetFramebufferSize(pWindow.GetWindow(), (int32*)&Width, (int32*)&Height);

		VkExtent2D extend;

		extend.width  = std::clamp(Width,  pSurfaceInfo.Capabilities.minImageExtent.width,  pSurfaceInfo.Capabilities.maxImageExtent.width);
		extend.height = std::clamp(Height, pSurfaceInfo.Capabilities.minImageExtent.height, pSurfaceInfo.Capabilities.maxImageExtent.height);

		return extend;
	}


	const VkImageView Vulkan::Swapchain::GetImage(uint32 pIndex) const noexcept
	{
		XE_ASSERT(pIndex < m_ImageViews.size(), "Out of imageView bound");
		return m_ImageViews[pIndex];
	}

};


