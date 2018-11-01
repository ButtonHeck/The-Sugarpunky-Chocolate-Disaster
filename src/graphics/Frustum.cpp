#include "graphics/Frustum.h"

Frustum::Frustum()
{
  for (unsigned int i = 0; i < 5; i++)
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
}

bool Frustum::isInsideXZ(float x, float z, float radius) const
{
  for (unsigned int i = 0; i < 5; i++)
    {
      if (frustumPlanes[i].x * x + frustumPlanes[i].z * z + frustumPlanes[i].w <= -radius)
        return false;
    }
  return true;
}

bool Frustum::isInside(float x, float y, float z, float radius) const
{
  for (unsigned int i = 0; i < 5; i++)
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
