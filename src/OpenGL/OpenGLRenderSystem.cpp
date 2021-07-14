//
// Created by lopea on 7/14/21.
//

#include "OpenGLRenderSystem.h"
#include "OpenGLContextWindow.h"

Epsilon::ContextWindow *Epsilon::OpenGL::RenderSystem::PushBackNewWindow(unsigned int width, unsigned int height)
{
  windows_.push_back(new OpenGL::ContextWindow(width,height));

  return windows_.back();
}
