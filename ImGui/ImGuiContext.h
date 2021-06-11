//
// Created by Javier on 6/10/2021.
//

#ifndef EPSILONRENDERER_IMGUICONTEXT_H
#define EPSILONRENDERER_IMGUICONTEXT_H

struct GLFWwindow;

#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include "../Vulkan/VulkanCommandPool.h"
#include "../Vulkan/VulkanSwapChain.h"

namespace Epsilon
{
    namespace Vulkan
    {
        class RenderPass;
    }
    class ImGuiHelper
    {
    public:
        static void Initialize();

        static void Shutdown();

        static void CreateForVulkan(GLFWwindow *window, Vulkan::Device &device, Vulkan::Instance &instance,
                                    Vulkan::SwapChain &swapchain);

    };

}
#endif //EPSILONRENDERER_IMGUICONTEXT_H
