//
// Created by Javier on 6/10/2021.
//

#ifndef EPSILONRENDERER_VULKANDESCRIPTORPOOL_H
#define EPSILONRENDERER_VULKANDESCRIPTORPOOL_H


#include "VulkanDevice.h"

namespace Epsilon::Vulkan
{
    class DescriptorPool
    {
    public:
        explicit DescriptorPool(Device &device);
        [[nodiscard]] VkDescriptorPool GetHandle() const {return handle_;}
        ~DescriptorPool();
    private:
        VkDescriptorPool handle_;
        Device& device_;
    };
}

#endif //EPSILONRENDERER_VULKANDESCRIPTORPOOL_H
