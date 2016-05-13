#include "Ray.h"

Ray::Ray(const glm::dvec3 &orig, const glm::dvec3 &dir)
  : origin(orig), direction(dir) {
}

Ray Ray::Reflect(const Ray &normalRay) const {
  // Normalize normalRay and call ReflectNormalized.
  glm::dvec3 normalizedNormalDirection = glm::normalize(normalRay.direction);
  return ReflectNormalized(Ray(normalRay.origin, normalizedNormalDirection));
}

Ray Ray::ReflectNormalized(const Ray &normalRay) const {
  // i - incident ray (*this).
  // r - reflected ray (result).
  // r = i - 2*(i,n)*n
  assert(std::abs(glm::length(direction) - 1.0) < 1.0e-7 &&
         "Direction of ray must be normalized!");
  assert(std::abs(glm::length(normalRay.direction) - 1.0) < 1.0e-7 &&
         "Direction of ray must be normalized!");
  assert(glm::dot(normalRay.direction, direction) < 0 &&
         "Wrong normal direction for reflection!");
  glm::dvec3 normalizedNormalDirection = glm::normalize(normalRay.direction);
  auto IDotN = glm::dot(direction, normalRay.direction);
  glm::dvec3 resDirection = direction - 2 * IDotN * normalRay.direction;
  return Ray(normalRay.origin, resDirection);
}