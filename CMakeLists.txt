cmake_minimum_required(VERSION 3.20)

project(game)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTASK_ROOT_PATH=\\\"\"${CMAKE_CURRENT_LIST_DIR}\\\"\"")

add_executable(game
  src/main.cpp
  src/perlin.cpp
  src/utils.cpp
  src/shader_utils.cpp
)

target_include_directories(game PRIVATE include)
target_link_libraries(game PRIVATE glfw OpenGL::GL GLEW::GLEW glm::glm-header-only)
