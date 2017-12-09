#include "Ray.h"

Ray::Ray(const glm::dvec3 &orig, const glm::dvec3 &dir)
  : origin(orig), direction(glm::normalize(dir)) {
}

Ray Ray::Reflect(const Ray &normalRay) const {
  // i - incident ray (*this).
  // r - reflected ray (result).
  // r = i - 2*(i,n)*n
  #ifndef NDEBUG
  AssertNormalized();
  normalRay.AssertNormalized();
  assert(glm::dot(normalRay.direction, direction) < 0 &&
         "Wrong normal direction for reflection!");
  #endif

  auto IDotN = glm::dot(direction, normalRay.direction);
  glm::dvec3 resDirection = direction - 2 * IDotN * normalRay.direction;
  return Ray(normalRay.origin, resDirection);
}
