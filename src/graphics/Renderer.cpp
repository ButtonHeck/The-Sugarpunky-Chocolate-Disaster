#include "graphics/Renderer.h"

Renderer::Renderer(Camera &camera)
  :
    camera(camera)
{

}

void Renderer::drawHills(bool useFC, const std::shared_ptr<HillsMapGenerator> generator, Shader& fc, Shader& nofc)
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

void Renderer::drawHillsDepthmap(const std::shared_ptr<HillsMapGenerator> generator)
{
  glBindVertexArray(generator->getVAO());
  glDrawElements(GL_TRIANGLES, 6 * generator->getTiles().size(), GL_UNSIGNED_INT, 0);
}

void Renderer::drawShore(const std::shared_ptr<BaseMapGenerator> generator)
{
  glBindVertexArray(generator->getShoreVao());
  glEnable(GL_BLEND);
  glDrawArrays(GL_TRIANGLES, 0, generator->getShoreVerticesToDraw());
  glDisable(GL_BLEND);
}

void Renderer::drawFlatTerrain(const std::shared_ptr<BaseMapGenerator> generator, Frustum& frustum, GLuint texture)
{
  glBindTextureUnit(TEX_FLAT, texture);
  //square chunks are better to render without FC
  glBindVertexArray(generator->getSquareVAO());
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, generator->getNumSquareInstances());

  //these ones should probably be FC-ed with multiDrawIndirect
  glBindVertexArray(generator->getCellVAO());
  auto& cellChunks = generator->getCellChunks();
  GLuint multiDrawIndirectData[cellChunks.size() * 5]; // { indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
  GLuint dataOffset = 0;
  GLuint drawIndirectCommandPrimCount = 0;
  for (unsigned int i = 0; i < cellChunks.size(); i++)
    {
      glm::vec2 chunkMidPoint = cellChunks[i].getMidPoint();
      if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE,
                           chunkMidPoint.y + HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(drawIndirectCommandPrimCount,
                                multiDrawIndirectData,
                                dataOffset, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE,
                           chunkMidPoint.y + HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(drawIndirectCommandPrimCount,
                                multiDrawIndirectData,
                                dataOffset, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x + HALF_CHUNK_SIZE,
                           chunkMidPoint.y - HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(drawIndirectCommandPrimCount,
                                multiDrawIndirectData,
                                dataOffset, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
          continue;
        }
      if (frustum.isInsideXZ(chunkMidPoint.x - HALF_CHUNK_SIZE,
                           chunkMidPoint.y - HALF_CHUNK_SIZE,
                           MODELS_FC_RADIUS))
        {
          addIndirectBufferData(drawIndirectCommandPrimCount,
                                multiDrawIndirectData,
                                dataOffset, cellChunks[i].getNumInstances(), cellChunks[i].getInstanceOffset());
        }
    }
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, generator->getCellDIBO());
  glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(GLuint) * 5 * drawIndirectCommandPrimCount, multiDrawIndirectData, GL_STATIC_DRAW);
  glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_BYTE, 0, drawIndirectCommandPrimCount, 0);
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

void Renderer::drawUnderwaterQuad(const UnderwaterQuadMapGenerator &generator)
{
  glBindVertexArray(generator.getVAO());
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}

void Renderer::drawBuildableTiles(const std::shared_ptr<BuildableMapGenerator> generator)
{
  glBindVertexArray(generator->getVAO());
  glEnable(GL_BLEND);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, generator->getNumInstances());
  glDisable(GL_BLEND);
}

void Renderer::drawSelectedTile(const std::shared_ptr<BuildableMapGenerator> generator)
{
  glBindVertexArray(generator->getSelectedTileVAO());
  glEnable(GL_BLEND);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
  glDisable(GL_BLEND);
}

void Renderer::drawWater(bool useFC, std::shared_ptr<WaterMapGenerator> generator, Shader& fc, Shader& nofc)
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

void Renderer::drawPlants(const std::shared_ptr<PlantGeneratorFacade> generatorFacade, Shader &shader,
                         bool enableFrustumCulling,
                         bool bindTexture,
                         bool updateIndirect,
                         bool screenDraw,
                         bool useFlatBlending)
{
  shader.use();
  if (useFlatBlending)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);

  auto& plainPlants = generatorFacade->getPlainModels();
  for (unsigned int i = 0; i < plainPlants.size(); i++)
    {
      Model& model = plainPlants[i];
      model.draw(enableFrustumCulling, bindTexture, updateIndirect);
    }

  auto& hillTrees = generatorFacade->getHillModels();
  for (unsigned int i = 0; i < hillTrees.size(); i++)
    {
      Model& model = hillTrees[i];
      model.draw(enableFrustumCulling, bindTexture, updateIndirect);
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
      model.draw(enableFrustumCulling, bindTexture, updateIndirect);
    }
  glEnable(GL_CULL_FACE);
  if (screenDraw)
    {
      shader.setBool("u_isGrass", false);
    }

  if (useFlatBlending)
    glDisable(GL_BLEND);
}
