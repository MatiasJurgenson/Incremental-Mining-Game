#include "shader_utils.hpp"
#include <stdexcept>
#include <cerrno>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstring>
#include <stdlib.h>

#include "texture_util.h"
using std::strcpy;


// ---------- code taken from labs ----------

// -------- Utility functions --------------
/**
 * Reads file contents into a string.
 */
std::string get_file_contents(const char *filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) return std::string((std::istreambuf_iterator<char>(in)),
                                std::istreambuf_iterator<char>());
    else throw(std::runtime_error(std::string("Failed to read file ") + filename));
}

block_renderer::block_renderer(shader_prog& sp) : shader(sp) {
    this->rocksTexture = load_texture(GL_TEXTURE_2D, GL_RGBA, TASK_ROOT_PATH "/data/Rocks.png");

    this->init_block();
}

/**
 * Allocates and compiles given shader in OpenGL
 */
GLuint compile(GLuint type, std::string source) {
    GLuint shader = glCreateShader(type);

    // Split the code into separate lines (then the compilation error messages are more informative)
    std::vector<GLchar *> lines;
    std::string line;
    std::istringstream ss(source);
    while(getline(ss, line)) {
        line += '\n';
        GLchar* c_line = (GLchar*)malloc(line.size()+1);
        strcpy(c_line, line.c_str());
        lines.push_back(c_line);
    }

    // Compile the source
    glShaderSource(shader, lines.size(), (const GLchar**)&lines[0], NULL);
    glCompileShader(shader);
    for (unsigned int i = 0; i < lines.size(); i++) free(lines[i]);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetShaderInfoLog(shader, length, &length, &log[0]);
        std::cout << "Shader compilation error: " << log << std::endl;
        throw std::logic_error(log);
        return false;
    }
    return shader;
}

/**
 * Default shaders.
 */
const GLchar *default_vertex_shader =
    "#version 120\n"
    "varying vec4 vertex_color\n;"
    "void main(void) {\n"
    "    gl_Position = ftransform();\n"
    "    vertex_color = gl_Color;\n"
    "}";

const GLchar *default_fragment_shader = // Only supports coloring. Textures require custom stuff.
    "#version 120\n"
    "varying vec4 vertex_color;\n"
    "void main() {\n"
    "    gl_FragColor = vertex_color;\n"
    "}";

shader_prog::shader_prog(const char* vertex_shader_filename, const char* fragment_shader_filename) {
    v_source = vertex_shader_filename == NULL ? std::string((const char*)default_vertex_shader) : get_file_contents(vertex_shader_filename);
    f_source = fragment_shader_filename == NULL ? std::string((const char*)default_fragment_shader) : get_file_contents(fragment_shader_filename);
}

void shader_prog::use() {
    vertex_shader = compile(GL_VERTEX_SHADER, v_source);
    fragment_shader = compile(GL_FRAGMENT_SHADER, f_source);
    prog = glCreateProgram();
    glAttachShader(prog, vertex_shader);
    glAttachShader(prog, fragment_shader);
    glLinkProgram(prog);
    glUseProgram(prog);
}

void shader_prog::free() {
    glDeleteProgram(prog);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glUseProgram(0);
}

shader_prog::operator GLuint() {
    return prog;
}

void shader_prog::uniform1i(const char* name, int i) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniform1i(loc, i);
}
void shader_prog::uniform1f(const char* name, float f) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniform1f(loc, f);
}
void shader_prog::uniform3f(const char* name, float x, float y, float z) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniform3f(loc, x, y, z);
}
void shader_prog::uniformMatrix4fv(const char* name, const float* matrix) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
}
void shader_prog::uniformMatrix4fv(const char* name, glm::mat4 matrix) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}
void shader_prog::attribute3fv(const char* name, GLfloat* vecArray, int numberOfVertices) {
    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numberOfVertices, vecArray, GL_STATIC_DRAW);

    GLuint loc = glGetAttribLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glEnableVertexAttribArray(loc);

    //printf("Enabled location: %d\n", loc);

    glVertexAttribPointer(
        loc, // attribute
        3,                 // number of elements per vertex, here (r,g,b)
        GL_FLOAT,          // the type of each element
        GL_FALSE,          // take our values as-is
        0,                 // no extra data between each position
        0                  // offset of first element
    );
}


GLuint shader_prog::attributeVectorVec3(const char* name, std::vector<glm::vec3> vectorVec3) {
    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vectorVec3.size()*3, &vectorVec3[0].x, GL_STATIC_DRAW);

    /*float* a = &vectorVec3[0].x;
    for (int i = 0; i < vectorVec3.size(); i++) {
        printf("%f, %f, %f \n", a[i], a[i+1], a[i+2]);
    }*/

    GLuint loc = glGetAttribLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glEnableVertexAttribArray(loc);

    //printf("Enabled location: %d\n", loc);

    glVertexAttribPointer(
        loc, // attribute
        3,                 // number of elements per vertex, here (r,g,b)
        GL_FLOAT,          // the type of each element
        GL_FALSE,          // take our values as-is
        0,                 // no extra data between each position
        0                  // offset of first element
    );

    return vboHandle;
}

// ---------- code taken from labs ----------


// ---------- rendering one block ----------

block_renderer::~block_renderer() {
    glDeleteVertexArrays(1, &this->blockVAO);
}

void block_renderer::init_block() {
  GLuint blockVBO;
  GLuint blockEBO;
  float vertices[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
  };

  unsigned int indices[] = {
    0, 2, 1,
    2, 3, 1
  };

  glGenVertexArrays(1, &this->blockVAO);
  glGenBuffers(1, &blockVBO);
  glGenBuffers(1, &blockEBO);

  glBindVertexArray(this->blockVAO);

  glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blockEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void block_renderer::draw_block(glm::vec2 position, glm::vec2 size, float rotation, int area) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));

  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
  model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  model = glm::scale(model, glm::vec3(size, 1.0f));

  this->shader.uniformMatrix4fv("model", model);

  //bind the texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->rocksTexture);
  this->shader.uniform1i("blockTexture", 0);
  this->shader.uniform1i("areaMask", area); // Pass the area mask to the shader
  glBindVertexArray(this->blockVAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

// ---------- rendering one block ----------


// ---------- rendering the world ----------

void draw_world(std::vector<double>& world,
                unsigned int world_width,
                unsigned int world_height,
                block_renderer& block,
                unsigned int block_size,
                float drawing_threshold) {
  for (int i{}; i < world_height; ++i) {
    for (int j{}; j < world_width; ++j) {
      if (world[i * world_width + j] > drawing_threshold) {
        glm::vec2 position = glm::vec2(j * block_size, i * block_size);
        glm::vec2 size = glm::vec2(block_size);

        // This vector will hold the 3x3 area around the current block,
        // where each element is 1 if the corresponding block is solid and 0 if it is empty.
        std::vector<int> area;

        for (int y = -1; y <= 1; ++y) {
          for (int x = -1; x <= 1; ++x) {
            int neighbor_x = j + x;
            int neighbor_y = i + y;

            if (neighbor_x >= 0 && neighbor_x < world_width &&
                neighbor_y >= 0 && neighbor_y < world_height) {
              area.push_back(world[neighbor_y * world_width + neighbor_x] > drawing_threshold ? 1 : 0);
            } else {
              area.push_back(1); // Treat out-of-bounds neighbors as solid blocks
            }
          }
        }

        int areaMask = 0;

        for (int i = 0; i < 9; ++i) {
            if (area[i] != 0) {
                areaMask |= (1 << i);
            }
        }

        block.draw_block(position, size, 0, areaMask);
      }
    }
  }
}

// ---------- rendering the world ----------
