#include "game/world/ambience/skysphere/SkysphereShader.h"

SkysphereShader::SkysphereShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void SkysphereShader::update(const glm::mat4 &projectionView, const glm::vec3 &lightDir)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_lightDir", -lightDir);
}

void SkysphereShader::setTransformMatrix(const glm::mat4 &transform)
{
  renderShader.setMat4("u_model", transform);
}

void SkysphereShader::setSkysphereType(int type)
{
  renderShader.setBool("u_type", type);
}
