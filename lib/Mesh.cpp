#include "Mesh.h"

// === MeshVertex struct ===
MeshVertex::MeshVertex(const Mesh *parent, glm::dvec3 p, glm::dvec3 n)
  : point(p)
  , normal(n)
  , faceIndexes()
  , parentMesh(parent)
{
  assert(parent && "Parent mesh of MeshVertex is null!");
}


void MeshVertex::CalculateNormal()
{
  if (faceIndexes.empty())
    return;

  if (glm::length(normal) > 1.0e-5)
    return;

  glm::dvec3 resultNormal(0.0, 0.0, 0.0);

  assert(parentMesh && "Parent mesh of MeshVertex is null!");
  for (const auto meshIdx : faceIndexes) {
    glm::dvec3 currentNormal = parentMesh->GetFaces()[meshIdx].GetNormalVectorCross();
    resultNormal = glm::normalize(resultNormal + currentNormal);
  }

  normal = resultNormal;
}


// === MeshFace struct ===
MeshFace::MeshFace(TMeshIndex idx1,
                   TMeshIndex idx2,
                   TMeshIndex idx3,
                   const Material *mat,
                   const Mesh *parent)
  : material(mat)
  , parentMesh(parent)
{
  assert(parentMesh && "Parent mesh of MeshFace is null!");
  // A face can only be constructed from pairwise-dfferent vertexes.
  assert(idx1 != idx2 && idx1 != idx3 && idx2 != idx3 &&
         "Cannot construct a face from less than 3 different vertexes!");
  vertexIndexes[0] = idx1;
  vertexIndexes[1] = idx2;
  vertexIndexes[2] = idx3;
}


const MeshVertex& MeshFace::GetVertex(TMeshIndex idx) const
{
  assert(idx <= 2 && "Vertex index in face out of bounds!");
  return parentMesh->GetVertexes()[vertexIndexes[idx]];
}


IntersectionResult MeshFace::Intersect(const Ray &ray) const
{
  #ifndef NDEBUG
  ray.AssertNormalized();
  #endif // !NDEBUG

  // Epsilon for floating-point comparisons.
  const double EPS = 1.0e-6;
  // Vertexes that form this face.
  auto V0 = GetVertex(0).point;
  auto V1 = GetVertex(1).point;
  auto V2 = GetVertex(2).point;
  // Resulting values.
  double d, u, v;

  auto E1 = V1 - V0;
  auto E2 = V2 - V0;
  auto P = glm::cross(ray.GetDirection(), E2);
  double det = glm::dot(E1, P);
  double invDet = 1.0 / det;

  if (det > -EPS && det < EPS)
    return IntersectionResult(); // No intersection.

  auto T = ray.GetOrigin() - V0;
  u = glm::dot(T, P) * invDet;
  if (u < 0.0 || u > 1.0)
    return IntersectionResult(); // No intersection.

  auto Q = glm::cross(T, E1);
  v = glm::dot(ray.GetDirection(), Q) * invDet;
  if (v < 0.0 || u + v > 1.0)
    return IntersectionResult(); // No intersection.

  d = glm::dot(E2, Q) * invDet;
  if (d < EPS)
    return IntersectionResult();

  return IntersectionResult(ray, d, GetNormalVector(u, v), material); // Intersection.
}


glm::dvec3 MeshFace::GetNormalVector(double u, double v) const
{
  assert(parentMesh && "Parent mesh of MeshFace is null!");
  return parentMesh->GetInterpolateNormals()
    ? GetNormalVectorInterpolated(u, v)
    : GetNormalVectorCross();
}


glm::dvec3 MeshFace::GetNormalVectorInterpolated(double u, double v) const
{
  auto V0 = GetVertex(0).normal;
  auto V1 = GetVertex(1).normal;
  auto V2 = GetVertex(2).normal;

#ifndef NDEBUG
  const double EPS = 1.0e-6;
  assert(glm::length(V0) > EPS && "Vertex 0 has incorrect normal!");
  assert(glm::length(V1) > EPS && "Vertex 1 has incorrect normal!");
  assert(glm::length(V2) > EPS && "Vertex 2 has incorrect normal!");
#endif

  glm::dvec3 result = (1.0 - u - v) * V0 + u * V1 + v * V2;
  return glm::normalize(result);
}


glm::dvec3 MeshFace::GetNormalVectorCross() const
{
  auto P0 = GetVertex(0).point;
  auto P1 = GetVertex(1).point;
  auto P2 = GetVertex(2).point;

  return glm::normalize(glm::cross(P0 - P1, P2 - P1));
}


double MeshFace::GetSquare() const
{
  auto P0 = GetVertex(0).point;
  auto P1 = GetVertex(1).point;
  auto P2 = GetVertex(2).point;

  double a = glm::length(P2 - P0);
  double b = glm::length(P1 - P0);
  double c = glm::length(P2 - P1);
  double p = (a + b + c) / 2;

  return sqrt(p * (p - a) * (p - b) * (p - c));
}


// === Mesh ===
TMeshIndex Mesh::AddVertex(const glm::dvec3 &p, const glm::dvec3 &n)
{
  vertexes.push_back(MeshVertex(this, p, n));
  return vertexes.size() - 1;
}


TMeshIndex Mesh::AddFace(TMeshIndex idx1,
                         TMeshIndex idx2,
                         TMeshIndex idx3)
{
  // Use material of this Mesh.
  return AddFace(idx1, idx2, idx3, material);
}


TMeshIndex
Mesh::AddFace(TMeshIndex idx1,
              TMeshIndex idx2,
              TMeshIndex idx3,
              const Material *mat)
{
  faces.push_back(MeshFace(idx1, idx2, idx3, mat, this));
  TMeshIndex newFaceIndex = faces.size() - 1;
  // Add current face index to its vertexes.
  vertexes[idx1].faceIndexes.insert(newFaceIndex);
  vertexes[idx2].faceIndexes.insert(newFaceIndex);
  vertexes[idx3].faceIndexes.insert(newFaceIndex);

  return newFaceIndex;
}


std::pair<TMeshIndex, TMeshIndex>
Mesh::AddQuadFace(TMeshIndex idx1, TMeshIndex idx2,
                  TMeshIndex idx3, TMeshIndex idx4)
{
  return AddQuadFace(idx1, idx2, idx3, idx4, material);
}


std::pair<TMeshIndex, TMeshIndex>
Mesh::AddQuadFace(TMeshIndex idx1, TMeshIndex idx2,
                  TMeshIndex idx3, TMeshIndex idx4,
                  const Material *mat)
{
  TMeshIndex i1 = AddFace(idx1, idx2, idx3, mat);
  TMeshIndex i2 = AddFace(idx1, idx3, idx4, mat);
  return std::make_pair(i1, i2);

}

void Mesh::CalculateNormals()
{
  for (auto &v : vertexes)
    v.CalculateNormal();
}


IntersectionResult Mesh::Intersect(const Ray &ray) const
{
  IntersectionResult finalResult;
  for (const auto &face : faces) {
    IntersectionResult currentResult = face.Intersect(ray);
    if (currentResult &&
        (!finalResult || currentResult < finalResult)) {
      finalResult = currentResult;
    }
  }

  return finalResult;
}
