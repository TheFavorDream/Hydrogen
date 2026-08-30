
/*

	Implemention for Vulkan Swapchain.

*/

#pragma once

#include "../../Common.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "../Window/Window.h"
#include "Image.h"	

namespace Hydrogen
{	
namespace Internal
{
namespace Vulkan
{
			
	struct SwapchainConfiguration
	{
		VkSurfaceKHR 				  Surface = VK_NULL_HANDLE;
		SurfaceInfo					  SurfaceCapabilities;
		uint32 						  ImageCount;
		std::vector<VkFormat>  		  DesiredFormats;
		std::vector<VkPresentModeKHR> DesiredPresentMode;
		VkExtent2D 					  ImageSize;
	};
	

	struct SwapchainRecreateConfiguration
	{
		VkExtent2D        NewExtent;
		VkFormat          NewFormat = VK_FORMAT_UNDEFINED; // use the old format
	};

	class Swapchain final
	{
	public:


		 Swapchain() noexcept;
		~Swapchain() noexcept;

		Swapchain(const Swapchain& pOther) = delete;
		Swapchain(Swapchain&& pOther)	   noexcept;

		uint32 CreateSwapchain(
			const SwapchainConfiguration& pConf
		)  noexcept;

		uint32 RecreateSwapchain(
			SwapchainRecreateConfiguration pConf
		) noexcept;


		uint32 AcquireImage(
			VkSemaphore pWaitSemaphore = VK_NULL_HANDLE,
			VkFence     pWaitFence	   = VK_NULL_HANDLE
		) noexcept;


		uint32 PresentImage(
			uint32 					 pImageIndex,
			VkQueue 				 pPresentQueue,
			std::vector<VkSemaphore> pWaitSemaphores={}
		) noexcept;

		uint32 DestroySwapchain() noexcept;



		//Selects the optimal image extent for swapchain images
		VkExtent2D SelectExtent(
			const SurfaceInfo& pSurfaceInfo,
			const Window&	   pWindow
		) noexcept;

		ImageView 	   		 GetImage(uint32 pIndex) const noexcept;

		inline VkSwapchainKHR  GetHandle()	          const { return m_Handle; }
		inline VkSwapchainKHR* GetHandlePtr()               { return &m_Handle;}
		inline ImageFormat	   GetImageFormat()       const { return (ImageFormat)m_SurfaceFormat.format; }
		inline VkColorSpaceKHR GetImageColorSpace()   const { return m_SurfaceFormat.colorSpace; }
		inline VkExtent2D	   GetImageExtent()       const { return m_ImageExtent; }
		inline uint32	       GetImageExtentWidth()  const { return m_ImageExtent.width; }
		inline uint32	       GetImageExtentHeight() const { return m_ImageExtent.height; }

		inline uint32		   ImageCount()	        const { return m_ImageCount; }

		
	private:


		//Selects the best available Surface format among pDesiredFormats, if failed, defaults to the first available format
		VkSurfaceFormatKHR SelectSurfaceFormat(
			const SurfaceInfo&		 pSurfaceInfo,
			const HYD_VEC<VkFormat>& pDesiredFormats
		) noexcept;

		//Selects the best available Presentation mode among pDesired modes, if failed, default to the VK_PRESENT_MODE_FIFO_KHR
		VkPresentModeKHR SelectPresentationMode(
			const SurfaceInfo&				 pSurfaceInfo,
			const HYD_VEC<VkPresentModeKHR>& pDesiredModes
		) noexcept;

	private:
		VkSwapchainKHR     m_Handle      = VK_NULL_HANDLE;
		VkSurfaceFormatKHR m_SurfaceFormat;
		VkExtent2D	       m_ImageExtent;
		uint32		       m_ImageCount = 0;
		VkPresentInfoKHR   m_PresentInfo;

		VkSwapchainCreateInfoKHR m_SwapchainCInfo;

		std::vector<Image>        m_Images;
		std::vector<ImageView>    m_ImageViews;

	};

};
};
};
