#pragma once

#include "glm\glm.hpp"

#include <cassert>

struct Ray {
public:
  Ray(const glm::dvec3 &orig, const glm::dvec3 &dir);

  // Cast a reflection ray using origin and direction of \p normalRay.
  // Normalize direction of \p normalRay and call ReflectNormalized.
  Ray Reflect(const Ray &normalRay) const;

  // Cast a reflection ray using origin and direction of \p normalRay.
  // Saves 1 vector normalization.
  // Preconditions:
  //   1. Direction of *this is normalized.
  //   2. Direction of normalRay is normalized.
  //   3. This ray and normal ray are not co-oriented.
  // Postcondition:
  //   1. Origin of resulting ray is the same as normalRay's.
  //   2. Direction of resulting ray is normalized.
  Ray ReflectNormalized(const Ray &normalRay) const;

  glm::dvec3 origin;
  glm::dvec3 direction;
};