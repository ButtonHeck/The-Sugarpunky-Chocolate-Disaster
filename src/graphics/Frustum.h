#ifndef FRUSTUM_H
#define FRUSTUM_H
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

enum FrustumSide
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
  Frustum();
  void updateFrustum(glm::mat4& projectionView);
  bool isInside(float x, float y, float z);
  glm::vec4 getPlane(FrustumSide side);
private:
  void normalizePlane(FrustumSide side);
  std::vector<glm::vec4> frustumPlanes;
};

#endif // FRUSTUM_H
