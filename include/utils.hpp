#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <random>

struct Vec2 {
  double x, y;
};

struct Vec3 {
  double x, y, z;
};

double dot_product_Vec2(Vec2&, Vec2&);
double dot_product_Vec3(Vec3&, Vec3&);
Vec2 divide_by_double_Vec2(Vec2&, double);
Vec3 divide_by_double_Vec3(Vec3&, double);
double length_Vec2(Vec2&);
double length_Vec3(Vec3&);
double uniform_random_nr(double, double);
Vec2 create_random_vector_Vec2(bool);

#endif
