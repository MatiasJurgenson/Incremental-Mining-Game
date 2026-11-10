cmake_minimum_required(VERSION 3.20)

project(game)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

FetchContent_Declare(
  stb
  URL https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
  DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(stb)

add_executable(game
  src/main.cpp
  src/perlin.cpp
  src/utils.cpp
  src/shader_utils.cpp
  src/texture_util.cpp
)

target_compile_definitions(game PRIVATE
  TASK_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_include_directories(game PRIVATE
  include
  ${stb_SOURCE_DIR}
)

target_link_libraries(game PRIVATE
  glfw
  OpenGL::GL
  GLEW::GLEW
  glm::glm-header-only
)
