#pragma once

#include "Ray.h"
#include "Material.h"

// Struct to store the results of intersection test.
class IntersectionResult {
public:
  // Default constructor crates an object describing NO intersection.
  IntersectionResult() :
    hasIntersection(false), distance(-1.0),
    // Fake ray, it must not be used in this case.
    ray(glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 0.0, 0.0)),
    // Fake normal vector, it must not be used in this case.
    normal(glm::dvec3(0.0, 0.0, 0.0)),
    // Empty material.
    material(nullptr) {}

  // Constructs an object when intersection occurred.
  IntersectionResult(const Ray &r, double d, const glm::dvec3 &n,
                     const Material *mat) :
    hasIntersection(true), distance(d), ray(r), normal(n),
    material(mat) {}

  operator bool() const { return hasIntersection; }

  bool operator<(const IntersectionResult &other) const {
    return distance < other.distance;
  }

public:
  double GetDistance() const { return distance; }

  Ray GetRay() const { return ray; }

  glm::dvec3 GetIntersectionPoint() const {
    #ifndef NDEBUG
    ray.AssertNormalized();
    #endif // !NDEBUG

    return ray.GetOrigin() + distance * ray.GetDirection();
  }

  Ray GetNormalRay() const {
    assert(std::abs(glm::length(normal) - 1.0) < 1.0e-6 &&
           "Normal must be normalized!");
    return Ray(GetIntersectionPoint(), normal);
  }

  glm::dvec3 GetNormalVector() const {
    return normal;
  }

  const Material *GetMaterialPtr() const { return material; }

private:
  // Indicates whether an intersection occured.
  // Values below are valid only if hasIntersection is true.
  bool hasIntersection;

  // Distance from ray's origin to the intersection point.
  double distance;

  // Ray which produced intersection.
  Ray ray;

  // Normal vector to the surface at the point of intersection.
  glm::dvec3 normal;

  // TODO: Meterial of intersection surface.
  const Material *material;
};
