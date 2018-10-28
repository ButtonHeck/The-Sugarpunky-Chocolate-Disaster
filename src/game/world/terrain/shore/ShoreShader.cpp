#include "ShoreShader.h"

ShoreShader::ShoreShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void ShoreShader::update(glm::mat4 &projectionView, bool useShadows)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setBool("u_shadowEnable", useShadows);
}
