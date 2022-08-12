#include "Popup.h"

Popup::Popup(float x, float y, float width, float height, std::string content)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->content = content;

    float vertices[6][2] = {
        {x, y},
        {x + width, y},
        {x + width, y + height},

        {x, y},
        {x, y + height},
        {x + width, y + height}
    };

    glGenVertexArrays(1, &windowVAO);
    glGenBuffers(1, &windowVBO);

    glBindVertexArray(windowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glBindVertexArray(0);

}

void Popup::draw()
{
    shader.use();
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    shader.setMat4("projection", projection);
    glBindVertexArray(windowVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    textRenderer.draw(content, x, y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));


}
