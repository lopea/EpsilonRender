//
// Created by Javier on 5/31/2021.
//

#ifndef EPSILONRENDERER_VULKANSHADER_H
#define EPSILONRENDERER_VULKANSHADER_H

#include <string>
#include "../Shader.h"
#include "vulkan/vulkan.h"
#include "VulkanContextWindow.h"

namespace Epsilon::Vulkan
{
    class vkShader : public Shader_
    {
    public:
        [[nodiscard]] SpecificationType GetShaderType() const override;

        vkShader(const std::string &vertFileLocation, const std::string &fragFileLocation,
                 Vulkan::SwapChain& swapChain_);

        [[nodiscard]] VkPipeline GetPipeline() const
        { return pipeline_; }

        ~vkShader();

    private:


        VkPipelineLayout layout_;
        VkPipeline pipeline_;
        VkDevice logicalDevice_;

        std::string name;
    };
}

#endif //EPSILONRENDERER_VULKANSHADER_H
