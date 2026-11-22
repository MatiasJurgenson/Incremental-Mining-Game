#pragma once

#include "geometry.hpp"
#include "shader_utils.hpp"
#include "character.hpp"

class CharacterRenderer {
private:
    shader_prog& shader;
    const Quad& quad;

public:
    CharacterRenderer(shader_prog& shader, const Quad& quad);

    void draw(const Character& character);
};