#ifndef POPUP_H
#define POPUP_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Text.h"

class Popup
{
public:
    Popup(float x, float y, float width, float height, std::string content);

    void draw();
private:
    float x;
    float y;
    float width;
    float height;

    std::string content;

    Shader shader = Shader("shaders/popup.vert", "shaders/popup.frag");
    Text textRenderer = Text();

    unsigned int windowVAO, windowVBO;
};

#endif // POPUP_H
