#include "geometry.hpp"

Quad createQuad() {
    GLfloat vertices[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f
    };

    GLubyte indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // attribute index (matches your vertex shader)
        2,                  // number of components (x, y)
        GL_FLOAT,           // data type
        GL_FALSE,           // should OpenGL normalize values?
        2 * sizeof(float),  // stride: total size of one vertex
        (void*)0            // offset: where this attribute starts
    );
    glEnableVertexAttribArray(0);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    return {vao, vbo, ebo};
}

void destroyQuad(const Quad& quad) {
    glDeleteVertexArrays(1, &quad.VAO);
    glDeleteBuffers(1, &quad.VBO);
    glDeleteBuffers(1, &quad.EBO);
}