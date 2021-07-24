//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONRENDERER_MATERIAL_H
#define EPSILONRENDERER_MATERIAL_H

#include <vector>
#include <unordered_map>

namespace Epsilon
{
    class RenderPipeline;

    class Material
    {
    public:
        void SetUniformData(unsigned bindingLocation, std::size_t size, void* data);
        void AddPass(RenderPipeline* pipeline);
        [[nodiscard]] const std::unordered_map<unsigned, std::vector<char>>& GetUniformData() const { return uniformBindings_;};
        [[nodiscard]] std::vector<RenderPipeline*> GetRenderPasses() const {return passes_;};
    private:
        std::vector<RenderPipeline *> passes_;
        std::unordered_map<unsigned, std::vector<char>> uniformBindings_;
    };

}
#endif //EPSILONRENDERER_MATERIAL_H
