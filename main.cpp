#include <random>
#include <cmath>
#include <vector>

#define WORLD_WIDTH  100
#define WORLD_HEIGHT 100

std::vector<double> world(WORLD_WIDTH * WORLD_HEIGHT, 0);

// data structs and their ops

struct Vec2 {
  double x, y;
};

struct Vec3 {
  double x, y, z;
};

double dot_product_Vec2(Vec2& v, Vec2& w) {
  return v.x * w.x + v.y * w.y;
}

double dot_product_Vec3(Vec3& v, Vec3& w) {
  return v.x * w.x + v.y * w.y + v.z + w.z;  
}

Vec2 divide_by_double_Vec2(Vec2& v, double d) {
  return Vec2 { v.x / d, v.y / d};
}

Vec3 divide_by_double_Vec3(Vec3& v, double d) {
  return Vec3 { v.x / d, v.y / d, v.z / d };
}

double length_Vec2(Vec2& v) {
  return pow(pow(v.x, 2) + pow(v.y, 2), 0.5);
}

double length_Vec3(Vec3& v) {
  return pow(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2), 0.5);
}

// end of data structures and ops

// rand nr's and vectors

// gen rand nr in range [low, hi] uniformly
double uniform_random_nr(double low, double hi) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<double> dist(low, hi);
  return dist(rng);
}

Vec2 create_random_vector_Vec2(bool normalize) {
  double low{-1.0}, high{1.0};
  double x{uniform_random_nr(low, high)}, y{uniform_random_nr(low, high)};

  Vec2 res = Vec2 { x, y };

  if (normalize) return divide_by_double_Vec2(res, length_Vec2(res));
  else return res;
}

// end of rand nr's and vectors

void generate_noise_perlin(std::vector<Vec2>& noise_map, int width, int height) {
  for (int i{}; i < height; ++i) {
    for (int j{}; j < width; ++j) {
      noise_map[i * height + j] = create_random_vector_Vec2(true);
    }
  }
}

double interpolate_linear(double t, double begin, double end) {
  return begin + t * (end - begin);
}

double interpolate_fifth_order_polynomial(double t) {
  return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3);
}

// map given perlin noise to the world
void map_perlin(std::vector<Vec2>& perlin_map,
                float perlin_width,
                float perlin_height,
                std::vector<double>& world,
                float world_width,
                float world_height) {
  double width_scale = perlin_width / world_width;
  double height_scale = perlin_height / world_height;

  for (int i{}; i < world_height; ++i) {
    for (int j{}; j < world_width; ++j) {
      double scaledX = width_scale * j;
      double scaledY = height_scale * i;

      Vec2 top_right = Vec2 {floor(scaledX + 1), floor(scaledY + 1)};
      Vec2 bot_right = Vec2 {floor(scaledX + 1), floor(scaledY)};
      Vec2 top_left = Vec2 {floor(scaledX), floor(scaledY + 1)};
      Vec2 bot_left = Vec2 {floor(scaledX), floor(scaledY)};

      Vec2 distance_vec_top_right = Vec2 { scaledX - top_right.x, scaledY - top_right.y};
      Vec2 distance_vec_bot_right = Vec2 { scaledX - bot_right.x, scaledY - bot_right.y};
      Vec2 distance_vec_top_left = Vec2 { scaledX - top_left.x, scaledY - top_left.y};
      Vec2 distance_vec_bot_left = Vec2 { scaledX - bot_left.x, scaledY - bot_left.y};

      double value_top_right =
        dot_product_Vec2(distance_vec_top_right, perlin_map[top_right.y * perlin_width + top_right.x]);
      double value_bot_right =
        dot_product_Vec2(distance_vec_bot_right, perlin_map[bot_right.y * perlin_width + bot_right.x]);
      double value_top_left =
        dot_product_Vec2(distance_vec_top_left, perlin_map[top_left.y * perlin_width + top_left.x]);
      double value_bot_left =
        dot_product_Vec2(distance_vec_bot_left, perlin_map[bot_left.y * perlin_width + bot_left.x]);

      double tx{scaledX - floor(scaledX)}, ty{scaledY - floor(scaledY)};

      double interp_x =
        interpolate_fifth_order_polynomial(tx);
      double interp_y =
        interpolate_fifth_order_polynomial(ty);

      double result_perlin =
        interpolate_linear(interp_x,
          interpolate_linear(interp_y, value_bot_left, value_top_left),
          interpolate_linear(interp_y, value_bot_right, value_top_right)
        ) + 0.5;
      world[i * world_width + j] = result_perlin;
    }
  }
}

void generate_world() {
  // create perlin noise grid
  std::vector<Vec2> perlin_map(WORLD_WIDTH * WORLD_HEIGHT, Vec2{0, 0});
  double perlin_width{WORLD_WIDTH / 5}, perlin_height{WORLD_HEIGHT / 5};
  generate_noise_perlin(perlin_map, perlin_width, perlin_height);

  // apply noise grid to the world grid
  map_perlin(perlin_map, perlin_width, perlin_height, world, WORLD_WIDTH, WORLD_HEIGHT);
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
