//
// Created by Javier on 6/10/2021.
//
#include "VulkanMeshHelper.h"

namespace Epsilon::Vulkan
{

    /*!
     * Gives a brief explaination on the vertex struct in a vulkan-readable format
     * @return the description of the vertex struct
     */
    VkVertexInputBindingDescription GetVertexBindingDescription()
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
    std::array<VkVertexInputAttributeDescription, 3> GetVertexDescription()
    {
      std::array<VkVertexInputAttributeDescription, 3> result{};

      //create the position description
      result[0].binding = 0;

      //set the location of the position variable
      result[0].location = 0;

      //set the format of the pos
      result[0].format = VK_FORMAT_R32G32B32_SFLOAT;

      //set the offset
      result[0].offset = offsetof(Vertex, pos);


      //create the uv description
      result[1].binding = 0;

      //set the location of the uv variable
      result[1].location = 1;

      //set the format of the uv
      result[1].format = VK_FORMAT_R32G32_SFLOAT;

      //set the offset
      result[1].offset = offsetof(Vertex, uv);


      //create the normal description
      result[2].binding = 0;

      //set the location of the normal variable
      result[2].location = 2;

      //set the format of the normal
      result[2].format = VK_FORMAT_R32G32B32_SFLOAT;

      //set the offset
      result[2].offset = offsetof(Vertex, normal);

      return result;
    }
}