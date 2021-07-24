//
// Created by lopea on 7/11/21.
//

#ifndef EPSILONRENDERER_OPENGLCONTEXTWINDOW_H
#define EPSILONRENDERER_OPENGLCONTEXTWINDOW_H

#include "../ContextWindow.h"

namespace Epsilon::OpenGL
{
    class ContextWindow : public Epsilon::ContextWindow
    {
    public:
        ContextWindow(int width, int height);

        void OnResize(unsigned int width, unsigned int height) override;
        void StartFrame() override;
        void EndFrame() override;



    };
}


#endif //EPSILONRENDERER_OPENGLCONTEXTWINDOW_H
