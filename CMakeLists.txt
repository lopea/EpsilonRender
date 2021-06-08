cmake_minimum_required(VERSION 3.19)

# set the name of the project
project(EpsilonRenderer)

# set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# include the library directory
add_subdirectory(lib)

# add all necessary files for the executable project
add_executable(EpsilonRenderer main.cpp Context.cpp Context.h ContextWindow.cpp ContextWindow.h VulkanContextWindow.cpp VulkanContextWindow.h Shader.cpp Shader.h VulkanShader.cpp VulkanShader.h VulkanSwapChain.cpp VulkanSwapChain.h VulkanInstance.cpp VulkanInstance.h VulkanException.h VulkanQueueFamilies.h SwapChainContext.h VulkanDevice.cpp VulkanDevice.h VulkanSurface.cpp VulkanSurface.h Mesh.cpp Mesh.h VulkanMeshHelper.h VulkanMesh.cpp VulkanMesh.h Renderer.cpp Renderer.h VulkanBuffer.cpp VulkanBuffer.h)

set_target_properties(EpsilonRenderer PROPERTIES COMPILE_FLAGS "/W4 /WX")

# add access to Vulkan
find_package(Vulkan REQUIRED)

#add preprocessor directive
target_compile_definitions(EpsilonRenderer PRIVATE VK_USE_PLATFORM_WIN32_KHR GLFW_INCLUDE_VULKAN GLFW_EXPOSE_NATIVE_WIN32)

# link the available libraries
target_link_libraries(EpsilonRenderer glfw Vulkan::Vulkan glm assimp)