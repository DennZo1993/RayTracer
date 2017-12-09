#include "Tests.h"
#include "Mesh.h"

class CubeMeshTests : public ::testing::Test {
public:
  static void SetUpTestCase() {
    Cube = new Mesh(false, &testMaterial1);

    auto v0 = Cube->AddVertex(glm::dvec3(0.0, 0.0, 0.0));
    auto v1 = Cube->AddVertex(glm::dvec3(5.0, 0.0, 0.0));
    auto v2 = Cube->AddVertex(glm::dvec3(5.0, 5.0, 0.0));
    auto v3 = Cube->AddVertex(glm::dvec3(0.0, 5.0, 0.0));
    // Back plane.
    auto v4 = Cube->AddVertex(glm::dvec3(0.0, 0.0, 5.0));
    auto v5 = Cube->AddVertex(glm::dvec3(5.0, 0.0, 5.0));
    auto v6 = Cube->AddVertex(glm::dvec3(5.0, 5.0, 5.0));
    auto v7 = Cube->AddVertex(glm::dvec3(0.0, 5.0, 5.0));
    // Faces (ccw).
    auto f0 = Cube->AddQuadFace(v0, v1, v2, v3);
    auto f1 = Cube->AddQuadFace(v1, v5, v6, v2);
    auto f2 = Cube->AddQuadFace(v3, v2, v6, v7);
    auto f3 = Cube->AddQuadFace(v4, v0, v3, v7);
    auto f4 = Cube->AddQuadFace(v5, v4, v7, v6);
    auto f5 = Cube->AddQuadFace(v4, v5, v1, v0);

    Cube->CalculateNormals();
  }

  static void TearDownTestCase() {
    delete Cube;
  }

protected:
  static Mesh *Cube;
};

Mesh *CubeMeshTests::Cube = nullptr;

// === Mesh tests ===
TEST_F(CubeMeshTests, FaceSquaresTest) {
  // Test squares
  ASSERT_EQ(Cube->GetNumVertexes(), 8);
  ASSERT_EQ(Cube->GetNumFaces(), 12);
  for (const auto &face : Cube->GetFaces()) {
    ASSERT_DOUBLE_EQ(face.GetSquare(), 12.5);
  }
}

TEST_F(CubeMeshTests, IntersectionTest) {
  IObject3D *object = Cube;

  Ray ray(glm::dvec3(3.0, 2.0, -1.0), Z_NORM_VEC);
  IntersectionResult res = object->Intersect(ray);
  ASSERT_TRUE(res);
  ASSERT_DOUBLE_EQ(1.0, res.GetDistance());
  ASSERT_VEC_NEAR(-Z_NORM_VEC, (res.GetNormalRay().GetDirection()), EPS_WEAK);

  // Test ray falling on edge of triangle.
  ray.SetOrigin(glm::dvec3(3.0, 3.0, -1.0));
  res = object->Intersect(ray);
  ASSERT_TRUE(res);
  ASSERT_DOUBLE_EQ(1.0, res.GetDistance());
  ASSERT_VEC_NEAR(-Z_NORM_VEC, (res.GetNormalRay().GetDirection()), EPS_WEAK);

  // Test ray falling on vertex.
  ray.SetOrigin(glm::dvec3(10.0, 5.0, 0.0));
  ray.SetDirection(-X_NORM_VEC);
  res = object->Intersect(ray);
  ASSERT_TRUE(res);
  ASSERT_DOUBLE_EQ(5.0, res.GetDistance());
  ASSERT_VEC_NEAR(X_NORM_VEC, (res.GetNormalRay().GetDirection()), EPS_WEAK);
}
