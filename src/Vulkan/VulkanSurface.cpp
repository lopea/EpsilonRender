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
                                                                      instance_(instance),dummy_(false)
    {
      bool deleteWindow = false;
      if(!windowHandle_)
      {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        windowHandle_ = glfwCreateWindow(100, 100, "Delete Me!!!", nullptr, nullptr);
        dummy_ = true;
        glfwMakeContextCurrent(windowHandle_);

      }

      window_ = windowHandle_;

      //create window connection for vulkan
      if(glfwCreateWindowSurface(instance.GetInstanceHandle(), windowHandle_, nullptr, &vkSurface_) != VK_SUCCESS)
      {
        if(dummy_)
          glfwDestroyWindow(window_);
        throw InitializationException("Unable to create Surface!");
      }

    }

    Surface::~Surface()
    {
      //destroy the surface handle and all of its contents
      vkDestroySurfaceKHR(instance_.GetInstanceHandle(), vkSurface_, nullptr);
      if(dummy_)
      {
        glfwDestroyWindow(window_);
      }

    }

}