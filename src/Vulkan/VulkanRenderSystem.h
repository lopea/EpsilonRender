//
// Created by lopea on 7/13/21.
//

#ifndef EPSILONRENDERER_VULKANRENDERSYSTEM_H
#define EPSILONRENDERER_VULKANRENDERSYSTEM_H


#include "../RenderSystem.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanSurface.h"

namespace Epsilon::Vulkan
{
    class RenderSystem : public Epsilon::RenderSystem
    {
    public:
        RenderSystem();

        ~RenderSystem() override;
        ContextWindow * PushBackNewWindow(unsigned int width, unsigned int height) override;

        [[nodiscard]] Instance& GetVulkanInstance()  {return instance_;}


        Shader GetShader(const std::string &shaderName) override;

    private:
        //! Stores the connection of the Vulkan API with our program
        Instance instance_;

    };
}

#endif //EPSILONRENDERER_VULKANRENDERSYSTEM_H
