//
// Created by Javier on 5/28/2021.
//

#include "ContextWindow.h"

namespace Epsilon
{

    void glfwOnResize(GLFWwindow* window, int width, int height)
    {
      //get the context
      ContextWindow* context = static_cast<ContextWindow*>(glfwGetWindowUserPointer(window));

      //if the context exists,
      if(context)
      {
        //call the resize callback on the context window
        context->OnResize(width,height);
        context->width_ = width;
        context->height_ = height;


      }
    }
    void ContextWindow::SetWindowHandle(GLFWwindow *newWindow)
    {
      //set the handle for the window
      handle_ = newWindow;

      //if the window is valid
      if (handle_)
      {
        //add all the states to the window
        PopulateGLFWHandle();
      }

    }

    void ContextWindow::PopulateGLFWHandle()
    {
      //let glfw have access to this object when it comes to callbacks
      glfwSetWindowUserPointer(handle_, this);

      //set the resize callback
      glfwSetWindowSizeCallback(handle_, glfwOnResize);
    }
}
