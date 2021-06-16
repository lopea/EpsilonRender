//
// Created by Javier on 6/4/2021.
//

#ifndef EPSILONRENDERER_VULKANINSTANCE_H
#define EPSILONRENDERER_VULKANINSTANCE_H


#include <vulkan/vulkan.h>
#include <vector>

namespace Epsilon::Vulkan
{
    //! Stores all necessary data for device specific handles in vulkan
    class Instance
    {
    public:
        Instance();
        ~Instance();
        [[nodiscard]] VkInstance GetInstanceHandle() const {return vkInstance_;}

    private:

        //! Establish a connection between vulkan and the application
        void InitInstance();

        //! enable any debug options for the vulkan
        void EnableValidationLayers();

        //! Gets all the vulkan extensions that are required by glfw and this program.
        [[nodiscard]] std::vector<const char *> GetRequiredExtensions() const;

        //! represents connection between vulkan and this program
        VkInstance vkInstance_;

        //! represents the messenger that will send error messages
        VkDebugUtilsMessengerEXT vkMessenger_;

        bool LayerValidationCheck();
    };
}

#endif //EPSILONRENDERER_VULKANINSTANCE_H
