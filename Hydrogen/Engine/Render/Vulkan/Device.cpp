#include "Device.h"
#include <vulkan/vulkan_core.h>
#include "../../Log/Log.h"
#include "../Renderer.h"

namespace Hydrogen
{
namespace Internal
{

    uint32 Vulkan::Device::CreateDevice(
        VkInstance                     pInstance,
        const std::vector<const char*> pExtensions
    ) noexcept
    {

        m_PhysicalDevice = SelectPhysicalDevice(pInstance);
        
        Log::SetInfo(Log::FmtStr("Device Named:%s Selected", m_DeviceName.c_str()));

        //Quarry Available Extensions:

        uint32                   ExtensionCount = 0;
        std::vector<VkExtensionProperties> AvailableExtensions;
        vkEnumerateDeviceExtensionProperties(
            m_PhysicalDevice, nullptr, &ExtensionCount, nullptr
        );
        AvailableExtensions.resize(ExtensionCount);
        vkEnumerateDeviceExtensionProperties(
            m_PhysicalDevice, nullptr, &ExtensionCount, AvailableExtensions.data()
        );



        //Check for Requested Extensions:
        for (uint32 Iter = 0 ; Iter < AvailableExtensions.size() ; ++Iter)
        {
            for (auto& ext :  pExtensions)
            {
                if (!strcmp(ext, AvailableExtensions[Iter].extensionName))
                {
                    m_DeviceExtensions.push_back(ext);
                
                    if (m_DeviceExtensions.size() == pExtensions.size())
                    {
                        Iter = AvailableExtensions.size();
                        break;
                    }
                }
            }
        }



        //choose best queue family
        m_QueueFamily = QuarryQueueProperties(m_PhysicalDevice);


        std::set<uint32> QueueFamilyIndices = {
            m_QueueFamily.Compute.value(),
            m_QueueFamily.Graphics.value(),
            m_QueueFamily.Present.value(),
            m_QueueFamily.Transfer.value()
        };

        std::vector<VkDeviceQueueCreateInfo> QueueCInfo;

        float Priority = 1.0f;
        for (auto& FamilyIndex : QueueFamilyIndices)
        {
            VkDeviceQueueCreateInfo CInfo{};
            CInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            CInfo.pNext            = nullptr;
            CInfo.flags            = 0;
            CInfo.queueFamilyIndex = FamilyIndex;
            CInfo.queueCount       = 1;
            CInfo.pQueuePriorities = &Priority;
            QueueCInfo.push_back(CInfo); 
        }


        if (pExtensions.size() != m_DeviceExtensions.size())
        {
            Log::SetWarning("Not All Requested Extensions are supported");
        }

        VkDeviceCreateInfo CInfo{};
        CInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        CInfo.pNext = nullptr;
        CInfo.flags = 0;

        CInfo.enabledExtensionCount   = m_DeviceExtensions.size();   
        CInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

        CInfo.queueCreateInfoCount = QueueCInfo.size();
        CInfo.pQueueCreateInfos    = QueueCInfo.data();
        
        VkResult Res = vkCreateDevice(
            m_PhysicalDevice,
            &CInfo,
            VULKAN_ALLOCATION_CALLBACK,
            &m_Handle
        );

        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Unable to Create Device. VkError:%i", int32(Res))
            );
            return HYD_FAILED;
        }


		vkGetDeviceQueue(m_Handle, m_QueueFamily.Graphics.value(), 0, &m_Queues.Graphics);
		vkGetDeviceQueue(m_Handle, m_QueueFamily.Present.value(),  0, &m_Queues.Present);
		vkGetDeviceQueue(m_Handle, m_QueueFamily.Transfer.value(), 0, &m_Queues.Transfer);
		vkGetDeviceQueue(m_Handle, m_QueueFamily.Compute.value(),  0, &m_Queues.Compute);
        
        
        //Quarry Memory Properties:
        vkGetPhysicalDeviceMemoryProperties(
            m_PhysicalDevice, &m_DeviceMemoryProperties
        );

        
        return HYD_OK;
    }


    void Vulkan::Device::DestroyDevice() noexcept
    {
        vkDestroyDevice(
            m_Handle, VULKAN_ALLOCATION_CALLBACK
        );

        m_Handle = VK_NULL_HANDLE;
    }




    VkDeviceMemory Vulkan::Device::AllocateMemory(
        VkMemoryPropertyFlags pMemProperties,
        VkMemoryRequirements  pMemReq
    ) noexcept
    {

        int32 MemoryType = FindMemoryType(pMemProperties, pMemReq.memoryTypeBits);
        if (MemoryType == -1)
            return VK_NULL_HANDLE;


        VkMemoryAllocateInfo AllocInfo{};
        AllocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        AllocInfo.pNext           = nullptr;
        AllocInfo.memoryTypeIndex = MemoryType;
        AllocInfo.allocationSize  = pMemReq.size;
    
        VkDeviceMemory MemAddress = VK_NULL_HANDLE;
        VkResult Res = vkAllocateMemory(
            m_Handle, &AllocInfo, VULKAN_ALLOCATION_CALLBACK,&MemAddress
        );

        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Memory Allocation Failed: VkError%i", int32(Res))
            );
            return VK_NULL_HANDLE;
        }

        return MemAddress;
    }

    void Vulkan::Device::FreeMemory(
        VkDeviceMemory* pMemory
    ) noexcept
    {
        vkFreeMemory(m_Handle, *pMemory, VULKAN_ALLOCATION_CALLBACK);
        *pMemory = VK_NULL_HANDLE;
    }

    VkPhysicalDevice Vulkan::Device::SelectPhysicalDevice(
        VkInstance pInstance
    ) noexcept
    {
        uint32                        DeviceCount = 0;
        std::vector<VkPhysicalDevice> PhysicalDevices;
    
        //Quarry Available Devices:
        vkEnumeratePhysicalDevices(
            pInstance, 
            &DeviceCount,
            nullptr
        );

        PhysicalDevices.resize(DeviceCount);

        vkEnumeratePhysicalDevices(
            pInstance,
            &DeviceCount,
            PhysicalDevices.data()
        );



        //Select the best Physical Device:

        if (DeviceCount == 1)
        {
			vkGetPhysicalDeviceProperties(PhysicalDevices[0],       &m_Properties);
			vkGetPhysicalDeviceFeatures(PhysicalDevices[0],         &m_DeviceFeatures);
			vkGetPhysicalDeviceMemoryProperties(PhysicalDevices[0], &m_DeviceMemoryProperties);
            m_DeviceName    = m_Properties.deviceName;
            return PhysicalDevices[0];
        }


        ASSERT(false, "MULTI DEVICE SELECTION NOT IMPLEMENTATED");

        VkPhysicalDevice PhyDevice = VK_NULL_HANDLE;
        return PhyDevice;
    }


    Hydrogen::QueueFamily Vulkan::Device::QuarryQueueProperties(
        VkPhysicalDevice pPhysicalDevice
    ) noexcept
    {
        
        uint32 FamilyCount = 0;
        std::vector<VkQueueFamilyProperties> FamilyProperties;
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &FamilyCount, nullptr);
        FamilyProperties.resize(FamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            m_PhysicalDevice,
            &FamilyCount,
            FamilyProperties.data()
        );


        QueueFamily Family;
        

        uint32 Index = 0;
        for (auto& family : FamilyProperties)
        {
            if (Family.IsComplete())
                break;

            if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
				VkBool32 IsPresentSupported = VK_FALSE;
				vkGetPhysicalDeviceSurfaceSupportKHR(
                    m_PhysicalDevice,
                  Index,
                           Renderer::Self().GetWindow().GetSurface(),
                       &IsPresentSupported);
                
                if (IsPresentSupported)
                {
                    Family.Graphics = Index;
                    Family.Present  = Index;
                }
            }
            if (family.queueFlags & VK_QUEUE_TRANSFER_BIT)
                Family.Transfer = Index;
            if (family.queueFlags & VK_QUEUE_COMPUTE_BIT) 
                Family.Compute = Index;

            ++Index;
        }

        return Family;
    }


    int32 Vulkan::Device::FindMemoryType(
        VkMemoryPropertyFlags pProperties,
        uint32                pMemReqBits,
        uint32                pSize         //= UINT32_MAX
    ) noexcept
    {

        for (uint32 memTypeIndex = 0 ; memTypeIndex < m_DeviceMemoryProperties.memoryTypeCount ; ++memTypeIndex)
        {
            const uint32 MemTypeBits    = (1 << memTypeIndex);
            const bool   IsRequiredBits = MemTypeBits & pMemReqBits;  
            
            const bool   HasProperties  = (
                pProperties & m_DeviceMemoryProperties.memoryTypes[memTypeIndex].propertyFlags
            ) == pProperties;

            if (HasProperties && IsRequiredBits)
            {
                return static_cast<int32>(memTypeIndex);
            }
        }
        return -1; //Failed to find memory 
    }


};
};