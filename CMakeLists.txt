cmake_minimum_required(VERSION 3.19)

# set the name of the project
project(EpsilonRenderer)

# set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# include the library directory
add_subdirectory(lib)

# add all necessary files for the executable project
add_executable(EpsilonRenderer main.cpp Context.cpp Context.h ContextWindow.cpp ContextWindow.h VulkanContextWindow.cpp VulkanContextWindow.h Shader.cpp Shader.h VulkanShader.cpp VulkanShader.h VulkanSwapChain.cpp VulkanSwapChain.h VulkanInstance.cpp VulkanInstance.h VulkanException.h VulkanQueueFamilies.h SwapChainContext.h VulkanDevice.cpp VulkanDevice.h VulkanSurface.cpp VulkanSurface.h Mesh.cpp Mesh.h VulkanMeshHelper.h VulkanMesh.cpp VulkanMesh.h Renderer.cpp Renderer.h VulkanBuffer.cpp VulkanBuffer.h Object.cpp Object.h ObjectManager.cpp ObjectManager.h VulkanMeshHelper.cpp)

#set compiler flags
if(MSVC)
    #windows compiler flags
    set_target_properties(EpsilonRenderer PROPERTIES COMPILE_FLAGS "/W4 /WX")
else()
    set_target_properties(EpsilonRenderer PROPERTIES COMPILE_FLAGS "-Wall -Werror -Wextra -pedantic \
    -O -Wno-missing-field-initializers -Wno-maybe-uninitialized -Wno-unused-parameter")
endif()
# add access to Vulkan
find_package(Vulkan REQUIRED)

#add preprocessor directive

# set preprocessor directive that are cross platform
set(PREPROCESSOR_DIR GLFW_INCLUDE_VULKAN)

# set platform specific directives
if(MSVC)
    set(PREPROCESSOR_DIR ${PREPROCESSOR_DIR} VK_USE_PLATFORM_WIN32_KHR  GLFW_EXPOSE_NATIVE_WIN32)
elseif(UNIX)
    set(PREPROCESSOR_DIR ${PREPROCESSOR_DIR} VK_USE_PLATFORM_XCB_KHR GLFW_EXPOSE_NATIVE_XCB)
endif()


# set the preprocessor directives to the project
target_compile_definitions(EpsilonRenderer PRIVATE ${PREPROCESSOR_DIR})

# link the available libraries
target_link_libraries(EpsilonRenderer glfw Vulkan::Vulkan glm assimp)