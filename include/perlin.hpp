#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <vector>
#include <random>
#include <glm/glm.hpp>
#include "utils.hpp"

void generate_gradient_vectors(std::vector<glm::vec2>&, int, int);
void generate_perlin_noise(std::vector<glm::vec2>&, float, float, std::vector<double>&, float, float);
void generate_world(std::vector<double>&, unsigned int, unsigned int, unsigned int, unsigned int);

#endif
