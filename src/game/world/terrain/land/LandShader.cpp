#include "LandShader.h"

LandShader::LandShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void LandShader::update(glm::mat4 &projectionView, bool useShadows)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setBool("u_shadowEnable", useShadows);
}
