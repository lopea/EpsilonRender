//
// Created by Javier on 6/7/2021.
//

#include <cstring>
#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "VulkanException.h"
#include "VulkanSwapChain.h"

namespace Epsilon::Vulkan
{
    Buffer::Buffer(Device& device, SwapChain& chain, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memFlags)
    : device_(device), chain_(chain), handle_(nullptr), memoryHandle_(nullptr), size_(size), usage_(usage), memFlags_(memFlags)
    {
      //start setting up vertex initialization
      VkBufferCreateInfo info {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};

      //set the size of the buffer
      info.size = size_;

      //set how the buffer is getting used
      info.usage = usage;

      //set how the buffer is getting accessed
      info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


      if(vkCreateBuffer(device_.GetLogicalHandle(), &info, nullptr,&handle_) != VK_SUCCESS)
        throw InitializationException("Mesh Error: Unable to create vertex buffer!");

      //create the memory requirements for the vertex buffer so that we can add data
      VkMemoryRequirements requirements{};

      //get the requirements
      vkGetBufferMemoryRequirements(device_.GetLogicalHandle(), handle_, &requirements);

      //create info for allocating memory
      VkMemoryAllocateInfo memInfo {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
      memInfo.allocationSize = requirements.size;
      memInfo.memoryTypeIndex = FindMemoryType(
          requirements.memoryTypeBits,memFlags_);

      //allocate memory for the vertex
      if(vkAllocateMemory(device_.GetLogicalHandle(), &memInfo, nullptr, &memoryHandle_) != VK_SUCCESS)
        throw InitializationException("Mesh Error: Failed to allocate memory!!");

      //bind the allocation with the buffer
      vkBindBufferMemory(device_.GetLogicalHandle(),handle_, memoryHandle_, 0);

    }

    void Buffer::SetData(void *data, VkDeviceSize size, uint64_t offset)
    {
      //fill the vertex buffer with data
      void* mapPtr;

      //avoid any overflow
      if(size + offset > size_)
        size = size_ - offset;

      //get the reference to the buffer's data
      vkMapMemory(device_.GetLogicalHandle(), memoryHandle_, offset, size_, 0, &mapPtr);

      //set the data on the buffer
      memcpy(mapPtr, data, size);

      //return the buffer to vulkan
      vkUnmapMemory(device_.GetLogicalHandle(), memoryHandle_);
    }


    uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

    Buffer::~Buffer()
    {
      //destroy the buffer
      vkDestroyBuffer(device_.GetLogicalHandle(), handle_, nullptr);

      //destroy the memory associated with the buffer
      vkFreeMemory(device_.GetLogicalHandle(), memoryHandle_, nullptr);
    }

    void Buffer::CopyBuffer(const Buffer &other)
    {
      (void) other;
      //create a command buffer for this operation
      VkCommandBufferAllocateInfo cbInfo {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
      cbInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      cbInfo.commandPool = chain_.GetCommnandPool();
      cbInfo.commandBufferCount = 1;

      //allocate the command buffer
      VkCommandBuffer commandBuffer;
      vkAllocateCommandBuffers(device_.GetLogicalHandle(), &cbInfo, &commandBuffer);

      //start writing to the buffer
      VkCommandBufferBeginInfo beginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      //start writing to the command buffer
      vkBeginCommandBuffer(commandBuffer, &beginInfo);

      //create a info struct for coping buffers
      VkBufferCopy copyInfo {};
#ifdef _MSC_VER
      copyInfo.size = min(size_, other.size_);
#else
      copyInfo.size = std::min(size_, other.size_);
#endif

      //start copying the buffer
      vkCmdCopyBuffer(commandBuffer, other.handle_, handle_, 1, &copyInfo);

      //stop writing to the command buffer
      vkEndCommandBuffer(commandBuffer);

      //submit the command for execution
      VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &commandBuffer;

      //start copy
      vkQueueSubmit(device_.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
      vkQueueWaitIdle(device_.GetGraphicsQueue());

      //free the command buffer
      vkFreeCommandBuffers(device_.GetLogicalHandle(), chain_.GetCommnandPool(), 1, &commandBuffer);
    }
}