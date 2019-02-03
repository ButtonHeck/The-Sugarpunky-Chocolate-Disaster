#include "game/world/ambience/theSun/TheSunShader.h"

TheSunShader::TheSunShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void TheSunShader::update(const glm::mat4 &projectionView, const glm::mat4& model)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setMat4("u_model", model);
}
