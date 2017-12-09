#pragma once

#include "IntersectionResult.h"

// Abstract class representing a 3D object.
class IObject3D {
public:
  virtual ~IObject3D() = default;

  virtual IntersectionResult Intersect(const Ray &ray) const = 0;
};
