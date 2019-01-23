#include "game/world/ambience/theSun/TheSunShader.h"

TheSunShader::TheSunShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void TheSunShader::update(glm::mat4 &projectionView, glm::mat4& model)
{
  renderShader.use();
  renderShader.setMat4("u_projectionView", projectionView);
  renderShader.setMat4("u_model", model);
}
