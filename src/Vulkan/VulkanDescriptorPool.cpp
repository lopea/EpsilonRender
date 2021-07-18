//
// Created by Javier on 6/10/2021.
//

#include "VulkanDescriptorPool.h"
#include "VulkanDevice.h"
#include "VulkanException.h"
#include <vulkan/vulkan.h>

namespace Epsilon::Vulkan
{
    DescriptorPool::DescriptorPool(Device &device) : handle_(nullptr), device_(device)
    {
      VkDescriptorPoolSize pool_sizes[] =
          {
              { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
              { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
              { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
              { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
              { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
              { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
              { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
              { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
              { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
              { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
              { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
          };
      VkDescriptorPoolCreateInfo pool_info = {};
      pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
      pool_info.maxSets = 1000 * static_cast<uint32_t>(std::size(pool_sizes));
      pool_info.poolSizeCount = (uint32_t)std::size(pool_sizes);
      pool_info.pPoolSizes = pool_sizes;
      if(vkCreateDescriptorPool(device.GetLogicalHandle(), &pool_info, nullptr, &handle_) != VK_SUCCESS)
        throw InitializationException("Cannot create description pool!");
    }

    DescriptorPool::~DescriptorPool()
    {
      vkDestroyDescriptorPool(device_.GetLogicalHandle(), handle_, nullptr);
    }
}