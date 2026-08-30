
/*

    Device class manages the device creation and device memory allocation

*/

#pragma once


#include "../../Common.h"
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <optional>


namespace Hydrogen
{
    struct QueueSharingMode
    {
        VkSharingMode   SharingMode;
        uint32          QueueFamilyIndexCount;
        const uint32_t* QueueFamilyIndices;
    };
    
    
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



namespace Internal
{
namespace Vulkan
{

    struct Queues
	{
		VkQueue Graphics = VK_NULL_HANDLE;
		VkQueue Present  = VK_NULL_HANDLE;
		VkQueue Transfer = VK_NULL_HANDLE;
		VkQueue Compute  = VK_NULL_HANDLE;
	};


    class Device
    {
    public:

         Device() = default;
        ~Device() = default;
        
        Device(const Device& ) = delete;
        Device(Device&& )      = delete;

        Device& operator=(const Device& ) = delete;
        Device& operator=(Device&& )      = delete;


        VkDeviceMemory AllocateMemory(
            VkMemoryPropertyFlags pMemProperties,
            VkMemoryRequirements  pMemReq
        ) noexcept; 


        void FreeMemory(
            VkDeviceMemory* pMemory
        ) noexcept;        


    private: //friend only

        uint32 CreateDevice(
            VkInstance                     pInstance,
            const std::vector<const char*> pExtensions
        ) noexcept;


        void DestroyDevice() noexcept;


    private: //private members

        VkPhysicalDevice SelectPhysicalDevice(
            VkInstance pInstance
        ) noexcept;
        

        Hydrogen::QueueFamily QuarryQueueProperties(
            VkPhysicalDevice pPhysicalDevice
        ) noexcept;

        int32 FindMemoryType(
            VkMemoryPropertyFlags pProperties,
            uint32                pMemReqBits,
            uint32                pSize         = UINT32_MAX
        ) noexcept;

    private:
        VkDevice                         m_Handle         = VK_NULL_HANDLE;
        VkPhysicalDevice                 m_PhysicalDevice = VK_NULL_HANDLE;
        std::string			             m_DeviceName;
        VkPhysicalDeviceProperties       m_Properties;
		VkPhysicalDeviceFeatures         m_DeviceFeatures;
		VkPhysicalDeviceMemoryProperties m_DeviceMemoryProperties;    
        std::vector<const char*>         m_DeviceExtensions;
		Queues				 		     m_Queues;
        Hydrogen::QueueFamily            m_QueueFamily;


    private: //Friend classes
        friend class Hydrogen::Renderer;
        friend class Swapchain;
    };
};
};
};