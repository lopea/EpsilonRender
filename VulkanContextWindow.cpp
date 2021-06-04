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
#include "VulkanShader.h"

namespace Epsilon::Vulkan
{

    GLFWwindow *ContextWindow::CreateHandleWindow(unsigned width, unsigned height)
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
      GLFWwindow *handle = glfwCreateWindow(width, height, "Epsilon", nullptr, nullptr);

      return handle;
    }

    std::string ContextWindow::GetName()
    {
      return "Vulkan";
    }

    ContextWindow::ContextWindow(unsigned width, unsigned height)
    : Epsilon::ContextWindow(width, height, CreateHandleWindow(width, height)),
    instance_(),
    surface_(GetWindow(), instance_),
    device_(instance_, surface_),
      swapChain_(device_, surface_, GetWindow())
    {}



//    void VulkanContextWindow::InitInstance()
//    {
//
//    }
//
//    void VulkanContextWindow::InitValidationLayers()
//    {
//      //set data to create a debug context
//      //this will be used to get call back info from vulkan itself
//
//      VkDebugUtilsMessengerCreateInfoEXT createInfoExt{};
//      createInfoExt.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//      createInfoExt.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
//                                      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
//                                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
//      createInfoExt.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
//                                  | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
//                                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
//      createInfoExt.pfnUserCallback = debugCallBack;
//
//      //create debug context
//      if (VK_CreateDebugUtilsMessengerEXT(vkInstance_, &createInfoExt, nullptr, &vkDebugMessenger_) != VK_SUCCESS)
//        //something went wrong, throw error
//        throw std::runtime_error("VULKAN: Failed to create a debug context!");
//
//    }
//
//    bool VulkanContextWindow::LayerValidationCheck()
//    {
//      uint32_t layerCount;
//
//      //dont do anything if no extensions are available
//      if (LayerExtensions.empty())
//        return true;
//
//      //get the amount of layers supported by the hardware
//      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
//
//      //store the location for all available layers
//      std::vector<VkLayerProperties> availableLayers(layerCount);
//
//      //get the properties for all the layers
//      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
//
//      for (const char *extName : LayerExtensions)
//      {
//        for (const VkLayerProperties &properties : availableLayers)
//          if (strcmp(properties.layerName, extName) == 0)
//            return true;
//      }
//
//      return false;
//    }
//
//    std::vector<const char *> VulkanContextWindow::GetRequiredExtensions()
//    {
//      //store the name of the extensions
//      const char **extensionNames;
//
//      //store the amount of extensions
//      uint32_t extensionCount;
//
//      //get all the extensions that are required by glfw
//      extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);
//
//      //store all extensions in a vector for dynamic use
//      std::vector<const char *> result(extensionNames, extensionNames + extensionCount);
//
//      //add any extensions that Epsilon uses
//#ifndef NDEBUG
//      result.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//#endif
//
//      return result;
//    }
//
//    void VulkanContextWindow::PickPhysicalDevice()
//    {
//      //store device count
//      uint32_t deviceCount = 0;
//
//      vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, nullptr);
//
//      //check if the device count is zero (either the gpu does not support vulkan or there are 0 gpus in the device(lmao) )
//      if (!deviceCount)
//      {
//        throw std::runtime_error("VULKAN ERROR: failed to find GPUs that support Vulkan!!!");
//      }
//
//      //store all physical device handles
//      std::vector<VkPhysicalDevice> devices(deviceCount);
//
//      //get all device handles available
//      vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, devices.data());
//
//      //check all objects and see if they are valid
//      for (VkPhysicalDevice currentDevice : devices)
//      {
//        if (CheckDeviceValidity(currentDevice))
//        {
//          //set the context device to the current one in the list
//          vkPhysDevice_ = currentDevice;
//
//          //get out of here!
//          break;
//        }
//      }
//
//      //if no proper device has been found
//      if (!vkPhysDevice_)
//        throw std::runtime_error("VULKAN ERROR: No devices properly support vulkan!");
//
//    }
//
//    bool VulkanContextWindow::CheckDeviceValidity(VkPhysicalDevice Device)
//    {
//      //get the context for this device
//      SwapChainContext context = GetSwapChainContext(Device);
//
//      //get the number of queues and check if the device has a graphics queue
//      return GetQueueFamlies(Device).IsComplete() && CheckDeviceExtensionCompatibility(Device)
//             && !context.presents.empty() && !context.formats.empty();
//    }
//
//    VulkanContextWindow::VkQueueFamilyIndices VulkanContextWindow::GetQueueFamlies(VkPhysicalDevice Device)
//    {
//      VkQueueFamilyIndices indices;
//      uint32_t indexCount;
//
//      //get the queues for the device
//      vkGetPhysicalDeviceQueueFamilyProperties(Device, &indexCount, nullptr);
//
//      //store the queue properties in a vector
//      std::vector<VkQueueFamilyProperties> queueProperties(indexCount);
//
//      //get the properties
//      vkGetPhysicalDeviceQueueFamilyProperties(Device, &indexCount, queueProperties.data());
//
//      for (uint32_t i = 0; i < queueProperties.size(); i++)
//      {
//        //get the first use of a graphics flag
//        if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
//          indices.graphicsInd_ = i;
//
//        //check if the device supports a window of some sort
//        VkBool32 isPresent = false;
//        vkGetPhysicalDeviceSurfaceSupportKHR(Device, i, vkSurface_, &isPresent);
//
//        //if it does add it to the family
//        if (isPresent)
//          indices.presentInd_ = i;
//
//        //if we have what we wanted, leave
//        if (indices.IsComplete())
//          break;
//      }
//
//      return indices;
//    }
//
//    void VulkanContextWindow::CreateLogicalDevice()
//    {
//      //store the priority of this logical device
//      float queuePriority = 1.0f;
//
//      //get the queue family index for the physical device
//      VkQueueFamilyIndices indices = GetQueueFamlies(vkPhysDevice_);
//
//      //store the struct to create queues for the logical device
//      std::vector<VkDeviceQueueCreateInfo> queues_;
//      //store all the unique queues in the physical device
//      std::set<uint32_t> uniqueFamilies = {indices.graphicsInd_.value(), indices.presentInd_.value()};
//
//      //go through all the unique values in the list
//      for (uint32_t index : uniqueFamilies)
//      {
//        //populate information about the current device queue
//        VkDeviceQueueCreateInfo info{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
//        info.queueFamilyIndex = index;
//        info.queueCount = 1;
//        info.pQueuePriorities = &queuePriority;
//        //add it to the list of values
//        queues_.push_back(info);
//      }
//
//      //store the physical device features
//      VkPhysicalDeviceFeatures deviceFeatures;
//      //get the device features of the current physical device
//      vkGetPhysicalDeviceFeatures(vkPhysDevice_, &deviceFeatures);
//
//      //store data to create the logical device
//      VkDeviceCreateInfo deviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
//
//
//      //store the queue family info for the new logical device
//      deviceCreateInfo.pQueueCreateInfos = queues_.data();
//
//      //store the amount of families there are in the device
//      deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queues_.size());
//
//      //store the features of the physical device to the logical one
//      deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
//
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
//
//      //create the logical device
//      if (vkCreateDevice(vkPhysDevice_, &deviceCreateInfo, nullptr, &vkLogicalDevice_) != VK_SUCCESS)
//        //something went wrong, logical device cannot be created
//        throw std::runtime_error("VULKAN ERROR: Unable to create logical device!!!");
//
//
//      //get the queue for the graphics side of the device
//      vkGetDeviceQueue(vkLogicalDevice_, indices.graphicsInd_.value(), 0, &vkGraphQueue_);
//
//      //get the queue for the present side of the device (screen, windows, etc)
//      vkGetDeviceQueue(vkLogicalDevice_, indices.presentInd_.value(), 0, &vkPresentQueue_);
//
//
//    }
//
//    void VulkanContextWindow::CreateWindowConnection()
//    {
//      //TODO: This is windows specific, make sure to do something similar for linux
//
//      //store info related to the window for the engine
//      VkWin32SurfaceCreateInfoKHR info{};
//
//      //populate information
//      info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
//      //give the window handle to vulkan
//      info.hwnd = glfwGetWin32Window(GetWindow());
//      //give the instance
//      info.hinstance = GetModuleHandle(nullptr);
//
//      //create the connection to the window and vulkan
//      if (vkCreateWin32SurfaceKHR(vkInstance_, &info, nullptr, &vkSurface_) != VK_SUCCESS)
//        //could not connect to the window properly, big problem
//        throw std::runtime_error("VULKAN ERROR: CANNOT ESTABLISH A CONNECTION WITH THE WINDOW!!!");
//
//    }
//
//    bool VulkanContextWindow::CheckDeviceExtensionCompatibility(VkPhysicalDevice Device)
//    {
//      //store the count of extensions supported by the device
//      uint32_t extensionCount = 0;
//
//      //get the count of extensions supported
//      vkEnumerateDeviceExtensionProperties(Device, nullptr, &extensionCount, nullptr);
//
//      //store all the extensions that are part of this device
//      std::vector<VkExtensionProperties> properties(extensionCount);
//
//      //get the extensions
//      vkEnumerateDeviceExtensionProperties(Device, nullptr, &extensionCount, properties.data());
//
//
//      //check every extension that is required and check if the device supports it
//      std::set<std::string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());
//
//      for (const auto &extension : properties)
//      {
//        requiredExtensions.erase(extension.extensionName);
//      }
//
//      //check if all required extensions are available in th device
//      return requiredExtensions.empty();
//    }
//
//    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    /// Swap chain functions
//    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    VulkanContextWindow::SwapChainContext VulkanContextWindow::GetSwapChainContext(VkPhysicalDevice Device)
//    {
//      //store all the info necessary for a swapchain
//      SwapChainContext context;
//      //store the format mode count
//      uint32_t formatCount;
//      //store the present mode count
//      uint32_t presentCount;
//      //get the capabilities of the device
//      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, vkSurface_, &context.capabilities);
//
//      //get the count of the format device
//      vkGetPhysicalDeviceSurfaceFormatsKHR(Device, vkSurface_, &formatCount, nullptr);
//
//      //resize the array of the format list
//      context.formats.resize(formatCount);
//
//      //get all the surface formats for the device
//      vkGetPhysicalDeviceSurfaceFormatsKHR(Device, vkSurface_, &formatCount, context.formats.data());
//
//      //get the surface formats of the device
//      vkGetPhysicalDeviceSurfacePresentModesKHR(Device, vkSurface_, &presentCount, nullptr);
//
//      //resize the present mode list for the context
//      context.presents.resize(presentCount);
//
//      //get all the present modes for the device
//      vkGetPhysicalDeviceSurfacePresentModesKHR(Device, vkSurface_, &presentCount, context.presents.data());
//
//      return context;
//    }
//
//    VkSurfaceFormatKHR VulkanContextWindow::GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
//    {
//      //go through all the available formats
//      for (const VkSurfaceFormatKHR &format : availableFormats)
//      {
//        //if the format is what we want, sent it
//        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
//          return format;
//
//      }
//
//      //return something at this point
//      return availableFormats[0];
//    }
//
//    VkPresentModeKHR VulkanContextWindow::GetSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availableModes)
//    {
//      //iterate through all modes that are available
//      for (VkPresentModeKHR currentMode : availableModes)
//      {
//        //only check for the mailbox mode if available
//        if (currentMode == VK_PRESENT_MODE_MAILBOX_KHR)
//          return currentMode;
//      }
//
//      //return the standard present mode
//      return VK_PRESENT_MODE_FIFO_KHR;
//    }
//
//    VkExtent2D VulkanContextWindow::GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
//    {
//      //check if the device capabilities already contain valid info
//      if (capabilities.currentExtent.width != UINT32_MAX)
//        return capabilities.currentExtent;
//
//      //get the framebuffer size for the window
//      int width, height;
//      glfwGetFramebufferSize(GetWindow(), &width, &height);
//
//      //create a handle for the size of the window
//      VkExtent2D extent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
//
//      //clamp values just in case
//      extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
//      extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
//
//      return extent;
//    }
//
//    void VulkanContextWindow::CreateSwapChain()
//    {
//      //get the context for the swapchain
//      SwapChainContext context = GetSwapChainContext(vkPhysDevice_);
//
//      //create minimum image count
//      unsigned imageCount = context.capabilities.minImageCount + 1;
//      if (context.capabilities.minImageCount > 0 && imageCount > context.capabilities.maxImageCount)
//        imageCount = context.capabilities.maxImageCount;
//      //create a struct for creating a swapchain
//      VkSwapchainCreateInfoKHR info{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
//
//      auto surfaceFormat = GetSwapChainFormat(context.formats);
//      //populate struct data
//      info.surface = vkSurface_;
//      info.minImageCount = imageCount;
//      info.imageFormat = surfaceFormat.format;
//      info.imageColorSpace = surfaceFormat.colorSpace;
//      info.imageExtent = GetSwapExtent(context.capabilities);
//      info.imageArrayLayers = 1;
//      info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
//
//      //get the queue family info for the device
//      VkQueueFamilyIndices indices = GetQueueFamlies(vkPhysDevice_);
//      uint32_t queueFamily[] = {indices.graphicsInd_.value(), indices.presentInd_.value()};
//
//      //set proper values based on how the queue families are set up
//      if (indices.graphicsInd_ != indices.presentInd_)
//      {
//        info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
//        info.queueFamilyIndexCount = 2;
//        info.pQueueFamilyIndices = queueFamily;
//      } else
//      {
//        info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//        info.queueFamilyIndexCount = 0;
//        info.pQueueFamilyIndices = nullptr;
//      }
//
//      info.preTransform = context.capabilities.currentTransform;
//      info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//      info.presentMode = GetSwapChainPresentMode(context.presents);
//      info.clipped = true;
//      info.oldSwapchain = VK_NULL_HANDLE;
//
//      //create the swapchain
//      if (vkCreateSwapchainKHR(vkLogicalDevice_, &info, nullptr, &vkSwapChain_) != VK_SUCCESS)
//        //something went terribly wrong
//        throw std::runtime_error("VULKAN ERROR: Could not create a swap chain!");
//
//      //get the swapchain images
//      unsigned int imageSize;
//      vkGetSwapchainImagesKHR(vkLogicalDevice_, vkSwapChain_, &imageSize, nullptr);
//      vkSwapChainImages_.resize(imageSize);
//      vkGetSwapchainImagesKHR(vkLogicalDevice_, vkSwapChain_, &imageSize, vkSwapChainImages_.data());
//
//
//      vkscExtent = info.imageExtent;
//      vkscImageFormat = info.imageFormat;
//
//
//    }
//
//    void VulkanContextWindow::CreateImageViews()
//    {
//      //resize the image view to the amount of images in the swap chain
//      vkImageViews_.reserve(vkSwapChainImages_.size());
//
//      //iterate through all the images in the swapchain
//      for (auto &vkSwapChainImage : vkSwapChainImages_)
//      {
//        //we gotta create a new fucking struct, again...
//        VkImageViewCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
//
//
//        //set the image for the view
//        info.image = vkSwapChainImage;
//
//        //set the view type and the image format
//        info.format = vkscImageFormat;
//
//        //set the type of the image format
//        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
//
//        //allow the color channels to be interchangeable
//        info.components.r = info.components.g = info.components.b = info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
//
//        //describe the image and what it does.
//        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//        info.subresourceRange.baseMipLevel = 0;
//        info.subresourceRange.levelCount = 1;
//        info.subresourceRange.baseArrayLayer = 0;
//        info.subresourceRange.layerCount = 1;
//
//        VkImageView view;
//
//        //create the image view for the swapchain
//        if (vkCreateImageView(vkLogicalDevice_, &info, nullptr, &view) != VK_SUCCESS)
//          //something went terribly wrong
//          throw std::runtime_error("VULKAN ERROR: Cannot create an image view!");
//
//        //add the view to the list
//        vkImageViews_.push_back(view);
//      }
//    }
//
//
//    void VulkanContextWindow::CreateRenderPass()
//    {
//      //setup the color attachemnt for 1 image that will be used for rendering
//      VkAttachmentDescription colorAttachment{};
//      colorAttachment.format = vkscImageFormat;
//
//      //only one sample since multisampling is not enabled
//      colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//
//      //clear all data before rendering
//      colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//
//      //keep data when we are done rendering
//      colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//
//      //we dont do anything with the stencil buffer, so we dont care about what happens to it
//      colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//      colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//
//      //we dont care how the layout of the render pass starts off
//      colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//
//      //we want the renderpass to be used for swap chain purposes
//      colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//
//      //create a sub pass for actual rendering (we can be a little more complicated once we do more advanced rendering)
//      VkAttachmentReference colorAttachmentRef{};
//
//      //connect the sub pass to the 0th index in the pass
//      colorAttachmentRef.attachment = 0;
//
//      //we want to optimize this subpass as much as possible
//      colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//      //we need to define the subpass that we want to create
//      VkSubpassDescription subpass{};
//
//      //describe that the subpass is used for graphics
//      subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//
//      //attach the subpass to the description
//      //NOTE: you can add more than one subpass for rendering!!!!!
//      subpass.colorAttachmentCount = 1;
//      subpass.pColorAttachments = &colorAttachmentRef;
//
//      //Create a dependency
//      VkSubpassDependency dependency{};
//
//      //define a subpass as implicit to the render pass
//      dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//
//      //set the dependancy to the only subpass
//      dependency.srcSubpass = 0;
//
//
//      //set when the subpass should wait
//      dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//      dependency.srcAccessMask = 0;
//
//      //set when the subpass should wait when the swap chain is currently getting written
//      dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//      dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//      dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//      //another stupid struct to create the render pass
//      VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
//
//      //only one color attachment
//      renderPassInfo.attachmentCount = 1;
//      renderPassInfo.pAttachments = &colorAttachment;
//
//      //set the subpass for rendering
//      renderPassInfo.pSubpasses = &subpass;
//      renderPassInfo.subpassCount = 1;
//
//      //add the dependency to the renderpass
//      renderPassInfo.pDependencies = &dependency;
//      renderPassInfo.dependencyCount = 1;
//
//      //create the render pass
//      if (vkCreateRenderPass(vkLogicalDevice_, &renderPassInfo, nullptr, &vkRenderPass_) != VK_SUCCESS)
//        throw std::runtime_error("VULKAN ERROR: Cannot create the proper render pass!!");
//
//
//    }
//
//    VkExtent2D VulkanContextWindow::GetWindowExtent() const
//    {
//      return vkscExtent;
//    }
//
//    VkDevice VulkanContextWindow::GetLogicalDevice() const
//    {
//      return vkLogicalDevice_;
//    }
//
//    VkRenderPass VulkanContextWindow::GetWindowRenderPass() const
//    {
//      return vkRenderPass_;
//    }
//
//    void VulkanContextWindow::CreateFrameBuffers()
//    {
//      //create the size for the framebuffers
//      vkSwapChainFramebuffers_.resize(vkSwapChainImages_.size());
//
//      //create every buffer for the swapchain
//      for (size_t i = 0; i < vkImageViews_.size(); i++)
//      {
//        //store all the attachments for the buffer
//        VkImageView attachments[] = {vkImageViews_[i]};
//
//        //create the struct that give info to the create the struct
//        VkFramebufferCreateInfo framebufferInfo{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
//
//        //populate the struct
//        framebufferInfo.renderPass = vkRenderPass_;
//        framebufferInfo.attachmentCount = 1;
//        framebufferInfo.pAttachments = attachments;
//        framebufferInfo.width = vkscExtent.width;
//        framebufferInfo.height = vkscExtent.height;
//        framebufferInfo.layers = 1;
//
//        //create the buffer
//        if (vkCreateFramebuffer(vkLogicalDevice_, &framebufferInfo, nullptr, &vkSwapChainFramebuffers_[i]) !=
//            VK_SUCCESS)
//          throw std::runtime_error("VULKAN ERROR: Failed to create a framebuffer!");
//      }
//    }
//
//    void VulkanContextWindow::CreateCommandPool()
//    {
//      VkQueueFamilyIndices queueFamilyIndices = GetQueueFamlies(vkPhysDevice_);
//
//      //another struct on the list
//      VkCommandPoolCreateInfo info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
//
//      //store the graphics index in the family
//      info.queueFamilyIndex = queueFamilyIndices.graphicsInd_.value();
//
//      //create the command pool
//      if (vkCreateCommandPool(vkLogicalDevice_, &info, nullptr, &vkCommandPool_) != VK_SUCCESS)
//        throw std::runtime_error("VULKAN ERROR: Failed to create a command buffer!");
//
//    }
//
//    void VulkanContextWindow::CreateCommandBuffers()
//    {
//      //resize the buffers based on the frame buffers
//      vkCommandBuffers_.resize(vkSwapChainFramebuffers_.size());
//
//      //please end my suffering with all these structs!!!!
//      VkCommandBufferAllocateInfo info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
//      info.commandPool = vkCommandPool_;
//      info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//      info.commandBufferCount = static_cast<uint32_t>(vkCommandBuffers_.size());
//
//      //create the command buffer
//      if (vkAllocateCommandBuffers(vkLogicalDevice_, &info, vkCommandBuffers_.data()) != VK_SUCCESS)
//        throw std::runtime_error("VULKAN ERROR: UNABLE TO ALLOCATE COMMANDBUFFERS!");
//
//
//      //start recording for command buffers
//      for (int i = 0; i < vkCommandBuffers_.size(); i++)
//      {
//        //if i had a cent for every struct i've made in this project, i would have enough to buy a gun so i can end my misery
//        VkCommandBufferBeginInfo cbinfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
//
//        //start the buffer for recording
//        if (vkBeginCommandBuffer(vkCommandBuffers_[i], &cbinfo) != VK_SUCCESS)
//          throw std::runtime_error("VULKAN ERROR: FAILED TO INITIALIZE COMMANDBUFFER!!!");
//
//        //start running the render pass
//
//        //another struct, another cent
//        VkRenderPassBeginInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
//        renderPassInfo.renderPass = vkRenderPass_;
//        renderPassInfo.framebuffer = vkSwapChainFramebuffers_[i];
//
//        renderPassInfo.renderArea.offset = {0, 0};
//        renderPassInfo.renderArea.extent = vkscExtent;
//
//        //set the clear color for the texture
//        VkClearValue clearColor = {0, 0, 0, 1};
//
//        renderPassInfo.clearValueCount = 1;
//        renderPassInfo.pClearValues = &clearColor;
//
//        //set the render pass
//        vkCmdBeginRenderPass(vkCommandBuffers_[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//
//        vkCmdBindPipeline(vkCommandBuffers_[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());
//        vkCmdDraw(vkCommandBuffers_[i], 3, 1, 0, 0);
//        vkCmdEndRenderPass(vkCommandBuffers_[i]);
//
//
//        if (vkEndCommandBuffer(vkCommandBuffers_[i]) != VK_SUCCESS)
//          throw std::runtime_error("VULKAN ERROR: Failed to record command buffer!!!");
//      }
//
//
//    }
//
//    void VulkanContextWindow::CreateSemaphores()
//    {
//      //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
//      VkSemaphoreCreateInfo semaphoreInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
//
//      //create the semaphores
//      if (vkCreateSemaphore(vkLogicalDevice_, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
//          vkCreateSemaphore(vkLogicalDevice_, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
//        throw std::runtime_error("VULKAN ERROR: Cannot create semaphores!!!");
//
//
//    }

    void ContextWindow::DrawFrame()
    {
//      //store the next image for rendering
//      uint32_t imageIndex;
//
//      //get the next image
//      vkAcquireNextImageKHR(vkLogicalDevice_, vkSwapChain_, UINT32_MAX, imageAvailableSemaphore, nullptr, &imageIndex);
//
//      //this is fucking torture
//      VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};
//
//      //store a location of the necessary semaphore in a sendable format
//      VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
//
//      //store the stage of the semaphore
//      VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
//
//      //populate struct
//      submitInfo.waitSemaphoreCount = 1;
//
//      //send wait requirements
//      submitInfo.pWaitSemaphores = waitSemaphores;
//      submitInfo.pWaitSemaphores = waitSemaphores;
//      submitInfo.pWaitDstStageMask = waitStages;
//
//      //send command buffers
//      submitInfo.commandBufferCount = 1;
//      submitInfo.pCommandBuffers = &vkCommandBuffers_[imageIndex];
//
//      //create the done timer in a sendable format
//      VkSemaphore signalSemaphore[] = {renderFinishedSemaphore};
//      submitInfo.signalSemaphoreCount = 1;
//      submitInfo.pSignalSemaphores = signalSemaphore;
//
//      //create send the render request to the queue
//      if (vkQueueSubmit(vkGraphQueue_, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
//        throw std::runtime_error("VULKAN ERROR: Failed to submit the command buffer!!!");
//
//      VkPresentInfoKHR presentinfo{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
//
//      presentinfo.waitSemaphoreCount =1;
//      presentinfo.pWaitSemaphores= signalSemaphore;
//
//      VkSwapchainKHR swapchains[] = {vkSwapChain_};
//
//      presentinfo.swapchainCount = 1;
//
//      presentinfo.pSwapchains = swapchains;
//      presentinfo.pImageIndices = &imageIndex;
//
//      vkQueuePresentKHR(vkPresentQueue_, &presentinfo);
//
//      vkQueueWaitIdle(vkPresentQueue_);
    }

}