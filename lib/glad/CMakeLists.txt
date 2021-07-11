
add_library(glad INTERFACE)

# get all the source files
file(GLOB GLAD_SRC src/*.c)

target_sources(glad INTERFACE ${GLAD_SRC})

target_include_directories(glad INTERFACE glad include)