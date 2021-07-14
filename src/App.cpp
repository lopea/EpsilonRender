//
// Created by Javier on 5/27/2021.
//

#include "App.h"
#include "Vulkan/VulkanContextWindow.h"
#include "ImGui/ImGuiContext.h"
#include "OpenGL/OpenGLContextWindow.h"
#include "Vulkan/VulkanRenderSystem.h"


#include <stdexcept>
#include <iostream>

namespace Epsilon
{
    SpecificationType switchTo_;
    App::App(unsigned width, unsigned height) : handle_(nullptr), renderer_(nullptr)
    {
      ImGuiEnvironment::Initialize();
      try
      {
        //create a new system for rendering vulkan
        renderer_ = new Vulkan::RenderSystem();

        renderer_->PushBackNewWindow(width,height);

        ImGuiEnvironment::LinkSystem(renderer_);


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
      ContextWindow* window = renderer_->GetWindow(0);
      //run the program until the user wants it to close
      while(!window->WillClose())
      {
        ImGuiEnvironment::StartFrame();
        ImGui::ShowDemoWindow();

        //clear the previous frame
        window->StartFrame();
        //run any renderer specific actions
        ImGuiEnvironment::RenderToContext();
        Update();
        window->EndFrame();

        glfwPollEvents();
      }

    }

    void App::Update()
    {
        objManager_.Update();
    }
}