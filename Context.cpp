//
// Created by Javier on 5/27/2021.
//

#include "Context.h"
#include "VulkanContextRenderer.h"


#include <stdexcept>
#include <iostream>


namespace Epsilon
{
    Context::Context(unsigned width, unsigned height) : handle_(nullptr), renderer_(nullptr)
    {
      renderer_ = new VulkanContextRenderer();
      handle_ = renderer_->CreateWindow(width, height);
    }

    Context::~Context()
    {
      //delete the renderer specification
      delete renderer_;

      //destroy the glfw window
      glfwDestroyWindow(handle_);

      //terminate glfw
      glfwTerminate();
    }

    void Context::Run()
    {
      //run the program until the user wants it to close
      while(!glfwWindowShouldClose(handle_))
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