#include "Camera.h"

Camera::Camera(const glm::vec3 &pos, const glm::vec3 &dir,
							 const glm::vec2 res)
	: position(pos), direction(dir), resolution(res) {
	Normalize();
}

Camera::~Camera() {}

void Camera::LookAt(const glm::vec3 &point) {
	if (point != position) {
		direction = point - position;
		Normalize();
	}
}

void Camera::MoveTo(const glm::vec3 &point) {
	glm::vec3 focusPoint = position + direction;
	position = point;
	if (point != focusPoint) {
		// If we try to move to the focus point, keep the direction.
		// Otherwise, calculate new direction.
		direction = focusPoint - position;
	}
	Normalize();
}

void Camera::MoveForward(float distance) {
	assert(std::abs(glm::length(direction) - 1.0f) < 0.00001f &&
				 "Camera normalization failed!");
	position += distance * direction;
}

void Camera::MoveBackward(float distance) {
	assert(std::abs(glm::length(direction) - 1.0f) < 0.00001f &&
				 "Camera normalization failed!");
	position -= distance * direction;
}

void Camera::Normalize() {
	direction = glm::normalize(direction);
	assert(std::abs(glm::length(direction) - 1.0f) < 0.00001f &&
				 "Camera normalization failed!");
}