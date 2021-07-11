//
// Created by Javier on 6/10/2021.
//

#ifndef EPSILONRENDERER_IMGUICONTEXT_H
#define EPSILONRENDERER_IMGUICONTEXT_H

struct GLFWwindow;

#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "../Vulkan/VulkanCommandPool.h"
#include "../Vulkan/VulkanSwapChain.h"
#include <functional>

namespace Epsilon
{
    class ContextWindow;
    class ImGuiEnvironment
    {
    public:
        static void Initialize();

        /*!
         * Set a window as the main window for Imgui. If a window is already linked, then the previous window is unlinked
         * and the new window will be set as the new context window
         * @param window The window to connect for ImGui related rendering systems
         */
        static void LinkContext(ContextWindow* window);
        static void UnlinkContext();


        static void StartFrame();
        static bool CanRender();
        static void RenderToContext();
        static void Shutdown();

    private:

        static inline ContextWindow* context = nullptr;

        static void CreateForVulkan(GLFWwindow *window, Vulkan::Device &device, Vulkan::Instance &instance,
                                    Vulkan::SwapChain &swapchain);
        static void CreateForOpenGL(GLFWwindow* window);

        static inline std::function<void()> OnShutdown, OnStartFrame;
        static inline std::function<void(ImDrawData*)> OnDraw;
    };

}
#endif //EPSILONRENDERER_IMGUICONTEXT_H
