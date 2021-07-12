//
// Created by Lopea on 6/7/2021.
//

#include <stdexcept>
#include "Renderer.h"
#include "Vulkan/VulkanContextWindow.h"
#include "OpenGL/OpenGLContextWindow.h"

namespace Epsilon
{
    Renderer::Renderer(Epsilon::SpecificationType type)
        : type_(type)
    {}

    Renderer::~Renderer()
    {
      //check for any windows that need to be removed
      if (!IsEmpty())
      {
        //shutdown any window
        for (auto window : windows_)
          delete window;
      }
    }

    ContextWindow *Renderer::InitializeNewWindow(unsigned int width, unsigned int height)
    {
      //add a new window based on the type of the renderer
      switch (type_)
      {

        //create for vulkan
        case SpecificationType::Vulkan:
          windows_.push_back(new Vulkan::ContextWindow(width, height));
          break;

        //create for OpenGL
        case SpecificationType::OpenGL:
          windows_.push_back(new OpenGL::ContextWindow(width, height));
          break;

        //any invalid type will go here
        default:
          throw std::runtime_error("RENDERER CRITICAL ERROR: Unable to create window with invalid specification!!!");
      }

      //return the current object
      return windows_.back();
    }
}