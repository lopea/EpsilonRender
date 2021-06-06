# add access to GLFW
add_subdirectory(glfw)

#add access to glm
add_subdirectory(glm)

#add access to assimp
add_subdirectory(assimp)

#add library to link with imgui
add_library(imgui INTERFACE
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/misc/cpp/imgui_stdlib.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/imgui_tables.cpp)


