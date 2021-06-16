
#find doxygen if it exists
find_package(Doxygen)

# if it does exist,
if(DOXYGEN_FOUND)

    # state message
    message(STATUS "Epsilon Message: Doxygen found.")

    # set files for doxygen
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    #execute file
    add_custom_target(Epsilon_Documentation ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Creating documentation for Epsilon Renderer"
            VERBATIM)
else()
    message(STATUS "Espilon Warning: Unable to find Doxygen. Documentation will be unavailable.")
endif()