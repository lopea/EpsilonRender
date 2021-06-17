//
// Created by Javier on 6/17/2021.
//

#ifndef EPSILONRENDERER_MATH_H
#define EPSILONRENDERER_MATH_H

//reduce warnings only for windows

#ifdef _MSC_VER

//check if SIMD operations are available

//use AVX2 instructions
#ifdef __AVX2__
#define GLM_FORCE_AVX2
#else

//use AVX instructions if AVX2 instructions are unavailable
#ifdef __AVX__
#define GLM_FORCE_AVX
#endif

#endif



#pragma warning(push, 2)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#pragma warning(pop)

#endif

#endif //EPSILONRENDERER_MATH_H
