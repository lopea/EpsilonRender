//
// Created by lopea on 7/14/21.
//

#ifndef EPSILONRENDERER_OPENGLRENDERSYSTEM_H
#define EPSILONRENDERER_OPENGLRENDERSYSTEM_H


#include <unordered_map>
#include "../RenderSystem.h"
#include "glShader.h"

namespace Epsilon::OpenGL
{
    class RenderSystem : public Epsilon::RenderSystem
    {
    public:
        RenderSystem(): Epsilon::RenderSystem(SpecificationType::OpenGL){};
        ContextWindow * PushBackNewWindow(unsigned int width, unsigned int height) override;
        Shader GetShader(const std::string &shaderName) override{ return nullptr;}
    private:
        std::unordered_map<std::string, Shader> shaders;
    };
}

#endif //EPSILONRENDERER_OPENGLRENDERSYSTEM_H
