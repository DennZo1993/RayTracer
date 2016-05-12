#pragma once

#include "glm\glm.hpp"
#include "Ray.h"
#include <set>
#include <vector>

// Forward-declaration of class Mesh.
class Mesh;

// Type of an abstract index.
typedef unsigned int MeshObjectIndexType;

// Struct representing a vertex in mesh.
// MeshVertex manages:
//   1. Its 3D coordinates;
//   2. Its normal vector (normalized, i. e. of length 1);
//   3. A set of indexes of adjacent faces.
//
// Normally in a mesh each vertex has at least 1 adjacent face.
struct MeshVertex {
	MeshVertex();
	MeshVertex(const glm::dvec3 &p, const glm::dvec3 &n);

	// 3D coordinates of the vertex.
	glm::dvec3 point;
	// Normal vector (of length 1).
	glm::dvec3 normal;
	// Vector of indexes of adjacent faces.
	std::set<MeshObjectIndexType> faceIndexes;
};


// Struct representing a (triangle) face in mesh.
// MeshFace manages:
//   1. A set of vertexes that form it;
//   2. Intersection with a ray.
//   3. Calculation of normal vectors that form a smooth vector field.
// TODO:
//   1. Material id;
//   2. <some other useful info>.
struct MeshFace {
	MeshFace(MeshObjectIndexType idx1,
					 MeshObjectIndexType idx2,
					 MeshObjectIndexType idx3,
					 Mesh &parent);

	// Struct to store the results of intersection test.
	struct IntersectionResult {
		// Default constructor crates an object describing NO intersection.
		IntersectionResult() : hasIntersection(false), distance(-1.0),
			barycentricU(-1.0), barycentricV(-1.0) {}

		// Constructs an object when intersection occurred.
		IntersectionResult(double d, double u, double v) :
			hasIntersection(true), distance(d), barycentricU(u), barycentricV(v) {}

		operator bool() const { return hasIntersection;	}

		// Indicates whether an intersection occured.
		// Values below are valid only if hasIntersection is true.
		bool hasIntersection;
		// Distance from ray's origin to the intersection point.
		double distance;
		// Barycentric coordinates of intersection point within the triangle.
		double barycentricU;
		double barycentricV;
	};

	// Ray intersection test.
	// Implements Möller–Trumbore intersection algorithm.
	IntersectionResult hasIntersection(const Ray &ray) const;

	// Returns a normal vector in a given point, represented by
	// its barycentric coordinates (returned by hasIntersection method).
	// Normals form a smooth vector field.
	glm::dvec3 getNormalVector(double u, double v) const;

	// Get one of 3 vertexes that form this face.
	// Index \p idx must be in range of [0..2].
	const MeshVertex& getVertex(MeshObjectIndexType idx) const;
	const MeshVertex& operator[](MeshObjectIndexType idx) const {
		return getVertex(idx);
	}

	static const unsigned int VertexesInFace = 3;

	// Array of indexes of vertexes that form this face.
	MeshObjectIndexType vertexIndexes[VertexesInFace];

	// Reference to a Mesh this face belongs to. Used to access vertexes.
	Mesh &parentMesh;
};


// Class representing an arbitrary mesh.
class Mesh {
public:
	Mesh() {}

	const std::vector<MeshVertex>& getVertexes() const {
		return vertexes;
	}
	const std::vector<MeshFace>& getFaces() const { return faces; }
private:
	std::vector<MeshVertex> vertexes;
	std::vector<MeshFace> faces;
};