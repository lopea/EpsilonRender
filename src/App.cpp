//
// Created by Javier on 5/27/2021.
//

#include "App.h"
#include "Vulkan/VulkanContextWindow.h"
#include "ImGui/ImGuiContext.h"
#include "OpenGL/OpenGLContextWindow.h"
#include "Vulkan/VulkanRenderSystem.h"
#include "OpenGL/OpenGLRenderSystem.h"
#include "RenderPipeline.h"

#include <stdexcept>
#include <iostream>
#include <fstream>


std::vector<Epsilon::Vertex> vertices = {
    // first triangle
    {glm::vec3(1, 1, 0.0f),glm::vec2(1)},  // top right
    {glm::vec3(1, -1, 0.0f),glm::vec2(1,0)},  // bottom right
    {glm::vec3(-1, 1, 0.0f),glm::vec2(0,1)},  // top left
    {glm::vec3(-1, -1, 0.0f),glm::vec2(1,0)},  // bottom left
};

std::vector<unsigned int> indices = {  // note that we start from 0!
    0, 2, 1,   // first triangle
    2, 3, 1    // second triangle
};
namespace Epsilon
{
    Vulkan::RenderSystem * system;
    SpecificationType switchTo_;

    App::App(unsigned width, unsigned height) : handle_(nullptr), renderer_(nullptr)
    {
      try
      {

        //create a new system for rendering vulkan
        renderer_ = new OpenGL::RenderSystem();

        renderer_->PushBackNewWindow(width, height);




      } catch (std::runtime_error &ex)
      {
        std::cerr << ex.what() << std::endl;
      }
    }

    App::~App()
    {


      //delete the renderer specification
      delete renderer_;

      delete system;

      //terminate glfw
      glfwTerminate();
    }

    void App::Run()
    {
      ContextWindow *window = renderer_->GetWindow(0);

      std::ifstream vertFile("Shaders/test.vert.spv", std::ios_base::in | std::ios_base::binary),
      fragFile("Shaders/test.frag.spv", std::ios_base::in | std::ios_base::binary);

      std::vector<char> vertData((std::istreambuf_iterator<char>(vertFile)),
                                          (std::istreambuf_iterator<char>())),
          fragData((std::istreambuf_iterator<char>(fragFile)), (std::istreambuf_iterator<char>()));



      ShaderModule vertModule("test.vert", SHADER_STAGE_VERTEX, vertData),
                  fragModule("test.frag", SHADER_STAGE_FRAGMENT, fragData);

      std::vector<ShaderModule*> modules { &vertModule, &fragModule};

      Epsilon::RenderPipeline pipeline("Test Shader", modules);

      Epsilon::Material material;
      material.AddPass(&pipeline);
      OpenGL::Mesh mesh(vertices, indices);

      float data{};
      //run the program until the user wants it to close
      while (!window->WillClose())
      {

        data = static_cast<float>(glfwGetTime());

        //clear the previous frame
        window->StartFrame();
        material.SetUniformData(3, sizeof(float), &data);
        dynamic_cast<OpenGL::RenderSystem*>(renderer_)->Render(material, mesh);
        //run any renderer specific actions
        Update();
        window->EndFrame();


        glfwPollEvents();
      }

    }

    void App::Update()
    {
    }
}