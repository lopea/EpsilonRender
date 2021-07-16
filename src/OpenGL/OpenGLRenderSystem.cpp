//
// Created by lopea on 7/14/21.
//

#include "OpenGLRenderSystem.h"
#include "OpenGLContextWindow.h"


    Epsilon::ContextWindow *Epsilon::OpenGL::RenderSystem::PushBackNewWindow(unsigned int width, unsigned int height)
    {
      windows_.push_back(new OpenGL::ContextWindow(width, height));

      return windows_.back();
    }

    void Epsilon::OpenGL::RenderSystem::Render(glShader *shader, Mesh &mesh)
    {
      unsigned program = *static_cast<const unsigned*>(shader->GetShaderHandle());
      glUseProgram(program);
      glBindVertexArray(*static_cast<const unsigned*>(mesh.GetMeshHandle()));
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT , nullptr);
      glBindVertexArray(0);
      glUseProgram(0);
    }
