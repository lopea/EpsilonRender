//
// Created by Javier on 6/4/2021.
//

#ifndef EPSILONRENDERER_SWAPCHAINCONTEXT_H
#define EPSILONRENDERER_SWAPCHAINCONTEXT_H

#include <vulkan/vulkan.h>

namespace Epsilon::Vulkan
{
    struct SwapChainContext
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presents;

        SwapChainContext(VkPhysicalDevice Device, VkSurfaceKHR surface)
        {

          //store the format mode count
          uint32_t formatCount;
          //store the present mode count
          uint32_t presentCount;
          //get the capabilities of the device
          vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, surface, &capabilities);

          //get the count of the format device
          vkGetPhysicalDeviceSurfaceFormatsKHR(Device, surface, &formatCount, nullptr);

          //resize the array of the format list
          formats.resize(formatCount);

          //get all the surface formats for the device
          vkGetPhysicalDeviceSurfaceFormatsKHR(Device, surface, &formatCount, formats.data());

          //get the surface formats of the device
          vkGetPhysicalDeviceSurfacePresentModesKHR(Device, surface, &presentCount, nullptr);

          //resize the present mode list for the context
          presents.resize(presentCount);

          //get all the present modes for the device
          vkGetPhysicalDeviceSurfacePresentModesKHR(Device, surface, &presentCount, presents.data());

        }
    };
}
#endif //EPSILONRENDERER_SWAPCHAINCONTEXT_H
