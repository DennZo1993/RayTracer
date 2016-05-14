#pragma once

#include "glm\glm.hpp"

struct PointLight {
  PointLight(glm::dvec3 p) :
    position(p), ambientColor(0.0, 0.0, 0.0),
    specularColor(0.0, 0.0, 0.0), diffuseColor(0.0, 0.0, 0.0) {}

  PointLight(const glm::dvec3 &p, const glm::dvec3 &a,
             const glm::dvec3 &s, const glm::dvec3 &d) :
    position(p), ambientColor(a), specularColor(s), diffuseColor(d) {
    #ifndef NDEBUG
    AssertValueBounds();
    #endif // !NDEBUG
  }

  glm::dvec3 position;
  glm::dvec3 ambientColor;
  glm::dvec3 specularColor;
  glm::dvec3 diffuseColor;

  #ifndef NDEBUG
  void AssertValueBounds() const {
    assert(ambientColor.r >= 0.0 && ambientColor.r <= 255.0 &&
           ambientColor.g >= 0.0 && ambientColor.g <= 255.0 &&
           ambientColor.b >= 0.0 && ambientColor.b <= 255.0 &&
           "Light's ambient color values out of bounds!");
    assert(specularColor.r >= 0.0 && specularColor.r <= 255.0 &&
           specularColor.g >= 0.0 && specularColor.g <= 255.0 &&
           specularColor.b >= 0.0 && specularColor.b <= 255.0 &&
           "Light's specular color values out of bounds!");
    assert(diffuseColor.r >= 0.0 && diffuseColor.r <= 255.0 &&
           diffuseColor.g >= 0.0 && diffuseColor.g <= 255.0 &&
           diffuseColor.b >= 0.0 && diffuseColor.b <= 255.0 &&
           "Light's diffuse color values out of bounds!");
  }
  #endif // !NDEBUG
};