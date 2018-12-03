#include "PlantsShader.h"

PlantsShader::PlantsShader(Shader &renderPhongShader, Shader &renderGouraudShader)
  :
    renderPhongShader(renderPhongShader),
    renderGouraudShader(renderGouraudShader)
{}

void PlantsShader::updateAllPlants(glm::vec3 &lightDir,
                                   glm::mat4 &lightSpaceMatrixNear,
                                   glm::mat4 &lightSpaceMatrixMiddle,
                                   glm::mat4 &lightSpaceMatrixFar,
                                   glm::mat4 &projectionView,
                                   glm::vec3 &viewPosition,
                                   bool usePhongShading,
                                   bool useShadows,
                                   bool useLandBlending)
{
  Shader& shader = usePhongShading ? renderPhongShader : renderGouraudShader;
  shader.use();
  shader.setMat4("u_projectionView", projectionView);
  shader.setVec3("u_viewPosition", viewPosition);
  shader.setBool("u_shadowEnable", useShadows);
  shader.setBool("u_useLandBlending", useLandBlending);
  shader.setVec3("u_lightDir", -lightDir);
  shader.setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrixNear);
  shader.setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrixMiddle);
  shader.setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrixFar);
}

void PlantsShader::updateGrass(bool usePhongShading)
{
  Shader& shader = usePhongShading ? renderPhongShader : renderGouraudShader;
  shader.use();
  shader.setFloat("u_grassPosDistrubution", glfwGetTime() * 4.3f);
}

void PlantsShader::switchToGrass(bool usePhongShading, bool isGrass)
{
  Shader& shader = usePhongShading ? renderPhongShader : renderGouraudShader;
  shader.use();
  shader.setBool("u_isGrass", isGrass);
}
