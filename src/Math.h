//
// Created by Javier on 6/17/2021.
//

#ifndef EPSILONRENDERER_MATH_H
#define EPSILONRENDERER_MATH_H

//reduce warnings only for windows



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

//remove warnings for glm on windows
#ifdef _WIN32
#pragma warning(push, 2)
#endif

//include glm and any other math related libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#ifdef _WIN32
#pragma warning(pop)
#endif


#endif //EPSILONRENDERER_MATH_H
