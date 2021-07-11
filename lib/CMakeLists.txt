# add access to GLFW
add_subdirectory(glfw)

#add access to glm
add_subdirectory(glm)

#add access to assimp
add_subdirectory(assimp)

#add documentation
add_subdirectory(docs)

#include gl extensions
add_subdirectory(glad)

#add library to link with imgui
add_library(imgui INTERFACE)

target_sources(imgui INTERFACE
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/misc/cpp/imgui_stdlib.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_vulkan.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/imgui_tables.cpp)

target_include_directories(imgui INTERFACE imgui ${GLFW_INCLUDE_DIR})

target_link_directories(imgui INTERFACE ${GLFW_LIBRARY})


