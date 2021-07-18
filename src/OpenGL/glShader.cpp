//
// Created by lopea on 7/13/21.
//

#include <stdexcept>
#include "glShader.h"

namespace Epsilon
{


    glShader::glShader(const std::vector<char> &vertexData, const std::vector<char> &fragData)
    {
      //create shader handles
      GLuint vs = glCreateShader(GL_VERTEX_SHADER), fs = glCreateShader(GL_FRAGMENT_SHADER);

      //start compiling the SPIR-V shaders
      glShaderBinary(1, &vs, GL_SHADER_BINARY_FORMAT_SPIR_V, vertexData.data(), static_cast<GLint>(vertexData.size()));
      glShaderBinary(1, &fs, GL_SHADER_BINARY_FORMAT_SPIR_V, fragData.data(), static_cast<GLint>(fragData.size()));

      //tell each shader how to be executed (this is the equivalent to compiling the shader)
      glSpecializeShader(vs, "main", 0, nullptr, nullptr);

      //check for errors
      int err;
      glGetShaderiv(vs, GL_COMPILE_STATUS, &err);

      //if an error has occurred,
      if(!err)
      {
        //store the length of the error message
        int maxLen =0;

        //get the length of the error message
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLen);

        //store the message itself onto an array
        std::string errLog(maxLen, 0);

        //get the error message from GL
        glGetShaderInfoLog(vs, maxLen,&maxLen, errLog.data());

        //clear data
        glDeleteShader(vs);

        //TODO: make this more viable for catching
        //send message
        throw std::runtime_error("GL ERROR: Vertex shader failed to compile!!!\nMessage: " + errLog);
      }

      glSpecializeShader(fs, "main", 0, nullptr, nullptr);

      //check for errors
      glGetShaderiv(fs, GL_COMPILE_STATUS, &err);

      //if an error has occurred,
      if(!err)
      {
        //store the length of the error message
        int maxLen =0;

        //get the length of the error message
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLen);

        //store the message itself onto an array
        std::string errLog(maxLen, 0);

        //get the error message from GL
        glGetShaderInfoLog(fs, maxLen,&maxLen, errLog.data());

        //clear data
        glDeleteShader(vs);
        glDeleteShader(fs);

        //TODO: make this more viable for catching
        //send message
        throw std::runtime_error("GL ERROR: Fragment shader failed to compile!!!\nMessage: " + errLog);
      }


      //create the shader program

      program_ = glCreateProgram();

      //link the shaders into the program
      glAttachShader(program_, vs);
      glAttachShader(program_, fs);

      //start the linking process to the program
      glLinkProgram(program_);

      glGetProgramiv(program_, GL_LINK_STATUS, &err);
      //if the linking process failed
      if(!err)
      {
        //store the length of the error message
        int maxLen =0;

        //get the length of the error message
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLen);

        //store the message itself onto an array
        std::string errLog(maxLen, 0);

        //get the error message from GL
        glGetShaderInfoLog(fs, maxLen,&maxLen, errLog.data());

        //clear data
        glDeleteShader(fs);
        glDeleteShader(vs);
        glDeleteProgram(program_);

        //TODO: make this more viable for catching
        //send message
        throw std::runtime_error("GL ERROR: Program failed to link!!!\nMessage: " + errLog);
      }
    }

}