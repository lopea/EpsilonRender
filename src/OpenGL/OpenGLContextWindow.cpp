//
// Created by lopea on 7/11/21.
//

#include "OpenGLContextWindow.h"
#include "OpenGLException.h"
#include <glad/glad.h>
#include <iostream>

namespace Epsilon::OpenGL
{
    void GLAPIENTRY
    MessageCallback( GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam )
    {
      fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
               ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
               type, severity, message );
    }


    ContextWindow::ContextWindow(int width, int height) : Epsilon::ContextWindow(width, height)
    {
      if(!glfwInit())
        throw InitializationException("Unable to initialize GLFW!!!");
      //make sure gl api is available
      glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

      //set gl version to 4.6 (needs SPIR-V support)
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

      //initialize window for gl
      GLFWwindow* newWindow;

      //check if the window has been created successfully
      if(!(newWindow = glfwCreateWindow(width, height, "Epsilon *OpenGL API*", nullptr, nullptr)))
      {
        const char* message;
        std::cerr << "GLFW ERROR #" << glfwGetError(&message) << ": " << message;
        throw InitializationException("Unable to create GLFW window!!!");
      }
      //set the new window as the main window
      glfwMakeContextCurrent(newWindow);

      //dont enable vsync for now
      glfwSwapInterval(0);
      //initialize glad extension
      gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
      //set the new window as the window this context
      SetWindowHandle(newWindow);
      // During init, enable debug output
      glEnable              ( GL_DEBUG_OUTPUT );
      glDebugMessageCallback( MessageCallback, 0 );
      glViewport(0,0, width, height);
      glScissor(0,0, width, height);
    }


    void ContextWindow::OnResize(unsigned int width, unsigned int height)
    {
      glViewport(0,0, width, height);
      glScissor(0,0, width, height);
    }

    void ContextWindow::StartFrame()
    {
      //set background color
      glClearColor(0,0,0,0);
      //clear current buffer
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void ContextWindow::EndFrame()
    {
      //swap all the buffers for the next frame
      glfwSwapBuffers(GetWindow());

    }

}