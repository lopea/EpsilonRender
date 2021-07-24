//
// Created by lopea on 7/13/21.
//

#include <stdexcept>
#include <iostream>
#include "OpenGLRenderPipeline.h"

namespace Epsilon::OpenGL
{
    void CheckForShaderError(GLuint shader)
    {
      int err;
      //check for errors
      glGetShaderiv(shader, GL_COMPILE_STATUS, &err);

      //if an error has occurred,
      if(!err)
      {
        //store the length of the error message
        int maxLen =0;

        //get the length of the error message
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLen);

        //store the message itself onto an array
        std::string errLog(maxLen, 0);

        //get the error message from GL
        glGetShaderInfoLog(shader, maxLen,&maxLen, errLog.data());

        //TODO: make this more viable for catching
        //send message
        throw std::runtime_error("GL ERROR: Shader failed to compile!!!\nMessage: " + errLog);
      }
    }
    GLenum ShaderStageFlagToGLFlag(ShaderStageFlagBits ShaderStage)
    {
      switch (ShaderStage)
      {

        case SHADER_STAGE_VERTEX:
          return GL_VERTEX_SHADER;
        case SHADER_STAGE_FRAGMENT:
          return GL_FRAGMENT_SHADER;
      }
      return 0;
    }
    GLuint CreateShader(ShaderModule* module)
    {
      //create shader based on the type
      GLuint newShader = glCreateShader(ShaderStageFlagToGLFlag(module->GetStageType()));

      //store the raw data from the module
      std::vector<char> spirvData = module->GetSPIRVData();

      //add spirv data to the shader
      glShaderBinary(1, &newShader, GL_SHADER_BINARY_FORMAT_SPIR_V, spirvData.data(), static_cast<GLint>(spirvData.size()));

      //compile shader
      glSpecializeShader(newShader, "main", 0, nullptr, nullptr);
      
      return newShader;
    }

    RenderPipeline::RenderPipeline(const std::vector<ShaderModule *> &modules)
    {
      //create program
      program_ = glCreateProgram();

      for (ShaderModule *module : modules)
      {
        //get shader
        GLuint newShader = CreateShader(module);

        //check for any errors
        CheckForShaderError(newShader);

        //attach shader for linking
        glAttachShader(program_, newShader);
      }

      //if everything went well, create shader program
      glLinkProgram(program_);

      //check if program completed successfully

      int err;

      glGetProgramiv(program_, GL_LINK_STATUS, &err);

      if (!err)
      {
        //store the length of the error message
        int maxLen = 0;

        //get the length of the error message
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &maxLen);

        //store the message itself onto an array
        std::string errLog(maxLen, 0);

        //get the error message from GL
        glGetProgramInfoLog(program_, maxLen, &maxLen, errLog.data());

        //TODO: make this more viable for catching
        //send message
        throw std::runtime_error("GL ERROR: Program Failed to link!!!!\nMessage: " + errLog);

      }
    }

    RenderPipeline::~RenderPipeline()
    {
      //cleanup the gl program
      glDeleteProgram(program_);
    }
}