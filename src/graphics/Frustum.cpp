#include "graphics/Frustum.h"

Frustum::Frustum()
{
  planes.reserve(NUMBER_OF_PLANES);
  for (unsigned int planeIndex = 0; planeIndex < NUMBER_OF_PLANES; planeIndex++)
    planes.emplace_back(0.0f);
}

void Frustum::normalizePlane(FRUSTUM_PLANE plane)
{
  float magnitude = (float)glm::sqrt(planes[plane].x * planes[plane].x +
                                     planes[plane].y * planes[plane].y +
                                     planes[plane].z * planes[plane].z);
  planes[plane] /= magnitude;
}

void Frustum::updateFrustum(const glm::mat4 &projectionView)
{
  BENCHMARK("Frustum: update", true);
  const float* projectionViewElements = glm::value_ptr(projectionView);
  planes[FRUSTUM_RIGHT].x = projectionViewElements[3] - projectionViewElements[0];
  planes[FRUSTUM_RIGHT].y = projectionViewElements[7] - projectionViewElements[4];
  planes[FRUSTUM_RIGHT].z = projectionViewElements[11] - projectionViewElements[8];
  planes[FRUSTUM_RIGHT].w = projectionViewElements[15] - projectionViewElements[12];
  normalizePlane(FRUSTUM_RIGHT);

  planes[FRUSTUM_LEFT].x = projectionViewElements[3] + projectionViewElements[0];
  planes[FRUSTUM_LEFT].y = projectionViewElements[7] + projectionViewElements[4];
  planes[FRUSTUM_LEFT].z = projectionViewElements[11] + projectionViewElements[8];
  planes[FRUSTUM_LEFT].w = projectionViewElements[15] + projectionViewElements[12];
  normalizePlane(FRUSTUM_LEFT);

  planes[FRUSTUM_BOTTOM].x = projectionViewElements[3] + projectionViewElements[1];
  planes[FRUSTUM_BOTTOM].y = projectionViewElements[7] + projectionViewElements[5];
  planes[FRUSTUM_BOTTOM].z = projectionViewElements[11] + projectionViewElements[9];
  planes[FRUSTUM_BOTTOM].w = projectionViewElements[15] + projectionViewElements[13];
  normalizePlane(FRUSTUM_BOTTOM);

  planes[FRUSTUM_TOP].x = projectionViewElements[3] - projectionViewElements[1];
  planes[FRUSTUM_TOP].y = projectionViewElements[7] - projectionViewElements[5];
  planes[FRUSTUM_TOP].z = projectionViewElements[11] - projectionViewElements[9];
  planes[FRUSTUM_TOP].w = projectionViewElements[15] - projectionViewElements[13];
  normalizePlane(FRUSTUM_TOP);

  planes[FRUSTUM_BACK].x = projectionViewElements[3] - projectionViewElements[2];
  planes[FRUSTUM_BACK].y = projectionViewElements[7] - projectionViewElements[6];
  planes[FRUSTUM_BACK].z = projectionViewElements[11] - projectionViewElements[10];
  planes[FRUSTUM_BACK].w = projectionViewElements[15] - projectionViewElements[14];
  normalizePlane(FRUSTUM_BACK);

  planes[FRUSTUM_FRONT].x = projectionViewElements[3] + projectionViewElements[2];
  planes[FRUSTUM_FRONT].y = projectionViewElements[7] + projectionViewElements[6];
  planes[FRUSTUM_FRONT].z = projectionViewElements[11] + projectionViewElements[10];
  planes[FRUSTUM_FRONT].w = projectionViewElements[15] + projectionViewElements[14];
  normalizePlane(FRUSTUM_FRONT);
}

void Frustum::calculateIntersectionPoints()
{
  const glm::vec4& FRONT = planes[FRUSTUM_FRONT];
  const glm::vec4& BACK = planes[FRUSTUM_BACK];
  const glm::vec4& LEFT = planes[FRUSTUM_LEFT];
  const glm::vec4& RIGHT = planes[FRUSTUM_RIGHT];
  const glm::vec4& TOP = planes[FRUSTUM_TOP];
  const glm::vec4& BOTTOM = planes[FRUSTUM_BOTTOM];
  nearLL = kramerIntersection(FRONT, BOTTOM, LEFT);
  nearLR = kramerIntersection(FRONT, BOTTOM, RIGHT);
  nearUR = kramerIntersection(FRONT, TOP, RIGHT);
  nearUL = kramerIntersection(FRONT, TOP, LEFT);
  farLL = kramerIntersection(BACK, BOTTOM, LEFT);
  farLR = kramerIntersection(BACK, BOTTOM, RIGHT);
  farUR = kramerIntersection(BACK, TOP, RIGHT);
  farUL = kramerIntersection(BACK, TOP, LEFT);
}

float Frustum::getMaxCoordX() const
{
  return std::max({nearLL.x, nearLR.x, nearUR.x, nearUL.x,
                   farLL.x, farLR.x, farUR.x, farUL.x});
}

float Frustum::getMinCoordX() const
{
  return std::min({nearLL.x, nearLR.x, nearUR.x, nearUL.x,
                   farLL.x, farLR.x, farUR.x, farUL.x});
}

float Frustum::getMaxCoordZ() const
{
  return std::max({nearLL.z, nearLR.z, nearUR.z, nearUL.z,
                   farLL.z, farLR.z, farUR.z, farUL.z});
}

float Frustum::getMinCoordZ() const
{
  return std::min({nearLL.z, nearLR.z, nearUR.z, nearUL.z,
                   farLL.z, farLR.z, farUR.z, farUL.z});
}

glm::vec3 Frustum::kramerIntersection(const glm::vec4& frontOrBack, const glm::vec4& topOrBottom, const glm::vec4& rightOrLeft)
{
  glm::mat3 xyzCoefficientMatrix;
  xyzCoefficientMatrix[0] = glm::vec3(frontOrBack);
  xyzCoefficientMatrix[1] = glm::vec3(topOrBottom);
  xyzCoefficientMatrix[2] = glm::vec3(rightOrLeft);
  glm::mat3 wyzCoefficientMatrix = xyzCoefficientMatrix;
  wyzCoefficientMatrix[0][0] = -frontOrBack.w;
  wyzCoefficientMatrix[1][0] = -topOrBottom.w;
  wyzCoefficientMatrix[2][0] = -rightOrLeft.w;
  glm::mat3 xwzCoefficientMatrix = xyzCoefficientMatrix;
  xwzCoefficientMatrix[0][1] = -frontOrBack.w;
  xwzCoefficientMatrix[1][1] = -topOrBottom.w;
  xwzCoefficientMatrix[2][1] = -rightOrLeft.w;
  glm::mat3 xywCoefficientMatrix = xyzCoefficientMatrix;
  xywCoefficientMatrix[0][2] = -frontOrBack.w;
  xywCoefficientMatrix[1][2] = -topOrBottom.w;
  xywCoefficientMatrix[2][2] = -rightOrLeft.w;

  float xyzCoefficientMatrixDeterminant = glm::determinant(xyzCoefficientMatrix);
  float wyzCoefficientMatrixDeterminant = glm::determinant(wyzCoefficientMatrix);
  float xwzCoefficientMatrixDeterminant = glm::determinant(xwzCoefficientMatrix);
  float xywCoefficientMatrixDeterminant = glm::determinant(xywCoefficientMatrix);

  glm::vec3 intersectionPoint;
  intersectionPoint.x = wyzCoefficientMatrixDeterminant / xyzCoefficientMatrixDeterminant;
  intersectionPoint.y = xwzCoefficientMatrixDeterminant / xyzCoefficientMatrixDeterminant;
  intersectionPoint.z = xywCoefficientMatrixDeterminant / xyzCoefficientMatrixDeterminant;
  return intersectionPoint;
}

bool Frustum::isInsideXZ(float x, float z, float radius) const
{
  for (unsigned int planeIndex = 0; planeIndex <= FRUSTUM_FRONT; planeIndex++)
    {
      if (planes[planeIndex].x * x + planes[planeIndex].z * z + planes[planeIndex].w <= -radius)
        return false;
    }
  return true;
}

bool Frustum::isInside(float x, float y, float z, float radius) const
{
  for (unsigned int planeIndex = 0; planeIndex < NUMBER_OF_PLANES; planeIndex++)
    {
      if (planes[planeIndex].x * x + planes[planeIndex].y * y + planes[planeIndex].z * z + planes[planeIndex].w <= -radius)
        return false;
    }
  return true;
}

const glm::vec4& Frustum::getPlane(FRUSTUM_PLANE plane) const
{
  return planes[plane];
}
