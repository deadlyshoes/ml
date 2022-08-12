#ifndef TEXT_H
#define TEXT_H

#include <glad/glad.h>

#include <iostream>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class Text
{
public:
    Text();

    void draw(std::string text, float x, float y, float scale, glm::vec3 color);
private:
    struct Character {
        unsigned int TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        unsigned int Advance;
    };
    std::map<char, Character> Characters;

    Shader shader = Shader("shaders/glyph.vert", "shaders/glyph.frag");

    unsigned int textVAO, textVBO;
};

#endif // TEXT_H
