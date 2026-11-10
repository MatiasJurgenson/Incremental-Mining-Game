#ifndef TEXTURE_UTIL_H
#define TEXTURE_UTIL_H

#include <GL/glew.h>

GLuint load_texture(GLenum target, GLint internalFormat, const char* filename);

#endif
