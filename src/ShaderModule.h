//
// Created by Javier on 5/31/2021.
//

#ifndef EPSILONRENDERER_SHADERMODULE_H
#define EPSILONRENDERER_SHADERMODULE_H

#include <string>
#include <utility>
#include <vector>
namespace Epsilon
{
    enum class SpecificationType;

    enum ShaderStageFlagBits
    {
        SHADER_STAGE_VERTEX = 1,
        SHADER_STAGE_FRAGMENT = 2,
    };

    class ShaderModule
    {
    public:
        ShaderModule(const std::string& name, ShaderStageFlagBits stage, std::vector<char> spirvData)
        : shaderName_(), spirvData_(std::move(spirvData)), stageFlag_(stage){}

        [[nodiscard]] std::string GetShaderName() const {return shaderName_;}
        [[nodiscard]] std::vector<char> GetSPIRVData() const {return spirvData_;}
        [[nodiscard]] ShaderStageFlagBits GetStageType() const { return stageFlag_;}
    private:
        std::string shaderName_;
        std::vector<char> spirvData_;
        ShaderStageFlagBits stageFlag_;
    };
}
#endif //EPSILONRENDERER_SHADERMODULE_H
