#include "SkyboxShader.h"

SkyboxShader::SkyboxShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void SkyboxShader::update(glm::mat4 &projectionView, glm::vec3 &viewPosition, int backgroundIndex, bool isStatic)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_viewPosition", viewPosition);
  renderShader.setInt("u_index", backgroundIndex);
  renderShader.setInt("u_static", isStatic);
}
