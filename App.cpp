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
      ImGuiHelper::Initialize();
      try
      {
        renderer_ = new Vulkan::ContextWindow(width,height);
      }catch (std::runtime_error& ex)
      {
        std::cerr << ex.what() << std::endl;
      }
    }

    App::~App()
    {
      //delete the renderer specification
      delete renderer_;

      ImGuiHelper::Shutdown();
      //terminate glfw
      glfwTerminate();
    }

    void App::Run()
    {
      //run the program until the user wants it to close
      while(!renderer_->WillClose())
      {
        //clear the previous frame
        renderer_->StartFrame();
        //run any renderer specific actions
        Update();
        renderer_->EndFrame();
        //update glfw and any of its events
        glfwPollEvents();
      }

    }

    void App::Update()
    {

    }
}