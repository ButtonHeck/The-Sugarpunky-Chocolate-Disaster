#include "graphics/Renderer.h"

void Renderer::setInitialGLState(bool useMultisample)
{
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_DITHER);
  if (useMultisample)
    glEnable(GL_MULTISAMPLE);
  else
    glDisable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.85f, 0.44f, 0.35f, 1.0f);
}

void Renderer::setAmbientRenderingState(bool isOn)
{
  if (isOn)
    {
      glDisable(GL_CULL_FACE);
      glDepthFunc(GL_LEQUAL);
      glEnable(GL_BLEND);
      glDepthMask(GL_FALSE);
    }
  else
    {
      glDepthMask(GL_TRUE);
      glDisable(GL_BLEND);
      glDepthFunc(GL_LESS);
      glEnable(GL_CULL_FACE);
    }
}

void Renderer::renderSun(TheSun *theSun)
{
  glPointSize(64.0f);
  setAmbientRenderingState(true);
  glBindVertexArray(theSun->getVAO());
  glDrawArrays(GL_POINTS, 0, 1);
  setAmbientRenderingState(false);
  glPointSize(1.0f);
}

void Renderer::renderPlants(const std::shared_ptr<PlantGeneratorFacade> generatorFacade, Shader &shader,
                         bool bindTexture,
                         bool screenDraw,
                         bool useFlatBlending)
{
  shader.use();
  if (useFlatBlending)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);

  auto& landPlants = generatorFacade->getLandModels();
  for (unsigned int i = 0; i < landPlants.size(); i++)
    {
      Model& model = landPlants[i];
      model.draw(bindTexture);
    }

  auto& hillTrees = generatorFacade->getHillModels();
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      Model& model = hillTrees[i];
      model.draw(bindTexture);
    }

  //draw grass without face culling
  auto& grass = generatorFacade->getGrassModels();
  glDisable(GL_CULL_FACE);
  if (screenDraw)
    {
      shader.setBool("u_shadow", false);
      shader.setBool("u_isGrass", true);
      shader.setFloat("u_grassPosDistribution", glfwGetTime());
      shader.setFloat("u_grassPosDistrubutionInfluence", glfwGetTime() * 4.2f);
    }
  for (unsigned int i = 0; i < grass.size(); i++)
    {
      Model& model = grass[i];
      model.draw(bindTexture);
    }
  glEnable(GL_CULL_FACE);

  if (screenDraw)
    shader.setBool("u_isGrass", false);
  if (useFlatBlending)
    glDisable(GL_BLEND);
}
