//
// Created by Javier on 5/28/2021.
//

#ifndef EPSILONRENDERER_CONTEXTWINDOW_H
#define EPSILONRENDERER_CONTEXTWINDOW_H

#include <string>
#include <GLFW/glfw3.h>
#include "Epsilon.h"
struct GLFWwindow;

namespace Epsilon
{

//! class that is the blueprint for renderers based on the render specification (GL, Vulkan, DirectX)
    class ContextWindow
    {
    public:
        /*!
         * Context Window Constructor,
         * Will initialize the width and height of the ContextWindow for default use
         * @param width the width of the window in pixel units
         * @param height the height of the window in pixel units
         * @param window window handle that will connected by this context class
         */
        ContextWindow(unsigned width, unsigned height, GLFWwindow *window = nullptr)
            : handle_(window), width_(width), height_(height)
        {if(handle_) PopulateGLFWHandle();}

        //! @return the handle associated with this window
        GLFWwindow *GetWindow()
        { return handle_; };

        //! @return Get the name of the render specification
        virtual SpecificationType GetName() = 0;

        //! making sure destructors work for derived classes
        virtual ~ContextWindow()
        {
          glfwDestroyWindow(handle_);
          handle_ = nullptr;
        }

        [[nodiscard]] unsigned GetWidth() const
        { return width_; }

        [[nodiscard]] unsigned GetHeight() const
        { return height_; }

        [[nodiscard]] bool WillClose() const
        {
          if (!handle_) return true;
          return glfwWindowShouldClose(handle_);
        }

        //delete any use of copy constructors, assignment operators,etc.
        ContextWindow(const ContextWindow &other) = delete;

        ContextWindow &operator=(const ContextWindow &other) = delete;

        //! draws all data to the screen
        virtual void EndFrame() = 0;

        virtual void StartFrame() = 0;

        virtual void OnResize(unsigned width, unsigned height)
            {(void)width;(void)height;}

    protected:
        void SetWindowHandle(GLFWwindow *newWindow);

    private:
        GLFWwindow *handle_;
        //! width of the renderer
        unsigned width_;
        //! height of the window the renderer will use
        unsigned height_;

        friend void glfwOnResize(GLFWwindow* window, int width, int height);

        void PopulateGLFWHandle();
    };
}

#endif //EPSILONRENDERER_CONTEXTWINDOW_H
