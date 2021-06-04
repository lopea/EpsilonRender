//
// Created by Javier on 6/4/2021.
//

#ifndef EPSILONRENDERER_VULKANSWAPCHAIN_H
#define EPSILONRENDERER_VULKANSWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>
#include "VulkanDevice.h"

namespace Epsilon::Vulkan
{
    //! Represents the buffer that is used to render to the screen
    class SwapChain
    {
    public:
        SwapChain(const Device &device, const Surface &screen, GLFWwindow *window);
        ~SwapChain();
        [[nodiscard]] VkSwapchainKHR GetSwapchainHandle() const { return handle_; }
        [[nodiscard]] VkExtent2D GetExtent() const {return extent_;}
    private:
        VkSwapchainKHR handle_;
        std::vector<VkImage> images_;
        std::vector<VkImageView> views_;
        VkExtent2D extent_;
        VkFormat format_;
        VkDevice logicalDevice_;
        VkRenderPass renderPass_;

        VkPresentModeKHR GetSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availableModes);

        VkExtent2D GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);

        VkSurfaceFormatKHR GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

        void CreateSwapchainHandle(const Epsilon::Vulkan::Device &device, const Epsilon::Vulkan::Surface &surface,
                                   GLFWwindow *window);

        void CreateImageViews(const Device& device);

        void CreateRenderPass();

    };
}

#endif //EPSILONRENDERER_VULKANSWAPCHAIN_H
