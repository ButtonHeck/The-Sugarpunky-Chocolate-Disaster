#include "graphics/Renderer.h"

Renderer::Renderer(Camera &camera)
  :
    camera(camera)
{

}

void Renderer::drawHills(bool useFC, HillsMapGenerator *generator, Shader& fc, Shader& nofc)
{
  if (useFC)
    {
      GLuint tfb = generator->getTransformFeedback();
      {
        BENCHMARK("Renderer: draw hills to TFB", true);
        fc.use();
        glBindVertexArray(generator->getVAO());
        glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfb);
        glBeginTransformFeedback(GL_TRIANGLES);
        glDrawElements(GL_TRIANGLES, 6 * generator->getTiles().size(), GL_UNSIGNED_INT, 0);
        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);
      }
      {
        BENCHMARK("Renderer: draw hills from TFB", true);
        nofc.use();
        glBindVertexArray(generator->getCulledVAO());
        glDrawTransformFeedback(GL_TRIANGLES, tfb);
      }
    }
  else
    {
      nofc.use();
      glBindVertexArray(generator->getVAO());
      glDrawElements(GL_TRIANGLES, 6 * generator->getTiles().size(), GL_UNSIGNED_INT, 0);
    }
}

void Renderer::drawHillsDepthmap(HillsMapGenerator *generator)
{
  glBindVertexArray(generator->getVAO());
  glDrawElements(GL_TRIANGLES, 6 * generator->getTiles().size(), GL_UNSIGNED_INT, 0);
}

void Renderer::drawShore(BaseMapGenerator *generator)
{
  glBindVertexArray(generator->getShoreVao());
  glDrawArrays(GL_TRIANGLES, 0, generator->getShoreVerticesToDraw());
}

void Renderer::drawFlatTerrain(BaseMapGenerator *generator, Frustum& frustum)
{
  //square chunks are better to render without FC
  glBindVertexArray(generator->getSquareVAO());
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, generator->getNumSquareInstances());

  //these ones should probably be FC-ed with multiDrawIndirect
  glBindVertexArray(generator->getCellVAO());
  auto& cellChunks = generator->getCellChunks();
  GLuint multiDrawIndirectData[cellChunks.size() * 5]; // { indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
  GLuint dataOffset = 0;
  GLuint multiDE_I_primCount = 0;
  for (unsigned int i = 0; i < cellChunks.size(); i++)
    {
      glm::vec2 chunkMidPoint = cellChunks[i].getMidPoint();
      if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE,
                           chunkMidPoint.y + HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(multiDE_I_primCount,
                                multiDrawIndirectData,
                                dataOffset, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE,
                           chunkMidPoint.y + HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(multiDE_I_primCount,
                                multiDrawIndirectData,
                                dataOffset, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE,
                           chunkMidPoint.y - HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(multiDE_I_primCount,
                                multiDrawIndirectData,
                                dataOffset, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE,
                           chunkMidPoint.y - HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(multiDE_I_primCount,
                                multiDrawIndirectData,
                                dataOffset, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
        }
    }
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, generator->getCellDIBO());
  glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(GLuint) * 5 * multiDE_I_primCount, multiDrawIndirectData, GL_STATIC_DRAW);
  glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_BYTE, 0, multiDE_I_primCount, 0);
}

void Renderer::addIndirectBufferData(GLuint& primCount,
                                     GLuint* buffer,
                                     GLuint& dataOffset,
                                     GLuint numInstances,
                                     GLuint instanceOffset)
{
  ++primCount;
  buffer[dataOffset++] = 6;
  buffer[dataOffset++] = numInstances;
  buffer[dataOffset++] = 0;
  buffer[dataOffset++] = 0;
  buffer[dataOffset++] = instanceOffset;
}

void Renderer::drawUnderwaterQuad(UnderwaterQuadMapGenerator *generator)
{
  glBindVertexArray(generator->getVAO());
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}

void Renderer::drawBuildableTiles(BuildableMapGenerator *generator)
{
  glBindVertexArray(generator->getVAO());
  glEnable(GL_BLEND);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, generator->getNumInstances());
  glDisable(GL_BLEND);
}

void Renderer::drawSelectedTile(BuildableMapGenerator *generator)
{
  glBindVertexArray(generator->getSelectedTileVAO());
  glEnable(GL_BLEND);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
  glDisable(GL_BLEND);
}

void Renderer::drawWater(bool useFC, WaterMapGenerator *generator, Shader& fc, Shader& nofc)
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
          glDrawArrays(GL_TRIANGLES, 0, 6 * generator->getTiles().size());
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
      glDrawArrays(GL_TRIANGLES, 0, 6 * generator->getTiles().size());
      glDisable(GL_BLEND);
    }
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

void Renderer::drawTrees(TreeGenerator *generator, Shader &shader, bool enableFrustumCulling, bool bindTexture, bool updateIndirect, bool screenDraw)
{
  glDisable(GL_BLEND);
  auto& plainTrees = generator->getPlainTrees();
  auto& hillTrees = generator->getHillTrees();

  for (unsigned int i = 0; i < plainTrees.size() - NUM_GRASS_MODELS; i++)
    {
      Model& model = plainTrees[i];
      model.draw(shader, enableFrustumCulling, bindTexture, updateIndirect);
    }

  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      Model& model = hillTrees[i];
      model.draw(shader, enableFrustumCulling, bindTexture, updateIndirect);
    }

  //draw grass without face culling
  glDisable(GL_CULL_FACE);
  if (screenDraw)
    shader.setBool("u_shadow", false);
  for (unsigned int i = plainTrees.size() - NUM_GRASS_MODELS; i < plainTrees.size(); i++)
    {
      Model& model = plainTrees[i];
      model.draw(shader, enableFrustumCulling, bindTexture, updateIndirect);
    }
  glEnable(GL_CULL_FACE);
}
