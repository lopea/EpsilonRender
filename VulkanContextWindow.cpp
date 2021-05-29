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

namespace Epsilon
{
    const std::vector<const char *> LayerExtensions = {
        "VK_LAYER_KHRONOS_validation"
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
    vkInstance_(nullptr), vkDebugMessenger_(nullptr), vkPhysDevice_(nullptr), vkLogicalDevice_(nullptr),
    vkGraphQueue_(nullptr), vkSurface_(nullptr)
    {
      //create the glfw window
      SetWindowHandle(CreateHandleWindow(width,height));

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

    }

    VulkanContextWindow::~VulkanContextWindow()
    {

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
      //get the number of queues and check if the device has a graphics queue
      return GetQueueFamlies(Device).IsComplete();
    }

    VkQueueFamilyIndices VulkanContextWindow::GetQueueFamlies(VkPhysicalDevice Device)
    {
      VkQueueFamilyIndices indices;
      uint32_t indexCount;

      //get the queues for the device
      vkGetPhysicalDeviceQueueFamilyProperties(Device, &indexCount, nullptr);

      //store the queue properties in a vector
      std::vector<VkQueueFamilyProperties> queueProperties(indexCount);

      //get the properties
      vkGetPhysicalDeviceQueueFamilyProperties(Device, &indexCount, queueProperties.data());

      for (int i = 0; i < queueProperties.size(); i++)
      {
        //get the first use of a graphics flag
        if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
          indices.graphicsInd_ = i;

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
      VkDeviceQueueCreateInfo queueCreateInfo{};

      //start populating the logical device info
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

      //set the queue to the same one as the physical device (is this some sort of link to the physical device?)
      queueCreateInfo.queueFamilyIndex = indices.graphicsInd_.value();

      //set the count of the amount of queues the new logical device will have
      queueCreateInfo.queueCount = 1;

      //set the queue priority
      queueCreateInfo.pQueuePriorities = &queuePriority;

      //store the device features of the physical device
      VkPhysicalDeviceFeatures deviceFeatures;

      //get the device features of the current physical device
      vkGetPhysicalDeviceFeatures(vkPhysDevice_, &deviceFeatures);

      //store data to create the logical device
      VkDeviceCreateInfo deviceCreateInfo{};

      deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

      //store the queue family info for the new logical device
      deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

      //store the amount of families there are in the device
      deviceCreateInfo.queueCreateInfoCount = 1;

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

      //create the logical device
      if(vkCreateDevice(vkPhysDevice_, &deviceCreateInfo, nullptr, &vkLogicalDevice_) != VK_SUCCESS)
        //something went wrong, logical device cannot be created
        throw std::runtime_error("VULKAN ERROR: Unable to create logical device!!!");


      //get the queue for the graphics side of the device
      vkGetDeviceQueue(vkLogicalDevice_, indices.graphicsInd_.value(), 0, &vkGraphQueue_);


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
      if(vkCreateWin32SurfaceKHR(vkInstance_, &info, nullptr, &vkSurface_) != VK_SUCCESS)
        //could not connect to the window properly, big problem
        throw std::runtime_error("VULKAN ERROR: CANNOT ESTABLISH A CONNECTION WITH THE WINDOW!!!");

    }


}