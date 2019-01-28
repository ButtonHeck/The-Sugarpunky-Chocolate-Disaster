#include "game/world/models/hemisphere/HemisphereShader.h"

HemisphereShader::HemisphereShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void HemisphereShader::update(const glm::mat4 &projectionView,
                              const glm::vec3 &viewPosition,
                              const glm::vec3 &lightDir)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_viewPosition", viewPosition);
  renderShader.setVec3("u_lightDir", -lightDir);
}

void HemisphereShader::setTransformMatrix(const glm::mat4 &transform)
{
  renderShader.setMat4("u_model", transform);
}

void HemisphereShader::setSkyMode(bool drawSky)
{

  renderShader.setBool("u_drawSky", drawSky);
}
