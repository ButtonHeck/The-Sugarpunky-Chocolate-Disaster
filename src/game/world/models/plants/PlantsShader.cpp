#include "game/world/models/plants/PlantsShader.h"

PlantsShader::PlantsShader(Shader &renderPhongShader, Shader &renderGouraudShader)
  :
    renderPhongShader(renderPhongShader),
    renderGouraudShader(renderGouraudShader)
{}

void PlantsShader::activateShader(bool usePhongShading)
{
  currentShader = usePhongShading ? &renderPhongShader : &renderGouraudShader;
  currentShader->use();
}

void PlantsShader::updateAllPlants(const glm::vec3 &lightDir,
                                   const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                                   const glm::mat4 &projectionView,
                                   const glm::vec3 &viewPosition,
                                   bool useShadows,
                                   bool useLandBlending)
{
  currentShader->setMat4("u_projectionView", projectionView);
  currentShader->setVec3("u_viewPosition", viewPosition);
  currentShader->setBool("u_shadowEnable", useShadows);
  currentShader->setBool("u_useLandBlending", useLandBlending);
  currentShader->setVec3("u_lightDir", -lightDir);
  currentShader->setMat4("u_lightSpaceMatrix[0]", lightSpaceMatrices[0]);
  currentShader->setMat4("u_lightSpaceMatrix[1]", lightSpaceMatrices[1]);
  currentShader->setMat4("u_lightSpaceMatrix[2]", lightSpaceMatrices[2]);
}

void PlantsShader::updateGrass()
{
  currentShader->setFloat("u_grassPosDistrubution", glfwGetTime() * 4.3f);
}

void PlantsShader::switchToGrass(bool isGrass)
{
  currentShader->setBool("u_isGrass", isGrass);
}

void PlantsShader::switchToLowPoly(bool isLowPoly)
{
  currentShader->setBool("u_isLowPoly", isLowPoly);
}
