//
// Created by Javier on 5/28/2021.
//

#ifndef EPSILONRENDERER_CONTEXTWINDOW_H
#define EPSILONRENDERER_CONTEXTWINDOW_H
#include <string>
#include <GLFW/glfw3.h>
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
         */
        ContextWindow(unsigned width, unsigned height): width_(width), height_(height), handle_(nullptr){}

        //! @return the handle associated with this window
        virtual GLFWwindow *GetWindow(){return handle_;};

        //! @return Get the name of the render specification
        virtual std::string GetName() = 0;

        //! making sure destructors work for derived classes
        virtual ~ContextWindow(){ glfwDestroyWindow(handle_); handle_ = nullptr; }

        [[nodiscard]] unsigned GetWidth() const { return width_;}
        [[nodiscard]] unsigned GetHeight() const { return height_;}

        [[nodiscard]] bool WillClose() const { if(!handle_) return true; return glfwWindowShouldClose(handle_);}

        //delete any use of copy constructors, assignment operators,etc.
        ContextWindow(const ContextWindow& other) = delete;
        ContextWindow& operator=(const ContextWindow& other) = delete;

    protected:
        void SetWindowHandle(GLFWwindow* newWindow) { handle_ = newWindow;};
    private:
        GLFWwindow* handle_;
        //! width of the renderer
        unsigned width_;
        //! height of the window the renderer will use
        unsigned height_;
    };
}

#endif //EPSILONRENDERER_CONTEXTWINDOW_H