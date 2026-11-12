#include "texture_util.h"

#include <cstdio>
#include <stdexcept>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint load_texture(GLenum target, GLint internalFormat, const char* filename) {
    int width = 0;
    int height = 0;
    int channels = 0;

    unsigned char* pixels = stbi_load(filename, &width, &height, &channels, 0);
    if (pixels == nullptr) {
        throw std::runtime_error(
            std::string("Failed to load image ") + filename + ": " + stbi_failure_reason()
        );
    }

    GLenum format = GL_RGB;
    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 4) {
        format = GL_RGBA;
    } else if (channels != 3) {
        stbi_image_free(pixels);
        throw std::runtime_error("Unsupported image channel count in " + std::string(filename));
    }

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(target, texture);
    glTexImage2D(target, 0, internalFormat, width, height, 0, format,
                 GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(target);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(pixels);
    glBindTexture(target, 0);
    return texture;
}
