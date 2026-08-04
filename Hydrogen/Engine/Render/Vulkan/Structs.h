#pragma once

#include "../../Common.h"
#include <vulkan/vulkan.h>
#include <string>
#include <optional>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{


	struct QueueFamily
	{
		//Queues supported by the family
		std::optional<uint32> Graphics;
		std::optional<uint32> Present;
		std::optional<uint32> Compute;
		std::optional<uint32> Transfer;

		inline bool IsComplete() const
		{
			return Graphics.has_value() && Present.has_value() && Compute.has_value() && Transfer.has_value();
		}
	};

	struct Queues
	{
		VkQueue Graphics = VK_NULL_HANDLE;
		VkQueue Present  = VK_NULL_HANDLE;
		VkQueue Transfer = VK_NULL_HANDLE;
		VkQueue Compute  = VK_NULL_HANDLE;
	};

	struct GPUInfo
	{
		std::string			             Name;
		uint32				             DriverVersion;
		uint32				             ApiVersion;
		VkPhysicalDeviceType             Type;
		VkPhysicalDeviceLimits           Limits;
		VkPhysicalDeviceFeatures         Features;
		VkPhysicalDeviceMemoryProperties MemoryProperties;

		QueueFamily				 		 Queues;
	};

	struct SurfaceInfo
	{
		VkSurfaceCapabilitiesKHR    Capabilities;
		HYD_VEC<VkSurfaceFormatKHR> Formats;
		HYD_VEC<VkPresentModeKHR>   PresentModes;
	};
};