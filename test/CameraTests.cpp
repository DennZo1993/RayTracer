#include "Tests.h"
#include "Camera.h"

// === Camera tests ===
TEST(CameraTests, NormalizationTest) {
  glm::dvec3 cameraDir(10.0, 0.0, 0.0);
  glm::uvec2 cameraRes(640, 480);

  Camera camera(ZERO_VEC, cameraDir, cameraRes);

  ASSERT_DOUBLE_EQ(glm::length(camera.GetDirection()), 1.0);
}

TEST(CameraTests, MoveTest) {
  glm::dvec3 cameraDir(10.0, 0.0, 0.0);
  glm::uvec2 cameraRes(640, 480);
  Camera camera(ZERO_VEC, cameraDir, cameraRes);

  // Look at (0, 25, 0). Expected direction: (0, 1, 0).
  camera.LookAt(glm::dvec3(0.0, 25.0, 0.0));
  ASSERT_VEC_NEAR(camera.GetDirection(), Y_NORM_VEC, EPS_STRONG);
  ASSERT_VEC_NEAR(camera.GetPosition(), ZERO_VEC, EPS_STRONG);

  // Look at camera's position.
  // FIXME: Check exception! Currently check that nothing changed.
  camera.LookAt(ZERO_VEC);
  ASSERT_VEC_NEAR(camera.GetDirection(), Y_NORM_VEC, EPS_STRONG);
  ASSERT_VEC_NEAR(camera.GetPosition(), ZERO_VEC, EPS_STRONG);

  // Move to.
  camera.MoveTo(glm::dvec3(0.0, 2.0, 0.0));
  ASSERT_EQ(camera.GetDirection(), glm::dvec3(0.0, -1.0, 0.0));
  ASSERT_EQ(camera.GetPosition(), glm::dvec3(0.0, 2.0, 0.0));

  // Move to focus point (0, 1, 0).
  camera.MoveTo(Y_NORM_VEC);
  ASSERT_VEC_NEAR(camera.GetDirection(), -Y_NORM_VEC, EPS_STRONG);
  ASSERT_VEC_NEAR(camera.GetPosition(), Y_NORM_VEC, EPS_STRONG);

  // Move forward.
  camera.MoveForward(10.0);
  ASSERT_VEC_NEAR(camera.GetDirection(), -Y_NORM_VEC, EPS_STRONG);
  ASSERT_VEC_NEAR(camera.GetPosition(), glm::dvec3(0.0, -9.0, 0.0), EPS_STRONG);

  camera.MoveForward(-1.0);
  ASSERT_VEC_NEAR(camera.GetDirection(), -Y_NORM_VEC, EPS_STRONG);
  ASSERT_VEC_NEAR(camera.GetPosition(), glm::dvec3(0.0, -8.0, 0.0), EPS_STRONG);

  // Move backward.
  camera.MoveBackward(5.0);
  ASSERT_VEC_NEAR(camera.GetDirection(), -Y_NORM_VEC, EPS_STRONG);
  ASSERT_VEC_NEAR(camera.GetPosition(), glm::dvec3(0.0, -3.0, 0.0), EPS_STRONG);

  camera.MoveBackward(-2.0);
  ASSERT_VEC_NEAR(camera.GetDirection(), -Y_NORM_VEC, EPS_STRONG);
  ASSERT_VEC_NEAR(camera.GetPosition(), glm::dvec3(0.0, -5.0, 0.0), EPS_STRONG);
}
