#include "Tests.h"
#include "Ray.h"

// === Ray tests ===
TEST(RayTests, ReflectionTest) {
  //                 ^
  // Reflection:     |/
  //             --->/
  Ray ray1(ZERO_VEC, X_NORM_VEC);
  Ray normRay1(glm::dvec3(5.0, 0.0, 0.0), glm::dvec3(-1.0, 1.0, 0.0));
  Ray reflected1 = ray1.Reflect(normRay1);
  ASSERT_VEC_NEAR(reflected1.GetOrigin(), normRay1.GetOrigin(), EPS_STRONG);
  ASSERT_VEC_NEAR(reflected1.GetDirection(), Y_NORM_VEC, EPS_WEAK);

  // Reflection exactly backwards.   ---> |
  //                                  <---|
  Ray normRay2(glm::dvec3(10.0, 0.0, 0.0), -X_NORM_VEC);
  Ray reflected2 = ray1.Reflect(normRay2);
  ASSERT_VEC_NEAR(reflected2.GetOrigin(), normRay2.GetOrigin(), EPS_STRONG);
  ASSERT_VEC_NEAR(reflected2.GetDirection(), -1.0 * ray1.GetDirection(), EPS_STRONG);
}
