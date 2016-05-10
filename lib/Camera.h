#pragma once

#include <glm\glm.hpp>
#include <cassert>

class Camera {
public:
	// === Constructors ===

	Camera(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec2 res);
	~Camera();

	// === Camera movement ===

	// Change camera's focus to \p point, preserving the position.
	// Normalizes direction.
	void LookAt(const glm::vec3 &point);

	// Change camera's position to \p point preserving the focus.
	// Normalizes direction.
	void MoveTo(const glm::vec3 &point);

	// Move camera forward (collinear to direction).
	void MoveForward(float distance);

	// Move camera backward (opposite to direction).
	void MoveBackward(float distance);

private:
	// Normalize direction - make it of length 1.
	void Normalize();

	glm::vec3 position;
	glm::vec3 direction;
	glm::uvec2 resolution;

public:
	// Getters.
	glm::vec3 getPosition() const { return position; }
	glm::vec3 getDirection() const { return direction; }
	glm::uvec2 getResolution() const { return resolution; }
};

