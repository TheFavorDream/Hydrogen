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

        if (m_AllocationCount)
        {
            Log::SetError(
                Log::FmtStr("Not All Allocated memory are destroyed. Remaining:%i", m_AllocationCount)
            );
        }


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

        m_AllocationCount += 1;
        return MemAddress;
    }

    void Vulkan::Device::FreeMemory(
        VkDeviceMemory* pMemory
    ) noexcept
    {
        vkFreeMemory(m_Handle, *pMemory, VULKAN_ALLOCATION_CALLBACK);
        *pMemory = VK_NULL_HANDLE;
        m_AllocationCount -= 1;
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


/*

    Instance Implementation

*/


    Vulkan::Instance:: Instance() noexcept
    {

    }
    Vulkan::Instance::~Instance() noexcept
    {
       // DestroyInstance();
    }
    

    bool Vulkan::Instance::IsExtensionAvailable(const char* pExtName) noexcept
    {
        if (!m_Extensions.size())
        {
            uint32 ExtensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);
            m_Extensions.resize(ExtensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, m_Extensions.data());
        }

        for (auto& ext : m_Extensions)
        {
            if (strcmp(ext.extensionName, pExtName) == 0)
                return true;
        }

        return false;
    }
    
    void Vulkan::Instance::PushExtension(const char* pExtName)        noexcept
    {
        if (!IsExtensionAvailable(pExtName))
            return;

        m_EnabledExtensions.push_back(pExtName);
    }

    uint32 Vulkan::Instance::CreateVkInstance(
        VkApplicationInfo pAppInfo,
        bool              pValidationLayers
    ) noexcept
    {
        const char* LayerNames[1] = {"VK_LAYER_KHRONOS_validation"};



        VkInstanceCreateInfo CInfo{};
        CInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        CInfo.pNext                   = nullptr;
        CInfo.flags                   = 0;
        CInfo.pApplicationInfo        = &pAppInfo;
        CInfo.enabledLayerCount       = 0;
        CInfo.ppEnabledLayerNames     = nullptr;

        if (pValidationLayers)
        {
            CInfo.enabledLayerCount   = 1;
            CInfo.ppEnabledLayerNames = LayerNames;
            PushExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);            
        }
        
        CInfo.enabledExtensionCount   = m_EnabledExtensions.size();
        CInfo.ppEnabledExtensionNames = m_EnabledExtensions.data();

        VkResult Res = vkCreateInstance(
            &CInfo,
             VULKAN_ALLOCATION_CALLBACK,
              &m_Handle
            );


        if (Res != VK_SUCCESS)
        {
            Log::SetError(
                Log::FmtStr("Failed to Create the Vulkan Instance. VkError:%i", int32(Res))
            );
            return HYD_FAILED;
        }



        if (!pValidationLayers)
            return HYD_OK;



        //Load the extension functions, because they aren't loaded by default:

        LD_vkCreateDebugUtilsMessengerEXT  = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_Handle, "vkCreateDebugUtilsMessengerEXT"));
        LD_vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_Handle, "vkDestroyDebugUtilsMessengerEXT"));


        if (LD_vkCreateDebugUtilsMessengerEXT == nullptr || LD_vkDestroyDebugUtilsMessengerEXT == nullptr)
            return HYD_FAILED;

        //Enable Message Callbacks
        VkDebugUtilsMessengerCreateInfoEXT MessengerCInfo{
            .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext           = nullptr,
            .flags           = 0,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT   | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
            .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
            .pfnUserCallback = Log::VulkanInfoWarningCallback, 
            .pUserData       = nullptr
        };

        LD_vkCreateDebugUtilsMessengerEXT(
            m_Handle, &MessengerCInfo, VULKAN_ALLOCATION_CALLBACK, &m_DebugCB1
        );

        MessengerCInfo = VkDebugUtilsMessengerCreateInfoEXT{
            .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext           = nullptr,
            .flags           = 0,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
            .pfnUserCallback = Log::VulkanErrorCallback, 
            .pUserData       = nullptr
        };

        LD_vkCreateDebugUtilsMessengerEXT(
            m_Handle, &MessengerCInfo, VULKAN_ALLOCATION_CALLBACK, &m_DebugCB2
        );

        return HYD_OK;
    }

    void   Vulkan::Instance::DestroyInstance()  noexcept
    {
        if (m_Handle == VK_NULL_HANDLE)
            return;


        if (m_DebugCB1 != VK_NULL_HANDLE)
        {
            LD_vkDestroyDebugUtilsMessengerEXT(m_Handle, m_DebugCB1, VULKAN_ALLOCATION_CALLBACK);
            LD_vkDestroyDebugUtilsMessengerEXT(m_Handle, m_DebugCB2, VULKAN_ALLOCATION_CALLBACK);
        }


        vkDestroyInstance(
            m_Handle, VULKAN_ALLOCATION_CALLBACK
        );

        m_Handle  = VK_NULL_HANDLE;
        m_Extensions.clear();
        m_EnabledExtensions.clear();


    }




};
};