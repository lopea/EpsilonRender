//
// Created by Javier on 6/4/2021.
//

#ifndef EPSILONRENDERER_VULKANQUEUEFAMILIES_H
#define EPSILONRENDERER_VULKANQUEUEFAMILIES_H

#include <vulkan/vulkan.h>
#include <optional>

namespace Epsilon::Vulkan
{
    //! used to store queues on a physical device
    struct QueueFamilyIndices
    {
        std::optional <uint32_t> graphicsInd_;
        std::optional <uint32_t> presentInd_;

        /*!Does this family of queues contain enough properties to be considered complete?
         * @return true if the entire family of queues contain valid data, false otherwise
         */
        [[nodiscard]] bool IsComplete() const
        {
          return graphicsInd_.has_value() && presentInd_.has_value();
        }

        /*!
         * Get all the queue families present in a physical device
         * @param device if the device contains all the proper families to be considered complete,
         * the function will return a complete family queue. Otherwise, the queue returned will be incomplete
         */
        explicit QueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface) : graphicsInd_(), presentInd_()
        {

          uint32_t indexCount;

          //get the queues for the device
          vkGetPhysicalDeviceQueueFamilyProperties(device, &indexCount, nullptr);

          //store the queue properties in a vector
          std::vector<VkQueueFamilyProperties> queueProperties(indexCount);

          //get the properties
          vkGetPhysicalDeviceQueueFamilyProperties(device, &indexCount, queueProperties.data());

          for (uint32_t i = 0; i < queueProperties.size(); i++)
          {
            //get the first use of a graphics flag
            if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
              graphicsInd_ = i;

            //check if the device supports a window of some sort
            VkBool32 isPresent = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &isPresent);

            //if it does add it to the family
            if (isPresent)
              presentInd_ = i;

            //if we have what we wanted, leave
            if (IsComplete())
              break;
          }
        }
    };

}
#endif //EPSILONRENDERER_VULKANQUEUEFAMILIES_H
