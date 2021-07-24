//
// Created by lopea on 7/14/21.
//

#ifndef EPSILONRENDERER_OPENGLRENDERSYSTEM_H
#define EPSILONRENDERER_OPENGLRENDERSYSTEM_H


#include <unordered_map>
#include <stack>
#include <vector>
#include "../RenderSystem.h"
#include "glMesh.h"
#include "OpenGLRenderPipeline.h"
#include "../SpecificationData.hpp"
#include "../Material.h"

namespace Epsilon::OpenGL
{
    class RenderSystem : public Epsilon::RenderSystem
    {
    public:
        RenderSystem (): Epsilon::RenderSystem(SpecificationType::OpenGL)
        {
        };
        void Render(Epsilon::Material &shader, OpenGL::Mesh& mesh);
        Epsilon::ContextWindow * PushBackNewWindow(unsigned int width, unsigned int height) override;
        Buffer* UpdateBuffer(unsigned location, const std::vector<char>& data);
    private:
        static inline SpecificationData<OpenGL::RenderPipeline, OpenGL::Mesh> openGLData_;
        static inline std::stack<Buffer> bufferPool_;
    };
}

#endif //EPSILONRENDERER_OPENGLRENDERSYSTEM_H
