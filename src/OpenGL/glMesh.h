//
// Created by lopea on 7/15/21.
//

#ifndef EPSILONRENDERER_GLMESH_H
#define EPSILONRENDERER_GLMESH_H

#include "../Mesh.h"
#include <glad/gl.h>
#include "OpenGLBuffer.h"
namespace Epsilon::OpenGL
{
    class Mesh : public Epsilon::Mesh
    {
    public:

        void Initialize();
        template<typename VertexContainer, typename IndexContainer>
        Mesh(const VertexContainer& vertices, const IndexContainer& indices) : Epsilon::Mesh(vertices,indices), vao_(0),
        IndexBuffer(indices_.size() * sizeof(unsigned int), GL_DYNAMIC_DRAW, GL_ELEMENT_ARRAY_BUFFER),
        VertexBuffer(vertices_.size() * sizeof(Vertex), GL_DYNAMIC_DRAW)
        {
          Initialize();
        }


        ~Mesh() override;


        //avoid any copying
        Mesh(const Mesh& mesh) = delete;
        Mesh& operator=(const Mesh& mesh) = delete;

        [[nodiscard]] std::size_t GetIndexCount() const { return indices_.size();}
        void * GetMeshHandle() override{return &vao_;}
    private:
        GLuint vao_;
        Buffer IndexBuffer, VertexBuffer;

    };
}

#endif //EPSILONRENDERER_GLMESH_H
