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
    inline VkVertexInputBindingDescription GetVertexBindingDescription()
    {
      VkVertexInputBindingDescription result{};

      result.stride = sizeof (Vertex);
      result.binding = 0;
      result.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
      return result;
    }
    /*!
     * Get the Description of a vertex that is more vulkan friendly for creating attribute inputs for the shader.
     * @return a list of the descriptions of all the variables inside a vertex
     */
    inline std::array<VkVertexInputAttributeDescription, 2> GetVertexDescription()
    {
      std::array<VkVertexInputAttributeDescription, 2> result{};

      //create the position description
      result[0].binding = 0;

      //set the location of the position variable
      result[0].location = 0;

      //set the format of the pos
      result[0].format = VK_FORMAT_R32G32B32_SFLOAT;

      //set the offset
      result[0].offset = offsetof(Vertex, pos);


      //create the uv description
      result[0].binding = 0;

      //set the location of the uv variable
      result[0].location = 1;

      //set the format of the uv
      result[0].format = VK_FORMAT_R32G32_SFLOAT;

      //set the offset
      result[0].offset = offsetof(Vertex, uv);


      //create the normal description
      result[0].binding = 0;

      //set the location of the normal variable
      result[0].location = 1;

      //set the format of the normal
      result[0].format = VK_FORMAT_R32G32B32_SFLOAT;

      //set the offset
      result[0].offset = offsetof(Vertex, normal);

      return result;
    }
}
#endif //EPSILONRENDERER_VULKANMESHHELPER_H
