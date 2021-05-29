//
// Created by Javier on 5/28/2021.
//

#ifndef EPSILONRENDERER_VULKANCONTEXTRENDERER_H
#define EPSILONRENDERER_VULKANCONTEXTRENDERER_H

#include <vector>
#include "ContextRenderer.h"
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

    class VulkanContextRenderer : public ContextRenderer
    {
    public:
        VulkanContextRenderer();

        GLFWwindow *CreateWindow(unsigned width, unsigned height) override;

        std::string GetName() override;

        ~VulkanContextRenderer() override;
    private:
        VkInstance vkInstance_;
        VkDebugUtilsMessengerEXT vkDebugMessenger_;
        VkPhysicalDevice vkPhysDevice_;



        //! create the instance for this context renderer
        void InitInstance();
        void InitValidationLayers();
        bool LayerValidationCheck();
        void PickPhysicalDevice();
        bool CheckDeviceValidity(VkPhysicalDevice Device);
        VkQueueFamilyIndices GetQueueFamlies(VkPhysicalDevice Device);
        std::vector<const char*> GetRequiredExtensions();
    };

}

#endif //EPSILONRENDERER_VULKANCONTEXTRENDERER_H
