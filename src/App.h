//
// Created by Javier on 5/27/2021.
//

#ifndef EPSILONRENDERER_APP_H
#define EPSILONRENDERER_APP_H


#include <GLFW/glfw3.h>

#include "ContextWindow.h"
#include "RenderSystem.h"

namespace Epsilon
{
    //! Handles any high-level functions including any glfw functionality
    class App
    {
    public:
        App(unsigned width, unsigned height);

        ~App();

        void Run();

    private:
        void Update();

        GLFWwindow *handle_;
        RenderSystem* renderer_;

    };
}

#endif //EPSILONRENDERER_APP_H
