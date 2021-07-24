//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONRENDERER_OPENGLBUFFER_H
#define EPSILONRENDERER_OPENGLBUFFER_H


#include <glad/gl.h>
#include <cstddef>

namespace Epsilon::OpenGL
{
    class Buffer
    {
    public:
        Buffer(size_t size, GLenum usage, GLenum bufferType = GL_ARRAY_BUFFER);
        ~Buffer();

        void SetData(const void *data, size_t size);

        [[nodiscard]] unsigned int GetHandle() const { return handle_;}

        //avoid any copying
        Buffer(const Buffer&) = delete;
        Buffer operator=(const Buffer&) = delete;
    private:
        unsigned int handle_;
        size_t size_;
        GLenum usage_, bufferType_;
    };
}

#endif //EPSILONRENDERER_OPENGLBUFFER_H
