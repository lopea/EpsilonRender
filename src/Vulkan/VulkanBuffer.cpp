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
    Buffer::Buffer(Device &device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memFlags,
                   CommandPool &pool)
    : device_(device), commandPool_(pool), handle_(nullptr), memoryHandle_(nullptr), size_(size), usage_(usage), memFlags_(memFlags)
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
      //create a command buffer for the copying process
      CommandBuffer copyCommand = commandPool_.CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

      //start the recording process
      copyCommand.BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

      //create a info struct for coping buffers
      VkBufferCopy copyInfo {};
#ifdef _WIN32
      copyInfo.size = min(size_, other.size_);
#else
      copyInfo.size = std::min(size_, other.size_);
#endif
      //start copying the buffer
      vkCmdCopyBuffer(copyCommand.GetHandle(), other.handle_, handle_, 1, &copyInfo);

      //stop recording
      copyCommand.EndRecording();


      //submit the command for execution
      VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};

      submitInfo.commandBufferCount = 1;

      submitInfo.pCommandBuffers = copyCommand.GetRawHandle();

      //start copy
      vkQueueSubmit(device_.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
      vkQueueWaitIdle(device_.GetGraphicsQueue());

      //remove any memory associated with the command buffer
      copyCommand.Free();
    }
}