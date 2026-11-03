#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <vector>
#include <random>
#include "utils.hpp"

double uniform_random_nr(double, double);
void generate_gradient_vectors(std::vector<Vec2>&, int, int);
void generate_perlin_noise(std::vector<Vec2>&, float, float, std::vector<double>&, float, float);

#endif
