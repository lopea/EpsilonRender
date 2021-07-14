//
// Created by Javier on 6/10/2021.
//

#include <vector>
#include "VulkanCommandPool.h"
#include "VulkanException.h"
#include "VulkanCommandBuffer.h"
namespace Epsilon::Vulkan
{
    CommandPool::~CommandPool()
    {
      vkDestroyCommandPool(device_.GetLogicalHandle(), handle_, nullptr);
    }

    CommandBuffer CommandPool::CreateCommandBuffer(VkCommandBufferLevel level)
    {
      return CommandBuffer(*this, level);
    }

    std::vector<CommandBuffer> CommandPool::CreateCommandBuffers(VkCommandBufferLevel level, uint32_t count)
    {
      VkCommandBufferAllocateInfo info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};

      //attach the command buffer to the pool
      info.commandPool = handle_;

      //set the level of the command buffer
      info.level = level;

      //set number of command buffers
      info.commandBufferCount = count;

      //create the alloc for the buffers
      std::vector<VkCommandBuffer> bufferHandles_;
      bufferHandles_.resize(count);

      //create the buffers
      if(vkAllocateCommandBuffers(device_.GetLogicalHandle(),&info,bufferHandles_.data()) != VK_SUCCESS)
        throw InitializationException("Unable to create command buffers!");

      //create the list of command buffers for the object
      std::vector<CommandBuffer> result;

      //resize the vector to the amount of buffers created
      result.reserve(count);

      //create command buffer handles for the device
      for(VkCommandBuffer buffers : bufferHandles_)
        result.emplace_back(buffers, *this);

      return result;
    }
    CommandPool::CommandPool(Device &device, const VkCommandPoolCreateInfo &info)
    : handle_(nullptr), device_(device)
    {
      if(vkCreateCommandPool(device.GetLogicalHandle(), &info, nullptr, &handle_) != VK_SUCCESS)
        throw InitializationException("Unable to create Command Pool!");
    }

    CommandPool::CommandPool(Device &device) : handle_(nullptr), device_(device)
    {
      VkCommandPoolCreateInfo info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
      info.queueFamilyIndex = device.GetGraphicsIndices();
      info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
      if(vkCreateCommandPool(device.GetLogicalHandle(), &info, nullptr, &handle_) != VK_SUCCESS)
        throw InitializationException("Unable to create Command Pool!");
    }
}