#ifndef FRUSTUM_H
#define FRUSTUM_H
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "util/BenchmarkTimer.h"

enum FRUSTUM_PLANE
{
  FRUSTUM_RIGHT = 0,
  FRUSTUM_LEFT = 1,
  FRUSTUM_BOTTOM = 2,
  FRUSTUM_TOP = 3,
  FRUSTUM_BACK = 4,
  FRUSTUM_FRONT = 5
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
  bool isInsideXZ(float x, float z, float radius) const;
  bool isInside(float x, float y, float z, float radius) const;
  const glm::vec4 &getPlane(FRUSTUM_PLANE plane) const;

private:
  friend class ShadowVolume;
  friend class FrustumRenderer;
  void normalizePlane(FRUSTUM_PLANE plane);
  glm::vec3 kramerIntersection(glm::vec4 frontOrBack, glm::vec4 topOrBottom, glm::vec4 rightOrLeft);
  std::vector<glm::vec4> frustumPlanes;
  glm::vec3 nearLL, nearLR, nearUR, nearUL, farLL, farLR, farUR, farUL;
};

#endif // FRUSTUM_H
