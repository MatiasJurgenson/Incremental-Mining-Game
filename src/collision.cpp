#include "collision.hpp"
#include "character.hpp"
#include "glm/glm.hpp"
#include <vector>

CollisionResult intersects(const Collision& a, const Collision& b) {
    float moveUp = 0.0f;
    float moveDown = 0.0f;
    float moveLeft = 0.0f;
    float moveRight = 0.0f;
    float min_move_y = 0.0f;
    float min_move_x = 0.0f;

    float top_a = a.position.y;
    float bot_b = b.position.y + b.size.y;
    float bot_a = a.position.y + a.size.y;
    float top_b = b.position.y;

    bool yCollision = top_a < bot_b && bot_a > top_b;
    if(yCollision) {
        moveUp = top_b - bot_a;
        moveDown = bot_b - top_a;
        min_move_y = std::abs(moveUp) < std::abs(moveDown) ? moveUp : moveDown;
    }

    float left_a = a.position.x;
    float right_b = b.position.x + b.size.x;
    float right_a = a.position.x + a.size.x;
    float left_b = b.position.x;

    bool xCollision = left_a < right_b && right_a > left_b;
    if(xCollision) {
        moveLeft = left_b - right_a;
        moveRight = right_b - left_a;
        min_move_x = std::abs(moveLeft) < std::abs(moveRight) ? moveLeft : moveRight;
    }

    if(yCollision && xCollision) {
        return {true, glm::vec2(min_move_x, min_move_y)};
    }

    return {false, glm::vec2(0.0f)};
}

void resolveCollision(Character& character, 
                      Collision& playerCollision, 
                      const std::vector<double>& world, 
                      float block_size, 
                      int world_width, 
                      int world_height, 
                      Axis axis) {

    playerCollision.position = character.position;

    glm::ivec2 worldPosMin = glm::ivec2(
      character.position.x / block_size, 
      character.position.y / block_size);
    glm::ivec2 worldPosMax = glm::ivec2(
      (character.position.x + character.size.x) / block_size, 
      (character.position.y + character.size.y) / block_size);

    for(int x=worldPosMin.x; x <= worldPosMax.x; x++) {
      for(int y=worldPosMin.y; y <= worldPosMax.y; y++) {
        if(x >= 0 && x < world_width && y >= 0 && y < world_height) {
          float current_tile = world[y * world_width +x];
          if(current_tile > 0.5f) {
            glm::vec2 tile_pos = glm::vec2(x * block_size, y * block_size); 
            Collision tile_col = {
              tile_pos,
              glm::vec2(block_size)
            };
            CollisionResult col_res = intersects(playerCollision, tile_col);
            if (col_res.isColliding) {
              if(axis == Axis::X) {
                character.position.x += col_res.overlap.x;
                character.velocity.x = 0.0f;
              }
              else if(axis == Axis::Y) {
                character.position.y += col_res.overlap.y;
                if (character.velocity.y > 0.0f && col_res.overlap.y < 0.0f) {
                    character.onGround = true;
                }
                character.velocity.y = 0.0f;
              }
            }

            playerCollision.position = character.position;
          }
        }
      }
    }               
}