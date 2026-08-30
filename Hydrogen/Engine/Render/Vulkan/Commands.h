/*
    Implementation for Vulkan Command Pools and Command Buffers
*/

#pragma once


#include "../../Common.h"
#include <vulkan/vulkan.h>



namespace Hydrogen
{

namespace Internal
{
namespace Vulkan
{


    //Command buffer

    class CommandBuffer final
    {
    public:

        CommandBuffer()  noexcept;
        ~CommandBuffer() noexcept;

        CommandBuffer(const CommandBuffer& pOther) = delete;

        //Move
        CommandBuffer(CommandBuffer&& pOther)            noexcept;
        CommandBuffer& operator=(CommandBuffer&& pOther) noexcept;


        uint32 CreateCommandBuffer(
            VkCommandPool        pPool,
            VkCommandBufferLevel pLevel
        ) noexcept;


        //Start Recording Commands:
        uint32 RecordCommandBuffer(
            VkCommandBufferUsageFlags pFlags=0
        ) noexcept;

        uint32 EndRecordingCommandBuffer(

        ) noexcept;




        //Resets the Command buffer to the intial state
        uint32 ResetCommandBuffer(
        ) noexcept;


        inline VkCommandBuffer      GetHandle()    const {return m_Handle;}
        inline VkCommandBuffer*     GetHandlePtr()  {return &m_Handle;}


    private:
        VkCommandBuffer m_Handle = VK_NULL_HANDLE;
    };


    //Command Pool
    class CommandPool final
    {
    public:

         CommandPool() noexcept;
        ~CommandPool() noexcept;

        //Copy
        CommandPool(const CommandPool& pOther) = delete;

        //Move
        CommandPool(CommandPool&& pOther) noexcept;
        CommandPool& operator=(CommandPool&& pOther) noexcept;


        uint32 CreatePool(
            uint32                      pQueueFamily,
            VkCommandPoolCreateFlags    pFlags       = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
        )  noexcept;

        uint32 DestroyPool() noexcept;



        //Command buffer
        CommandBuffer AllocateCommandBuffer(
            VkCommandBufferLevel pLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY
        ) noexcept;



        std::vector<CommandBuffer> AllocateCommandBuffers(
            uint32               pCount,
            VkCommandBufferLevel pLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY
        ) noexcept;


        inline VkCommandPool GetHandle() const {return m_Handle;}

    private:
        VkCommandPool m_Handle           = VK_NULL_HANDLE;
        uint32        m_AllocatedBuffers = 0;
    };



};
};
};