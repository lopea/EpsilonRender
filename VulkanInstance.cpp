//
// Created by Javier on 6/4/2021.
//

#include "VulkanInstance.h"
#include "VulkanInitializationException.h"
#include "SwapChainContext.h"
#include "VulkanQueueFamilies.h"
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Epsilon::Vulkan
{
    const std::vector<const char *> LayerExtensions = {
        "VK_LAYER_KHRONOS_validation"
    };

    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                 const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
                                                 void *userData)
    {
      (void) severity;
      (void) userData;
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

    void Instance::InitInstance()
    {

    }

    std::vector<const char *> Instance::GetRequiredExtensions() const
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

    void Instance::EnableValidationLayers()
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
      if (VK_CreateDebugUtilsMessengerEXT(vkInstance_, &createInfoExt, nullptr, &vkMessenger_) != VK_SUCCESS)
        //something went wrong, throw error
        throw InitializationException("Failed to create a debug context!");

    }
    bool Instance::LayerValidationCheck()
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
    Instance::Instance()
    {

#ifndef NDEBUG
      //check if the validation layer is not valid
      if (!LayerValidationCheck())
        throw InitializationException("Validation layers are invalid!!");
#endif
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
      VkInstanceCreateInfo vkCreateInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};

      //add the application info
      vkCreateInfo.pApplicationInfo = &info;

      //
      //set extensions
      //

      //get the extensions required for the renderer
      auto extensions = GetRequiredExtensions();

      //set the extensions to the instance info
      vkCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
      vkCreateInfo.ppEnabledExtensionNames = extensions.data();

      //setup extensions if necessary

#ifndef NDEBUG
      vkCreateInfo.enabledLayerCount = static_cast<uint32_t>(LayerExtensions.size());
      vkCreateInfo.ppEnabledLayerNames = LayerExtensions.data();
#else
      vkCreateInfo.enabledLayerCount = 0;
#endif


      //create the instance!
      if (vkCreateInstance(&vkCreateInfo, nullptr, &vkInstance_) != VK_SUCCESS)
        //something went wrong, quit program
        throw InitializationException("Failed to create vulkan instance!!");

#ifndef NDEBUG
      EnableValidationLayers();
#endif
    }

    Instance::~Instance()
    {
      //cleanup the instance
      vkDestroyInstance(vkInstance_, nullptr);
    }

}