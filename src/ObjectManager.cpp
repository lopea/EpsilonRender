//
// Created by Javier on 6/10/2021.
//

#include "ObjectManager.h"
#include "Object.h"

namespace Epsilon
{

    void ObjectManager::DeleteObject(Object *obj)
    {
      delete_.push_back(obj->ref);
    }

    void ObjectManager::DeleteMarkedObjects()
    {
      //go through all the objects in the renderer
      for(auto& iterator : delete_)
      {
        //remove the object from the list
        objects.erase(iterator);
      }
    }

    void ObjectManager::Update()
    {
      for(auto& object : objects)
      {
        object->Update();
      }
    }

    void ObjectManager::UpdateImGuiCalls()
    {
      //go through all objects and call their uses to imgui
      for(auto& object : objects)
        object->OnImGuiDraw();
    }
}
