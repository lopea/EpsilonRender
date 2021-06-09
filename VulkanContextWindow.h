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
#include "Shader.h"
#include <optional>


namespace Epsilon::Vulkan
{
    class ContextWindow : public Epsilon::ContextWindow
    {
    public:
        ContextWindow(unsigned width, unsigned height);

        void Render(Shader);
        SpecificationType GetName() override;
        void EndFrame() override;
        void StartFrame() override;
        void OnResize(unsigned int width, unsigned int height) override;
    private:
        Instance instance_;
        Surface surface_;
        Device device_;
        SwapChain swapChain_;

        //! create the instance for this context renderer
        static GLFWwindow *CreateHandleWindow(unsigned width, unsigned height);

    };

}

#endif //EPSILONRENDERER_VULKANCONTEXTWINDOW_H
