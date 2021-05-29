//
// Created by Javier on 5/28/2021.
//

#ifndef EPSILONRENDERER_CONTEXTRENDERER_H
#define EPSILONRENDERER_CONTEXTRENDERER_H
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
struct GLFWwindow;

namespace Epsilon
{
//! class that is the blueprint for renderers based on the render specification (GL, Vulkan, DirectX)
    class ContextRenderer
    {
    public:
        //!creates a window based on the specification of the program
        virtual GLFWwindow *CreateWindow(unsigned width, unsigned height) = 0;

        //!Get the name of the render specification
        virtual std::string GetName() = 0;

        //! making sure destructors work for derived classes
        virtual ~ContextRenderer() = default;

    };
}

#endif //EPSILONRENDERER_CONTEXTRENDERER_H
