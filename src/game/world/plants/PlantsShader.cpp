#include "PlantsShader.h"

PlantsShader::PlantsShader(Shader &renderPhongShader, Shader &renderGouraudShader)
  :
    renderPhongShader(renderPhongShader),
    renderGouraudShader(renderGouraudShader)
{}

void PlantsShader::updateAllPlants(glm::mat4 &projectionView,
                                glm::vec3 &viewPosition,
                                bool usePhongShading,
                                bool shadowOnTrees,
                                bool useShadows,
                                bool useFlatBlending)
{
  Shader& shader = usePhongShading ? renderPhongShader : renderGouraudShader;
  shader.use();
  shader.setMat4("u_projectionView", projectionView);
  shader.setVec3("u_viewPosition", viewPosition);
  shader.setBool("u_shadow", shadowOnTrees);
  shader.setBool("u_shadowEnable", useShadows);
  shader.setBool("u_useFlatBlending", useFlatBlending);
}

void PlantsShader::updateGrass(bool usePhongShading)
{
  Shader& shader = usePhongShading ? renderPhongShader : renderGouraudShader;
  shader.use();
  shader.setBool("u_shadow", false);
  shader.setFloat("u_grassPosDistribution", glfwGetTime());
  shader.setFloat("u_grassPosDistrubutionInfluence", glfwGetTime() * 4.2f);
}

void PlantsShader::switchToGrass(bool usePhongShading, bool isGrass)
{
  Shader& shader = usePhongShading ? renderPhongShader : renderGouraudShader;
  shader.use();
  shader.setBool("u_isGrass", isGrass);
}
