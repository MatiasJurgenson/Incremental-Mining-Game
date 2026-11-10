#include "perlin.hpp"

// ---------- interpolation ----------

double interpolate_linear(double t, double begin, double end) {
  return begin + t * (end - begin);
}

double interpolate_fifth_order_polynomial(double t) {
  return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3);
}

// ---------- interpolation ----------

void generate_gradient_vectors(std::vector<glm::vec2>& noise_map, unsigned int width, unsigned int height) {
  for (int i{}; i < height; ++i) {
    for (int j{}; j < width; ++j) {
      noise_map[i * width + j] = create_random_vector_vec2(true);
    }
  }
}

void generate_perlin_noise(std::vector<glm::vec2>& perlin_map,
                float perlin_width,
                float perlin_height,
                std::vector<double>& target,
                float target_width,
                float target_height) {
  double width_scale = perlin_width / target_width;
  double height_scale = perlin_height / target_height;

  for (int i{}; i < target_height; ++i) {
    for (int j{}; j < target_width; ++j) {
      double scaledX = width_scale * j;
      double scaledY = height_scale * i;

      glm::vec2 top_right(floor(scaledX + 1), floor(scaledY + 1)),
        bot_right(floor(scaledX + 1), floor(scaledY)),
        top_left(floor(scaledX), floor(scaledY + 1)),
        bot_left(floor(scaledX), floor(scaledY));

      glm::vec2 distance_vec_top_right(scaledX - top_right.x, scaledY - top_right.y),
        distance_vec_bot_right(scaledX - bot_right.x, scaledY - bot_right.y),
        distance_vec_top_left(scaledX - top_left.x, scaledY - top_left.y),
        distance_vec_bot_left(scaledX - bot_left.x, scaledY - bot_left.y);

      double value_top_right{glm::dot(distance_vec_top_right, perlin_map[top_right.y * perlin_width + top_right.x])},
        value_bot_right{glm::dot(distance_vec_bot_right, perlin_map[bot_right.y * perlin_width + bot_right.x])},
        value_top_left{glm::dot(distance_vec_top_left, perlin_map[top_left.y * perlin_width + top_left.x])},
        value_bot_left{glm::dot(distance_vec_bot_left, perlin_map[bot_left.y * perlin_width + bot_left.x])};

      double tx{scaledX - floor(scaledX)}, ty{scaledY - floor(scaledY)};

      double interp_x = interpolate_fifth_order_polynomial(tx);
      double interp_y = interpolate_fifth_order_polynomial(ty);

      double result_perlin =
        interpolate_linear(interp_x,
          interpolate_linear(interp_y, value_bot_left, value_top_left),
          interpolate_linear(interp_y, value_bot_right, value_top_right)
        ) + 0.5;
      target[i * target_width + j] = result_perlin;
    }
  }
}

void generate_world(std::vector<double>& world,
                    unsigned int world_width,
                    unsigned int world_height,
                    unsigned int perlin_width,
                    unsigned int perlin_height) {
  std::vector<glm::vec2> perlin_map(world_width * world_height, glm::vec2(0));
  generate_gradient_vectors(perlin_map, perlin_width, perlin_height);
  generate_perlin_noise(perlin_map, perlin_width, perlin_height, world, world_width, world_height);

  // 10 highest blocks of world are air
  for (int i{}; i < 10; ++i) {
    for (int j{}; j < world_width; ++j) {
      world[i * world_width + j] = 0;
    }
  }
}
