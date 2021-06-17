//
// Created by Javier on 6/17/2021.
//

#ifndef EPSILONRENDERER_TRANSFORM_H
#define EPSILONRENDERER_TRANSFORM_H

#include "Math.h"

struct Transform
{
    glm::mat4 CreateMatrix();
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
};


#endif //EPSILONRENDERER_TRANSFORM_H
