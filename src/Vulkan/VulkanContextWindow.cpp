//
// Created by Javier on 5/28/2021.
//

#include "VulkanContextWindow.h"

#include <stdexcept>
#include <iostream>
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
      GLFWwindow *handle = glfwCreateWindow(width, height, "Epsilon *Vulkan API*", nullptr, nullptr);

      glfwMakeContextCurrent(handle);

      return handle;
    }

    SpecificationType ContextWindow::GetName()
    {
      return SpecificationType::Vulkan;
    }

    ContextWindow::ContextWindow(unsigned width, unsigned height, Instance& instance)
    : Epsilon::ContextWindow(width, height, CreateHandleWindow(width, height)),
      surface_(GetWindow(), instance),
      device_(instance, surface_),
      swapChain_(device_, surface_, GetWindow())
    {

    }

    void ContextWindow::EndFrame()
    {
      //stop the swapchain from receiving messages
      swapChain_.FinishFrame();

      //show the current frame in the swap chain and swap buffers
      swapChain_.Present();

    }

    void ContextWindow::Render(Shader shader)
    {
      if(shader->GetShaderType() == SpecificationType::Vulkan)
      {
        vkShader* vShader = dynamic_cast<vkShader*>(shader);

        swapChain_.RenderShader(vShader);
      }
      else
      {
        std::cerr << "Unknown Shader Type!" <<std::endl;
      }
    }

    void ContextWindow::StartFrame()
    {
      //clear the selected frame buffer
      swapChain_.ClearFrame();

    }

    void ContextWindow::OnResize(unsigned int width, unsigned int height)
    {
      (void)width;
      (void)height;

      //recreate the swapchain
      swapChain_.MarkForRefresh();
    }

}