//
// Created by Javier on 5/27/2021.
//

#include "Context.h"
#include "VulkanContextWindow.h"


#include <stdexcept>
#include <iostream>

namespace Epsilon
{
    Context::Context(unsigned width, unsigned height) : handle_(nullptr), renderer_(nullptr)
    {
      try
      {
        renderer_ = new Vulkan::ContextWindow(width,height);
      }catch (std::runtime_error& ex)
      {
        std::cerr << ex.what() << std::endl;
      }
    }

    Context::~Context()
    {
      //delete the renderer specification
      delete renderer_;
      //terminate glfw
      glfwTerminate();
    }

    void Context::Run()
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

    void Context::Update()
    {

    }
}