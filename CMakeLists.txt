cmake_minimum_required(VERSION 3.20)

project(game)

set(CMAKE_CXX_STANDARD 23)

add_executable(game
  src/main.cpp
  src/perlin.cpp
  src/utils.cpp
)

target_include_directories(game PRIVATE include)
