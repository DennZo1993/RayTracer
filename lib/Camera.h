#pragma once

#include <glm/glm.hpp>
#include <cassert>

class Camera {
public:
  // === Constructors ===

  Camera(const glm::dvec3 &pos, const glm::dvec3 &dir, const glm::vec2 &res);
  ~Camera();

  // === Camera movement ===

  // Change camera's focus to \p point, preserving the position.
  // Normalizes direction.
  void LookAt(const glm::dvec3 &point);

  // Change camera's position to \p point preserving the focus.
  // Normalizes direction.
  void MoveTo(const glm::dvec3 &point);

  // Move camera forward (collinear to direction).
  void MoveForward(double distance);

  // Move camera backward (opposite to direction).
  void MoveBackward(double distance);

private:
  // Normalize direction - make it of length 1.
  void Normalize();

  glm::dvec3 position;
  glm::dvec3 direction;
  glm::uvec2 resolution;

public:
  // Getters.
  glm::dvec3 getPosition() const { return position; }
  glm::dvec3 getDirection() const { return direction; }
  glm::uvec2 getResolution() const { return resolution; }
};

