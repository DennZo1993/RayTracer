#include "gtest\gtest.h"

#include "Camera.h"
#include "Ray.h"

#define ASSERT_VEC_NEAR(vec1, vec2, epsilon) \
	ASSERT_NEAR(glm::length(vec1 - vec2), 0.0f, epsilon)

// Constants for floating-point comparison.
const double EPS_WEAK = 1.0e-7;
const double EPS_STRONG = 1.0e-9;

// Some normalized vec3 constants.
const glm::vec3 &X_NORM_VEC = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 &Y_NORM_VEC = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 &Z_NORM_VEC = glm::vec3(0.0f, 0.0f, 1.0f);

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

// === Ray tests ===
TEST(RayTests, ReflectionTest) {
	glm::vec3 origin(0.0f, 0.0f, 0.0f);
	glm::vec3 direction(1.0f, 0.0f, 0.0f);

	//                 ^
	// Reflection:     |/
	//             --->/
	Ray ray1(origin, direction);
	Ray normRay1(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 1.0f, 0.0f));
	Ray reflected1 = ray1.Reflect(normRay1);
	ASSERT_VEC_NEAR(reflected1.origin, normRay1.origin, EPS_STRONG);
	ASSERT_VEC_NEAR(reflected1.direction, glm::vec3(0.0f, 1.0f, 0.0f), EPS_WEAK);

	// Reflection exactly backwards.   ---> |
	// Test normalized version.         <---|
	Ray normRay2(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	Ray reflected2 = ray1.ReflectNormalized(normRay2);
	ASSERT_VEC_NEAR(reflected2.origin, normRay2.origin, EPS_STRONG);
	ASSERT_VEC_NEAR(reflected2.direction, -1.0f * ray1.direction, EPS_STRONG);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	return 0;
}