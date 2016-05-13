#include "Mesh.h"

// === MeshVertex struct ===
MeshVertex::MeshVertex(Mesh &parent) :
	point(0.0, 0.0, 0.0), normal(0.0, 0.0, 0.0), faceIndexes(),
	parentMesh(parent) {}

MeshVertex::MeshVertex(const glm::dvec3 &p, Mesh &parent) :
	point(p), normal(0.0, 0.0, 0.0), faceIndexes(),
	parentMesh(parent) {}

MeshVertex::MeshVertex(const glm::dvec3 &p,
											 const glm::dvec3 &n,
											 Mesh &parent) :
	point(p), normal(n), faceIndexes(), parentMesh(parent) {}

void MeshVertex::CalculateNormal() {
	if (faceIndexes.empty())
		return;

	glm::dvec3 sumOfNormals(0.0, 0.0, 0.0);
	double sumOfSquares = 0.0;

	// Calculate sum of squares for weights calculation.
	for (const auto meshIdx : faceIndexes)
		sumOfSquares += parentMesh.getFaces()[meshIdx].getSquare();

	for (const auto meshIdx : faceIndexes) {
		const auto &currentFace = parentMesh.getFaces()[meshIdx];

		double currentSquare = currentFace.getSquare();
		glm::dvec3 currentNormal = currentFace.getNormalVectorCross();

		sumOfNormals += (currentSquare / sumOfSquares) * currentNormal;
	}

	normal = glm::normalize(sumOfNormals);
}


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
	return parentMesh.getInterpolateNormals()
		? getNormalVectorInterpolated(u, v)
		: getNormalVectorCross();
}

glm::dvec3 MeshFace::getNormalVectorInterpolated(double u, double v) const {
	const double EPS = 1.0e-6;

	auto V0 = getVertex(0).normal;
	auto V1 = getVertex(1).normal;
	auto V2 = getVertex(2).normal;

	assert(glm::length(V0) > EPS && "Vertex 0 has incorrect normal!");
	assert(glm::length(V1) > EPS && "Vertex 1 has incorrect normal!");
	assert(glm::length(V2) > EPS && "Vertex 2 has incorrect normal!");

	glm::dvec3 result = (1.0 - u - v) * V0 + u * V1 + v * V2;
	return glm::normalize(result);
}

glm::dvec3 MeshFace::getNormalVectorCross() const {
	auto P0 = getVertex(0).point;
	auto P1 = getVertex(1).point;
	auto P2 = getVertex(2).point;

	return glm::normalize(glm::cross(P1 - P0, P2 - P0));
}

double MeshFace::getSquare() const {
	auto P0 = getVertex(0).point;
	auto P1 = getVertex(1).point;
	auto P2 = getVertex(2).point;

	double a = glm::length(P2 - P0);
	double b = glm::length(P1 - P0);
	double c = glm::length(P2 - P1);
	double p = (a + b + c) / 2;

	return sqrt(p * (p - a) * (p - b) * (p - c));
}


// === Mesh ===
MeshObjectIndexType Mesh::addVertex(const glm::dvec3 &p, 
																		const glm::dvec3 &n) {
	vertexes.push_back(MeshVertex(p, n, *this));
	return vertexes.size() - 1;
}

MeshObjectIndexType
Mesh::addFace(MeshObjectIndexType idx1,
							MeshObjectIndexType idx2,
							MeshObjectIndexType idx3) {
	faces.push_back(MeshFace(idx1, idx2, idx3, *this));
	MeshObjectIndexType newFaceIndex = faces.size() - 1;
	// Add current face index to its vertexes.
	vertexes[idx1].faceIndexes.insert(newFaceIndex);
	vertexes[idx2].faceIndexes.insert(newFaceIndex);
	vertexes[idx3].faceIndexes.insert(newFaceIndex);

	return newFaceIndex;
}

std::pair<MeshObjectIndexType, MeshObjectIndexType>
Mesh::addQuadFace(MeshObjectIndexType idx1, MeshObjectIndexType idx2,
									MeshObjectIndexType idx3, MeshObjectIndexType idx4) {
	MeshObjectIndexType i1 = addFace(idx1, idx2, idx3);
	MeshObjectIndexType i2 = addFace(idx1, idx3, idx4);
	return std::make_pair(i1, i2);
}

void Mesh::CalculateNormals() {
	for (auto &v : vertexes)
		v.CalculateNormal();
}