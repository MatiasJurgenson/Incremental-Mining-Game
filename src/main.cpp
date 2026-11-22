#include <iostream>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "perlin.hpp"
#include "shader_utils.hpp"

#include "geometry.hpp"
#include "character.hpp"
#include "characterRenderer.hpp"
#include "collision.hpp"

// for now define these as macros
#define WORLD_WIDTH  64
#define WORLD_HEIGHT 256
#define PERLIN_WIDTH WORLD_WIDTH / 5
#define PERLIN_HEIGHT WORLD_HEIGHT / 5
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

#define CONCAT_PATHS(A, B) A "/" B
#define ADD_ROOT(B) CONCAT_PATHS(TASK_ROOT_PATH, B)

std::vector<double> world(WORLD_WIDTH * WORLD_HEIGHT, 0);
shader_prog shader(ADD_ROOT("shaders/block.vert.glsl"), ADD_ROOT("shaders/block.frag.glsl"));


// ---------- camera movement ----------

struct Camera {
  glm::vec3 position;
  glm::mat4 projection;
  glm::mat4 view; 
  Camera(glm::mat4 in_projection, glm::mat4 in_view) {
    position = glm::vec3(0, 0, 5);
    projection = in_projection;
    view = in_view;
  }
};

std::map<int, bool> keyboard = std::map<int, bool>();

void init_keyboard(std::map<int, bool>& keyboard) {
  keyboard.insert(std::make_pair(GLFW_KEY_UP, false));
  keyboard.insert(std::make_pair(GLFW_KEY_DOWN, false));
  keyboard.insert(std::make_pair(GLFW_KEY_LEFT, false));
  keyboard.insert(std::make_pair(GLFW_KEY_RIGHT, false));
  keyboard.insert(std::make_pair(GLFW_KEY_W, false));
  keyboard.insert(std::make_pair(GLFW_KEY_A, false));
  keyboard.insert(std::make_pair(GLFW_KEY_S, false));
  keyboard.insert(std::make_pair(GLFW_KEY_D, false));
}

static void input(float delta_time, float speed, Camera* camera, Character& character) {
  // Camera
  if (keyboard.at(GLFW_KEY_UP)) {
    camera->position.y -= speed * delta_time;
  }
  if (keyboard.at(GLFW_KEY_DOWN)) {
    camera->position.y += speed * delta_time;
  }
  if (keyboard.at(GLFW_KEY_LEFT)) {
    camera->position.x -= speed * delta_time;
  }
  if (keyboard.at(GLFW_KEY_RIGHT)) {
    camera->position.x += speed * delta_time;
  }

  // Character
  glm::vec2 direction(0.0f);
  if (keyboard.at(GLFW_KEY_W)) {
    character.jump();
  }
  if (keyboard.at(GLFW_KEY_A)) {
    direction.x -= 1.0f;
  }
  //if (keyboard.at(GLFW_KEY_S)) {
    //direction.y +=1.0f;
  //}
  if (keyboard.at(GLFW_KEY_D)) {
    direction.x += 1.0f;
  }
  character.move(direction);
}

void update_camera(float delta_time, Camera* camera) {
  camera->view = glm::translate(glm::mat4(1.f), -camera->position);
}

// ---------- camera movement ----------


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    if (key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (keyboard.find(key) != keyboard.end()) {
      keyboard.at(key) = true;
    }
  }
  if (action == GLFW_RELEASE) {
    if (keyboard.find(key) != keyboard.end()) {
      keyboard.at(key) = false;
    }
  }
}

int main(int argc, char* argv[]) {
  GLFWwindow* win;

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Incremental mining game", NULL, NULL);

  if (!win) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(win);
  glewExperimental = GL_TRUE;
  GLenum status = glewInit();

  if (status != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(status));
  }

  init_keyboard(keyboard);

  Camera* main_camera = new Camera(
    glm::ortho(0.f, static_cast<float>(WINDOW_WIDTH),
               static_cast<float>(WINDOW_HEIGHT), 0.f,
               -10.f, 10.f),
    glm::mat4(1.f)
  );

  glfwSetKeyCallback(win, key_callback);

  shader.use();

  generate_world(world, WORLD_WIDTH, WORLD_HEIGHT, PERLIN_WIDTH, PERLIN_HEIGHT);
  block_renderer block = block_renderer(shader);

  // Character
  Quad charModel = createQuad();
  Character character = Character(glm::vec2(640.0f, 50.0f));
  CharacterRenderer renderChar = CharacterRenderer(shader, charModel);
  Collision playerCollision = {
    character.position,
    character.size
  };

  glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // background color
  double previous_time = glfwGetTime();
  // main game loop
  while (!glfwWindowShouldClose(win)) {
    glClear(GL_COLOR_BUFFER_BIT);
    double current_time = glfwGetTime();
    double delta_time = current_time - previous_time;

    input(delta_time, 300.f, main_camera, character);

    update_camera(delta_time, main_camera);

    shader.uniformMatrix4fv("projection", main_camera->projection);
    shader.uniformMatrix4fv("view", main_camera->view);

    draw_world(world, WORLD_WIDTH, WORLD_HEIGHT, block, 20.f, .5f);

    // Character
    character.update(delta_time);
    
    // Collision: x-axis
    character.position.x += character.velocity.x * delta_time;
    resolveCollision(character, playerCollision, world, 20.0f, WORLD_WIDTH, WORLD_HEIGHT, Axis::X);

    // Collision: y-axis
    character.position.y += character.velocity.y * delta_time;
    character.onGround = false;
    resolveCollision(character, playerCollision, world, 20.0f, WORLD_WIDTH, WORLD_HEIGHT, Axis::Y);

    renderChar.draw(character);

    glfwSwapBuffers(win);
    glfwPollEvents();
    previous_time = current_time;
  }

  destroyQuad(charModel);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
