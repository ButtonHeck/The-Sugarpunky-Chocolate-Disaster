#include "game/world/ambience/skybox/SkyboxShader.h"

SkyboxShader::SkyboxShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void SkyboxShader::update(const glm::mat4 &projectionView,
                          const glm::vec3 &viewPosition,
                          const glm::vec3 &lightDir)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setVec3("u_viewPosition", viewPosition);
  renderShader.setVec3("u_lightDir", -lightDir);
}

void SkyboxShader::selectSkyboxType(int type)
{
  renderShader.setInt("u_type", type);
}
