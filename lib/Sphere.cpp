#include "Sphere.h"


IntersectionResult Sphere::Intersect(const Ray &ray) const {
  #ifndef NDEBUG
  ray.AssertNormalized();
  #endif // !NDEBUG

  auto M = ray.GetOrigin() - center;
  double b = glm::dot(M, ray.GetDirection());
  double c = glm::dot(M, M) - radius * radius;

  // Rays origin is outside (c > 0) and ray is pointing away (b > 0).
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
  glm::dvec3 intersectionPoint = ray.GetOrigin() + dist * ray.GetDirection();

  // Normal vector for sphere's surface.
  glm::dvec3 normal = glm::normalize(intersectionPoint - center);

  return IntersectionResult(ray, dist, normal, &material);
}
