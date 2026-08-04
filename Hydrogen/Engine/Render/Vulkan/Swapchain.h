
/*

	Implemention for Vulkan Swapchain.

*/

#pragma once

#include "../../Common.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "Structs.h"
#include "../Window/Window.h"	

namespace Hydrogen
{
namespace Vulkan
{

	class Swapchain final
	{
	public:


		 Swapchain() noexcept;
		~Swapchain() noexcept;

		Swapchain(const Swapchain& pOther) = delete;
		Swapchain(Swapchain&& pOther)	   noexcept;

		uint32 CreateSwapchain(
			const SurfaceInfo&			  pSurfaceInfo,
			VkDevice				      pDevice,
			VkSurfaceKHR			      pSurface,
			uint32					      pImageCount,
			VkExtent2D				      pExtent,
			VkSurfaceFormatKHR		      pSurfaceFormat,
			VkPresentModeKHR			  pPresentMode    = VK_PRESENT_MODE_FIFO_KHR
		)  noexcept;


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



		SurfaceInfo QuarrySurfaceInfo(
			VkPhysicalDevice pDevice,
			VkSurfaceKHR	 pSurface
		) noexcept;

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

		//Selects the optimal image extent for swapchain images
		VkExtent2D SelectExtent(
			const SurfaceInfo& pSurfaceInfo,
			const Window&	   pWindow
		) noexcept;


		const VkImageView 	   GetImage(uint32 pIndex) const noexcept;

		inline VkSwapchainKHR  GetHandle()	        const { return m_Handle; }
		inline VkSwapchainKHR* GetHandlePtr()             { return &m_Handle;}
		inline VkFormat		   GetImageFormat()     const { return m_SurfaceFormat.format; }
		inline VkColorSpaceKHR GetImageColorSpace() const { return m_SurfaceFormat.colorSpace; }
		inline VkExtent2D	   GetImageExtent()     const { return m_ImageExtent; }
		inline uint32		   ImageCount()	        const { return m_ImageCount; }

		

	private:
		VkDevice	       m_Device		 = VK_NULL_HANDLE;
		VkSwapchainKHR     m_Handle      = VK_NULL_HANDLE;
		VkSurfaceFormatKHR m_SurfaceFormat;
		VkExtent2D	       m_ImageExtent;
		uint32		       m_ImageCount = 0;

		VkPresentInfoKHR   m_PresentInfo;

		//Temp: These will be removed from here once i implemented Image Pool.
		std::vector<VkImage>     m_Images;
		std::vector<VkImageView> m_ImageViews;

	};

};
};
