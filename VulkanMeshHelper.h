//
// Created by Javier on 6/6/2021.
//

#ifndef EPSILONRENDERER_VULKANMESHHELPER_H
#define EPSILONRENDERER_VULKANMESHHELPER_H
#include <vulkan/vulkan.h>
#include "Mesh.h"
#include <array>

namespace Epsilon::Vulkan
{

    /*!
     * Gives a brief explaination on the vertex struct in a vulkan-readable format
     * @return the description of the vertex struct
     */
    VkVertexInputBindingDescription GetVertexBindingDescription();

    /*!
     * Get the Description of a vertex that is more vulkan friendly for creating attribute inputs for the shader.
     * @return a list of the descriptions of all the variables inside a vertex
     */
    std::array<VkVertexInputAttributeDescription, 3> GetVertexDescription();
}
#endif //EPSILONRENDERER_VULKANMESHHELPER_H
