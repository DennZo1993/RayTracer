#include "gtest\gtest.h"

#include "Camera.h"


// === Camera tests ===
TEST(CameraTests, NormalizationTest) {
	glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDir(10.0f, 0.0f, 0.0f);
	glm::uvec2 cameraRes(640, 480);

	Camera camera(cameraPos, cameraDir, cameraRes);

	ASSERT_FLOAT_EQ(glm::length(camera.getDirection()), 1.0);
}

TEST(CameraTests, MoveTest) {
	glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDir(10.0f, 0.0f, 0.0f);
	glm::uvec2 cameraRes(640, 480);
	Camera camera(cameraPos, cameraDir, cameraRes);

	// Look at (0, 25, 0). Expected direction: (0, 1, 0).
	camera.LookAt(glm::vec3(0.0f, 25.0f, 0.0f));
	ASSERT_EQ(camera.getDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
	ASSERT_EQ(camera.getPosition(), glm::vec3(0.0f, 0.0f, 0.0f));

	// Look at camera's position.
	// FIXME: Check exception! Currently check that nothing changed.
	camera.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	ASSERT_EQ(camera.getDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
	ASSERT_EQ(camera.getPosition(), glm::vec3(0.0f, 0.0f, 0.0f));

	// Move to.
	camera.MoveTo(glm::vec3(0.0f, 2.0f, 0.0f));
	ASSERT_EQ(camera.getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
	ASSERT_EQ(camera.getPosition(), glm::vec3(0.0f, 2.0f, 0.0f));

	// Move to focus point (0, 1, 0).
	camera.MoveTo(glm::vec3(0.0f, 1.0f, 0.0f));
	ASSERT_EQ(camera.getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
	ASSERT_EQ(camera.getPosition(), glm::vec3(0.0f, 1.0f, 0.0f));

	// Move forward.
	camera.MoveForward(10.0f);
	ASSERT_EQ(camera.getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
	ASSERT_EQ(camera.getPosition(), glm::vec3(0.0f, -9.0f, 0.0f));

	camera.MoveForward(-1.0f);
	ASSERT_EQ(camera.getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
	ASSERT_EQ(camera.getPosition(), glm::vec3(0.0f, -8.0f, 0.0f));

	// Move backward.
	camera.MoveBackward(5.0f);
	ASSERT_EQ(camera.getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
	ASSERT_EQ(camera.getPosition(), glm::vec3(0.0f, -3.0f, 0.0f));

	camera.MoveBackward(-2.0f);
	ASSERT_EQ(camera.getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
	ASSERT_EQ(camera.getPosition(), glm::vec3(0.0f, -5.0f, 0.0f));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	return 0;
}