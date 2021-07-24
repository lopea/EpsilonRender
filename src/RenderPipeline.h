//
// Created by Javier on 7/23/2021.
//

#ifndef EPSILONRENDERER_RENDERPIPELINE_H
#define EPSILONRENDERER_RENDERPIPELINE_H

#include <vector>

namespace Epsilon
{
    class ShaderModule;
    class RenderPipeline
    {

    public:
        explicit RenderPipeline(const std::string &pipelineName, std::vector<ShaderModule *> shaderModules);

        [[nodiscard]] std::string GetName() const { return pipelineName_;}

        [[nodiscard]] std::vector<ShaderModule*> GetShaderModules() const{return shaderModules_;}
    private:
        std::string pipelineName_;
        std::vector<ShaderModule*> shaderModules_;
    };
}

#endif //EPSILONRENDERER_RENDERPIPELINE_H
