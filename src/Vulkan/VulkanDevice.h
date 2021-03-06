//
// Created by Javier on 6/4/2021.
//

#ifndef EPSILONRENDERER_VULKANDEVICE_H
#define EPSILONRENDERER_VULKANDEVICE_H
#include <vulkan/vulkan.h>
#include "VulkanQueueFamilies.h"


namespace Epsilon::Vulkan
{
    class Instance;
    class Surface;
    class Device
    {
    public:
        explicit Device(Instance &instance, const Surface& surface);
        [[nodiscard]] VkPhysicalDevice GetPhysicalHandle() const{return vkPhysDevice_;}
        [[nodiscard]] VkDevice GetLogicalHandle() const{ return vkLogicalDevice_;}
        [[nodiscard]] VkQueue GetPresentQueue() const { return presentQueue_;}
        [[nodiscard]] VkQueue GetGraphicsQueue() const { return graphicsQueue_;}
        [[nodiscard]] uint32_t GetGraphicsIndices() const { return family.graphicsInd_.value();}
        [[nodiscard]] uint32_t GetPresentIndices() const { return family.presentInd_.value();}
        ~Device();
        //no copies allowed
        Device& operator=(const Device& other) = delete;
        Device(const Device& other) = delete;

    private:
        void PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
        bool CheckDeviceValidity(VkPhysicalDevice Device, VkSurfaceKHR Surface);

        VkPhysicalDevice vkPhysDevice_;
        VkDevice vkLogicalDevice_;
        VkQueue presentQueue_, graphicsQueue_;
        QueueFamilyIndices family;

        bool CheckDeviceExtensionCompatibility(VkPhysicalDevice Device);

        void CreateLogicalDevice(VkSurfaceKHR surface);
    };
}

#endif //EPSILONRENDERER_VULKANDEVICE_H
