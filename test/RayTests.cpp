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
  ASSERT_VEC_NEAR(reflected1.getOrigin(), normRay1.getOrigin(), EPS_STRONG);
  ASSERT_VEC_NEAR(reflected1.getDirection(), glm::dvec3(0.0, 1.0, 0.0), EPS_WEAK);

  // Reflection exactly backwards.   ---> |
  //                                  <---|
  Ray normRay2(glm::dvec3(10.0, 0.0, 0.0), glm::dvec3(-1.0, 0.0, 0.0));
  Ray reflected2 = ray1.Reflect(normRay2);
  ASSERT_VEC_NEAR(reflected2.getOrigin(), normRay2.getOrigin(), EPS_STRONG);
  ASSERT_VEC_NEAR(reflected2.getDirection(), -1.0 * ray1.getDirection(), EPS_STRONG);
}
