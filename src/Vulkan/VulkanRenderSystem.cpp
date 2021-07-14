//
// Created by lopea on 7/13/21.
//

#include "VulkanRenderSystem.h"
#include "VulkanContextWindow.h"

namespace Epsilon::Vulkan
{
    RenderSystem::RenderSystem()
    : Epsilon::RenderSystem(SpecificationType::Vulkan),
      instance_()
    {}

    Epsilon::ContextWindow *RenderSystem::PushBackNewWindow(unsigned int width, unsigned int height)
    {
      auto* window = new Vulkan::ContextWindow(width, height, instance_);

      windows_.push_back(window);

      return window;
    }

    Shader RenderSystem::GetShader(const std::string &shaderName)
    {
      return nullptr;
    }

    RenderSystem::~RenderSystem()
    {
      ClearAllWindows();
    }

}
