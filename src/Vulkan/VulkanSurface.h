//
// Created by Javier on 6/4/2021.
//

#ifndef EPSILONRENDERER_VULKANSURFACE_H
#define EPSILONRENDERER_VULKANSURFACE_H

#include <vulkan/vulkan.h>
#include "VulkanInstance.h"

struct GLFWwindow;
namespace Epsilon::Vulkan
{
    class Surface
    {
    public:
        Surface(GLFWwindow *windowHandle_, Instance &instance);
        [[nodiscard]] VkSurfaceKHR GetSurfaceHandle() const { return vkSurface_;}
        ~Surface();

        Surface(const Surface&) = delete;
        Surface& operator=(const Surface&) = delete;


    private:
        VkSurfaceKHR vkSurface_;
        Instance& instance_;
        GLFWwindow * window_;
        bool dummy_;
    };
}



#endif //EPSILONRENDERER_VULKANSURFACE_H
