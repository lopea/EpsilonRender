//
// Created by Javier on 6/10/2021.
//

#ifndef EPSILONRENDERER_VULKANRENDERPASS_H
#define EPSILONRENDERER_VULKANRENDERPASS_H

#include <vulkan/vulkan.h>
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include <vector>
namespace Epsilon::Vulkan
{
    class RenderPass
    {
    public:
        /*!
         * Create a Renderpass with a settings similar to what the default swapchain uses
         * @param device Device that will used as context for this render pass
         * @param format the type of rendering that will be done.
         */
        explicit RenderPass(Device &device, VkFormat format = VK_FORMAT_B8G8R8A8_SRGB);

        /*!
         * Create a render pass with custom settings using a create struct.
         * @param device The device that will be associated with the render
         * @param info the struct that will contain all the info to create the render pass
         */
        RenderPass(Device& device, const VkRenderPassCreateInfo& info);

        ~RenderPass();

        void StartRendering(CommandBuffer &commandBuffer);

        void EndRendering();

        void SetExtent(const VkExtent2D& extent);
        void SetClearColorValue(const VkClearValue &value, uint32_t index = 0);
        void SetOffset(const VkOffset2D &offset);
        void SetFrameBuffer(const VkFramebuffer& buffer);

        [[nodiscard]] VkRenderPass GetHandle() const {return handle_;};
    private:
        VkRenderPass handle_;
        Device& deviceHandle_;

        VkRenderPassBeginInfo info_;
        CommandBuffer* buffer_{};
        std::vector<VkClearValue> clearColorValues;
    };
}

#endif //EPSILONRENDERER_VULKANRENDERPASS_H
