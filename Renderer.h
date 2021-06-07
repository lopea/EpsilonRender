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
        Shader shader;
        Mesh mesh;
    };
}

#endif //EPSILONRENDERER_RENDERER_H
