//
// Created by Javier on 6/10/2021.
//

#ifndef EPSILONRENDERER_VULKANCOMMANDPOOL_H
#define EPSILONRENDERER_VULKANCOMMANDPOOL_H

#include <vector>
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"

namespace Epsilon::Vulkan
{
    class CommandPool
    {
    public:
        CommandPool(Device &device, uint32_t queueFamilyIndex);
        CommandPool(Device& device, const VkCommandPoolCreateInfo& info);

        [[nodiscard]] VkCommandPool GetHandle() const {return handle_;}
        Device& GetDevice() {return device_;}

        ~CommandPool();

        Epsilon::Vulkan::CommandBuffer CreateCommandBuffer(VkCommandBufferLevel level);

        std::vector<CommandBuffer> CreateCommandBuffers(VkCommandBufferLevel level, uint32_t count);

    private:
        VkCommandPool handle_;
        Device& device_;

    };
}

#endif //EPSILONRENDERER_VULKANCOMMANDPOOL_H
