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
      renderer_ = new VulkanContextWindow(width,height);
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
        //run any renderer specific actions
        Update();

        //update glfw and any of its events
        glfwPollEvents();
      }
    }

    void Context::Update()
    {

    }
}