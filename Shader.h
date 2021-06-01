//
// Created by Javier on 5/31/2021.
//

#ifndef EPSILONRENDERER_SHADER_H
#define EPSILONRENDERER_SHADER_H

#include <string>
class Shader_{public: virtual std::string GetShaderType() = 0;};
using Shader = Shader_*;

#endif //EPSILONRENDERER_SHADER_H
