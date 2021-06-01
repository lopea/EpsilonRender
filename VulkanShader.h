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
    ~VulkanShader();
private:

    void CreatePipeline(const Epsilon::VulkanContextWindow &context,
                        VkPipelineShaderStageCreateInfo *shaderStages);
    VkPipelineLayout layout_;
    VkPipeline pipeline_;
    VkDevice logicalDevice_;

    std::string name;
};


#endif //EPSILONRENDERER_VULKANSHADER_H
