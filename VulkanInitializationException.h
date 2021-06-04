//
// Created by Javier on 6/4/2021.
//

#ifndef EPSILONRENDERER_VULKANINITIALIZATIONEXCEPTION_H
#define EPSILONRENDERER_VULKANINITIALIZATIONEXCEPTION_H

#include <exception>
#include <stdexcept>

namespace Epsilon::Vulkan
{

    class InitializationException : public std::runtime_error
    {
    public:
        explicit InitializationException(const std::string &msg)
            : std::runtime_error("VULKAN CRITICAL INIT ERROR: " + msg)
        {}
    };
}

#endif //EPSILONRENDERER_VULKANINITIALIZATIONEXCEPTION_H
