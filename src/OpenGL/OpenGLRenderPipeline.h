//
// Created by lopea on 7/13/21.
//

#ifndef EPSILONRENDERER_OPENGLRENDERPIPELINE_H
#define EPSILONRENDERER_OPENGLRENDERPIPELINE_H

#include"../ShaderModule.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "../Epsilon.h"

namespace Epsilon::OpenGL
{
    class RenderPipeline
    {
    public:
        explicit RenderPipeline(const std::vector<ShaderModule*>& modules);
        ~RenderPipeline();

        //avoid copying
        RenderPipeline(const RenderPipeline&) = delete;
        RenderPipeline& operator=(const RenderPipeline&) = delete;
        [[nodiscard]] const void* GetShaderHandle() const { return &program_;}
    private:
        GLuint program_;
    };
}

#endif //EPSILONRENDERER_OPENGLRENDERPIPELINE_H
