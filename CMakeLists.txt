cmake_minimum_required(VERSION 3.19)

# set the name of the project
project(EpsilonRenderer)

# set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# include the library directory
add_subdirectory(lib)

# add all necessary files for the executable project
add_executable(EpsilonRenderer main.cpp Context.cpp Context.h ContextWindow.cpp ContextWindow.h VulkanContextWindow.cpp VulkanContextWindow.h)

set_target_properties(EpsilonRenderer PROPERTIES COMPILE_FLAGS "/W4 /WX")

# add access to Vulkan
find_package(Vulkan REQUIRED)

#add preprocessor directive
target_compile_definitions(EpsilonRenderer PRIVATE VK_USE_PLATFORM_WIN32_KHR GLFW_INCLUDE_VULKAN GLFW_EXPOSE_NATIVE_WIN32)

# link the available libraries
target_link_libraries(EpsilonRenderer glfw Vulkan::Vulkan)