#include "src/graphics/Renderer.h"

Renderer::Renderer(Camera &camera)
  :
    camera(camera)
{

}

void Renderer::drawHills(HillsMapGenerator *generator)
{
    glBindVertexArray(generator->getVAO());
    glDrawElements(GL_TRIANGLES, 6 * generator->getTiles().size(), GL_UNSIGNED_INT, 0);
}

void Renderer::drawShore(BaseMapGenerator *generator, Frustum &frustum)
{
  auto shoreChunks = generator->getShoreChunks();
  for (unsigned int i = 0; i < shoreChunks.size(); i++)
    {
      GLuint vao = generator->getShoreVao(i);
      glm::vec2 chunkMidPoint = shoreChunks[i].getMidPoint();
      glm::vec2 chunkLL = glm::vec2(chunkMidPoint.x - CHUNK_SIZE / 2.0f, chunkMidPoint.y + CHUNK_SIZE / 2.0f);
      glm::vec2 chunkLR = glm::vec2(chunkMidPoint.x + CHUNK_SIZE / 2.0f, chunkMidPoint.y + CHUNK_SIZE / 2.0f);
      glm::vec2 chunkUR = glm::vec2(chunkMidPoint.x + CHUNK_SIZE / 2.0f, chunkMidPoint.y - CHUNK_SIZE / 2.0f);
      glm::vec2 chunkUL = glm::vec2(chunkMidPoint.x - CHUNK_SIZE / 2.0f, chunkMidPoint.y - CHUNK_SIZE / 2.0f);
      float radius = CHUNK_SIZE / 2.0f * glm::sqrt(2);
      if (frustum.isInside(chunkLL.x, 0.0f, chunkLL.y, radius))
        {
          glBindVertexArray(vao);
          glDrawArrays(GL_TRIANGLES, 0, 6 * shoreChunks[i].getNumInstances());
          continue;
        }
      if (frustum.isInside(chunkLR.x, 0.0f, chunkLR.y, radius))
        {
          glBindVertexArray(vao);
          glDrawArrays(GL_TRIANGLES, 0, 6 * shoreChunks[i].getNumInstances());
          continue;
        }
      if (frustum.isInside(chunkUR.x, 0.0f, chunkUR.y, radius))
        {
          glBindVertexArray(vao);
          glDrawArrays(GL_TRIANGLES, 0, 6 * shoreChunks[i].getNumInstances());
          continue;
        }
      if (frustum.isInside(chunkUL.x, 0.0f, chunkUL.y, radius))
        {
          glBindVertexArray(vao);
          glDrawArrays(GL_TRIANGLES, 0, 6 * shoreChunks[i].getNumInstances());
        }
    }
}

void Renderer::drawFlatTerrain(BaseMapGenerator *generator, Frustum& frustum)
{
  glBindVertexArray(generator->getChunkVAO());
  auto baseChunks = generator->getFlatChunks();
  float radius = CHUNK_SIZE / 2.0f * glm::sqrt(2);
  for (unsigned int i = 0; i < baseChunks.size(); i++)
    {
      glm::vec2 chunkMidPoint = baseChunks[i].getMidPoint();
      glm::vec2 chunkLL = glm::vec2(chunkMidPoint.x - CHUNK_SIZE / 2.0f, chunkMidPoint.y + CHUNK_SIZE / 2.0f);
      glm::vec2 chunkLR = glm::vec2(chunkMidPoint.x + CHUNK_SIZE / 2.0f, chunkMidPoint.y + CHUNK_SIZE / 2.0f);
      glm::vec2 chunkUR = glm::vec2(chunkMidPoint.x + CHUNK_SIZE / 2.0f, chunkMidPoint.y - CHUNK_SIZE / 2.0f);
      glm::vec2 chunkUL = glm::vec2(chunkMidPoint.x - CHUNK_SIZE / 2.0f, chunkMidPoint.y - CHUNK_SIZE / 2.0f);
      if (frustum.isInside(chunkLL.x, 0.0f, chunkLL.y, radius))
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInside(chunkLR.x, 0.0f, chunkLR.y, radius))
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInside(chunkUR.x, 0.0f, chunkUR.y, radius))
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInside(chunkUL.x, 0.0f, chunkUL.y, radius))
        glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
    }

  glBindVertexArray(generator->getCellVAO());
  auto cellChunks = generator->getCellChunks();
  for (unsigned int i = 0; i < cellChunks.size(); i++)
    {
      glm::vec2 chunkMidPoint = cellChunks[i].getMidPoint();
      glm::vec2 chunkLL = glm::vec2(chunkMidPoint.x - CHUNK_SIZE / 2.0f, chunkMidPoint.y + CHUNK_SIZE / 2.0f);
      glm::vec2 chunkLR = glm::vec2(chunkMidPoint.x + CHUNK_SIZE / 2.0f, chunkMidPoint.y + CHUNK_SIZE / 2.0f);
      glm::vec2 chunkUR = glm::vec2(chunkMidPoint.x + CHUNK_SIZE / 2.0f, chunkMidPoint.y - CHUNK_SIZE / 2.0f);
      glm::vec2 chunkUL = glm::vec2(chunkMidPoint.x - CHUNK_SIZE / 2.0f, chunkMidPoint.y - CHUNK_SIZE / 2.0f);
      if (frustum.isInside(chunkLL.x, 0.0f, chunkLL.y, radius))
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInside(chunkLR.x, 0.0f, chunkLR.y, radius))
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInside(chunkUR.x, 0.0f, chunkUR.y, radius))
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInside(chunkUL.x, 0.0f, chunkUL.y, radius))
        glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
    }
}

void Renderer::drawUnderwaterQuad(UnderwaterQuadMapGenerator *generator)
{
  glBindVertexArray(generator->getVAO());
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::drawBuildableTiles(BuildableMapGenerator *generator)
{
  glBindVertexArray(generator->getVAO());
  glEnable(GL_BLEND);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, generator->getNumInstances());
  glDisable(GL_BLEND);
}

void Renderer::drawSelectedTile(BuildableMapGenerator *generator)
{
  glBindVertexArray(generator->getSelectedTileVAO());
  glEnable(GL_BLEND);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glDisable(GL_BLEND);
}

void Renderer::drawWater(WaterMapGenerator *generator, bool animateWater)
{
  glBindVertexArray(generator->getVAO());
  if (animateWater)
    generator->updateAnimationFrame();
  glEnable(GL_BLEND);
  glDrawArrays(GL_TRIANGLES, 0, 6 * generator->getTiles().size());
  glDisable(GL_BLEND);
}

void Renderer::drawSkybox(Skybox *skybox)
{
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);
  glBindVertexArray(skybox->getVAO());
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
}

void Renderer::drawTrees(TreeGenerator *generator, Shader &shader, bool enableFrustumCulling)
{
  auto plainTrees = generator->getPlainTrees();
  auto hillTrees = generator->getHillTrees();
  auto treeModelChunks = generator->getTreeModelChunks();
  auto hillTreeModelChunks = generator->getHillTreeModelChunks();
  for (unsigned int i = 0; i < plainTrees.size(); i++)
    {
      Model& model = plainTrees[i];
      model.draw(shader, camera, treeModelChunks, i, enableFrustumCulling, CHUNK_LOADING_DISTANCE);
    }
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      Model& model = hillTrees[i];
      model.draw(shader, camera, hillTreeModelChunks, i, enableFrustumCulling, CHUNK_LOADING_DISTANCE);
    }
}
