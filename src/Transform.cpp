//
// Created by Javier on 6/17/2021.
//

#include "Transform.h"



glm::mat4 Transform::CreateMatrix()
{
  //create identity matrix
  glm::mat4 result(1);

  //apply SRT to the matrix based on the transform
  result = glm::translate(result,position) * glm::yawPitchRoll(rotation.x, rotation.y, rotation.z) * glm::scale(result, scale);

  return result;
}
