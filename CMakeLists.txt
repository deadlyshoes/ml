project(ML)

include_directories(C:/opt/glad/include)
add_library(
        glad
        C:/opt/glad/src/glad.c
)

include_directories(C:/opt/glfw-3.3.6.bin.WIN64/include)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
include_directories(
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backend
)
add_library(
    imgui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)

find_package(glm CONFIG REQUIRED)

add_executable(ml main.cpp Shader.cpp Shader.h render.h render.cpp nn.h nn.cpp node.vert node.frag)
target_link_libraries(ml glad C:/opt/glfw-3.3.6.bin.WIN64/lib-vc2022/glfw3.lib glm::glm imgui)
