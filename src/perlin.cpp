#include "perlin.hpp"
#include "utils.hpp"


double interpolate_linear(double t, double begin, double end) {
  return begin + t * (end - begin);
}

double interpolate_fifth_order_polynomial(double t) {
  return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3);
}

void generate_gradient_vectors(std::vector<Vec2>& noise_map, int width, int height) {
  for (int i{}; i < height; ++i) {
    for (int j{}; j < width; ++j) {
      noise_map[i * width + j] = create_random_vector_Vec2(true);
    }
  }
}

void generate_perlin_noise(std::vector<Vec2>& perlin_map,
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

      Vec2 top_right{floor(scaledX + 1), floor(scaledY + 1)},
        bot_right{floor(scaledX + 1), floor(scaledY)},
        top_left{floor(scaledX), floor(scaledY + 1)},
        bot_left{floor(scaledX), floor(scaledY)};

      Vec2 distance_vec_top_right{ scaledX - top_right.x, scaledY - top_right.y},
        distance_vec_bot_right{ scaledX - bot_right.x, scaledY - bot_right.y},
        distance_vec_top_left{ scaledX - top_left.x, scaledY - top_left.y},
        distance_vec_bot_left{ scaledX - bot_left.x, scaledY - bot_left.y};

      double value_top_right{dot_product_Vec2(distance_vec_top_right, perlin_map[top_right.y * perlin_width + top_right.x])},
        value_bot_right{dot_product_Vec2(distance_vec_bot_right, perlin_map[bot_right.y * perlin_width + bot_right.x])},
        value_top_left{dot_product_Vec2(distance_vec_top_left, perlin_map[top_left.y * perlin_width + top_left.x])},
        value_bot_left{dot_product_Vec2(distance_vec_bot_left, perlin_map[bot_left.y * perlin_width + bot_left.x])};

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
