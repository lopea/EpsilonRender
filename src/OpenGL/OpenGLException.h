//
// Created by lopea on 7/11/21.
//

#ifndef EPSILONRENDERER_OPENGLEXCEPTION_H
#define EPSILONRENDERER_OPENGLEXCEPTION_H


#include <stdexcept>

namespace Epsilon::OpenGL
{
    class InitializationException : public std::runtime_error
    {
    public:
        explicit InitializationException(const std::string& message)
        : std::runtime_error("OPENGL INITIALIZATION ERROR: " + message){};
    };
}

#endif //EPSILONRENDERER_OPENGLEXCEPTION_H
