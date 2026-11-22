#include "characterRenderer.hpp"
#include <stack>
#include "shader_utils.hpp"

CharacterRenderer::CharacterRenderer(shader_prog& shader, const Quad& quad)
    : shader(shader),
      quad(quad) {

      }

void CharacterRenderer::draw(const Character& character) {
    std::stack<glm::mat4> ms;
    ms.push(glm::mat4(1.0));
        ms.top() = glm::translate(ms.top(), glm::vec3(character.position, 0.0f));
        ms.push(ms.top());
            ms.top() = glm::scale(ms.top(), glm::vec3(character.size.x, character.size.y, 1.0f));
            shader.uniformMatrix4fv("model", ms.top());
            glBindVertexArray(quad.VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
        ms.pop();
    ms.pop();
}