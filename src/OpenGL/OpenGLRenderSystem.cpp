//
// Created by lopea on 7/14/21.
//

#include "OpenGLRenderSystem.h"
#include "OpenGLContextWindow.h"

namespace Epsilon::OpenGL
{
    Epsilon::ContextWindow *RenderSystem::PushBackNewWindow(unsigned int width, unsigned int height)
    {
      windows_.push_back(new OpenGL::ContextWindow(width, height));

      return windows_.back();
    }

    void RenderSystem::Render(glShader *shader, Mesh &mesh)
    {
      unsigned program = *static_cast<const unsigned*>(shader->GetShaderHandle());
      glUseProgram(program);
      glBindVertexArray(*static_cast<unsigned*>(mesh.GetMeshHandle()));
      glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT , nullptr);
      glBindVertexArray(0);
      glUseProgram(0);
    }
}