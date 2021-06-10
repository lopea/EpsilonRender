//
// Created by Javier on 6/4/2021.
//

#ifndef EPSILONRENDERER_VULKANSWAPCHAIN_H
#define EPSILONRENDERER_VULKANSWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>
#include "VulkanDevice.h"

struct GLFWwindow;

namespace Epsilon::Vulkan
{
    class vkShader;
    //! Represents the buffer that is used to render to the screen
    class SwapChain
    {
    public:
        SwapChain(Device &device, Surface &screen, GLFWwindow *window);
        ~SwapChain();
        [[nodiscard]] VkSwapchainKHR GetSwapchainHandle() const { return handle_; }
        [[nodiscard]] VkExtent2D GetExtent() const {return extent_;}
        void Present();
        void ClearFrame();
        void FinishFrame();
        void RenderShader(vkShader* shader);

        [[nodiscard]] VkRenderPass GetRenderPass() const { return renderPass_;}
        [[nodiscard]] VkDevice GetDevice() const { return Device_.GetLogicalHandle();}
        [[nodiscard]] VkCommandPool GetCommnandPool() const { return commandPool_; }

        void MarkForRefresh();
    private:

        Device& Device_;
        Surface& surface_;
        GLFWwindow *windowHandle_;
        VkFormat format_;
        VkSwapchainKHR handle_;
        std::vector<VkImage> images_;
        std::vector<VkImageView> views_;
        VkExtent2D extent_;
        VkRenderPass renderPass_;
        std::vector<VkFramebuffer> frameBuffers_;
        VkCommandPool commandPool_;
        std::vector<VkCommandBuffer> commandBuffers_;
        std::vector<VkSemaphore> imageAvailableSemaphore_, renderFinishedSemaphore_;
        std::vector<VkFence> fences_;
        std::size_t currentFrame_ = 0;
        const int framesPerFlight = 2;
        //store the next image for rendering
        uint32_t imageIndex = 0;

        //! check if the swapchain is not up to date with the surface and needs to be recreated
        bool needsRefresh = false;

        //! check if the swapchain is not valid for rendering (having a screen size of zero or minimized)
        bool allowRendering = true;

        VkPresentModeKHR GetSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availableModes);

        VkExtent2D GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);

        VkSurfaceFormatKHR GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

        void CreateSwapchainHandle(const Epsilon::Vulkan::Device &device, const Epsilon::Vulkan::Surface &surface,
                                   GLFWwindow *window);

        void CreateImageViews(const Device& device);

        void CreateRenderPass();

        void CreateCommandPool(const Device &device, const Surface &screen);

        void CreateFrameBuffers();

        void CreateCommandBuffers();

        void CreateSemaphores();

        void Initialize();
        void Cleanup();
        void Refresh();

    };
}

#endif //EPSILONRENDERER_VULKANSWAPCHAIN_H
