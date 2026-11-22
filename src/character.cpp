#include "character.hpp"
#include "glm/glm.hpp"
#include <algorithm>
#include <cmath>

Character::Character(glm::vec2 startPosition)
    : position(startPosition),
      velocity(glm::vec2(0.0f, 0.0f)),
      moveDirection(glm::vec2(0.0f, 0.0f)),
      size(glm::vec2(32.0f, 48.0f)),
      acceleration(500.0f),
      deceleration(700.0f),
      maxSpeed(200.0f),
      gravity(1000.0f),
      onGround(false),
      jumpStrength(350.0f) {}

void Character::update(float deltaTime) {
    velocity.x += moveDirection.x * acceleration * deltaTime;
    velocity.x = glm::clamp(velocity.x, -maxSpeed, maxSpeed);

    float speed = std::abs(velocity.x);

    if(moveDirection.x == 0.0f) {
        float newSpeed = std::max(0.0f, speed - deceleration * deltaTime);
        if(speed != 0.0f) {
            velocity.x = std::copysign(newSpeed, velocity.x);
        }
    }

    velocity.y += gravity * deltaTime;
}

void Character::move(glm::vec2 direction) {
    moveDirection = direction;
}

void Character::jump() {
    if(onGround) {
        velocity.y = -jumpStrength;
        onGround = false;
    }
}