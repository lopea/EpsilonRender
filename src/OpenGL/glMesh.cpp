//
// Created by lopea on 7/15/21.
//

#include "glMesh.h"

namespace Epsilon::OpenGL
{
    void Mesh::Initialize()
    {

      //set data for the vertex buffer
      VertexBuffer.SetData(vertices_.data(), vertices_.size() * sizeof(Vertex));

      //set data for the index buffer
      IndexBuffer.SetData(indices_.data(), indices_.size() * sizeof(unsigned int));

      //create the vertex array object
      glGenVertexArrays(1, &vao_);

      //create the connection of buffers
      glBindVertexArray(vao_);

      //vertex buffer
      glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer.GetHandle());


      //index buffer
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer.GetHandle());

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
      glBindVertexArray(0);
    }

    Mesh::~Mesh()
    {
      glDeleteVertexArrays(1, &vao_);
    }
}