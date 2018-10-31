#include "PlantsRenderer.h"

void PlantsRenderer::render(std::vector<Model> &landModels,
                            std::vector<Model> &hillModels,
                            std::vector<Model> &grassModels,
                            Shader &shader,
                            bool bindTexture,
                            bool screenDraw,
                            bool useFlatBlending)
{
  shader.use();
  if (useFlatBlending)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);

  for (unsigned int i = 0; i < landModels.size(); i++)
    {
      Model& model = landModels[i];
      model.draw(bindTexture);
    }

  for (unsigned int i = 0; i < hillModels.size(); i++)
    {
      Model& model = hillModels[i];
      model.draw(bindTexture);
    }

  //draw grass without face culling
  glDisable(GL_CULL_FACE);
  if (screenDraw)
    {
      shader.setBool("u_shadow", false);
      shader.setBool("u_isGrass", true);
      shader.setFloat("u_grassPosDistribution", glfwGetTime());
      shader.setFloat("u_grassPosDistrubutionInfluence", glfwGetTime() * 4.2f);
    }
  for (unsigned int i = 0; i < grassModels.size(); i++)
    {
      Model& model = grassModels[i];
      model.draw(bindTexture);
    }
  glEnable(GL_CULL_FACE);

  if (screenDraw)
    shader.setBool("u_isGrass", false);
  if (useFlatBlending)
    glDisable(GL_BLEND);
}
