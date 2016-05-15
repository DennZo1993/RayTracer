#pragma once

#include "glm\glm.hpp"

class Material {
public:
  Material(const glm::dvec3 &a, const glm::dvec3 &s,
           const glm::dvec3 &d, double shine) :
    ambientColor(a), specularColor(s), diffuseColor(d),
    shininess(shine) {}

private:
  glm::dvec3 ambientColor;
  glm::dvec3 specularColor;
  glm::dvec3 diffuseColor;
  double shininess;

public:
  glm::dvec3 getAmbient() const { return ambientColor; }
  glm::dvec3 getSpecular() const { return specularColor; }
  glm::dvec3 getDiffuse() const { return diffuseColor; }
  double getShininess() const { return shininess; }

  #ifndef NDEBUG
  void AssertValueBounds() const {
    assert(ambientColor.r >= 0.0 && ambientColor.r <= 1.0 &&
           ambientColor.g >= 0.0 && ambientColor.g <= 1.0 &&
           ambientColor.b >= 0.0 && ambientColor.b <= 1.0 &&
           "Material's ambient color values out of bounds!");
    assert(specularColor.r >= 0.0 && specularColor.r <= 1.0 &&
           specularColor.g >= 0.0 && specularColor.g <= 1.0 &&
           specularColor.b >= 0.0 && specularColor.b <= 1.0 &&
           "Material's specular color values out of bounds!");
    assert(diffuseColor.r >= 0.0 && diffuseColor.r <= 1.0 &&
           diffuseColor.g >= 0.0 && diffuseColor.g <= 1.0 &&
           diffuseColor.b >= 0.0 && diffuseColor.b <= 1.0 &&
           "Material's diffuse color values out of bounds!");
  }
  #endif // !NDEBUG
};