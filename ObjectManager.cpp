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
}