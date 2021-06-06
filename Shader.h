//
// Created by Javier on 5/31/2021.
//

#ifndef EPSILONRENDERER_SHADER_H
#define EPSILONRENDERER_SHADER_H

#include <string>

namespace Epsilon
{
    enum class SpecificationType;

    class Shader_
    {
    public:
        virtual SpecificationType GetShaderType() const = 0;
    };

    using Shader = Shader_ *;
}
#endif //EPSILONRENDERER_SHADER_H
