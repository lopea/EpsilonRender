//
// Created by Javier on 6/6/2021.
//

#ifndef EPSILONRENDERER_MESH_H
#define EPSILONRENDERER_MESH_H
#include <glm/glm.hpp>
#include <vector>
#include "ContextWindow.h"

namespace Epsilon
{
    struct Vertex
    {
        //! position of the vertex in object space
        glm::vec3 pos;

        //! normalized texture coorinate position of the vertex
        glm::vec2 uv;

        //! object space normal direction for the vertex's corresponding triangles
        glm::vec3 normal;
    };
    class Mesh
    {
    public:

        virtual ~Mesh() = default;
        /*!
         * Create a mesh with a set of vertices and indices for those vertices
         * @tparam VertContainer the type of the container for the vertices ( needs a .begin() and a .end() )
         * @tparam IndContainer the type of the container for the indices ( needs a .begin() and a .end() )
         * @param vertices all the vertices for the new mesh
         * @param indices all the indices for the new mesh
         */
        template<typename VertContainer, typename IndContainer>
        Mesh(const VertContainer& vertices, const IndContainer& indices)
        :vertices_(vertices.begin(), vertices.end()),
        indices_(indices.begin(), indices.end()){}

        [[nodiscard]] std::vector<Vertex> GetVertices() const{ return vertices_;}
        [[nodiscard]] std::vector<unsigned> GetIndices() const { return indices_;}

        /*!
         * @return Get the reference of the mesh for specification specific purposes
         */
        virtual void* GetMeshHandle() = 0;

    protected:
        std::vector<Vertex> vertices_;
        std::vector<unsigned> indices_;
    };

}

#endif //EPSILONRENDERER_MESH_H
