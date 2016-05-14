#include "Tests.h"
#include "Sphere.h"

TEST(SphereTests, IntersectionTest) {
  Object3d *s1 = new Sphere(glm::dvec3(0.0, 0.0, 0.0), 5.0);

  // 2-points intersection.
  Ray ray1(glm::dvec3(10.0, 0.0, 0.0), glm::dvec3(-1.0, 0.0, 0.0));
  IntersectionResult res1 = s1->intersect(ray1);
  ASSERT_TRUE(res1);
  ASSERT_DOUBLE_EQ(5.0, res1.getDistance());
  ASSERT_VEC_NEAR(glm::dvec3(5.0, 0.0, 0.0), res1.getNormalRay().getOrigin(), EPS_WEAK);
  ASSERT_VEC_NEAR(glm::dvec3(1.0, 0.0, 0.0), res1.getNormalRay().getDirection(), EPS_WEAK);

  // 1-point intersection.
  Ray ray2(glm::dvec3(10.0, 5.0, 0.0), glm::dvec3(-1.0, 0.0, 0.0));
  res1 = s1->intersect(ray2);
  ASSERT_TRUE(res1);
  ASSERT_DOUBLE_EQ(10.0, res1.getDistance());
  ASSERT_VEC_NEAR(glm::dvec3(0.0, 5.0, 0.0), res1.getNormalRay().getOrigin(), EPS_WEAK);
  ASSERT_VEC_NEAR(glm::dvec3(0.0, 1.0, 0.0), res1.getNormalRay().getDirection(), EPS_WEAK);

  // No intersection.
  Ray ray3(glm::dvec3(10.0, 6.0, 0.0), glm::dvec3(-1.0, 0.0, 0.0));
  res1 = s1->intersect(ray3);
  ASSERT_FALSE(res1);

  delete s1;
}