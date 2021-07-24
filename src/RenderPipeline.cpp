//
// Created by Javier on 7/23/2021.
//

#include <string>
#include "RenderPipeline.h"

Epsilon::RenderPipeline::RenderPipeline(const std::string &pipelineName, std::vector<ShaderModule *> shaderModules)
: pipelineName_(pipelineName), shaderModules_(std::move(shaderModules)) {}
