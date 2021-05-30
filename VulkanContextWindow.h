//
// Created by Javier on 5/28/2021.
//

#ifndef EPSILONRENDERER_VULKANCONTEXTWINDOW_H
#define EPSILONRENDERER_VULKANCONTEXTWINDOW_H

#include <vector>
#include "ContextWindow.h"
#include <optional>
namespace Epsilon
{


    class VulkanContextWindow : public ContextWindow
    {
    public:
        VulkanContextWindow(unsigned width, unsigned height);


        std::string GetName() override;

        ~VulkanContextWindow() override;
    private:

        //!used to store queues on a physical device
        struct VkQueueFamilyIndices
        {
            std::optional<uint32_t> graphicsInd_;
            std::optional<uint32_t> presentInd_;

            [[nodiscard]] bool IsComplete() const
            {
              return graphicsInd_.has_value() && presentInd_.has_value();
            }
        };

        struct SwapChainContext
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presents;
        };

        VkInstance vkInstance_;
        VkDebugUtilsMessengerEXT vkDebugMessenger_;
        VkPhysicalDevice vkPhysDevice_;
        VkDevice vkLogicalDevice_;
        VkQueue vkGraphQueue_, vkPresentQueue_;
        VkSurfaceKHR vkSurface_;


        VkSwapchainKHR vkSwapChain_;
        std::vector<VkImage> vkSwapChainImages_;
        std::vector<VkImageView> vkImageView_;
        VkFormat vkscImageFormat;
        VkExtent2D vkscExtent;
        //! create the instance for this context renderer
        GLFWwindow *CreateHandleWindow(unsigned width, unsigned height);

        //Init functions
        void InitInstance();
        void InitValidationLayers();
        bool LayerValidationCheck();
        void CreateWindowConnection();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapChain();
        void CreateImageViews();

        bool CheckDeviceValidity(VkPhysicalDevice Device);
        static std::vector<const char *> GetRequiredExtensions();
        static bool CheckDeviceExtensionCompatibility(VkPhysicalDevice Device);
        VkQueueFamilyIndices GetQueueFamlies(VkPhysicalDevice Device);

        //Swapchain functions

        SwapChainContext GetSwapChainContext(VkPhysicalDevice Device);
        static VkSurfaceFormatKHR GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& avaialbleFormats);
        static VkPresentModeKHR GetSwapChainPresentMode(const std::vector<VkPresentModeKHR>& availableModes);
        VkExtent2D GetSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    };

}

#endif //EPSILONRENDERER_VULKANCONTEXTWINDOW_H
