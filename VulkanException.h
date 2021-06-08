//
// Created by Javier on 6/4/2021.
//

#ifndef EPSILONRENDERER_VULKANEXCEPTION_H
#define EPSILONRENDERER_VULKANEXCEPTION_H

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

    class RuntimeException : public std::runtime_error
    {
    public:
        explicit RuntimeException(const std::string& msg):
            std::runtime_error("Vulkan Critical Runtime Error: " + msg){}

    };
}

#endif //EPSILONRENDERER_VULKANEXCEPTION_H
