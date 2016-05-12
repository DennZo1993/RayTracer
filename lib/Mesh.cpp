#include "Mesh.h"

// === MeshVertex struct ===
MeshVertex::MeshVertex() :
	point(0.0, 0.0, 0.0), normal(0.0, 0.0, 0.0), faceIndexes() {}

MeshVertex::MeshVertex(const glm::dvec3 &p, const glm::dvec3 &n) :
	point(p), normal(n), faceIndexes() {}


// === MeshFace struct ===
MeshFace::MeshFace(MeshObjectIndexType idx1,
									 MeshObjectIndexType idx2,
									 MeshObjectIndexType idx3,
									 Mesh &parent) : parentMesh(parent) {
	// A face can only be constructed from pairwise-dfferent vertexes.
	assert(idx1 != idx2 && idx1 != idx3 && idx2 != idx3 &&
				 "Cannot construct a face from less than 3 different vertexes!");
	vertexIndexes[0] = idx1;
	vertexIndexes[1] = idx2;
	vertexIndexes[2] = idx3;
}

const MeshVertex& MeshFace::getVertex(MeshObjectIndexType idx) const {
	assert(idx >= 0 && idx <= 2 && "Vertex index in face out of bounds!");
	return parentMesh.getVertexes()[vertexIndexes[idx]];
}

MeshFace::IntersectionResult
MeshFace::hasIntersection(const Ray &ray) const {
	// Epsilon for floating-point comparisons.
	const double EPS = 1.0e-6;
	// Vertexes that form this face.
	auto V0 = getVertex(0).point;
	auto V1 = getVertex(1).point;
	auto V2 = getVertex(2).point;
	// Resulting values.
	double d, u, v;

	auto E1 = V1 - V0;
	auto E2 = V2 - V0;
	auto P = glm::cross(ray.direction, E2);
	auto det = glm::dot(E1, P);
	if (det < EPS)
		return IntersectionResult(); // No intersection.
	auto T = ray.origin - V0;
	u = glm::dot(T, P);
	if (u < 0.0 || u > det)
		return IntersectionResult(); // No intersection.
	auto Q = glm::cross(T, E1);
	v = glm::dot(ray.direction, Q);
	if (v < 0.0 || u + v > det)
		return IntersectionResult(); // No intersection.
	d = glm::dot(E2, Q);

	d /= det;
	u /= det;
	v /= det;
	return IntersectionResult(d, u, v); // Intersection.
}

glm::dvec3 MeshFace::getNormalVector(double u, double v) const {
	auto V0 = getVertex(0).normal;
	auto V1 = getVertex(1).normal;
	auto V2 = getVertex(2).normal;

	glm::dvec3 result = (1.0 - u - v) * V0 + u * V1 + v * V2;
	return glm::normalize(result);
}