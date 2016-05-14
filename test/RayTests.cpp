#include "Tests.h"
#include "Ray.h"

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
