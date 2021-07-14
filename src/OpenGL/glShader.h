//
// Created by lopea on 7/13/21.
//

#ifndef EPSILONRENDERER_GLSHADER_H
#define EPSILONRENDERER_GLSHADER_H

#include "../Shader.h"
#include <glad/glad.h>

namespace Epsilon
{
    class glShader : public Shader_
    {
    public:
        glShader();
    private:
        GLuint program_;
    };
}

#endif //EPSILONRENDERER_GLSHADER_H
