//
// Created by Javier on 6/10/2021.
//

#ifndef EPSILONRENDERER_OBJECTMANAGER_H
#define EPSILONRENDERER_OBJECTMANAGER_H
#include <list>
#include <memory>
#include <deque>
#include "Object.h"

namespace Epsilon
{
    class ObjectManager
    {
    public:
        template<typename T>
        Object *CreateObject();

        void DeleteObject(Object *obj);

        void Update();

        void UpdateImGuiCalls();

    private:
        void DeleteMarkedObjects();

        std::list<std::unique_ptr<Object>> objects;
        std::deque<std::list<std::unique_ptr<Object>>::iterator> delete_;
    };

    template<typename T>
    Object *ObjectManager::CreateObject()
    {
      //check if the T value is valid
      static_assert(std::is_base_of<Object, T>::value);

      //create a new T value
      objects.push_back(std::move(std::make_unique<T>()));

      //get new object to add reference to the manager
      objects.back()->ref = --objects.end();

      //send the reference to the object
      return objects.back().get();
    }


}

#endif //EPSILONRENDERER_OBJECTMANAGER_H
