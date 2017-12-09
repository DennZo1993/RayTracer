#pragma once

#include "Object3d.h"
#include "Material.h"

class Sphere : public IObject3D {
public:
  Sphere(const glm::dvec3 c, double r, const Material &mat)
    : center(c)
    , radius(r)
    , material(mat)
  {}

  IntersectionResult Intersect(const Ray &ray) const override;

public:
  double GetRadius() const { return radius; }

private:
  glm::dvec3 center;
  double radius;
  const Material &material;
};
