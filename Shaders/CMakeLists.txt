file(GLOB_RECURSE VERTEX_SHADERS *.vert)
file(GLOB_RECURSE FRAGMENT_SHADERS *.frag)
set(SPV_FILES)
foreach(SHADER_FILE IN ITEMS ${VERTEX_SHADERS} ${FRAGMENT_SHADERS})
    message(${SHADER_FILE})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WLE)
    get_filename_component(SHADER_EXT ${SHADER_FILE} LAST_EXT)
    set(SHADER_OUTPUT_FILE "${SHADER_NAME}${SHADER_EXT}.spv")
    message(${CMAKE_CURRENT_BINARY_DIR})
    set(SPV_FILES ${SPV_FILES} "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_OUTPUT_FILE}")
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_OUTPUT_FILE}"
            COMMAND "glslangValidator"
            ARGS -G
            -V
            ${SHADER_FILE}
            -o ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_OUTPUT_FILE}
            DEPENDS ${SHADER_FILE})
endforeach()


add_custom_target(spv_shaders DEPENDS ${SPV_FILES})