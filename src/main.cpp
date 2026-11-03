#include <random>
#include <cmath>
#include <vector>

#include "perlin.hpp"

#define WORLD_WIDTH  60
#define WORLD_HEIGHT 100

std::vector<double> world(WORLD_WIDTH * WORLD_HEIGHT, 0);

void generate_world() {
  // create perlin noise grid
  std::vector<Vec2> perlin_map(WORLD_WIDTH * WORLD_HEIGHT, Vec2{0, 0});
  double perlin_width{WORLD_WIDTH / 5}, perlin_height{WORLD_HEIGHT / 5};
  generate_gradient_vectors(perlin_map, perlin_width, perlin_height);

  // apply noise grid to the world grid
  generate_perlin_noise(perlin_map, perlin_width, perlin_height, world, WORLD_WIDTH, WORLD_HEIGHT);
  // draw the world
  // approx upper 1/8 of the world is 'overground', so just a flat surface and air
  for (int i{}; i < WORLD_HEIGHT / 8; ++i) {
    for (int j{}; j < WORLD_WIDTH; ++j) {
      world[i * WORLD_WIDTH + j] = 0;                                  
    }
  }
  // other 7/8 of world is underground and made with the noise  
}

int main() {
  generate_world();
  for (int i{}; i < WORLD_HEIGHT; ++i) {
    for (int j{}; j < WORLD_WIDTH; ++j) {
      if (world[i * WORLD_WIDTH + j] > 0.5) printf("# ");
      else printf(". ");
    }
    printf("\n");
  }
  return 0;
}
