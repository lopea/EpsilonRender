//
// Created by Javier on 5/28/2021.
//

#ifndef EPSILONRENDERER_VULKANCONTEXTWINDOW_H
#define EPSILONRENDERER_VULKANCONTEXTWINDOW_H

#include <vector>
#include "ContextWindow.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include <optional>


namespace Epsilon::Vulkan
{
    class VulkanShader;

    class ContextWindow : public Epsilon::ContextWindow
    {
    public:
        ContextWindow(unsigned width, unsigned height);

        std::string GetName() override;
        void DrawFrame() override;
    private:
        Instance instance_;
        Surface surface_;
        Device device_;
        SwapChain swapChain_;

        VulkanShader* shader;

        //! create the instance for this context renderer
        static GLFWwindow *CreateHandleWindow(unsigned width, unsigned height);

    };

}

#endif //EPSILONRENDERER_VULKANCONTEXTWINDOW_H
