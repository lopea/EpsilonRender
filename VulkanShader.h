//
// Created by Javier on 5/31/2021.
//

#ifndef EPSILONRENDERER_VULKANSHADER_H
#define EPSILONRENDERER_VULKANSHADER_H

#include <string>
#include "Shader.h"
#include "vulkan/vulkan.h"
#include "VulkanContextWindow.h"

class VulkanShader : public Shader_
{
public:
    std::string GetShaderType() override;
    VulkanShader(const std::string &vertFileLocation, const std::string &fragFileLocation,
                 Epsilon::VulkanContextWindow &context);
    VkPipeline GetPipeline() const {return pipeline_;}
    ~VulkanShader();
private:


    VkPipelineLayout layout_;
    VkPipeline pipeline_;
    VkDevice logicalDevice_;

    std::string name;
};


#endif //EPSILONRENDERER_VULKANSHADER_H
