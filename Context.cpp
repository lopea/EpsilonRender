//
// Created by Javier on 5/27/2021.
//

#include "Context.h"


#include <stdexcept>
#include <iostream>


namespace Epsilon
{
    Context::Context(unsigned width, unsigned height) : handle_(nullptr)
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
      handle_ = glfwCreateWindow(width, height, "Epsilon", nullptr, nullptr);

      uint32_t extensionCount = 0;

      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

      std::cout << "Extension Count: " << extensionCount << std::endl;


    }

    Context::~Context()
    {
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