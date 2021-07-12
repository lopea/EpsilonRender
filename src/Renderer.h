//
// Created by Javier on 6/7/2021.
//

#ifndef EPSILONRENDERER_RENDERER_H
#define EPSILONRENDERER_RENDERER_H

#include "Shader.h"
#include "Mesh.h"

namespace Epsilon
{
    class Renderer
    {
    public:
        explicit Renderer(SpecificationType type);

        ~Renderer();

        SpecificationType GetSpecificationType() const { return type_;}

        ContextWindow* InitializeNewWindow(unsigned width, unsigned height);

        bool IsEmpty() const { return windows_.empty();}

    private:
        std::vector<ContextWindow*> windows_;
        SpecificationType type_;

    };
}

#endif //EPSILONRENDERER_RENDERER_H
