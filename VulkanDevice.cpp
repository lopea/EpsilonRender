//
// Created by Javier on 6/4/2021.
//

#include <cstdint>
#include <vector>
#include <set>
#include "VulkanDevice.h"
#include "VulkanInitializationException.h"
#include "SwapChainContext.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanQueueFamilies.h"

namespace Epsilon::Vulkan
{

    const std::vector<const char *> DeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };


    void Device::PickPhysicalDevice(VkInstance vkInstance_, VkSurfaceKHR surface)
    {
      //store device count
      uint32_t deviceCount = 0;

      vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, nullptr);

      //check if the device count is zero (either the gpu does not support vulkan or there are 0 gpus in the device(lmao) )
      if (!deviceCount)
      {
        throw InitializationException("failed to find GPUs that support Vulkan!!!");
      }

      //store all physical device handles
      std::vector <VkPhysicalDevice> devices(deviceCount);

      //get all device handles available
      vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, devices.data());

      //check all objects and see if they are valid
      for (VkPhysicalDevice currentDevice : devices)
      {
        if (CheckDeviceValidity(currentDevice, surface))
        {
          //set the context device to the current one in the list
          vkPhysDevice_ = currentDevice;

          //get out of here!
          break;
        }
      }

      //if no proper device has been found
      if (!vkPhysDevice_)
        throw InitializationException("VULKAN ERROR: No devices properly support vulkan!");
    }
    void Device::CreateLogicalDevice(VkSurfaceKHR surface)
    {
      //store the priority of this logical device
      float queuePriority = 1.0f;

      //get the queue family index for the physical device
      QueueFamilyIndices indices(vkPhysDevice_, surface);

      //store the struct to create queues for the logical device
      std::vector<VkDeviceQueueCreateInfo> queues_;
      //store all the unique queues in the physical device
      std::set<uint32_t> uniqueFamilies = {indices.graphicsInd_.value(), indices.presentInd_.value()};

      //go through all the unique values in the list
      for (uint32_t index : uniqueFamilies)
      {
        //populate information about the current device queue
        VkDeviceQueueCreateInfo info{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
        info.queueFamilyIndex = index;
        info.queueCount = 1;
        info.pQueuePriorities = &queuePriority;
        //add it to the list of values
        queues_.push_back(info);
      }

      //store the physical device features
      VkPhysicalDeviceFeatures deviceFeatures;
      //get the device features of the current physical device
      vkGetPhysicalDeviceFeatures(vkPhysDevice_, &deviceFeatures);

      //store data to create the logical device
      VkDeviceCreateInfo deviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};


      //store the queue family info for the new logical device
      deviceCreateInfo.pQueueCreateInfos = queues_.data();

      //store the amount of families there are in the device
      deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queues_.size());

      //store the features of the physical device to the logical one
      deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

//      /*
//       * not really necessary anymore but on older versions of vulkan, there was a point where you had to specify the
//       * the same validation layers on the instance to the logical device. That is not the case anymore but it will still get
//       * added for compatibility
//       */
//
//#ifndef NDEBUG
//      deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(LayerExtensions.size());
//      deviceCreateInfo.ppEnabledLayerNames = LayerExtensions.data();
//
//#else
//      deviceCreateInfo.enabledLayerCount = 0;
//#endif
//      deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
//      deviceCreateInfo.ppEnabledExtensionNames = DeviceExtensions.data();

      //create the logical device
      if (vkCreateDevice(vkPhysDevice_, &deviceCreateInfo, nullptr, &vkLogicalDevice_) != VK_SUCCESS)
        //something went wrong, logical device cannot be created
        throw InitializationException("Unable to create logical device!!!");


      //get the queue for the graphics side of the device
      vkGetDeviceQueue(vkLogicalDevice_, indices.graphicsInd_.value(), 0, &graphicsQueue_);

      //get the queue for the present side of the device (screen, windows, etc)
      vkGetDeviceQueue(vkLogicalDevice_, indices.presentInd_.value(), 0, &presentQueue_);


    }

    Device::Device(Instance &instance, const Surface &surface)
    {
      PickPhysicalDevice(instance.GetInstanceHandle(), surface.GetSurfaceHandle());
      CreateLogicalDevice(surface.GetSurfaceHandle());
    }

    bool Device::CheckDeviceValidity(VkPhysicalDevice Device, VkSurfaceKHR Surface)
    {
      //get the context for this device
      SwapChainContext context(Device, Surface);

      //get the number of queues and check if the device has a graphics queue
      return QueueFamilyIndices(Device,Surface).IsComplete() && CheckDeviceExtensionCompatibility(Device)
             && !context.presents.empty() && !context.formats.empty();
    }



    bool Device::CheckDeviceExtensionCompatibility(VkPhysicalDevice Device)
    {
      //store the count of extensions supported by the device
      uint32_t extensionCount = 0;

      //get the count of extensions supported
      vkEnumerateDeviceExtensionProperties(Device, nullptr, &extensionCount, nullptr);

      //store all the extensions that are part of this device
      std::vector<VkExtensionProperties> properties(extensionCount);

      //get the extensions
      vkEnumerateDeviceExtensionProperties(Device, nullptr, &extensionCount, properties.data());


      //check every extension that is required and check if the device supports it
      std::set<std::string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());

      for (const auto &extension : properties)
      {
        requiredExtensions.erase(extension.extensionName);
      }

      //check if all required extensions are available in th device
      return requiredExtensions.empty();
    }
    Device::~Device()
    {
      //destroy the logical device
      vkDestroyDevice(vkLogicalDevice_, nullptr);
    }
}

