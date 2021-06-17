//
// Created by Javier on 6/17/2021.
//

#include "Transform.h"



glm::mat4 Transform::CreateMatrix()
{
  glm::mat4 result(1);
  result = glm::translate(result,position) * glm::yawPitchRoll(rotation.x, rotation.y, rotation.z) * glm::scale(result, scale);
  return result;
}
