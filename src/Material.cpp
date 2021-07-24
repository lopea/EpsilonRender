//
// Created by Javier on 7/24/2021.
//

#include "Material.h"

namespace Epsilon
{
    void Material::SetUniformData(unsigned int bindingLocation, std::size_t size, void *data)
    {
      uniformBindings_[bindingLocation] = std::vector<char>(static_cast<char *>(data),
                                                            static_cast<char *>(data) + size);
    }

    void Material::AddPass(RenderPipeline *pipeline)
    {
      if (!pipeline)
        return;
      passes_.push_back(pipeline);
    }
}