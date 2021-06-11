cmake_minimum_required(VERSION 3.19)

# set the name of the project
project(EpsilonRenderer)

# set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# include the library directory
add_subdirectory(lib)

# add all necessary files for the executable project
add_executable(EpsilonRenderer
        main.cpp
        App.cpp
        App.h
        ContextWindow.cpp
        ContextWindow.h
        Vulkan/VulkanContextWindow.cpp
        Vulkan/VulkanContextWindow.h
        Shader.cpp
        Shader.h
        Vulkan/VulkanShader.cpp
        Vulkan/VulkanShader.h
        Vulkan/VulkanSwapChain.cpp
        Vulkan/VulkanSwapChain.h
        Vulkan/VulkanInstance.cpp
        Vulkan/VulkanInstance.h
        Vulkan/VulkanException.h
        Vulkan/VulkanQueueFamilies.h
        SwapChainContext.h
        Vulkan/VulkanDevice.cpp
        Vulkan/VulkanDevice.h
        Vulkan/VulkanSurface.cpp
        Vulkan/VulkanSurface.h
        Mesh.cpp
        Mesh.h
        Vulkan/VulkanMeshHelper.h
        Vulkan/VulkanMesh.cpp
        Vulkan/VulkanMesh.h
        Renderer.cpp
        Renderer.h
        Vulkan/VulkanBuffer.cpp
        Vulkan/VulkanBuffer.h
        Object.cpp
        Object.h
        ObjectManager.cpp
        ObjectManager.h
        Vulkan/VulkanMeshHelper.cpp 
        Vulkan/VulkanRenderPass.cpp
        Vulkan/VulkanRenderPass.h
        Vulkan/VulkanCommandBuffer.cpp
        Vulkan/VulkanCommandBuffer.h
        Vulkan/VulkanCommandPool.cpp
        Vulkan/VulkanCommandPool.h
        ImGui/ImGuiContext.cpp
        ImGui/ImGuiContext.h
        Vulkan/VulkanDescriptorPool.cpp
        Vulkan/VulkanDescriptorPool.h)

#set compiler flags
if(MSVC)
    #windows compiler flags
    set_target_properties(EpsilonRenderer PROPERTIES COMPILE_FLAGS "/W4 /WX")
else()
    set_target_properties(EpsilonRenderer PROPERTIES COMPILE_FLAGS "-Wall -Werror -Wextra -pedantic \
    -O -Wno-missing-field-initializers -Wno-maybe-uninitialized -Wno-unused-parameter")
endif()
# add access to Vulkan/Vulkan
find_package(Vulkan REQUIRED)

#add preprocessor directive

# set preprocessor directive that are cross platform
set(PREPROCESSOR_DIR GLFW_INCLUDE_VULKAN)

# set platform specific directives
if(MSVC)
    set(PREPROCESSOR_DIR ${PREPROCESSOR_DIR} VK_USE_PLATFORM_WIN32_KHR)
elseif(UNIX)
    set(PREPROCESSOR_DIR ${PREPROCESSOR_DIR} VK_USE_PLATFORM_XCB_KHR GLFW_EXPOSE_NATIVE_XCB)
endif()


# set the preprocessor directives to the project
target_compile_definitions(EpsilonRenderer PRIVATE ${PREPROCESSOR_DIR})

# link the available libraries
target_link_libraries(EpsilonRenderer glfw Vulkan::Vulkan glm assimp imgui)