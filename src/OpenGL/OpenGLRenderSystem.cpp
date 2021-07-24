//
// Created by lopea on 7/14/21.
//

#include <stdexcept>
#include <iostream>
#include "OpenGLRenderSystem.h"
#include "OpenGLContextWindow.h"
#include "../RenderPipeline.h"
#include "../Material.h"

namespace Epsilon::OpenGL
{
    Epsilon::ContextWindow *RenderSystem::PushBackNewWindow(unsigned int width, unsigned int height)
    {
      windows_.push_back(new OpenGL::ContextWindow(width, height));

      return windows_.back();
    }

    void RenderSystem::Render(Epsilon::Material &shader, Mesh &mesh)
    {
      auto uniformData = shader.GetUniformData();



      //start finding the proper opengl specific pipeline

      OpenGL::RenderPipeline* glPipeline = openGLData_.GetPipeline(shader.GetRenderPasses().front());
      unsigned program = *static_cast<const unsigned*>(glPipeline->GetShaderHandle());
      for(const auto& [key,value] : uniformData)
      {
        Buffer* buffer = UpdateBuffer(key, value);

        glBindBuffer(GL_UNIFORM_BUFFER, buffer->GetHandle());

        glBindBufferBase(GL_UNIFORM_BUFFER, key, buffer->GetHandle());

        int err = glGetError();
        if(err)
        {
          std::cerr << "nope" <<std::endl;
        }
      }

      glUseProgram(program);
      glBindVertexArray(*static_cast<unsigned*>(mesh.GetMeshHandle()));
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.GetIndexCount()), GL_UNSIGNED_INT , nullptr);
      glBindVertexArray(0);
      glUseProgram(0);
    }

    Buffer *RenderSystem::UpdateBuffer(unsigned int location, const std::vector<char> &data)
    {
      //get the next buffer,
      if(bufferPool_.empty())
      {
        bufferPool_.emplace(data.size(), GL_DYNAMIC_DRAW, GL_UNIFORM_BUFFER);
      }

      Buffer& top = bufferPool_.top();
      top.SetData(data.data(),data.size() * sizeof(char));
      return &top;
    }


}