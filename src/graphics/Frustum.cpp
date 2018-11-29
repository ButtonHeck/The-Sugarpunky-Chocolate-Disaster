#include "graphics/Frustum.h"

Frustum::Frustum()
{
  for (unsigned int i = 0; i < 6; i++)
    {
      frustumPlanes.emplace_back(0.0f);
    }
}

void Frustum::normalizePlane(FRUSTUM_SIDE side)
{
  float magnitude = (float)glm::sqrt(frustumPlanes[side].x * frustumPlanes[side].x +
                                     frustumPlanes[side].y * frustumPlanes[side].y +
                                     frustumPlanes[side].z * frustumPlanes[side].z);
  frustumPlanes[side] /= magnitude;
}

void Frustum::updateFrustum(glm::mat4 &projectionView)
{
  BENCHMARK("Frustum: update", true);
  float* clip = glm::value_ptr(projectionView);
  frustumPlanes[FRUSTUM_RIGHT].x = clip[3] - clip[0];
  frustumPlanes[FRUSTUM_RIGHT].y = clip[7] - clip[4];
  frustumPlanes[FRUSTUM_RIGHT].z = clip[11] - clip[8];
  frustumPlanes[FRUSTUM_RIGHT].w = clip[15] - clip[12];
  normalizePlane(FRUSTUM_RIGHT);

  frustumPlanes[FRUSTUM_LEFT].x = clip[3] + clip[0];
  frustumPlanes[FRUSTUM_LEFT].y = clip[7] + clip[4];
  frustumPlanes[FRUSTUM_LEFT].z = clip[11] + clip[8];
  frustumPlanes[FRUSTUM_LEFT].w = clip[15] + clip[12];
  normalizePlane(FRUSTUM_LEFT);

  frustumPlanes[FRUSTUM_BOTTOM].x = clip[3] + clip[1];
  frustumPlanes[FRUSTUM_BOTTOM].y = clip[7] + clip[5];
  frustumPlanes[FRUSTUM_BOTTOM].z = clip[11] + clip[9];
  frustumPlanes[FRUSTUM_BOTTOM].w = clip[15] + clip[13];
  normalizePlane(FRUSTUM_BOTTOM);

  frustumPlanes[FRUSTUM_TOP].x = clip[3] - clip[1];
  frustumPlanes[FRUSTUM_TOP].y = clip[7] - clip[5];
  frustumPlanes[FRUSTUM_TOP].z = clip[11] - clip[9];
  frustumPlanes[FRUSTUM_TOP].w = clip[15] - clip[13];
  normalizePlane(FRUSTUM_TOP);

  frustumPlanes[FRUSTUM_BACK].x = clip[3] - clip[2];
  frustumPlanes[FRUSTUM_BACK].y = clip[7] - clip[6];
  frustumPlanes[FRUSTUM_BACK].z = clip[11] - clip[10];
  frustumPlanes[FRUSTUM_BACK].w = clip[15] - clip[14];
  normalizePlane(FRUSTUM_BACK);

  frustumPlanes[FRUSTUM_FRONT].x = clip[3] + clip[2];
  frustumPlanes[FRUSTUM_FRONT].y = clip[7] + clip[6];
  frustumPlanes[FRUSTUM_FRONT].z = clip[11] + clip[10];
  frustumPlanes[FRUSTUM_FRONT].w = clip[15] + clip[14];
  normalizePlane(FRUSTUM_FRONT);
}

void Frustum::calculateIntersectionPoints()
{
  glm::vec4 front = frustumPlanes[FRUSTUM_FRONT];
  glm::vec4 back = frustumPlanes[FRUSTUM_BACK];
  glm::vec4 left = frustumPlanes[FRUSTUM_LEFT];
  glm::vec4 right = frustumPlanes[FRUSTUM_RIGHT];
  glm::vec4 top = frustumPlanes[FRUSTUM_TOP];
  glm::vec4 bottom = frustumPlanes[FRUSTUM_BOTTOM];
  nearLL = kramerIntersection(front, bottom, left);
  nearLR = kramerIntersection(front, bottom, right);
  nearUR = kramerIntersection(front, top, right);
  nearUL = kramerIntersection(front, top, left);
  farLL = kramerIntersection(back, bottom, left);
  farLR = kramerIntersection(back, bottom, right);
  farUR = kramerIntersection(back, top, right);
  farUL = kramerIntersection(back, top, left);
}

glm::vec3 Frustum::kramerIntersection(glm::vec4 frontOrBack, glm::vec4 topOrBottom, glm::vec4 rightOrLeft)
{
  glm::mat3 xyz;
  xyz[0] = glm::vec3(frontOrBack);
  xyz[1] = glm::vec3(topOrBottom);
  xyz[2] = glm::vec3(rightOrLeft);
  glm::mat3 wyz = xyz;
  wyz[0][0] = -frontOrBack.w;
  wyz[1][0] = -topOrBottom.w;
  wyz[2][0] = -rightOrLeft.w;
  glm::mat3 xwz = xyz;
  xwz[0][1] = -frontOrBack.w;
  xwz[1][1] = -topOrBottom.w;
  xwz[2][1] = -rightOrLeft.w;
  glm::mat3 xyw = xyz;
  xyw[0][2] = -frontOrBack.w;
  xyw[1][2] = -topOrBottom.w;
  xyw[2][2] = -rightOrLeft.w;

  float xyzDet = glm::determinant(xyz);
  float wyzDet = glm::determinant(wyz);
  float xwzDet = glm::determinant(xwz);
  float xywDet = glm::determinant(xyw);

  glm::vec3 result;
  result.x = wyzDet / xyzDet;
  result.y = xwzDet / xyzDet;
  result.z = xywDet / xyzDet;
  return result;
}

bool Frustum::isInsideXZ(float x, float z, float radius) const
{
  for (unsigned int i = 0; i < 6; i++)
    {
      if (frustumPlanes[i].x * x + frustumPlanes[i].z * z + frustumPlanes[i].w <= -radius)
        return false;
    }
  return true;
}

bool Frustum::isInside(float x, float y, float z, float radius) const
{
  for (unsigned int i = 0; i < 6; i++)
    {
      if (frustumPlanes[i].x * x + frustumPlanes[i].y * y + frustumPlanes[i].z * z + frustumPlanes[i].w <= -radius)
        return false;
    }
  return true;
}

glm::vec4 Frustum::getPlane(FRUSTUM_SIDE side)
{
  return frustumPlanes[side];
}
