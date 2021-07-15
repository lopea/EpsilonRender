//
// Created by lopea on 7/14/21.
//

#ifndef EPSILONRENDERER_RENDERCOMMAND_H
#define EPSILONRENDERER_RENDERCOMMAND_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Shader.h"

namespace Epsilon
{
    class RenderCommand
    {
    public:
        RenderCommand(const std::vector<Shader> shaderLocation, const std::string &meshLocation) :
            shaders_(shaderLocation), mesh_(meshLocation)
        {}

        template<typename Array>
        void SetUniformLocation(unsigned uniformLocation, const Array &data)
        {
          //set the data for the specific uniform location
          uniformData[uniformLocation] = std::vector<unsigned char>(&data, &data + sizeof(data));
        }

    private:
        std::vector<Shader> shaders_;
        std::string mesh_;
        std::unordered_map<unsigned, std::vector<unsigned char>> uniformData;

    };
}


#endif //EPSILONRENDERER_RENDERCOMMAND_H
