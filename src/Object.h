//
// Created by Javier on 6/10/2021.
//

#ifndef EPSILONRENDERER_OBJECT_H
#define EPSILONRENDERER_OBJECT_H

#include "Renderer.h"
#include <list>
#include <memory>

namespace Epsilon
{

    class Object
    {
    public:

        //! Gets called every frame the object is active
        virtual void Update() = 0;

        virtual void OnRender() {}

        virtual ~Object() = default;

        [[nodiscard]] bool IsDeleted() const {return isDeleted_;}

        //avoid any copying
        Object& operator=(Object&) = delete;
        Object(Object&) = delete;

    protected:
        Renderer rend{};
    private:
        std::list<std::unique_ptr<Object>>::iterator ref;
        bool isDeleted_ = false;
        friend class ObjectManager;
    };

}
#endif //EPSILONRENDERER_OBJECT_H
