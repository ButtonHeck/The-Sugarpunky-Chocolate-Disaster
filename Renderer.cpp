#include "Renderer.h"

Renderer::Renderer(Camera &camera)
  :
    camera(camera)
{

}

void Renderer::updateDrawVariables()
{
  float cameraOnMapX = glm::clamp(camera.getPosition().x, -(float)HALF_TILES_WIDTH, (float)HALF_TILES_WIDTH);
  float cameraOnMapZ = glm::clamp(camera.getPosition().z, -(float)HALF_TILES_HEIGHT, (float)HALF_TILES_HEIGHT);
  cameraOnMapCoordX = glm::clamp((int)(TILES_WIDTH + cameraOnMapX) - HALF_TILES_WIDTH, 0, TILES_WIDTH - 1);
  cameraOnMapCoordZ = glm::clamp((int)(TILES_HEIGHT + cameraOnMapZ) - HALF_TILES_HEIGHT, 0, TILES_HEIGHT - 1);
  cameraPosition = glm::vec2(cameraOnMapX, cameraOnMapZ);
  viewDirection = glm::normalize(glm::vec2(camera.getDirection().x, camera.getDirection().z));
  cameraCorrectedFOVDOT = FOV_DOT_PRODUCT - camera.getPosition().y / 20.0f;
}

void Renderer::drawHills(HillsMapGenerator *generator, bool enableFrustumCulling)
{
  auto chunks = generator->getChunks();
  if (!enableFrustumCulling)
    {
      for (unsigned int i = 0; i < chunks.size(); i++)
        {
          glBindVertexArray(generator->getChunkVao(i));
          glDrawElements(GL_TRIANGLES, 6 * chunks[i].getNumInstances(), GL_UNSIGNED_INT, 0);
        }
      return;
    }
  for (unsigned int i = 0; i < chunks.size(); i++)
    {
      GLuint vao = generator->getChunkVao(i);
      if (chunks[i].containsPoint(cameraOnMapCoordX, cameraOnMapCoordZ))
        {
          glBindVertexArray(vao);
          glDrawElements(GL_TRIANGLES, 6 * chunks[i].getNumInstances(), GL_UNSIGNED_INT, 0);
          continue;
        }
      glm::vec2 directionToChunkUL =  glm::normalize(glm::vec2(chunks[i].getLeft() - (float)HALF_TILES_WIDTH, chunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUL, viewDirection) > cameraCorrectedFOVDOT)
        {
          glBindVertexArray(vao);
          glDrawElements(GL_TRIANGLES, 6 * chunks[i].getNumInstances(), GL_UNSIGNED_INT, 0);
          continue;
        }
      glm::vec2 directionToChunkUR =  glm::normalize(glm::vec2(chunks[i].getRight() - (float)HALF_TILES_WIDTH, chunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glBindVertexArray(vao);
          glDrawElements(GL_TRIANGLES, 6 * chunks[i].getNumInstances(), GL_UNSIGNED_INT, 0);
          continue;
        }
      glm::vec2 directionToChunkLR =  glm::normalize(glm::vec2(chunks[i].getRight() - (float)HALF_TILES_WIDTH, chunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glBindVertexArray(vao);
          glDrawElements(GL_TRIANGLES, 6 * chunks[i].getNumInstances(), GL_UNSIGNED_INT, 0);
          continue;
        }
      glm::vec2 directionToChunkLL =  glm::normalize(glm::vec2(chunks[i].getLeft() - (float)HALF_TILES_WIDTH, chunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLL, viewDirection) > cameraCorrectedFOVDOT)
        {
          glBindVertexArray(vao);
          glDrawElements(GL_TRIANGLES, 6 * chunks[i].getNumInstances(), GL_UNSIGNED_INT, 0);
        }
    }
}

void Renderer::drawShore(BaseMapGenerator *generator)
{
  auto shoreChunks = generator->getShoreChunks();
  for (unsigned int i = 0; i < shoreChunks.size(); i++)
    {
      GLuint vao = generator->getShoreVao(i);
      if (shoreChunks[i].containsPoint(cameraOnMapCoordX, cameraOnMapCoordZ))
        {
          glBindVertexArray(vao);
          glDrawArrays(GL_TRIANGLES, 0, 6 * shoreChunks[i].getNumInstances());
          continue;
        }
      glm::vec2 directionToChunkUL =  glm::normalize(glm::vec2(shoreChunks[i].getLeft() - (float)HALF_TILES_WIDTH, shoreChunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUL, viewDirection) > cameraCorrectedFOVDOT)
        {
          glBindVertexArray(vao);
          glDrawArrays(GL_TRIANGLES, 0, 6 * shoreChunks[i].getNumInstances());
          continue;
        }
      glm::vec2 directionToChunkUR =  glm::normalize(glm::vec2(shoreChunks[i].getRight() - (float)HALF_TILES_WIDTH, shoreChunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glBindVertexArray(vao);
          glDrawArrays(GL_TRIANGLES, 0, 6 * shoreChunks[i].getNumInstances());
          continue;
        }
      glm::vec2 directionToChunkLR =  glm::normalize(glm::vec2(shoreChunks[i].getRight() - (float)HALF_TILES_WIDTH, shoreChunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glBindVertexArray(vao);
          glDrawArrays(GL_TRIANGLES, 0, 6 * shoreChunks[i].getNumInstances());
          continue;
        }
      glm::vec2 directionToChunkLL =  glm::normalize(glm::vec2(shoreChunks[i].getLeft() - (float)HALF_TILES_WIDTH, shoreChunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLL, viewDirection) > cameraCorrectedFOVDOT)
        {
          glBindVertexArray(vao);
          glDrawArrays(GL_TRIANGLES, 0, 6 * shoreChunks[i].getNumInstances());
        }
    }
}

void Renderer::drawFlatTerrain(BaseMapGenerator *generator)
{
  glBindVertexArray(generator->getChunkVAO());
  auto baseChunks = generator->getFlatChunks();
  for (unsigned int i = 0; i < baseChunks.size(); i++)
    {
      if (baseChunks[i].containsPoint(cameraOnMapCoordX, cameraOnMapCoordZ))
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkUL =  glm::normalize(glm::vec2(baseChunks[i].getLeft() - (float)HALF_TILES_WIDTH, baseChunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUL, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkUR =  glm::normalize(glm::vec2(baseChunks[i].getRight() - (float)HALF_TILES_WIDTH, baseChunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkLR =  glm::normalize(glm::vec2(baseChunks[i].getRight() - (float)HALF_TILES_WIDTH, baseChunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkLL =  glm::normalize(glm::vec2(baseChunks[i].getLeft() - (float)HALF_TILES_WIDTH, baseChunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLL, viewDirection) > cameraCorrectedFOVDOT)
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1, baseChunks[i].getInstanceOffset());
    }

  glBindVertexArray(generator->getCellVAO());
  auto cellChunks = generator->getCellChunks();
  for (unsigned int i = 0; i < cellChunks.size(); i++)
    {
      if (cellChunks[i].containsPoint(cameraOnMapCoordX, cameraOnMapCoordZ))
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkUL =  glm::normalize(glm::vec2(cellChunks[i].getLeft() - (float)HALF_TILES_WIDTH, cellChunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUL, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkUR =  glm::normalize(glm::vec2(cellChunks[i].getRight() - (float)HALF_TILES_WIDTH, cellChunks[i].getTop() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkUR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkLR =  glm::normalize(glm::vec2(cellChunks[i].getRight() - (float)HALF_TILES_WIDTH, cellChunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLR, viewDirection) > cameraCorrectedFOVDOT)
        {
          glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      glm::vec2 directionToChunkLL =  glm::normalize(glm::vec2(cellChunks[i].getLeft() - (float)HALF_TILES_WIDTH, cellChunks[i].getBottom() - (float)HALF_TILES_HEIGHT) - cameraPosition);
      if (glm::dot(directionToChunkLL, viewDirection) > cameraCorrectedFOVDOT)
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
