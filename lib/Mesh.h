#pragma once

#include "glm\glm.hpp"
#include "Ray.h"
#include "IntersectionResult.h"
#include "Object3d.h"
#include "Material.h"
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
  MeshVertex(Mesh &parent);
  MeshVertex(const glm::dvec3 &p, Mesh &parent);
  MeshVertex(const glm::dvec3 &p, const glm::dvec3 &n, Mesh &parent);

  // Calculate normal vector from adjacent faces.
  // For each adjacent face use its flat normal (cross product),
  // resulting normal vector is normalized weighted sum of faces' normals.
  void CalculateNormal();

  // 3D coordinates of the vertex.
  glm::dvec3 point;
  // Normal vector (of length 1).
  glm::dvec3 normal;
  // Vector of indexes of adjacent faces.
  std::set<MeshObjectIndexType> faceIndexes;

  // Reference to a Mesh this face belongs to. Used to access faces.
  Mesh &parentMesh;
};


// Struct representing a (triangle) face in mesh.
// MeshFace manages:
//   1. A set of vertexes that form it;
//   2. Intersection with a ray.
//   3. Calculation of normal vectors that form a smooth vector field.
// TODO:
//   1. <some other useful info>.
struct MeshFace {
  MeshFace(MeshObjectIndexType idx1,
           MeshObjectIndexType idx2,
           MeshObjectIndexType idx3,
           const Material &mat,
           Mesh &parent);

  // Ray intersection test.
  // Implements Möller–Trumbore intersection algorithm.
  IntersectionResult intersect(const Ray &ray) const;

  // Returns a normal vector in a given point, represented by
  // its barycentric coordinates (returned by hasIntersection method).
  // Normals form a smooth vector field.
  glm::dvec3 getNormalVector(double u, double v) const;
  glm::dvec3 getNormalVectorInterpolated(double u, double v) const;
  glm::dvec3 getNormalVectorCross() const;

  // Get square of the triangle.
  double getSquare() const;

  // Get one of 3 vertexes that form this face.
  // Index \p idx must be in range of [0..2].
  const MeshVertex& getVertex(MeshObjectIndexType idx) const;
  const MeshVertex& operator[](MeshObjectIndexType idx) const {
    return getVertex(idx);
  }

  static const unsigned int VertexesInFace = 3;

  // Array of indexes of vertexes that form this face.
  MeshObjectIndexType vertexIndexes[VertexesInFace];

  // Material.
  const Material &material;

  // Reference to a Mesh this face belongs to. Used to access vertexes.
  Mesh &parentMesh;
};


// Class representing an arbitrary mesh.
class Mesh : public Object3d {
public:
  Mesh(bool interpolate, const Material &mat) :
    interpolateNormals(interpolate), material(mat) {}

  bool getInterpolateNormals() const { return interpolateNormals; }
  const std::vector<MeshVertex>& getVertexes() const {
    return vertexes;
  }
  const std::vector<MeshFace>& getFaces() const { return faces; }

  std::size_t getNumVertexes() const { return vertexes.size(); }
  std::size_t getNumFaces() const { return faces.size(); }

  MeshObjectIndexType addVertex(const glm::dvec3 &p,
                                const glm::dvec3 &n = glm::dvec3(0.0, 0.0, 0.0));

  MeshObjectIndexType addFace(MeshObjectIndexType idx1,
                              MeshObjectIndexType idx2,
                              MeshObjectIndexType idx3);

  MeshObjectIndexType addFace(MeshObjectIndexType idx1,
                              MeshObjectIndexType idx2,
                              MeshObjectIndexType idx3,
                              const Material &mat);

  // Add quad face (ccw).
  // Adds two triangle faces (ccw) and returns their indexes.
  std::pair<MeshObjectIndexType, MeshObjectIndexType>
  addQuadFace(MeshObjectIndexType idx1, MeshObjectIndexType idx2,
              MeshObjectIndexType idx3, MeshObjectIndexType idx4);

  // Add quad face (ccw).
  // Adds two triangle faces (ccw) and returns their indexes.
  std::pair<MeshObjectIndexType, MeshObjectIndexType>
  addQuadFace(MeshObjectIndexType idx1, MeshObjectIndexType idx2,
              MeshObjectIndexType idx3, MeshObjectIndexType idx4,
              const Material &mat);

  // Calculate normals for each vertex.
  void CalculateNormals();

  IntersectionResult intersect(const Ray &ray) const;
private:
  // Flag indicating whether normal vectors are interpolated or not.
  bool interpolateNormals;

  std::vector<MeshVertex> vertexes;
  std::vector<MeshFace> faces;

  const Material &material;
};