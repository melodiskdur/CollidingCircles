cmake_minimum_required(VERSION 3.12)
project(CollidingCircles)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_DIR src)

file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")

include_directories(
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/dependencies/GLFW/include
    ${CMAKE_SOURCE_DIR}/dependencies/GLEW/include
    ${CMAKE_SOURCE_DIR}/extern
)

# Add GLFW and GLEW as subdirectories.
add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/glew/build/cmake)

# Add ImGui.
add_library(imgui
    extern/imgui/imgui.cpp
    extern/imgui/imgui_impl_glfw.cpp
    extern/imgui/imgui_impl_opengl3.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_demo.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_widgets.cpp
)

# Set the link directories.
link_directories(
    ${CMAKE_SOURCE_DIR}/dependencies/glfw/src
    ${CMAKE_BINARY_DIR}/dependencies/glew/lib
)

# Executable.
add_executable(CollidingCircles ${SOURCE_FILES})

# Link against included libraries.
target_link_libraries(CollidingCircles
    glfw
    glew
    opengl32
    imgui
)

# Copy GLEW DLL to the output directory.
add_custom_command(TARGET CollidingCircles POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/build/bin/Release/glew32.dll"
        $<TARGET_FILE_DIR:CollidingCircles>
)

# Copy shader files to the output directory.
add_custom_command(TARGET CollidingCircles POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res/shader"
        $<TARGET_FILE_DIR:CollidingCircles>/shader
    COMMENT "Copying shader files"
)