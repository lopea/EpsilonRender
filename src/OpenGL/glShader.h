//
// Created by lopea on 7/13/21.
//

#ifndef EPSILONRENDERER_GLSHADER_H
#define EPSILONRENDERER_GLSHADER_H

#include"../Shader.h"
#include <glad/glad.h>
#include <vector>
#include "../Epsilon.h"

namespace Epsilon
{
    class glShader : public ShaderSpecificationBase
    {
    public:
        explicit glShader(const std::vector<char> &vertexData, const std::vector<char> &fragData);
        [[nodiscard]] SpecificationType GetShaderType() const override { return SpecificationType::OpenGL;}
        [[nodiscard]] const void* GetShaderHandle() const { return &program_;}

    private:
        GLuint program_;
    };
}

#endif //EPSILONRENDERER_GLSHADER_H
