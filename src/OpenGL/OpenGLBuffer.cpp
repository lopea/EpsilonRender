//
// Created by Javier on 7/24/2021.
//

#include "OpenGLBuffer.h"
#include <glad/gl.h>
#include <cstring>

namespace Epsilon::OpenGL
{
    Buffer::Buffer(size_t size, GLenum usage, GLenum bufferType) : handle_ (0), size_(size), bufferType_(bufferType)
    {
      //create handle for the buffer
      glGenBuffers(1, &handle_);

      //bind the buffer as an array
      glBindBuffer(bufferType_, handle_);

      //create the size of the buffer,
      glBufferData(bufferType_, static_cast<GLsizeiptr>(size_), nullptr,usage);

      //unbind the buffer
      glBindBuffer(bufferType_, 0);
    }

    void Buffer::SetData(const void *data, size_t size)
    {
      //bind the buffer
      glBindBuffer(bufferType_, handle_);

      //get the location of the buffer
      void* buffer = glMapBuffer(bufferType_, GL_WRITE_ONLY);

      //write the new data into the buffer
      std::memcpy(buffer, data, size > size_ ? size_ : size);

      //return buffer handle and unbind
      glUnmapBuffer(bufferType_);
      glBindBuffer(bufferType_, 0);
    }

    Buffer::~Buffer()
    {
      //delete the buffer from memory
      glDeleteBuffers(1, &handle_);
    }
}