#pragma once

#include "glm/glm.hpp"
#include "Ray.h"
#include "IntersectionResult.h"
#include "Object3d.h"
#include "Material.h"
#include <set>
#include <vector>

// Forward-declaration of class Mesh.
class Mesh;

// Type of an abstract index.
using TMeshIndex = unsigned int;

// Struct representing a vertex in mesh.
// MeshVertex manages:
//   1. Its 3D coordinates;
//   2. Its normal vector (normalized, i. e. of length 1);
//   3. A set of indexes of adjacent faces.
//
// Normally in a mesh each vertex has at least 1 adjacent face.
struct MeshVertex {
  MeshVertex(const Mesh *parent,
             glm::dvec3 p = glm::dvec3(0.0, 0.0, 0.0),
             glm::dvec3 n = glm::dvec3(0.0, 0.0, 0.0));

  // Calculate normal vector from adjacent faces.
  // For each adjacent face use its flat normal (cross product),
  // resulting normal vector is normalized weighted sum of faces' normals.
  void CalculateNormal();

  // 3D coordinates of the vertex.
  glm::dvec3 point;
  // Normal vector (of length 1).
  glm::dvec3 normal;
  // Vector of indexes of adjacent faces.
  std::set<TMeshIndex> faceIndexes;

  // Pointer to a Mesh this face belongs to. Used to access faces.
  const Mesh *parentMesh;
};


// Struct representing a (triangle) face in mesh.
// MeshFace manages:
//   1. A set of vertexes that form it;
//   2. Intersection with a ray.
//   3. Calculation of normal vectors that form a smooth vector field.
// TODO:
//   1. <some other useful info>.
struct MeshFace {
  MeshFace(TMeshIndex idx1,
           TMeshIndex idx2,
           TMeshIndex idx3,
           const Material *mat,
           const Mesh *parent);

  // Ray intersection test.
  // Implements Möller-Trumbore intersection algorithm.
  IntersectionResult Intersect(const Ray &ray) const;

  // Returns a normal vector in a given point, represented by
  // its barycentric coordinates (returned by hasIntersection method).
  // Normals form a smooth vector field.
  glm::dvec3 GetNormalVector(double u, double v) const;
  glm::dvec3 GetNormalVectorInterpolated(double u, double v) const;
  glm::dvec3 GetNormalVectorCross() const;

  // Get square of the triangle.
  double GetSquare() const;

  // Get one of 3 vertexes that form this face.
  // Index \p idx must be in range of [0..2].
  const MeshVertex& GetVertex(TMeshIndex idx) const;
  const MeshVertex& operator[](TMeshIndex idx) const {
    return GetVertex(idx);
  }

  static const unsigned int VertexesInFace = 3;

  // Array of indexes of vertexes that form this face.
  TMeshIndex vertexIndexes[VertexesInFace];

  // Material.
  const Material *material;

  // Pointer to a Mesh this face belongs to. Used to access vertexes.
  const Mesh *parentMesh;
};


// Class representing an arbitrary mesh.
class Mesh : public IObject3D {
public:
  Mesh(bool interpolate, const Material *mat) :
    interpolateNormals(interpolate), material(mat) {}

public:
  using TVertexes = std::vector<MeshVertex>;
  using TFaces = std::vector<MeshFace>;

  bool GetInterpolateNormals() const { return interpolateNormals; }
  const TVertexes& GetVertexes() const { return vertexes; }
  const TFaces&    GetFaces() const { return faces; }

  std::size_t GetNumVertexes() const { return vertexes.size(); }
  std::size_t GetNumFaces() const { return faces.size(); }

  TMeshIndex AddVertex(const glm::dvec3 &p,
                       const glm::dvec3 &n = glm::dvec3(0.0, 0.0, 0.0));

  TMeshIndex AddFace(TMeshIndex idx1,
                     TMeshIndex idx2,
                     TMeshIndex idx3);

  TMeshIndex AddFace(TMeshIndex idx1,
                     TMeshIndex idx2,
                     TMeshIndex idx3,
                     const Material *mat);

  // Add quad face (ccw).
  // Adds two triangle faces (ccw) and returns their indexes.
  std::pair<TMeshIndex, TMeshIndex>
  AddQuadFace(TMeshIndex idx1, TMeshIndex idx2,
              TMeshIndex idx3, TMeshIndex idx4);

  // Add quad face (ccw).
  // Adds two triangle faces (ccw) and returns their indexes.
  std::pair<TMeshIndex, TMeshIndex>
  AddQuadFace(TMeshIndex idx1, TMeshIndex idx2,
              TMeshIndex idx3, TMeshIndex idx4,
              const Material *mat);

  // Calculate normals for each vertex.
  void CalculateNormals();

  IntersectionResult Intersect(const Ray &ray) const override;

private:
  // Flag indicating whether normal vectors are interpolated or not.
  bool interpolateNormals;

  TVertexes vertexes;
  TFaces faces;

  const Material *material;
};
