#include "PlantsShader.h"

PlantsShader::PlantsShader(Shader &renderPhongShader, Shader &renderGouraudShader, Shader &shadowShader)
  :
    renderPhongShader(renderPhongShader),
    renderGouraudShader(renderGouraudShader),
    shadowShader(shadowShader)
{}

void PlantsShader::update(glm::mat4 &projectionView,
                          glm::vec3 &viewPosition,
                          bool shadowOnTrees,
                          bool useShadows,
                          bool useFlatBlending)
{
  renderGouraudShader.use();
  renderGouraudShader.setMat4("u_projectionView", projectionView);
  renderGouraudShader.setVec3("u_viewPosition", viewPosition);
  renderGouraudShader.setBool("u_shadow", shadowOnTrees);
  renderGouraudShader.setBool("u_shadowEnable", useShadows);
  renderGouraudShader.setBool("u_useFlatBlending", useFlatBlending);

  renderPhongShader.use();
  renderPhongShader.setMat4("u_projectionView", projectionView);
  renderPhongShader.setVec3("u_viewPosition", viewPosition);
  renderPhongShader.setBool("u_shadow", shadowOnTrees);
  renderPhongShader.setBool("u_shadowEnable", useShadows);
  renderPhongShader.setBool("u_useFlatBlending", useFlatBlending);
}
