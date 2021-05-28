cmake_minimum_required(VERSION 3.19)

# set the name of the project
project(EpsilonRenderer)

# set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# include the library directory
add_subdirectory(lib)

# add all necessary files for the executable project
add_executable(EpsilonRenderer main.cpp Context.cpp Context.h)

# add access to Vulkan
find_package(Vulkan REQUIRED)

# link the available libraries
target_link_libraries(EpsilonRenderer glfw Vulkan::Vulkan)