//
// Created by Javier on 6/7/2021.
//

#ifndef EPSILONRENDERER_RENDERSYSTEM_H
#define EPSILONRENDERER_RENDERSYSTEM_H

#include "Shader.h"
#include "Mesh.h"

namespace Epsilon
{
    class RenderSystem
    {
    public:
        explicit RenderSystem(SpecificationType type);

        virtual ~RenderSystem();

        [[nodiscard]] virtual SpecificationType GetSpecificationType() {return type_;};

        virtual ContextWindow* PushBackNewWindow(unsigned width, unsigned height) = 0;

        ContextWindow* GetWindow(unsigned index = 0) {return windows_[index];}
        [[nodiscard]] const ContextWindow* GetWindow(unsigned index) const {return windows_[index];}

        [[nodiscard]] bool IsEmpty() const { return windows_.empty();}


        virtual Shader GetShader(const std::string& shaderName) = 0;




    protected:
        std::vector<ContextWindow*> windows_;
        SpecificationType type_;


        void ClearAllWindows();
    };
}

#endif //EPSILONRENDERER_RENDERSYSTEM_H
