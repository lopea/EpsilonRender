//
// Created by Javier on 6/10/2021.
//

#ifndef EPSILONRENDERER_VULKANCOMMANDBUFFER_H
#define EPSILONRENDERER_VULKANCOMMANDBUFFER_H


#include <vulkan/vulkan.h>
#include "VulkanDevice.h"


namespace Epsilon::Vulkan
{
    class CommandPool;
    class CommandBuffer
    {
    public:
        CommandBuffer(Epsilon::Vulkan::CommandPool &pool_, VkCommandBufferLevel level);
        CommandBuffer(VkCommandBuffer handle, CommandPool &device);
        void BeginRecording(VkCommandBufferUsageFlags recordingFlags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        void EndRecording();

        void SubmitCommands();

        [[nodiscard]] VkCommandBuffer GetHandle() const { return handle_;}
        void Reset(VkCommandBufferResetFlags flags = 0);
    private:
        VkCommandBuffer handle_;
        CommandPool& pool_;
    };
}

#endif //EPSILONRENDERER_VULKANCOMMANDBUFFER_H
