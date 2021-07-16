//
// Created by lopea on 7/14/21.
//

#ifndef EPSILONRENDERER_OPENGLRENDERSYSTEM_H
#define EPSILONRENDERER_OPENGLRENDERSYSTEM_H


#include <unordered_map>
#include "../RenderSystem.h"
#include "glShader.h"
#include "glMesh.h"

namespace Epsilon::OpenGL
{
    class RenderSystem : public Epsilon::RenderSystem
    {
    public:
        RenderSystem (): Epsilon::RenderSystem(SpecificationType::OpenGL)
        {
        };
        void Render(glShader* shader, OpenGL::Mesh& mesh);
        Epsilon::ContextWindow * PushBackNewWindow(unsigned int width, unsigned int height) override;
        Shader GetShader(const std::string &shaderName) override{ return Shader();}
    private:
        std::unordered_map<std::string, glShader> shaders_;
    };
}

#endif //EPSILONRENDERER_OPENGLRENDERSYSTEM_H
