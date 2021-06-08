//
// Created by Javier on 6/7/2021.
//

#ifndef EPSILONRENDERER_VULKANMESH_H
#define EPSILONRENDERER_VULKANMESH_H


#include "Mesh.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanSwapChain.h"
#include <vulkan/vulkan.h>

namespace Epsilon::Vulkan
{
    class vkMesh : public Epsilon::Mesh_
    {
    public:
        template<typename VertContainer, typename IndContainer>
        vkMesh(const VertContainer& verts, const IndContainer& indices, Device& device, SwapChain& swapChain);

    private:
        Device& device_;
        Buffer vertexBuffer, indexBuffer;

    };

    template<typename VertContainer, typename IndContainer>
    vkMesh::vkMesh(const VertContainer &verts, const IndContainer &indices, Device& device, SwapChain& swapChain) : Epsilon::Mesh_(verts, indices),
    device_(device),
    vertexBuffer(device_, swapChain,vertices_.size() * sizeof(vertices_[0]), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {
      //create a stage buffer
      Buffer stageBuffer(device_, swapChain,
                         vertices_.size() * sizeof(vertices_[0]),
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      //set the data in the buffer
      stageBuffer.SetData(vertices_.data(),vertices_.size() * sizeof(vertices_[0]));

      //copy the buffer
      vertexBuffer.CopyBuffer(stageBuffer);

    }

}

#endif //EPSILONRENDERER_VULKANMESH_H
