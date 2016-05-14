#pragma once

#include "glm\glm.hpp"

#include <cassert>

class Ray {
public:
  Ray(const glm::dvec3 &orig, const glm::dvec3 &dir);

  // Cast a reflection ray using origin and direction of \p normalRay.
  // Preconditions:
  //   1. Direction of *this is normalized.
  //   2. Direction of normalRay is normalized.
  //   3. This ray and normal ray are not co-oriented.
  // Postcondition:
  //   1. Origin of resulting ray is the same as normalRay's.
  //   2. Direction of resulting ray is normalized.
  Ray Reflect(const Ray &normalRay) const;

private:
  glm::dvec3 origin;
  glm::dvec3 direction;

public:
  void setOrigin(const glm::dvec3 &o) { origin = o; }
  glm::dvec3 getOrigin() const { return origin; }

  void setDirection(const glm::dvec3 &d) { direction = glm::normalize(d); }
  glm::dvec3 getDirection() const { return direction; }

  // Debug assertion: ray's direction must be normalized.
  #ifndef NDEBUG
  void AssertNormalized() const {
    assert(std::abs(glm::length(direction) - 1.0) < 1.0e-6 &&
           "Ray's direction must be normalized!");
  }
  #endif
};