//
// Created by lopea on 7/15/21.
//

#ifndef EPSILONRENDERER_GLMESH_H
#define EPSILONRENDERER_GLMESH_H

#include "../Mesh.h"
#include <glad/glad.h>

namespace Epsilon::OpenGL
{
    class Mesh : public Epsilon::Mesh_
    {
    public:
        template<typename VertexContainer, typename IndexContainer>
        Mesh(const VertexContainer& vertices, const IndexContainer& indices) : Epsilon::Mesh_(vertices,indices), vao_(0),
        vbo_(0), ebo_(0)
        {
          //create the vertex and index buffers
          glCreateBuffers(1, &vbo_);
          glCreateBuffers(1, &ebo_);

          //create the vertex array object
          glCreateVertexArrays(1, &vao_);

          //create the connection of buffers
          glBindVertexArray(vao_);

          //vertex buffer
          glBindBuffer(GL_ARRAY_BUFFER, vbo_);
          glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Vertex) * vertices_.size()), vertices_.data(), GL_DYNAMIC_DRAW);

          //for now, we will say that vertices are on position 0 in the attrib
          glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), nullptr);
          glEnableVertexAttribArray(0);

          //uv coordinates will be on location 1
          glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
          glEnableVertexAttribArray(1);

          //normal coordinates will be on 2
          glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
          glEnableVertexAttribArray(2);

          //index buffer
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(unsigned int) * indices_.size()), indices_.data(), GL_DYNAMIC_DRAW);


          glBindVertexArray(0);


        }


        ~Mesh() { }

        std::size_t GetIndexCount() const { return indices_.size();}
        SpecificationType GetMeshType() override {return SpecificationType::OpenGL;}
        void * GetMeshHandle() override{return &vao_;}
    private:
        GLuint vao_, vbo_, ebo_;

    };
}

#endif //EPSILONRENDERER_GLMESH_H
