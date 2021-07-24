//
// Created by Javier on 6/10/2021.
//

#include <stdexcept>
#include "ImGuiContext.h"
#include "../Vulkan/VulkanRenderPass.h"
#include "../Vulkan/VulkanSwapChain.h"
#include "../Vulkan/VulkanInstance.h"
#include "../ContextWindow.h"
#include "../Vulkan/VulkanContextWindow.h"
#include "../RenderSystem.h"
#include "../Vulkan/VulkanRenderSystem.h"

namespace Epsilon
{
    bool initialized_ = false;

    void ImGuiEnvironment::Initialize()
    {
      if (initialized_)
        return;

      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();
      initialized_ = true;

    }


    void ImGuiEnvironment::Shutdown()
    {
      //check for some fucked setup
      if (!initialized_)
        throw std::runtime_error("ImGui Error: Unable to shutdown properly!!!");

      //delete context if necessary
      if (context)
        UnlinkSystem();

      //call the imgui shutdown
      ImGui::DestroyContext();
    }


    void ImGuiEnvironment::CreateForVulkan(GLFWwindow *window, Vulkan::Device &device, Vulkan::Instance &instance,
                                           Vulkan::CommandPool &pool, Vulkan::SwapChain &swapChain)
    {

      ImGui_ImplVulkan_InitInfo info
          {instance.GetInstanceHandle(),
           device.GetPhysicalHandle(),
           device.GetLogicalHandle(),
           device.GetGraphicsIndices(),
           device.GetGraphicsQueue(),
           nullptr,
           swapChain.GetDescriptorPool().GetHandle(),
           0,
           swapChain.GetMinImageCount(),
           swapChain.GetImageCount(),
           VK_SAMPLE_COUNT_1_BIT,
           nullptr,
           nullptr,
          };

      //setup glfw handle for vulkan
      ImGui_ImplGlfw_InitForVulkan(window, true);

      //setup vulkan handle
      ImGui_ImplVulkan_Init(&info, swapChain.GetRenderPass());

      //initialize vulkan fonts
      Vulkan::CommandBuffer buffer = pool.CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

      //start recording
      buffer.BeginRecording();

      ImGui_ImplVulkan_CreateFontsTexture(buffer.GetHandle());

      buffer.EndRecording();

      buffer.SubmitCommands();

      //Set New Frame function
      OnStartFrame =
          []()
          {
              //Setup glfw for the new frame
              ImGui_ImplGlfw_NewFrame();
              //Setup Vulkan for the new frame
              ImGui_ImplVulkan_NewFrame();
          };

      //set render function
      OnDraw =
          [](ImDrawData *data)
          {
              //get the vulkan context from the window
              Vulkan::ContextWindow *VulkanWindow = dynamic_cast<Vulkan::ContextWindow *> (context->GetWindow(0));

              //draw all imgui data to the vulkan command buffer
              ImGui_ImplVulkan_RenderDrawData(data,
                                              VulkanWindow->GetVulkanSwapChain().GetCurrentCommandBuffer().GetHandle());
          };
      //set shutdown function
      OnShutdown =
          []()
          {
              //destroy all the fonts
              ImGui_ImplVulkan_DestroyFontUploadObjects();

              //destroy vulkan context
              ImGui_ImplVulkan_Shutdown();
          };


    }

    void ImGuiEnvironment::CreateForOpenGL(GLFWwindow *window)
    {
      //initialize for GLFW
      ImGui_ImplGlfw_InitForOpenGL(window, true);

      //Connect imgui with the glfw context
      ImGui_ImplOpenGL3_Init("#version 460");

      //set the new frame function
      OnStartFrame =
          []()
          {

              //start a new frame for opengl
              ImGui_ImplOpenGL3_NewFrame();
              //start a new frame for glfw
              ImGui_ImplGlfw_NewFrame();


          };

      //set the on draw function
      OnDraw =
          [](ImDrawData *data)
          {
              //start drawing data for opengl
              ImGui_ImplOpenGL3_RenderDrawData(data);
          };

      //set the shutdown function
      OnShutdown =
          []()
          {
              //Destroy any GL context with imgui
              ImGui_ImplOpenGL3_Shutdown();
          };
    }


    void ImGuiEnvironment::LinkSystem(Epsilon::RenderSystem *system, unsigned windowIndex)
    {
      //check if ImGui is even properly initialized
      if (!initialized_)
        throw std::runtime_error("Unable to intitalize ImGui vulkan context: ImGui context not initialized!!!");

      //dont do anything if the window is null or invalid
      if (!system)
        throw std::runtime_error("ImGui Link Context Error: Window cannot be null!");

      //if a context already exists, unlink it
      if (context)
        UnlinkSystem();

      //check the type of window
      switch (system->GetSpecificationType())
      {
        case SpecificationType::Vulkan:
        {
          //create handle for vulkan
          Vulkan::RenderSystem *VulkanSystem = dynamic_cast<Vulkan::RenderSystem*>(system);
          Vulkan::ContextWindow *VulkanWindow = dynamic_cast<Vulkan::ContextWindow *>(system->GetWindow(windowIndex));

          //start initializing for vulkan
          CreateForVulkan(VulkanWindow->GetWindow(),
                          VulkanSystem->GetVulkanDevice(),
                          VulkanSystem->GetVulkanInstance(),
                          VulkanSystem->GetCommandPool(),
                          VulkanWindow->GetVulkanSwapChain());
        }
          break;
        case SpecificationType::OpenGL:
          CreateForOpenGL(system->GetWindow(windowIndex)->GetWindow());
          break;
        default:
          break;
      }
      context = system;
      windowIndex_ = windowIndex;
    }

    void ImGuiEnvironment::UnlinkSystem()
    {
      //show error if unlinking is unnecessary
      if (!context)
        throw std::runtime_error(
            "ImGui Unlink Context Error: Failed to Unlink! Did you set the window context properly?");

      //call the proper shutdown function
      OnShutdown();

      //clear context flag
      context = nullptr;
    }

    void ImGuiEnvironment::StartFrame()
    {
      //start context specific frame init
      OnStartFrame();

      //start new frame for imgui
      ImGui::NewFrame();
    }


    bool ImGuiEnvironment::CanRender()
    {
      //context is set and imgui is properly initialized for rendering
      return context && initialized_;
    }

    void ImGuiEnvironment::RenderToContext()
    {
      //if everything is properly setup
      if (CanRender())
      {
        //render all imgui calls
        ImGui::Render();
        //get all the render calls
        ImDrawData *drawData = ImGui::GetDrawData();

        //draw all data
        OnDraw(drawData);
      }
    }


}