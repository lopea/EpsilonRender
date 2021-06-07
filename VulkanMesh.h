//
// Created by Javier on 6/7/2021.
//

#ifndef EPSILONRENDERER_VULKANMESH_H
#define EPSILONRENDERER_VULKANMESH_H


#include "Mesh.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include <vulkan/vulkan.h>

namespace Epsilon::Vulkan
{
    class vkMesh : public Epsilon::Mesh_
    {
    public:
        template<typename VertContainer, typename IndContainer>
        vkMesh(const VertContainer& verts, const IndContainer& indices, Device& device);

    private:
        Device& device_;
        Buffer vertexBuffer, stageBuffer;

    };

    template<typename VertContainer, typename IndContainer>
    vkMesh::vkMesh(const VertContainer &verts, const IndContainer &indices, Device& device): Epsilon::Mesh_(verts, indices),
    device_(device),
    vertexBuffer(device_, vertices_.size() * sizeof(vertices_[0]), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
    {
      //set the data in the buffer
      vertexBuffer.SetData(vertices_.data(),vertices_.size() * sizeof(vertices_[0]));
    }

}

#endif //EPSILONRENDERER_VULKANMESH_H
