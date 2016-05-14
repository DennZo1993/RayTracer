#pragma once

#include "Object3d.h"

class Sphere : public Object3d {
public:
  Sphere(const glm::dvec3 c, double r) :
    center(c), radius(r) {}

  IntersectionResult intersect(const Ray &ray) const {
    #ifndef NDEBUG
    ray.AssertNormalized();
    #endif // !NDEBUG

    auto M = ray.getOrigin() - center;
    double b = glm::dot(M, ray.getDirection());
    double c = glm::dot(M, M) - radius * radius;

    // Ray’s origin is outside (c > 0) and ray is pointing away (b > 0).
    if (c > 0.0 && b > 0.0)
      return IntersectionResult(); // No intersection.

    double discr = b * b - c;

    // Negative discr means no intersection.
    if (discr < 0)
      return IntersectionResult(); // No intersection.

    double dist = -b - sqrt(discr);

    // If dist is negative, ray started inside of sphere.
    if (dist < 0.0) {
      // Use the other root then.
      dist = -b + sqrt(discr);
    }

    // Point of intersection.
    glm::dvec3 intersectionPoint = ray.getOrigin() + dist * ray.getDirection();

    // Normal vector for sphere's surface.
    glm::dvec3 normal = glm::normalize(intersectionPoint - center);

    return IntersectionResult(ray, dist, normal);
  }

private:
  glm::dvec3 center;
  double radius;

public:
  double getRadius() const { return radius; }
};