//
// Created by lopea on 7/13/21.
//

#include "VulkanRenderSystem.h"
#include "VulkanContextWindow.h"

namespace Epsilon::Vulkan
{
    RenderSystem::RenderSystem()
    : Epsilon::RenderSystem(SpecificationType::Vulkan)
    {
      instance_ = new Instance();
      Surface * surface = new Surface(nullptr,*instance_);
      device_ = new Device(*instance_, *surface);
      pool_ = new CommandPool(*device_);

      delete surface;

    }

    Epsilon::ContextWindow *RenderSystem::PushBackNewWindow(unsigned int width, unsigned int height)
    {
      auto window = new Vulkan::ContextWindow(width, height, *instance_, *device_, *pool_);

      windows_.push_back(window);

      return window;
    }


    RenderSystem::~RenderSystem()
    {
      ClearAllWindows();
      delete pool_;
      delete device_;
      delete instance_;
    }

}
