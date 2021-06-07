//
// Created by Javier on 6/7/2021.
//

#include "VulkanMesh.h"
#include "VulkanInitializationException.h"

namespace Epsilon::Vulkan
{
    void vkMesh::CreateVertexBuffer()
    {
      //start setting up vertex initialization
      VkBufferCreateInfo info {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};

      //set the size of the buffer
      info.size = sizeof(Vertex) * vertices_.size();

      //set how the buffer is getting used
      info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

      //set how the buffer is getting accessed
      info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


      if(vkCreateBuffer(device_.GetLogicalHandle(), &info, nullptr,&vertexBuffer_) != VK_SUCCESS)
        throw InitializationException("Mesh Error: Unable to create vertex buffer!");

      //create the memory requirements for the vertex buffer so that we can add data
      VkMemoryRequirements requirements{};

      //get the requirements
      vkGetBufferMemoryRequirements(device_.GetLogicalHandle(), vertexBuffer_, &requirements);

      //create info for allocating memory
      VkMemoryAllocateInfo memInfo {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
      memInfo.allocationSize = requirements.size;
      memInfo.memoryTypeIndex = FindMemoryType(
          requirements.memoryTypeBits,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      //allocate memory for the vertex
      if(vkAllocateMemory(device_.GetLogicalHandle(), &memInfo, nullptr, &vertexBufferMemory_) != VK_SUCCESS)
        throw InitializationException("Mesh Error: Failed to allocate memory!!");

      //bind the allocation with the buffer
      vkBindBufferMemory(device_.GetLogicalHandle(),vertexBuffer_, vertexBufferMemory_, 0);

      //fill the vertex buffer with data
      void* data;
      vkMapMemory(device_.GetLogicalHandle(), vertexBufferMemory_, 0, info.size, 0, &data);
      memcpy(data, vertices_.data(), static_cast<uint32_t>(info.size));
      vkUnmapMemory(device_.GetLogicalHandle(), vertexBufferMemory_);
    }

    void vkMesh::CreateIndexBuffer()
    {

    }

    vkMesh::~vkMesh()
    {
      //destroy the vertex buffer
      vkDestroyBuffer(device_.GetLogicalHandle(), vertexBuffer_, nullptr);
      //destroy the memory associated with the buffer
      vkFreeMemory(device_.GetLogicalHandle(), vertexBufferMemory_, nullptr);
    }

    uint32_t vkMesh::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
      //get the memory properties
      VkPhysicalDeviceMemoryProperties memProperties;
      vkGetPhysicalDeviceMemoryProperties(device_.GetPhysicalHandle(), &memProperties);
      for(uint32_t i = 0; i < memProperties.memoryTypeCount; i ++)
      {
        if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
          return i;
      }
      throw InitializationException("Mesh Error: Cannot find a suitable memory type");
    }
}