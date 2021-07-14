//
// Created by Javier on 5/28/2021.
//

#ifndef EPSILONRENDERER_VULKANCONTEXTWINDOW_H
#define EPSILONRENDERER_VULKANCONTEXTWINDOW_H

#include <vector>
#include "../ContextWindow.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "../Shader.h"
#include <optional>


namespace Epsilon::Vulkan
{
    class ContextWindow : public Epsilon::ContextWindow
    {
    public:
        ContextWindow(unsigned width, unsigned height, Instance& instance);


        void Render(Shader shader);
        SpecificationType GetName() override;
        void EndFrame() override;
        void StartFrame() override;
        void OnResize(unsigned int width, unsigned int height) override;


        //! @return Any screen related vulkan handles
        [[nodiscard]] Surface& GetVulkanSurface() { return surface_; }

        //! @return the information relating on what type of device is being used for rendering (usually GPU)
        [[nodiscard]] Device&  GetVulkanDevice() { return device_; }

        /*! @return render context for this window including the buffer layout and setup
         for presenting buffers to the screen.
         */
        [[nodiscard]] SwapChain& GetVulkanSwapChain() { return swapChain_; }
    private:
        Surface surface_;
        Device device_;
        SwapChain swapChain_;


        //! create the instance for this context renderer
        static GLFWwindow *CreateHandleWindow(unsigned width, unsigned height);

    };

}

#endif //EPSILONRENDERER_VULKANCONTEXTWINDOW_H
