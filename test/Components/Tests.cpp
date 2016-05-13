#include "gtest\gtest.h"

#include "Camera.h"
#include "Ray.h"
#include "Mesh.h"

#define ASSERT_VEC_NEAR(vec1, vec2, epsilon) \
  ASSERT_NEAR(glm::length(vec1 - vec2), 0.0, epsilon)

// Constants for floating-point comparison.
const double EPS_WEAK = 1.0e-7;
const double EPS_STRONG = 1.0e-9;

// Some normalized vec3 constants.
const glm::dvec3 &X_NORM_VEC = glm::dvec3(1.0, 0.0, 0.0);
const glm::dvec3 &Y_NORM_VEC = glm::dvec3(0.0, 1.0, 0.0);
const glm::dvec3 &Z_NORM_VEC = glm::dvec3(0.0, 0.0, 1.0);

// === Camera tests ===
TEST(CameraTests, NormalizationTest) {
  glm::dvec3 cameraPos(0.0, 0.0, 0.0);
  glm::dvec3 cameraDir(10.0, 0.0, 0.0);
  glm::uvec2 cameraRes(640, 480);

  Camera camera(cameraPos, cameraDir, cameraRes);

  ASSERT_DOUBLE_EQ(glm::length(camera.getDirection()), 1.0);
}

TEST(CameraTests, MoveTest) {
  glm::dvec3 cameraPos(0.0, 0.0, 0.0);
  glm::dvec3 cameraDir(10.0, 0.0, 0.0);
  glm::uvec2 cameraRes(640, 480);
  Camera camera(cameraPos, cameraDir, cameraRes);

  // Look at (0, 25, 0). Expected direction: (0, 1, 0).
  camera.LookAt(glm::dvec3(0.0, 25.0, 0.0));
  ASSERT_EQ(camera.getDirection(), glm::dvec3(0.0, 1.0, 0.0));
  ASSERT_EQ(camera.getPosition(), glm::dvec3(0.0, 0.0, 0.0));

  // Look at camera's position.
  // FIXME: Check exception! Currently check that nothing changed.
  camera.LookAt(glm::dvec3(0.0, 0.0, 0.0));
  ASSERT_EQ(camera.getDirection(), glm::dvec3(0.0, 1.0, 0.0));
  ASSERT_EQ(camera.getPosition(), glm::dvec3(0.0, 0.0, 0.0));

  // Move to.
  camera.MoveTo(glm::dvec3(0.0, 2.0, 0.0));
  ASSERT_EQ(camera.getDirection(), glm::dvec3(0.0, -1.0, 0.0));
  ASSERT_EQ(camera.getPosition(), glm::dvec3(0.0, 2.0, 0.0));

  // Move to focus point (0, 1, 0).
  camera.MoveTo(glm::dvec3(0.0, 1.0, 0.0));
  ASSERT_EQ(camera.getDirection(), glm::dvec3(0.0, -1.0, 0.0));
  ASSERT_EQ(camera.getPosition(), glm::dvec3(0.0, 1.0, 0.0));

  // Move forward.
  camera.MoveForward(10.0);
  ASSERT_EQ(camera.getDirection(), glm::dvec3(0.0, -1.0, 0.0));
  ASSERT_EQ(camera.getPosition(), glm::dvec3(0.0, -9.0, 0.0));

  camera.MoveForward(-1.0);
  ASSERT_EQ(camera.getDirection(), glm::dvec3(0.0, -1.0, 0.0));
  ASSERT_EQ(camera.getPosition(), glm::dvec3(0.0, -8.0, 0.0));

  // Move backward.
  camera.MoveBackward(5.0);
  ASSERT_EQ(camera.getDirection(), glm::dvec3(0.0, -1.0, 0.0));
  ASSERT_EQ(camera.getPosition(), glm::dvec3(0.0, -3.0, 0.0));

  camera.MoveBackward(-2.0);
  ASSERT_EQ(camera.getDirection(), glm::dvec3(0.0, -1.0, 0.0));
  ASSERT_EQ(camera.getPosition(), glm::dvec3(0.0, -5.0, 0.0));
}

// === Ray tests ===
TEST(RayTests, ReflectionTest) {
  glm::dvec3 origin(0.0, 0.0, 0.0);
  glm::dvec3 direction(1.0, 0.0, 0.0);

  //                 ^
  // Reflection:     |/
  //             --->/
  Ray ray1(origin, direction);
  Ray normRay1(glm::dvec3(5.0, 0.0, 0.0), glm::dvec3(-1.0, 1.0, 0.0));
  Ray reflected1 = ray1.Reflect(normRay1);
  ASSERT_VEC_NEAR(reflected1.origin, normRay1.origin, EPS_STRONG);
  ASSERT_VEC_NEAR(reflected1.direction, glm::dvec3(0.0, 1.0, 0.0), EPS_WEAK);

  // Reflection exactly backwards.   ---> |
  // Test normalized version.         <---|
  Ray normRay2(glm::dvec3(10.0, 0.0, 0.0), glm::dvec3(-1.0, 0.0, 0.0));
  Ray reflected2 = ray1.ReflectNormalized(normRay2);
  ASSERT_VEC_NEAR(reflected2.origin, normRay2.origin, EPS_STRONG);
  ASSERT_VEC_NEAR(reflected2.direction, -1.0 * ray1.direction, EPS_STRONG);
}

// === Mesh tests ===
TEST(MeshTests, FaceSquaresTest) {
  Mesh mesh(true);

  // Cube.
  // Front plane.
  auto v0 = mesh.addVertex(glm::dvec3(0.0, 0.0, 0.0));
  auto v1 = mesh.addVertex(glm::dvec3(5.0, 0.0, 0.0));
  auto v2 = mesh.addVertex(glm::dvec3(5.0, 5.0, 0.0));
  auto v3 = mesh.addVertex(glm::dvec3(0.0, 5.0, 0.0));
  // Back plane.
  auto v4 = mesh.addVertex(glm::dvec3(0.0, 0.0, 5.0));
  auto v5 = mesh.addVertex(glm::dvec3(5.0, 0.0, 5.0));
  auto v6 = mesh.addVertex(glm::dvec3(5.0, 5.0, 5.0));
  auto v7 = mesh.addVertex(glm::dvec3(0.0, 5.0, 5.0));
  // Faces (ccw).
  auto f0 = mesh.addQuadFace(v0, v1, v2, v3);
  auto f1 = mesh.addQuadFace(v1, v5, v6, v2);
  auto f2 = mesh.addQuadFace(v3, v2, v6, v7);
  auto f3 = mesh.addQuadFace(v4, v0, v3, v7);
  auto f4 = mesh.addQuadFace(v5, v4, v7, v6);
  auto f5 = mesh.addQuadFace(v4, v5, v1, v0);

  mesh.CalculateNormals();

  // Test squares
  ASSERT_EQ(mesh.getNumVertexes(), 8);
  ASSERT_EQ(mesh.getNumFaces(), 12);
  for (const auto &face : mesh.getFaces()) {
    ASSERT_DOUBLE_EQ(face.getSquare(), 12.5);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
  return 0;
}