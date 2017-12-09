#include "Tests.h"
#include "Sphere.h"

TEST(SphereTests, IntersectionTest) {
  IObject3D *s1 = new Sphere(ZERO_VEC, 5.0, testMaterial1);

  // 2-points Intersection.
  Ray ray1(glm::dvec3(10.0, 0.0, 0.0), -X_NORM_VEC);
  IntersectionResult res1 = s1->Intersect(ray1);
  ASSERT_TRUE(res1);
  ASSERT_DOUBLE_EQ(5.0, res1.GetDistance());
  ASSERT_VEC_NEAR(glm::dvec3(5.0, 0.0, 0.0), res1.GetNormalRay().GetOrigin(), EPS_WEAK);
  ASSERT_VEC_NEAR(X_NORM_VEC, res1.GetNormalRay().GetDirection(), EPS_WEAK);

  // 1-point Intersection.
  Ray ray2(glm::dvec3(10.0, 5.0, 0.0), -X_NORM_VEC);
  res1 = s1->Intersect(ray2);
  ASSERT_TRUE(res1);
  ASSERT_DOUBLE_EQ(10.0, res1.GetDistance());
  ASSERT_VEC_NEAR(glm::dvec3(0.0, 5.0, 0.0), res1.GetNormalRay().GetOrigin(), EPS_WEAK);
  ASSERT_VEC_NEAR(Y_NORM_VEC, res1.GetNormalRay().GetDirection(), EPS_WEAK);

  // No Intersection.
  Ray ray3(glm::dvec3(10.0, 6.0, 0.0), -X_NORM_VEC);
  res1 = s1->Intersect(ray3);
  ASSERT_FALSE(res1);

  delete s1;
}
