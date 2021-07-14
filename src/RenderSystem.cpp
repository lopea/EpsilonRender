//
// Created by Lopea on 6/7/2021.
//

#include <stdexcept>
#include "RenderSystem.h"
#include "Vulkan/VulkanContextWindow.h"
#include "OpenGL/OpenGLContextWindow.h"

namespace Epsilon
{
    RenderSystem::RenderSystem(Epsilon::SpecificationType type)
        : type_(type)
    {if(!glfwInit()) throw std::runtime_error("CRITICAL ERROR: GLFW CANNOT INITIALIZE!!!");}


    RenderSystem::~RenderSystem()
    {
      //one final check just in case
      ClearAllWindows();
    }

    void RenderSystem::ClearAllWindows()
    {//check for any windows that need to be removed
      if (!IsEmpty())
      {
        //shutdown any window
        for (auto window : windows_)
          delete window;
      }
      windows_.clear();
    }


}