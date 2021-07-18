//
// Created by Javier on 5/27/2021.
//

#include "App.h"
#include "Vulkan/VulkanContextWindow.h"
#include "ImGui/ImGuiContext.h"
#include "OpenGL/OpenGLContextWindow.h"
#include "Vulkan/VulkanRenderSystem.h"
#include "OpenGL/OpenGLRenderSystem.h"
#include "RenderCommand.h"


#include <stdexcept>
#include <iostream>
#include <fstream>


std::vector<Epsilon::Vertex> vertices = {
    // first triangle
    {glm::vec3(0.5f, 0.5f, 0.0f)},  // top right
    {glm::vec3(0.5f, -0.5f, 0.0f)},  // bottom right
    {glm::vec3(-0.5f, 0.5f, 0.0f)},  // top left
    {glm::vec3(-0.5f, -0.5f, 0.0f)},  // bottom left
};

std::vector<unsigned int> indices = {  // note that we start from 0!
    0, 2, 1,   // first triangle
    2, 3, 1    // second triangle
};
namespace Epsilon
{
    SpecificationType switchTo_;

    App::App(unsigned width, unsigned height) : handle_(nullptr), renderer_(nullptr)
    {
      ImGuiEnvironment::Initialize();
      try
      {
        //create a new system for rendering vulkan
        renderer_ = new OpenGL::RenderSystem();

        renderer_->PushBackNewWindow(width, height);

        ImGuiEnvironment::LinkSystem(renderer_);


      } catch (std::runtime_error &ex)
      {
        std::cerr << ex.what() << std::endl;
      }
    }

    App::~App()
    {

      ImGuiEnvironment::Shutdown();
      //delete the renderer specification
      delete renderer_;

      //terminate glfw
      glfwTerminate();
    }

    void App::Run()
    {
      ContextWindow *window = renderer_->GetWindow(0);

      std::ifstream vertFile("test.vert.spv", std::ios_base::in | std::ios_base::binary),
      fragFile("test.frag.spv", std::ios_base::in | std::ios_base::binary);
      std::vector<char> vertData((std::istreambuf_iterator<char>(vertFile)),
                                          (std::istreambuf_iterator<char>())),
          fragData((std::istreambuf_iterator<char>(fragFile)), (std::istreambuf_iterator<char>()));

      glShader shader(vertData, fragData);

      OpenGL::Mesh mesh(vertices, indices);

      //run the program until the user wants it to close
      while (!window->WillClose())
      {
        ImGuiEnvironment::StartFrame();
        ImGui::ShowDemoWindow();

        //clear the previous frame
        window->StartFrame();
        dynamic_cast<OpenGL::RenderSystem*>(renderer_)->Render(&shader, mesh);
        //run any renderer specific actions
        ImGuiEnvironment::RenderToContext();
        Update();
        window->EndFrame();

        glfwPollEvents();
      }

    }

    void App::Update()
    {
      objManager_.Update();
    }
}