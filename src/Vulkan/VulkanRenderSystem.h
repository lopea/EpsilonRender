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

        [[nodiscard]] Instance& GetVulkanInstance()  {return *instance_;}
        Device& GetVulkanDevice() {return *device_;}
        CommandPool& GetCommandPool() {return *pool_;}


    private:
        //! Stores the connection of the Vulkan API with our program
        Instance* instance_;

        Device* device_;

        CommandPool* pool_;

    };
}

#endif //EPSILONRENDERER_VULKANRENDERSYSTEM_H
