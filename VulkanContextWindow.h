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
    //!used to store queues on a physical device
    struct VkQueueFamilyIndices
    {
        std::optional<uint32_t> graphicsInd_;

        [[nodiscard]] bool IsComplete() const
        {
          return graphicsInd_.has_value();
        }
    };

    class VulkanContextWindow : public ContextWindow
    {
    public:
        VulkanContextWindow(unsigned width, unsigned height);


        std::string GetName() override;

        ~VulkanContextWindow() override;
    private:
        VkInstance vkInstance_;
        VkDebugUtilsMessengerEXT vkDebugMessenger_;
        VkPhysicalDevice vkPhysDevice_;
        VkDevice vkLogicalDevice_;
        VkQueue vkGraphQueue_;
        VkSurfaceKHR vkSurface_;


        //! create the instance for this context renderer
        GLFWwindow *CreateHandleWindow(unsigned width, unsigned height);
        void InitInstance();
        void InitValidationLayers();
        bool LayerValidationCheck();
        void CreateWindowConnection();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        bool CheckDeviceValidity(VkPhysicalDevice Device);
        VkQueueFamilyIndices GetQueueFamlies(VkPhysicalDevice Device);

        std::vector<const char*> GetRequiredExtensions();
    };

}

#endif //EPSILONRENDERER_VULKANCONTEXTWINDOW_H
