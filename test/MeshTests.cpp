#include "Tests.h"
#include "Mesh.h"

class CubeMeshTests : public ::testing::Test {
public:
  static void SetUpTestCase() {
    Cube = new Mesh(false);

    auto v0 = Cube->addVertex(glm::dvec3(0.0, 0.0, 0.0));
    auto v1 = Cube->addVertex(glm::dvec3(5.0, 0.0, 0.0));
    auto v2 = Cube->addVertex(glm::dvec3(5.0, 5.0, 0.0));
    auto v3 = Cube->addVertex(glm::dvec3(0.0, 5.0, 0.0));
    // Back plane.
    auto v4 = Cube->addVertex(glm::dvec3(0.0, 0.0, 5.0));
    auto v5 = Cube->addVertex(glm::dvec3(5.0, 0.0, 5.0));
    auto v6 = Cube->addVertex(glm::dvec3(5.0, 5.0, 5.0));
    auto v7 = Cube->addVertex(glm::dvec3(0.0, 5.0, 5.0));
    // Faces (ccw).
    auto f0 = Cube->addQuadFace(v0, v1, v2, v3);
    auto f1 = Cube->addQuadFace(v1, v5, v6, v2);
    auto f2 = Cube->addQuadFace(v3, v2, v6, v7);
    auto f3 = Cube->addQuadFace(v4, v0, v3, v7);
    auto f4 = Cube->addQuadFace(v5, v4, v7, v6);
    auto f5 = Cube->addQuadFace(v4, v5, v1, v0);

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
  ASSERT_EQ(Cube->getNumVertexes(), 8);
  ASSERT_EQ(Cube->getNumFaces(), 12);
  for (const auto &face : Cube->getFaces()) {
    ASSERT_DOUBLE_EQ(face.getSquare(), 12.5);
  }
}

TEST_F(CubeMeshTests, IntersectionTest) {
  Object3d *object = Cube;

  Ray ray(glm::dvec3(3.0, 2.0, -1.0), glm::dvec3(0.0, 0.0, 1.0));
  IntersectionResult res = object->intersect(ray);
  ASSERT_TRUE(res);
  ASSERT_DOUBLE_EQ(1.0, res.getDistance());
  ASSERT_VEC_NEAR(glm::dvec3(0.0, 0.0, -1.0), (res.getNormalRay().getDirection()), EPS_WEAK);

  // Test ray falling on edge of triangle.
  ray.setOrigin(glm::dvec3(3.0, 3.0, -1.0));
  res = object->intersect(ray);
  ASSERT_TRUE(res);
  ASSERT_DOUBLE_EQ(1.0, res.getDistance());
  ASSERT_VEC_NEAR(glm::dvec3(0.0, 0.0, -1.0), (res.getNormalRay().getDirection()), EPS_WEAK);

  // Test ray falling on vertex.
  ray.setOrigin(glm::dvec3(10.0, 5.0, 0.0));
  ray.setDirection(glm::dvec3(-1.0, 0.0, 0.0));
  res = object->intersect(ray);
  ASSERT_TRUE(res);
  ASSERT_DOUBLE_EQ(5.0, res.getDistance());
  ASSERT_VEC_NEAR(glm::dvec3(1.0, 0.0, 0.0), (res.getNormalRay().getDirection()), EPS_WEAK);
}
