//
// Created by Javier on 5/28/2021.
//

#include "VulkanContextWindow.h"

#include <exception>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <optional>
#include <GLFW/glfw3native.h>
#include <set>
#include <algorithm>

namespace Epsilon
{
    const std::vector<const char *> LayerExtensions = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char *> DeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                 const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
                                                 void *userData)
    {
      std::cerr << "VULKAN MESSAGE" << std::endl << "TYPE: " << messageType << std::endl << callbackData->pMessage
                << std::endl;

      return VK_FALSE;
    }

    //used to access an extension function in Vulkan, specifically one to get error messages from vulkan itself
    VkResult VK_CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *info,
                                             const VkAllocationCallbacks *alloc, VkDebugUtilsMessengerEXT *messenger)
    {
      //get the function from vulkan
      auto debugFunc = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                                  "vkCreateDebugUtilsMessengerEXT");
      //if the function exists,
      if (debugFunc)
        return debugFunc(instance, info, alloc, messenger);

      //something went wrong, (maybe extension does not exist, bad driver installation)
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    //used to access function to remove any debug tools and shut them down properly
    VkResult VK_DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks *alloc)
    {

      auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                              "vkDestroyDebugUtilsMessengerEXT");
      if (func)
      {
        func(instance, debugMessenger, alloc);
        return VK_SUCCESS;
      }
      //something went wrong (bad driver installation)
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    GLFWwindow *VulkanContextWindow::CreateHandleWindow(unsigned width, unsigned height)
    {
      //initialize glfw and check if it successfully initialized
      if (!glfwInit())
      {
        //did not initialize properly
        throw std::runtime_error("CRITICAL ERROR: GLFW CANNOT INITIALIZE!!!");
      }

      //tell glfw that we are not using any gl handles
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

      //create the window
      GLFWwindow *handle_ = glfwCreateWindow(width, height, "Epsilon", nullptr, nullptr);

      return handle_;
    }

    std::string VulkanContextWindow::GetName()
    {
      return "Vulkan";
    }

    VulkanContextWindow::VulkanContextWindow(unsigned width, unsigned height) : ContextWindow(width, height),
                                                                                vkInstance_(nullptr),
                                                                                vkDebugMessenger_(nullptr),
                                                                                vkPhysDevice_(nullptr),
                                                                                vkLogicalDevice_(nullptr),
                                                                                vkGraphQueue_(nullptr),
                                                                                vkSurface_(nullptr),
                                                                                vkSwapChain_(nullptr)
    {
      //create the glfw window
      SetWindowHandle(CreateHandleWindow(width, height));

#ifndef NDEBUG
      if (!LayerValidationCheck())
        throw std::runtime_error("VULKAN: Validation layers are invalid!!");
#endif
      //create the instance
      InitInstance();


#ifndef NDEBUG
      //initialize any debug checks for vulkan
      InitValidationLayers();
#endif

      //start a connection with the glfw window
      CreateWindowConnection();

      //get the physical device for rendering
      PickPhysicalDevice();

      //create the logical device for rendering
      CreateLogicalDevice();

      //create a handle for sending pixel data from vulkan to the screen
      CreateSwapChain();

    }

    VulkanContextWindow::~VulkanContextWindow()
    {

      //destroy the swap chain
      vkDestroySwapchainKHR(vkLogicalDevice_,vkSwapChain_, nullptr);

      //destroy the debug context if necessary
#ifndef NDEBUG
      VK_DestroyDebugUtilsMessengerEXT(vkInstance_, vkDebugMessenger_, nullptr);
#endif

      vkDestroySurfaceKHR(vkInstance_, vkSurface_, nullptr);
      //clear the logical device
      vkDestroyDevice(vkLogicalDevice_, nullptr);

      //destroy the instance before shutting down
      vkDestroyInstance(vkInstance_, nullptr);
    }

    void VulkanContextWindow::InitInstance()
    {
//store the infomation that will be given to Vulkan
      VkApplicationInfo info{};

      //set the type of the application info
      info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

      //set the name of our application
      info.pApplicationName = "Epsilon";

      //set the name of the engine
      info.pEngineName = "Epsilon Engine";

      //set the version of the vulkan instance and of our engine
      info.engineVersion = VK_MAKE_API_VERSION(1, 0, 1, 0);
      info.apiVersion = VK_API_VERSION_1_2;

      //store the information needed to create an instance
      VkInstanceCreateInfo vkCreateInfo{};

      //set the type
      vkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

      //add the application info
      vkCreateInfo.pApplicationInfo = &info;

      //
      //set extensions
      //

      //get the extensions required for the renderer
      auto extensions = GetRequiredExtensions();

      //set the extensions to the instance info
      vkCreateInfo.enabledExtensionCount = extensions.size();
      vkCreateInfo.ppEnabledExtensionNames = extensions.data();

      //setup extensions if necessary

#ifndef NDEBUG
      vkCreateInfo.enabledLayerCount = LayerExtensions.size();
      vkCreateInfo.ppEnabledLayerNames = LayerExtensions.data();
#else
      vkCreateInfo.enabledLayerCount = 0;
#endif


      //create the instance!
      if (vkCreateInstance(&vkCreateInfo, nullptr, &vkInstance_) != VK_SUCCESS)
        //something went wrong, quit program
        throw std::runtime_error("VULKAN: Failed to create vulkan instance!!");

    }

    void VulkanContextWindow::InitValidationLayers()
    {
      //set data to create a debug context
      //this will be used to get call back info from vulkan itself

      VkDebugUtilsMessengerCreateInfoEXT createInfoExt{};
      createInfoExt.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      createInfoExt.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      createInfoExt.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                  | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      createInfoExt.pfnUserCallback = debugCallBack;

      //create debug context
      if (VK_CreateDebugUtilsMessengerEXT(vkInstance_, &createInfoExt, nullptr, &vkDebugMessenger_) != VK_SUCCESS)
        //something went wrong, throw error
        throw std::runtime_error("VULKAN: Failed to create a debug context!");

    }

    bool VulkanContextWindow::LayerValidationCheck()
    {
      uint32_t layerCount;

      //dont do anything if no extensions are available
      if (LayerExtensions.empty())
        return true;

      //get the amount of layers supported by the hardware
      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

      //store the location for all available layers
      std::vector<VkLayerProperties> availableLayers(layerCount);

      //get the properties for all the layers
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

      for (const char *extName : LayerExtensions)
      {
        for (const VkLayerProperties &properties : availableLayers)
          if (strcmp(properties.layerName, extName) == 0)
            return true;
      }

      return false;
    }

    std::vector<const char *> VulkanContextWindow::GetRequiredExtensions()
    {
      //store the name of the extensions
      const char **extensionNames;

      //store the amount of extensions
      uint32_t extensionCount;

      //get all the extensions that are required by glfw
      extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);

      //store all extensions in a vector for dynamic use
      std::vector<const char *> result(extensionNames, extensionNames + extensionCount);

      //add any extensions that Epsilon uses
#ifndef NDEBUG
      result.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

      return result;
    }

    void VulkanContextWindow::PickPhysicalDevice()
    {
      //store device count
      uint32_t deviceCount = 0;

      vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, nullptr);

      //check if the device count is zero (either the gpu does not support vulkan or there are 0 gpus in the device(lmao) )
      if (!deviceCount)
      {
        throw std::runtime_error("VULKAN ERROR: failed to find GPUs that support Vulkan!!!");
      }

      //store all physical device handles
      std::vector<VkPhysicalDevice> devices(deviceCount);

      //get all device handles available
      vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, devices.data());

      //check all objects and see if they are valid
      for (VkPhysicalDevice currentDevice : devices)
      {
        if (CheckDeviceValidity(currentDevice))
        {
          //set the context device to the current one in the list
          vkPhysDevice_ = currentDevice;

          //get out of here!
          break;
        }
      }

      //if no proper device has been found
      if (!vkPhysDevice_)
        throw std::runtime_error("VULKAN ERROR: No devices properly support vulkan!");

    }

    bool VulkanContextWindow::CheckDeviceValidity(VkPhysicalDevice Device)
    {
      //get the context for this device
      SwapChainContext context = GetSwapChainContext(Device);

      //get the number of queues and check if the device has a graphics queue
      return GetQueueFamlies(Device).IsComplete() && CheckDeviceExtensionCompatibility(Device)
             && !context.presents.empty() && !context.formats.empty();
    }

    VulkanContextWindow::VkQueueFamilyIndices VulkanContextWindow::GetQueueFamlies(VkPhysicalDevice Device)
    {
      VkQueueFamilyIndices indices;
      uint32_t indexCount;

      //get the queues for the device
      vkGetPhysicalDeviceQueueFamilyProperties(Device, &indexCount, nullptr);

      //store the queue properties in a vector
      std::vector<VkQueueFamilyProperties> queueProperties(indexCount);

      //get the properties
      vkGetPhysicalDeviceQueueFamilyProperties(Device, &indexCount, queueProperties.data());

      for (uint32_t i = 0; i < queueProperties.size(); i++)
      {
        //get the first use of a graphics flag
        if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
          indices.graphicsInd_ = i;

        //check if the device supports a window of some sort
        VkBool32 isPresent = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(Device, i, vkSurface_, &isPresent);

        //if it does add it to the family
        if (isPresent)
          indices.presentInd_ = i;

        //if we have what we wanted, leave
        if (indices.IsComplete())
          break;
      }

      return indices;
    }

    void VulkanContextWindow::CreateLogicalDevice()
    {
      //store the priority of this logical device
      float queuePriority = 1.0f;

      //get the queue family index for the physical device
      VkQueueFamilyIndices indices = GetQueueFamlies(vkPhysDevice_);

      //store the struct to create queues for the logical device
      std::vector<VkDeviceQueueCreateInfo> queues_;
      //store all the unique queues in the physical device
      std::set<uint32_t> uniqueFamilies = {indices.graphicsInd_.value(), indices.presentInd_.value()};

      //go through all the unique values in the list
      for (uint32_t index : uniqueFamilies)
      {
        //populate information about the current device queue
        VkDeviceQueueCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
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
      VkDeviceCreateInfo deviceCreateInfo{};

      deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

      //store the queue family info for the new logical device
      deviceCreateInfo.pQueueCreateInfos = queues_.data();

      //store the amount of families there are in the device
      deviceCreateInfo.queueCreateInfoCount = queues_.size();

      //store the features of the physical device to the logical one
      deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

      /*
       * not really necessary anymore but on older versions of vulkan, there was a point where you had to specify the
       * the same validation layers on the instance to the logical device. That is not the case anymore but it will still get
       * added for compatibility
       */

#ifndef NDEBUG
      deviceCreateInfo.enabledLayerCount = LayerExtensions.size();
      deviceCreateInfo.ppEnabledLayerNames = LayerExtensions.data();

#else
      deviceCreateInfo.enabledLayerCount = 0;
#endif
      deviceCreateInfo.enabledExtensionCount = DeviceExtensions.size();
      deviceCreateInfo.ppEnabledExtensionNames = DeviceExtensions.data();

      //create the logical device
      if (vkCreateDevice(vkPhysDevice_, &deviceCreateInfo, nullptr, &vkLogicalDevice_) != VK_SUCCESS)
        //something went wrong, logical device cannot be created
        throw std::runtime_error("VULKAN ERROR: Unable to create logical device!!!");


      //get the queue for the graphics side of the device
      vkGetDeviceQueue(vkLogicalDevice_, indices.graphicsInd_.value(), 0, &vkGraphQueue_);

      //get the queue for the present side of the device (screen, windows, etc)
      vkGetDeviceQueue(vkLogicalDevice_, indices.presentInd_.value(), 0, &vkPresentQueue_);


    }

    void VulkanContextWindow::CreateWindowConnection()
    {
      //TODO: This is windows specific, make sure to do something similar for linux

      //store info related to the window for the engine
      VkWin32SurfaceCreateInfoKHR info{};

      //populate information
      info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
      //give the window handle to vulkan
      info.hwnd = glfwGetWin32Window(GetWindow());
      //give the instance
      info.hinstance = GetModuleHandle(nullptr);

      //create the connection to the window and vulkan
      if (vkCreateWin32SurfaceKHR(vkInstance_, &info, nullptr, &vkSurface_) != VK_SUCCESS)
        //could not connect to the window properly, big problem
        throw std::runtime_error("VULKAN ERROR: CANNOT ESTABLISH A CONNECTION WITH THE WINDOW!!!");

    }

    bool VulkanContextWindow::CheckDeviceExtensionCompatibility(VkPhysicalDevice Device)
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

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Swap chain functions
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    VulkanContextWindow::SwapChainContext VulkanContextWindow::GetSwapChainContext(VkPhysicalDevice Device)
    {
      //store all the info necessary for a swapchain
      SwapChainContext context;
      //store the format mode count
      uint32_t formatCount;
      //store the present mode count
      uint32_t presentCount;
      //get the capabilities of the device
      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, vkSurface_, &context.capabilities);

      //get the count of the format device
      vkGetPhysicalDeviceSurfaceFormatsKHR(Device, vkSurface_, &formatCount, nullptr);

      //resize the array of the format list
      context.formats.resize(formatCount);

      //get all the surface formats for the device
      vkGetPhysicalDeviceSurfaceFormatsKHR(Device, vkSurface_, &formatCount, context.formats.data());

      //get the surface formats of the device
      vkGetPhysicalDeviceSurfacePresentModesKHR(Device, vkSurface_, &presentCount, nullptr);

      //resize the present mode list for the context
      context.presents.resize(presentCount);

      //get all the present modes for the device
      vkGetPhysicalDeviceSurfacePresentModesKHR(Device, vkSurface_, &presentCount, context.presents.data());

      return context;
    }

    VkSurfaceFormatKHR VulkanContextWindow::GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
      //go through all the available formats
      for (const VkSurfaceFormatKHR &format : availableFormats)
      {
        //if the format is what we want, sent it
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
          return format;

      }

      //return something at this point
      return availableFormats[0];
    }

    VkPresentModeKHR VulkanContextWindow::GetSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availableModes)
    {
      //iterate through all modes that are available
      for (VkPresentModeKHR currentMode : availableModes)
      {
        //only check for the mailbox mode if available
        if (currentMode == VK_PRESENT_MODE_MAILBOX_KHR)
          return currentMode;
      }

      //return the standard present mode
      return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanContextWindow::GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
    {
      //check if the device capabilities already contain valid info
      if (capabilities.currentExtent.width != UINT32_MAX)
        return capabilities.currentExtent;

      //get the framebuffer size for the window
      int width, height;
      glfwGetFramebufferSize(GetWindow(), &width, &height);

      //create a handle for the size of the window
      VkExtent2D extent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

      //clamp values just in case
      extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
      extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    }

    void VulkanContextWindow::CreateSwapChain()
    {
      //get the context for the swapchain
      SwapChainContext context = GetSwapChainContext(vkPhysDevice_);

      //create minimum image count
      unsigned imageCount = context.capabilities.minImageCount + 1;
      if(context.capabilities.minImageCount > 0 && imageCount > context.capabilities.maxImageCount)
        imageCount = context.capabilities.maxImageCount;
      //create a struct for creating a swapchain
      VkSwapchainCreateInfoKHR info{};

      auto surfaceFormat = GetSwapChainFormat(context.formats);
      //populate struct data
      info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      info.surface = vkSurface_;
      info.minImageCount = imageCount;
      info.imageFormat = surfaceFormat.format;
      info.imageColorSpace = surfaceFormat.colorSpace;
      info.imageExtent = GetSwapExtent(context.capabilities);
      info.imageArrayLayers = 1;
      info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

      //get the queue family info for the device
      VkQueueFamilyIndices indices = GetQueueFamlies(vkPhysDevice_);
      uint32_t queueFamily[] = {indices.graphicsInd_.value(), indices.presentInd_.value()};

      //set proper values based on how the queue families are set up
      if(indices.graphicsInd_ != indices.presentInd_)
      {
        info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        info.queueFamilyIndexCount = 2;
        info.pQueueFamilyIndices = queueFamily;
      }
      else
      {
        info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        info.queueFamilyIndexCount = 0;
        info.pQueueFamilyIndices = nullptr;
      }

      info.preTransform = context.capabilities.currentTransform;
      info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      info.presentMode = GetSwapChainPresentMode(context.presents);
      info.clipped = true;
      info.oldSwapchain = VK_NULL_HANDLE;

      //create the swapchain
      if(vkCreateSwapchainKHR(vkLogicalDevice_, &info, nullptr, &vkSwapChain_) != VK_SUCCESS)
        //something went terribly wrong
        throw std::runtime_error("VULKAN ERROR: Could not create a swap chain!");


    }


}