#include "Camera.h"

Camera::Camera(const glm::dvec3 &pos, const glm::dvec3 &dir,
               const glm::vec2 res)
  : position(pos), direction(dir), resolution(res) {
  Normalize();
}

Camera::~Camera() {}

void Camera::LookAt(const glm::dvec3 &point) {
  if (point != position) {
    direction = point - position;
    Normalize();
  }
}

void Camera::MoveTo(const glm::dvec3 &point) {
  glm::dvec3 focusPoint = position + direction;
  position = point;
  if (point != focusPoint) {
    // If we try to move to the focus point, keep the direction.
    // Otherwise, calculate new direction.
    direction = focusPoint - position;
  }
  Normalize();
}

void Camera::MoveForward(double distance) {
  assert(std::abs(glm::length(direction) - 1.0) < 0.00001 &&
         "Camera normalization failed!");
  position += distance * direction;
}

void Camera::MoveBackward(double distance) {
  assert(std::abs(glm::length(direction) - 1.0) < 0.00001 &&
         "Camera normalization failed!");
  position -= distance * direction;
}

void Camera::Normalize() {
  direction = glm::normalize(direction);
  assert(std::abs(glm::length(direction) - 1.0) < 0.00001 &&
         "Camera normalization failed!");
}