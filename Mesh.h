//
// Created by Javier on 6/6/2021.
//

#ifndef EPSILONRENDERER_MESH_H
#define EPSILONRENDERER_MESH_H
#include <glm/glm.hpp>
#include <vector>

namespace Epsilon
{
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec2 uv;
        glm::vec3 normal;
    };
    class Mesh
    {
    private:
        std::vector<Vertex> vertices_;
        std::vector<uint64_t> indices_;
    };
}

#endif //EPSILONRENDERER_MESH_H
