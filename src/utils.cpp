#include "utils.hpp"

double uniform_random_nr(double low, double hi) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<double> dist(low, hi);
  return dist(rng);
}

glm::vec2 create_random_vector_vec2(bool normalize) {
  double low{-1.0}, high{1.0};
  double x{uniform_random_nr(low, high)}, y{uniform_random_nr(low, high)};

  glm::vec2 res = glm::vec2(x, y);

  if (normalize) return res / glm::length(res);
  else return res;
}
