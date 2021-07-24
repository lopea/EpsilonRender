//
// Created by Javier on 6/4/2021.
//

#ifndef EPSILONRENDERER_VULKANSWAPCHAIN_H
#define EPSILONRENDERER_VULKANSWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanRenderPass.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDescriptorPool.h"

struct GLFWwindow;

namespace Epsilon::Vulkan
{
    class vkShader;
    //! Represents the buffer that is used to render to the screen
    class SwapChain
    {
    public:
        SwapChain(Device &device, Surface &screen, GLFWwindow *window, CommandPool& pool);
        ~SwapChain();
        [[nodiscard]] VkSwapchainKHR GetSwapchainHandle() const { return handle_; }
        [[nodiscard]] VkExtent2D GetExtent() const {return extent_;}
        void Present();
        void ClearFrame();
        void FinishFrame();
        void RenderShader(vkShader* shader);

        [[nodiscard]] VkRenderPass GetRenderPass() const { return renderPass_->GetHandle();}
        [[nodiscard]] VkDevice GetDevice() const { return Device_.GetLogicalHandle();}
        [[nodiscard]] DescriptorPool& GetDescriptorPool() const { return *DescriptorPool_;}
        [[nodiscard]] uint32_t GetMinImageCount() const { return minImageCount_;}
        [[nodiscard]] uint32_t GetImageCount() const { return imageCount;}
        [[nodiscard]] CommandBuffer& GetCurrentCommandBuffer(){ return commandBuffers_[currentFrame_];}

        void MarkForRefresh();
    private:

        Device& Device_;
        Surface& surface_;
        CommandPool& pool_;
        GLFWwindow *windowHandle_;
        VkFormat format_;
        VkSwapchainKHR handle_;
        std::vector<VkImage> images_;
        std::vector<VkImageView> views_;
        VkExtent2D extent_;
        RenderPass* renderPass_;
        std::vector<VkFramebuffer> frameBuffers_;
        std::vector<CommandBuffer> commandBuffers_;
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

        DescriptorPool* DescriptorPool_;

        uint32_t minImageCount_;
        uint32_t imageCount;

        VkPresentModeKHR GetSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availableModes);

        VkExtent2D GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);

        VkSurfaceFormatKHR GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

        void CreateSwapchainHandle(const Epsilon::Vulkan::Device &device, const Epsilon::Vulkan::Surface &surface,
                                   GLFWwindow *window);

        void CreateImageViews(const Device& device);


        void CreateFrameBuffers();
        void CreateSemaphores();

        void Initialize();
        void Cleanup();
        void Refresh();

    };
}

#endif //EPSILONRENDERER_VULKANSWAPCHAIN_H
