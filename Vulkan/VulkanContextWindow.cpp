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
#include "../ImGui/ImGuiContext.h"

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

    SpecificationType ContextWindow::GetName()
    {
      return SpecificationType::Vulkan;
    }

    ContextWindow::ContextWindow(unsigned width, unsigned height)
    : Epsilon::ContextWindow(width, height, CreateHandleWindow(width, height)),
    instance_(),
    surface_(GetWindow(), instance_),
    device_(instance_, surface_),
      swapChain_(device_, surface_, GetWindow())
    {
      ImGuiHelper::CreateForVulkan(GetWindow(), device_, instance_, swapChain_);
    }

    void ContextWindow::EndFrame()
    {
      //stop the swapchain from receiving messages
      swapChain_.FinishFrame();

      //show the current frame in the swap chain and swap buffers
      swapChain_.Present();

      //check if the swapchain needs updating

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
      ImGui_ImplVulkan_NewFrame();
      ImGui_ImplGlfw_NewFrame();

      ImGui::NewFrame();
      ImGui::ShowDemoWindow();
      ImGui::Render();
      ImDrawData* data = ImGui::GetDrawData();
      //clear the selected frame buffer
      swapChain_.ClearFrame();

      ImGui_ImplVulkan_RenderDrawData(data,swapChain_.GetCurrentCommandBuffer().GetHandle());

    }

    void ContextWindow::OnResize(unsigned int width, unsigned int height)
    {
      (void)width;
      (void)height;

      //recreate the swapchain
      swapChain_.MarkForRefresh();
    }

}