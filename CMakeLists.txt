project(ML)

include_directories(/home/ilmar/opt/glad/include)
add_library(
        glad
        /home/ilmar/opt/glad/src/glad.c
)

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(ml main.cpp Text.h Text.cpp Popup.h Popup.cpp Shader.cpp Shader.h render.h render.cpp nn.h nn.cpp glyph.vert glyph.frag node.vert node.frag popup.vert popup.frag )
target_link_libraries(ml glad glfw freetype)
