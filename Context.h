//
// Created by Javier on 5/27/2021.
//

#ifndef EPSILONRENDERER_CONTEXT_H
#define EPSILONRENDERER_CONTEXT_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "ContextRenderer.h"

namespace Epsilon
{
    //! Handles any high-level functions including any glfw functionality
    class Context
    {
    public:
        Context(unsigned width, unsigned height);

        ~Context();

        void Run();

    private:
        void Update();

        GLFWwindow *handle_;
        ContextRenderer* renderer_;
    };
}

#endif //EPSILONRENDERER_CONTEXT_H
