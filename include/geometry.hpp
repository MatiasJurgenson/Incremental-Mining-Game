#pragma once
#include <GL/glew.h>

struct Quad {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

Quad createQuad();
void destroyQuad(const Quad& quad);