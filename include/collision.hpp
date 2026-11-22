#pragma once

#include <glm/glm.hpp>
#include "character.hpp"
#include <vector>

enum class Axis {
    X,
    Y
};

struct Collision {
    glm::vec2 position;
    glm::vec2 size;
};

struct CollisionResult {
    bool isColliding;
    glm::vec2 overlap;
};


CollisionResult intersects(const Collision& a, const Collision& b);

void resolveCollision(Character& character,
                      Collision& playerCollision, 
                      const std::vector<double>& world, 
                      float block_size, 
                      int world_width, 
                      int world_height, 
                      Axis axis);
