cmake_minimum_required(VERSION 3.19)

# set the name of the project
project(EpsilonRenderer)

# set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# include the library directory
add_subdirectory(lib)

#Any files that are used by all specifications will be here
set(EPSILON_CORE_FILES
        src/main.cpp
        src/App.cpp
        src/App.h
        src/ContextWindow.cpp
        src/ContextWindow.h
        src/RenderSystem.cpp
        src/RenderSystem.h
        src/Mesh.cpp
        src/Mesh.h
        src/Shader.cpp
        src/Shader.h
        src/Object.cpp
        src/Object.h
        src/ObjectManager.cpp
        src/ObjectManager.h
        src/Transform.cpp
        src/Transform.h
        src/Math.h
        src/Epsilon.h
        src/Transform.cpp
        src/Transform.h
        src/ImGui/ImGuiContext.cpp
        src/ImGui/ImGuiContext.h)

#OpenGL related files
set(EPSILON_OPENGL_FILES
        src/OpenGL/glShader.cpp
        src/OpenGL/glShader.h
        src/OpenGL/OpenGLContextWindow.cpp
        src/OpenGL/OpenGLContextWindow.h
        src/OpenGL/OpenGLException.h
        src/OpenGL/OpenGLRenderSystem.cpp
        src/OpenGL/OpenGLRenderSystem.h)

#Vulkan files
set(EPSILON_VULKAN_FILES
        src/Vulkan/VulkanContextWindow.cpp
        src/Vulkan/VulkanContextWindow.h
        src/Vulkan/VulkanShader.cpp
        src/Vulkan/VulkanShader.h
        src/Vulkan/VulkanSwapChain.cpp
        src/Vulkan/VulkanSwapChain.h
        src/Vulkan/VulkanInstance.cpp
        src/Vulkan/VulkanInstance.h
        src/Vulkan/VulkanException.h
        src/Vulkan/VulkanQueueFamilies.h
        src/SwapChainContext.h
        src/Vulkan/VulkanDevice.cpp
        src/Vulkan/VulkanDevice.h
        src/Vulkan/VulkanSurface.cpp
        src/Vulkan/VulkanSurface.h
        src/Vulkan/VulkanMeshHelper.h
        src/Vulkan/VulkanMesh.cpp
        src/Vulkan/VulkanMesh.h
        src/Vulkan/VulkanBuffer.cpp
        src/Vulkan/VulkanBuffer.h
        src/Vulkan/VulkanMeshHelper.cpp
        src/Vulkan/VulkanRenderPass.cpp
        src/Vulkan/VulkanRenderPass.h
        src/Vulkan/VulkanCommandBuffer.cpp
        src/Vulkan/VulkanCommandBuffer.h
        src/Vulkan/VulkanCommandPool.cpp
        src/Vulkan/VulkanCommandPool.h
        src/Vulkan/VulkanDescriptorPool.cpp
        src/Vulkan/VulkanDescriptorPool.h
        src/Vulkan/VulkanRenderSystem.cpp
        src/Vulkan/VulkanRenderSystem.h)

# add all necessary files for the executable project
add_executable(EpsilonRenderer ${EPSILON_CORE_FILES} ${EPSILON_VULKAN_FILES} ${EPSILON_OPENGL_FILES})

#set compiler flags
if(MSVC)
    #windows compiler flags
    set_target_properties(EpsilonRenderer PROPERTIES COMPILE_FLAGS "/W4 /WX /arch:AVX2 /Od")
else()
    # linux compile flags
    set_target_properties(EpsilonRenderer PROPERTIES COMPILE_FLAGS "-Wall -Werror -Wextra  \
    -Wno-missing-field-initializers -Wno-maybe-uninitialized -Wno-unused-parameter")
endif()
# add access to Vulkan/Vulkan
find_package(Vulkan REQUIRED)

#add preprocessor directive

# set preprocessor directive that are cross platform
set(PREPROCESSOR_DIR GLFW_INCLUDE_VULKAN GLFW_INCLUDE_NONE IMGUI_IMPL_OPENGL_LOADER_GLAD)

# set platform specific directives
if(MSVC)
    set(PREPROCESSOR_DIR ${PREPROCESSOR_DIR} VK_USE_PLATFORM_WIN32_KHR)
elseif(UNIX)
    set(PREPROCESSOR_DIR ${PREPROCESSOR_DIR} VK_USE_PLATFORM_XCB_KHR GLFW_EXPOSE_NATIVE_XCB)
endif()


# set the preprocessor directives to the project
target_compile_definitions(EpsilonRenderer PRIVATE ${PREPROCESSOR_DIR})

# link the available libraries
target_link_libraries(EpsilonRenderer glfw Vulkan::Vulkan glm assimp imgui glad)