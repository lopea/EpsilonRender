//
// Created by Javier on 6/7/2021.
//

#ifndef EPSILONRENDERER_VULKANMESH_H
#define EPSILONRENDERER_VULKANMESH_H


#include "Mesh.h"
#include "VulkanDevice.h"
#include <vulkan/vulkan.h>

namespace Epsilon::Vulkan
{
    class vkMesh : public Epsilon::Mesh_
    {
    public:
        template<typename VertContainer, typename IndContainer>
        vkMesh(const VertContainer& verts, const IndContainer& indices, Device& device);

        ~vkMesh();
    private:
        Device& device_;
        VkBuffer vertexBuffer_;
        VkDeviceMemory vertexBufferMemory_;

        void CreateVertexBuffer();
        void CreateIndexBuffer();
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };

    template<typename VertContainer, typename IndContainer>
    vkMesh::vkMesh(const VertContainer &verts, const IndContainer &indices, Device& device):vkMesh(verts, indices), device_(device)
    {
      CreateVertexBuffer();
      CreateIndexBuffer();
    }

}

#endif //EPSILONRENDERER_VULKANMESH_H
