//
// Created by Javier on 6/10/2021.
//

#include "VulkanCommandBuffer.h"
#include "VulkanException.h"
#include "VulkanCommandPool.h"
namespace Epsilon::Vulkan
{
    CommandBuffer::CommandBuffer(Epsilon::Vulkan::CommandPool &pool_, VkCommandBufferLevel level)
        : handle_(nullptr), pool_(pool_)
    {
      //please end my suffering with all these structs!!!!
      VkCommandBufferAllocateInfo info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};

      //attach the command buffer to the pool
      info.commandPool = pool_.GetHandle();

      //set the level of the command buffer
      info.level = level;

      //set number of command buffers
      info.commandBufferCount = 1;

      //create command buffer
      if(vkAllocateCommandBuffers(pool_.GetDevice().GetLogicalHandle(), &info, &handle_) != VK_SUCCESS)
        throw InitializationException("Unable to create Command Buffer!");

    }

    void CommandBuffer::BeginRecording(VkCommandBufferUsageFlags recordingFlags)
    {
      //if i had a cent for every struct i've made in this project, i would have enough to buy a gun so i can end my misery
      VkCommandBufferBeginInfo cbinfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
      cbinfo.flags = recordingFlags;
      //start the recording process for the command buffer
      if(vkBeginCommandBuffer(handle_, &cbinfo) != VK_SUCCESS)
        throw RuntimeException("Unable to begin command buffer recording");
    }

    void CommandBuffer::EndRecording()
    {
      //stop the buffer from recording
      if (vkEndCommandBuffer(handle_) != VK_SUCCESS)
        throw std::runtime_error("Failed to record command buffer!!!");
    }

    void CommandBuffer::Reset(VkCommandBufferResetFlags flags)
    {
      //reset the buffer and flush all of its contents
      if(vkResetCommandBuffer(handle_, flags) != VK_SUCCESS)
        throw RuntimeException("Unable to reset command buffer!!!");
    }

    CommandBuffer::CommandBuffer(VkCommandBuffer handle, CommandPool &pool) : handle_(handle), pool_(pool)
    {}

    void CommandBuffer::SubmitCommands()
    {
      Device& device_ = pool_.GetDevice();
      //create an info struct
      VkSubmitInfo info {VK_STRUCTURE_TYPE_SUBMIT_INFO};

      //set to only submit one command
      info.commandBufferCount = 1;
      info.pCommandBuffers = &handle_;

      vkQueueSubmit(device_.GetGraphicsQueue(), 1, &info, nullptr);
      vkQueueWaitIdle(device_.GetGraphicsQueue());
    }

    void CommandBuffer::Free()
    {
      //get the device to free the buffer
      Device& device_ = pool_.GetDevice();

      //free the command buffer
      vkFreeCommandBuffers(device_.GetLogicalHandle(),pool_.GetHandle(), 1, &handle_);

      //clear the handle to avoid any issues
      handle_ = nullptr;
    }

}