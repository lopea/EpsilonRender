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
    vertexBuffer(device_,
                 swapChain,
                 vertices_.size() * sizeof(vertices_[0]),
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
                 indexBuffer(device_,
                             swapChain,
                             indices_.size(),
                             VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {
      //create a stage buffer for the vertex
      Buffer vertStageBuffer(device_, swapChain,
                         vertices_.size() * sizeof(vertices_[0]),
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      //create a buffer for the index
      Buffer indStageBuffer(device_, swapChain, indices_.size() * sizeof(indices_[0]),
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      //set the data in the buffer
      vertStageBuffer.SetData(vertices_.data(), vertices_.size() * sizeof(vertices_[0]));

      //copy the buffer
      vertexBuffer.CopyBuffer(vertStageBuffer);

      //set the data for the indices
     indStageBuffer.SetData(indices_.data(), sizeof(indices_[0]) * indices.size_);

     //copy the data to the index buffer
     indexBuffer.CopyBuffer(indStageBuffer);
    }

}

#endif //EPSILONRENDERER_VULKANMESH_H
