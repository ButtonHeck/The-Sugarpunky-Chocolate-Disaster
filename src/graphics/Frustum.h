#ifndef FRUSTUM_H
#define FRUSTUM_H
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <algorithm>
#include "../../src/util/BenchmarkTimer.h"

enum FRUSTUM_PLANE
{
  FRUSTUM_RIGHT = 0,
  FRUSTUM_LEFT = 1,
  FRUSTUM_BACK = 2,
  FRUSTUM_FRONT = 3,
  FRUSTUM_BOTTOM = 4,
  FRUSTUM_TOP = 5
};

class Frustum
{
public:
  constexpr static unsigned int NUMBER_OF_DISTINCT_VERTICES = 8;
  constexpr static unsigned int NUMBER_OF_PLANES = 6;

  Frustum();
  void updateFrustum(const glm::mat4 &projectionView);
  void calculateIntersectionPoints();
  float getMaxCoordX() const;
  float getMinCoordX() const;
  float getMaxCoordZ() const;
  float getMinCoordZ() const;
  bool isInside(float x, float y, float z, float radius) const;
  const glm::vec4 &getPlane(FRUSTUM_PLANE plane) const;

private:
  friend class ShadowVolume;
  friend class FrustumRenderer;

  void normalizePlane(FRUSTUM_PLANE plane);
  glm::vec3 kramerIntersection(const glm::vec4 &frontOrBack, const glm::vec4 &topOrBottom, const glm::vec4 &rightOrLeft);
  std::vector<glm::vec4> planes;
  //8 vertices defined in the world space represents intersection points of frustum planes
  glm::vec3 nearLL, nearLR, nearUR, nearUL, farLL, farLR, farUR, farUL;
};

#endif // FRUSTUM_H
