#pragma once

#include "gtest\gtest.h"
#include "glm\glm.hpp"
#include "Material.h"

#define ASSERT_VEC_NEAR(vec1, vec2, epsilon) \
  ASSERT_NEAR(glm::length(vec1 - vec2), 0.0, epsilon)

// Constants for floating-point comparison.
const double EPS_WEAK = 1.0e-7;
const double EPS_STRONG = 1.0e-9;

// Some normalized vec3 constants.
const glm::dvec3 X_NORM_VEC = glm::dvec3(1.0, 0.0, 0.0);
const glm::dvec3 Y_NORM_VEC = glm::dvec3(0.0, 1.0, 0.0);
const glm::dvec3 Z_NORM_VEC = glm::dvec3(0.0, 0.0, 1.0);

// Some materials.
const Material testMaterial1(/*ambient=*/glm::dvec3(0.3, 0.3, 0.3),
                             /*specular=*/glm::dvec3(0.1, 0.1, 0.1),
                             /*diffuse=*/glm::dvec3(0.8, 0.8, 0.8),
                             /*shihiness=*/10.0);