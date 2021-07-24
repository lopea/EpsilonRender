//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONRENDERER_SPECIFICATIONDATA_HPP
#define EPSILONRENDERER_SPECIFICATIONDATA_HPP

#include "Mesh.h"
#include "RenderPipeline.h"
#include <unordered_map>

namespace Epsilon
{
    template<typename Pipeline_, typename Mesh_>
    class SpecificationData
    {
    public:
      using pipeline_type = Pipeline_;
      using mesh_type = Mesh_;

      pipeline_type* GetPipeline(const RenderPipeline* pipeline)
      {
        //dont do anything if the handle is null.
        if(!pipeline)
          return nullptr;

        //check if the pipeline has been already created.
        auto pos = pipelines.find(pipeline->GetName());

        //if the pipeline has not been created,
        if(pos == pipelines.end())
        {
          //create pipeline
          return &pipelines.emplace(pipeline->GetName(), pipeline->GetShaderModules()).first->second;
        }
        return &pos->second;
      }

      mesh_type* GetMeshPipeline(const Mesh* mesh)
      {
        //TODO:
        return nullptr;
      }

    private:
        std::unordered_map<std::string,pipeline_type> pipelines;
        std::unordered_map<std::string,mesh_type> meshTypes_;

    };


}

#endif //EPSILONRENDERER_SPECIFICATIONDATA_HPP
