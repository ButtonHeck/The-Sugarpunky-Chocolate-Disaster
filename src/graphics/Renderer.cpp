#include "graphics/Renderer.h"

Renderer::Renderer(Camera &camera)
  :
    camera(camera)
{}

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

void Renderer::renderWater(bool useFC, std::shared_ptr<WaterGenerator> generator, Shader& fc, Shader& nofc)
{
  if (useFC)
    {
      {
        fc.use();
        glBindVertexArray(generator->getVAO());
        {
          BENCHMARK("Renderer: draw water to TFB", true);
          glEnable(GL_RASTERIZER_DISCARD);
          glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, generator->getTransformFeedback());
          glBeginTransformFeedback(GL_TRIANGLES);
          glDrawArrays(GL_TRIANGLES, 0, generator->getTiles().size() * VERTICES_PER_TILE);
          glEndTransformFeedback();
          glDisable(GL_RASTERIZER_DISCARD);
        }
      }
      {
        BENCHMARK("Renderer: draw water from TFB", true);
        nofc.use();
        glBindVertexArray(generator->getCulledVAO());
        glEnable(GL_BLEND);
        glDrawTransformFeedback(GL_TRIANGLES, generator->getTransformFeedback());
        glDisable(GL_BLEND);
      }
    }
  else
    {
      BENCHMARK("Renderer: draw water no FC", true);
      nofc.use();
      glBindVertexArray(generator->getVAO());
      glEnable(GL_BLEND);
      glDrawArrays(GL_TRIANGLES, 0, generator->getTiles().size() * VERTICES_PER_TILE);
      glDisable(GL_BLEND);
    }
}

void Renderer::renderSkybox(Skybox *skybox)
{
  setAmbientRenderingState(true);
  glBindVertexArray(skybox->getVAO());
  glDrawArrays(GL_TRIANGLES, 0, VERTICES_PER_TILE * VERTICES_PER_TILE);
  setAmbientRenderingState(false);
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
