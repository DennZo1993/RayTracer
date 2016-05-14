#include "Tests.h"
#include "Camera.h"

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