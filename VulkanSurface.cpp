//
// Created by Javier on 6/4/2021.
//
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "VulkanSurface.h"
#include "VulkanException.h"

namespace Epsilon::Vulkan
{
    Surface::Surface(GLFWwindow *windowHandle_, Instance &instance) : vkSurface_(nullptr),
                                                                      instance_(instance)
    {
      //TODO: This is windows specific, make sure to do something similar for linux

      //store info related to the window for the engine
      VkWin32SurfaceCreateInfoKHR info{};

      //populate information
      info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
      //give the window handle to vulkan
      info.hwnd = glfwGetWin32Window(windowHandle_);
      //give the instance
      info.hinstance = GetModuleHandle(nullptr);

      //create the connection to the window and vulkan
      if (vkCreateWin32SurfaceKHR(instance_.GetInstanceHandle(), &info, nullptr, &vkSurface_) != VK_SUCCESS)
        //could not connect to the window properly, big problem
        throw InitializationException("CANNOT ESTABLISH A CONNECTION WITH THE WINDOW!!!");
    }

    Surface::~Surface()
    {
      //destroy the surface handle and all of its contents
      vkDestroySurfaceKHR(instance_.GetInstanceHandle(), vkSurface_, nullptr);
    }

}