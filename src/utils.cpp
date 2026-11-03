#include "utils.hpp"

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
