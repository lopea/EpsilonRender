//
// Created by Javier on 6/10/2021.
//

#include <stdexcept>
#include "ImGuiContext.h"
#include "../Vulkan/VulkanRenderPass.h"
#include "../Vulkan/VulkanSwapChain.h"
#include "../Vulkan/VulkanInstance.h"

namespace Epsilon
{
    void (*shutdown)() = nullptr;

    bool initialized_ = false;
    void ImGuiHelper::Initialize()
    {
      if(initialized_)
        return;

      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();
      initialized_  = true;

    }


    void ImGuiHelper::Shutdown()
    {
      //check for some fucked setup
      if(!initialized_)
        throw std::runtime_error("ImGui Error: Unable to shutdown properly!!!");

      //call proper shutdown function
      if(shutdown) shutdown();

      //call the glfw shutdown
      ImGui_ImplGlfw_Shutdown();

      //call the imgui shutdown
      ImGui::DestroyContext();
    }

    void ImGuiHelper::CreateForVulkan(GLFWwindow *window, Vulkan::Device &device, Vulkan::Instance &instance,
                                      Vulkan::SwapChain &swapchain)
    {
      if (!initialized_)
        throw std::runtime_error("Unable to intitalize ImGui vulkan context: ImGui context not initialized!!!");

      ImGui_ImplVulkan_InitInfo info
      {instance.GetInstanceHandle(),
       device.GetPhysicalHandle(),
       device.GetLogicalHandle(),
       device.GetGraphicsIndices(),
       device.GetGraphicsQueue(),
       nullptr,
       swapchain.GetDescriptorPool().GetHandle(),
       0,
       swapchain.GetMinImageCount(),
       swapchain.GetImageCount(),
       VK_SAMPLE_COUNT_1_BIT,
       nullptr,
       nullptr,
       };

      //setup glfw handle for vulkan
      ImGui_ImplGlfw_InitForVulkan(window, true);

      //setup vulkan handle
      ImGui_ImplVulkan_Init(&info, swapchain.GetRenderPass());

      //initialize vulkan fonts
      Vulkan::CommandBuffer buffer = swapchain.GetCommnandPool().CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

      //start recording
      buffer.BeginRecording();

      ImGui_ImplVulkan_CreateFontsTexture(buffer.GetHandle());

      buffer.EndRecording();

      buffer.SubmitCommands();

      //set shutdown function
      shutdown =
          []()
          {
            //destroy all the fonts
            ImGui_ImplVulkan_DestroyFontUploadObjects();

            //destroy vulkan context
            ImGui_ImplVulkan_Shutdown();
          };

    }
}