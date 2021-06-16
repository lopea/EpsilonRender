//
// Created by Javier on 5/27/2021.
//

#include "App.h"
#include "Vulkan/VulkanContextWindow.h"
#include "ImGui/ImGuiContext.h"


#include <stdexcept>
#include <iostream>

namespace Epsilon
{
    App::App(unsigned width, unsigned height) : handle_(nullptr), renderer_(nullptr)
    {
      ImGuiEnvironment::Initialize();
      try
      {
        renderer_ = new Vulkan::ContextWindow(width,height);
        ImGuiEnvironment::LinkContext(renderer_);
      }catch (std::runtime_error& ex)
      {
        std::cerr << ex.what() << std::endl;
      }
    }

    App::~App()
    {

      ImGuiEnvironment::Shutdown();
      //delete the renderer specification
      delete renderer_;

      //terminate glfw
      glfwTerminate();
    }

    void App::Run()
    {
      //run the program until the user wants it to close
      while(!renderer_->WillClose())
      {
        ImGuiEnvironment::StartFrame();
        ImGui::ShowDemoWindow();

        //clear the previous frame
        renderer_->StartFrame();
        //run any renderer specific actions
        ImGuiEnvironment::RenderToContext();
        Update();
        renderer_->EndFrame();

        glfwPollEvents();
      }

    }

    void App::Update()
    {
        objManager_.Update();
    }
}