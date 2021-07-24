//
// Created by Javier on 6/7/2021.
//

#ifndef EPSILONRENDERER_RENDERSYSTEM_H
#define EPSILONRENDERER_RENDERSYSTEM_H

#include <vector>
#include <unordered_map>

namespace Epsilon
{
    enum class SpecificationType
    {
        Vulkan = 0,
        OpenGL = 1,
        Count,
    };
    class ContextWindow;
    class RenderPipeline;
    class Mesh;
    class RenderSystem
    {
    public:
        explicit RenderSystem(SpecificationType type);

        virtual ~RenderSystem();

        [[nodiscard]] virtual SpecificationType GetSpecificationType() {return type_;};

        virtual ContextWindow* PushBackNewWindow(unsigned width, unsigned height) = 0;

        ContextWindow* GetWindow(unsigned index = 0) {return windows_[index];}

        void Render(const RenderPipeline *pipeline, const Mesh *mesh, unsigned window = 0);
        [[nodiscard]] const ContextWindow* GetWindow(unsigned index) const {return windows_[index];}

        [[nodiscard]] bool IsEmpty() const { return windows_.empty();}

    protected:
        std::vector<ContextWindow*> windows_;
        SpecificationType type_;


        void ClearAllWindows();
    };
}

#endif //EPSILONRENDERER_RENDERSYSTEM_H
