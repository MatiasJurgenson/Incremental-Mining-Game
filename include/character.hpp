#pragma once
#include "glm/glm.hpp"

class Character {
    public:
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec2 moveDirection;
        glm::vec2 size;
        float acceleration;
        float deceleration;
        float maxSpeed;
        float gravity;
        bool onGround;
        float jumpStrength;

        Character(glm::vec2 startPosition);

        void update(float deltaTime);
        void move(glm::vec2 direction);
        void jump();
};