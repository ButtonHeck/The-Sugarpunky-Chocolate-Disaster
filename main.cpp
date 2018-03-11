#include <iostream>
#include <vector>
#include <ctime>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include "Settings.h"
#include "Timer.h"
#include "Shader.h"
#include "Camera.h"
#include "InputController.h"
#include "TextureLoader.h"
#include "TerrainTile.h"
#include "WaterMapGenerator.h"
#include "HillsMapGenerator.h"
#include "UnderwaterQuadMapGenerator.h"

//base terrain related declarations
void generateBaseTerrainMap(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& waterMap);
void smoothBaseTerrainMap(std::vector<std::vector<float>>& baseMap);
void correctBaseTerrainMapAtEdges(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& waterMap);
void compressHeightBaseTerrainMap(std::vector<std::vector<float>>& baseMap, float ratio, bool entireRange);
void denyBaseTerrainMapInvisibleTiles(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
void splitBaseTerrainToChunks(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& chunkMap, std::vector<TerrainTile>& baseChunks, int chunkSize, bool overlap);
void removeBaseTerrainUnderwaterTiles(std::vector<std::vector<float>>& baseMap, float thresholdValue);
//general functions declarations
GLFWwindow* initGLFW();
void resetAllGLBuffers();
void setupGLBuffersAttributes();
void createTiles(std::vector<std::vector<float>>& map, std::vector<TerrainTile>& tiles, bool flat, bool createOnZeroTiles);
template <typename T> void initializeMap(std::vector<std::vector<T>>& map)
{
  map.clear();
  map.reserve(TILES_HEIGHT + 1);
  for (size_t row = 0; row < TILES_HEIGHT + 1; row++)
    {
      std::vector<T> rowN;
      rowN.reserve(TILES_WIDTH + 1);
      map.emplace_back(rowN);
    }
  for (auto& row : map)
    for (size_t elem = 0; elem < TILES_WIDTH + 1; elem++)
      row.emplace_back(0);
}

GLFWwindow* window;
Timer timer;
Camera cam(glm::vec3(0.0f, 3.0f, 0.0f));
InputController input;
std::default_random_engine RANDOMIZER_ENGINE;
TextureLoader textureLoader;
WaterMapGenerator waterMapGenerator;
HillsMapGenerator hillMapGenerator(waterMapGenerator.getMap());
UnderwaterQuadMapGenerator underwaterQuadGenerator;

int main()
{
  //GLOBAL SETUP
  window = initGLFW();
  glewExperimental = GL_TRUE;
  glewInit();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //SHADER STUFF
  Shader scene(PROJ_PATH + "/shaders/scene.vs", PROJ_PATH + "/shaders/scene.fs");
  scene.use();

  //TEXTURE LOADING
  glActiveTexture(GL_TEXTURE0);
  GLuint flatTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/grass.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE1);
  GLuint hillTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/grassHill.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE2);
  GLuint waterTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/water2.png", GL_REPEAT);
  glActiveTexture(GL_TEXTURE3);
  GLuint sandTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/sand.jpg", GL_REPEAT);
  GLuint underwaterSandTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/underwater_sand.jpg", GL_REPEAT);
  scene.setInt("grassTexture", 0);
  scene.setInt("hillTexture", 1);
  scene.setInt("waterTexture", 2);
  scene.setInt("sandTexture", 3);
  scene.setFloat("waterLevel", WATER_LEVEL);

  //prepare water map
  waterMapGenerator.prepareMap();

  //generating hill height map
  hillMapGenerator.prepareMap();
  //fill hills buffer
  hillMapGenerator.fillHillsBuffersData();

  //generating base terrain data
  std::vector<std::vector<float>> baseMap;
  std::vector<std::vector<float>> chunkMap;
  std::vector<TerrainTile> baseTiles;
  baseTiles.reserve(NUM_TILES);
  std::vector<TerrainTile> baseChunkTiles[5];
  for (int i = 0; i < 5; i++)
    baseChunkTiles[i].reserve(NUM_TILES / BASE_TERRAIN_CHUNK_SIZES[i] * BASE_TERRAIN_CHUNK_SIZES[i]);
  initializeMap(baseMap);
  initializeMap(chunkMap);
  generateBaseTerrainMap(baseMap, waterMapGenerator.getMap());
  smoothBaseTerrainMap(baseMap);
  compressHeightBaseTerrainMap(baseMap, 2.0f, true);
  correctBaseTerrainMapAtEdges(baseMap, waterMapGenerator.getMap());
  //split as more tiles into chunks as possible
  for (int i = 0; i < 5; i++)
    {
      splitBaseTerrainToChunks(baseMap, chunkMap, baseChunkTiles[i], BASE_TERRAIN_CHUNK_SIZES[i], (bool)i);
      baseChunkTiles[i].shrink_to_fit();
    }
  denyBaseTerrainMapInvisibleTiles(baseMap, hillMapGenerator.getMap());
  removeBaseTerrainUnderwaterTiles(baseMap, UNDERWATER_REMOVAL_LEVEL);
  baseTiles.shrink_to_fit();
  createTiles(baseMap, baseTiles, false, true);
  //fill base terrain vertex data
  GLuint baseVAO, baseVBO, baseEBO;
  glGenVertexArrays(1, &baseVAO);
  glGenBuffers(1, &baseVBO);
  glGenBuffers(1, &baseEBO);
  GLfloat baseVertices[baseTiles.size() * 20];
  GLuint baseIndices[baseTiles.size() * 6];
  for (unsigned int c = 0; c < baseTiles.size(); c++)
    {
      TerrainTile& tile = baseTiles[c];
      int offset = c * 20;
      int indexArrayOffset = c * 6;
      int index = c * 4;
      //ll
      baseVertices[offset] = -1- TILES_WIDTH / 2 + tile.mapX;
      baseVertices[offset+1] = tile.lowLeft;
      baseVertices[offset+2] = - TILES_HEIGHT / 2 + tile.mapY;
      baseVertices[offset+3] = 0.0f;
      baseVertices[offset+4] = 0.0f;
      //lr
      baseVertices[offset+5] = - TILES_WIDTH / 2 + tile.mapX;
      baseVertices[offset+6] = tile.lowRight;
      baseVertices[offset+7] = - TILES_HEIGHT / 2 + tile.mapY;
      baseVertices[offset+8] = 1.0f;
      baseVertices[offset+9] = 0.0f;
      //ur
      baseVertices[offset+10] = - TILES_WIDTH / 2 + tile.mapX;
      baseVertices[offset+11] = tile.upperRight;
      baseVertices[offset+12] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      baseVertices[offset+13] = 1.0f;
      baseVertices[offset+14] = 1.0f;
      //ul
      baseVertices[offset+15] = -1 - TILES_WIDTH / 2 + tile.mapX;
      baseVertices[offset+16] = tile.upperLeft;
      baseVertices[offset+17] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      baseVertices[offset+18] = 0.0f;
      baseVertices[offset+19] = 1.0f;

      baseIndices[indexArrayOffset] = index;
      baseIndices[indexArrayOffset+1] = index + 1;
      baseIndices[indexArrayOffset+2] = index + 2;
      baseIndices[indexArrayOffset+3] = index + 2;
      baseIndices[indexArrayOffset+4] = index + 3;
      baseIndices[indexArrayOffset+5] = index;
    }
  glBindVertexArray(baseVAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, baseEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(baseIndices), baseIndices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, baseVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(baseVertices), baseVertices, GL_STATIC_DRAW);
  setupGLBuffersAttributes();
  resetAllGLBuffers();

  //generating data for chunk instance rendering
  int NUM_INSTANCES[5];
  GLuint baseChunkInstanceVAOs[5], baseChunkInstanceVBOs[5], baseChunkInstanceEBOs[5], baseChunkInstanceModelVBOs[5];
  glGenVertexArrays(5, baseChunkInstanceVAOs);
  glGenBuffers(5, baseChunkInstanceVBOs);
  glGenBuffers(5, baseChunkInstanceEBOs);
  glGenBuffers(5, baseChunkInstanceModelVBOs);
  for (unsigned int vao = 0; vao < 5; vao++)
    {
      GLfloat baseChunkInstanceVertices[20] = {
          -1.0f, 0.0f,  1.0f, 0.0f,                                      0.0f,
           1.0f, 0.0f,  1.0f, (float)BASE_TERRAIN_CHUNK_SIZES[vao] - 1,  0.0f,
           1.0f, 0.0f, -1.0f, (float)BASE_TERRAIN_CHUNK_SIZES[vao] - 1,  (float)BASE_TERRAIN_CHUNK_SIZES[vao] - 1,
          -1.0f, 0.0f, -1.0f, 0.0f,                                      (float)BASE_TERRAIN_CHUNK_SIZES[vao] - 1
      };
      glBindVertexArray(baseChunkInstanceVAOs[vao]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, baseChunkInstanceEBOs[vao]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceVBOs[vao]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(baseChunkInstanceVertices), baseChunkInstanceVertices, GL_STATIC_DRAW);
      setupGLBuffersAttributes();
      NUM_INSTANCES[vao] = baseChunkTiles[vao].size();
      glm::mat4* baseInstanceChunkModels = new glm::mat4[NUM_INSTANCES[vao]];
      std::vector<TerrainTile>& baseTerrainChunk = baseChunkTiles[vao];
      for (unsigned int i = 0; i < baseTerrainChunk.size(); i++)
        {
          glm::mat4 model;
          TerrainTile& tile = baseTerrainChunk[i];
          model = glm::translate(model, glm::vec3(- TILES_WIDTH / 2 + tile.mapX + BASE_TERRAIN_CHUNK_SIZES[vao] / 2 - 0.5f, 0.0f, - TILES_HEIGHT / 2 + tile.mapY + BASE_TERRAIN_CHUNK_SIZES[vao] / 2 - 0.5f));
          model = glm::scale(model, glm::vec3(BASE_TERRAIN_CHUNK_SIZES[vao] / 2 - 0.5f, 0.0f, BASE_TERRAIN_CHUNK_SIZES[vao] / 2 - 0.5f));
          baseInstanceChunkModels[i] = model;
        }
      glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceModelVBOs[vao]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * baseTerrainChunk.size(), &baseInstanceChunkModels[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), 0);
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
      glEnableVertexAttribArray(4);
      glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
      glEnableVertexAttribArray(5);
      glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
      glVertexAttribDivisor(2, 1);
      glVertexAttribDivisor(3, 1);
      glVertexAttribDivisor(4, 1);
      glVertexAttribDivisor(5, 1);
      resetAllGLBuffers();
      delete[] baseInstanceChunkModels;
    }

  //fill water buffer
  waterMapGenerator.fillWaterBufferData();

  //generating underwater flat tile
  underwaterQuadGenerator.fillQuadBufferData();

  //print info
  std::cout << "Water tiles:      " << waterMapGenerator.getTiles().size() << std::endl;
  std::cout << "Hills tiles:      " << hillMapGenerator.getTiles().size() << std::endl;
  std::cout << "Base tiles:       " << baseTiles.size() << std::endl;
  for (unsigned int i = 0; i < 5; i++)
    std::cout << "x" << BASE_TERRAIN_CHUNK_SIZES[i] << "\ttiles:    " << baseChunkTiles[i].size() << std::endl;
  std::cout << "Summary: " << (waterMapGenerator.getTiles().size() + hillMapGenerator.getTiles().size() + baseTiles.size()) << std::endl;

  //pre-setup
  glm::mat4 model;
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(cam.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 500.0f);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //MAIN LOOP
  while(!glfwWindowShouldClose(window))
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      float delta = timer.tick();
      input.processKeyboard(delta);

      //scene update
      scene.use();
      scene.setMat4("projection", projection);
      scene.setMat4("view", cam.getViewMatrix());
      scene.setMat4("model", model);

      //height tiles
      hillMapGenerator.draw(scene);

      //base terrain tiles
      scene.setInt("surfaceTextureEnum", 0);
      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_2D, sandTexture);
      glBindVertexArray(baseVAO);
      glDrawElements(GL_TRIANGLES, 6 * baseTiles.size(), GL_UNSIGNED_INT, 0);

      //underwater tile
      underwaterQuadGenerator.draw(underwaterSandTexture);

      //base terrain chunk tiles
      scene.setBool("instanceRender", true);
      for (unsigned int vao = 0; vao < 5; vao++)
        {
          glBindVertexArray(baseChunkInstanceVAOs[vao]);
          glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, NUM_INSTANCES[vao]);
        }

      //water tiles
      waterMapGenerator.draw(scene);

      glfwPollEvents();
      glfwSwapBuffers(window);
    }

  //CLEANUP
  glDeleteTextures(1, &flatTexture);
  glDeleteTextures(1, &hillTexture);
  glDeleteTextures(1, &waterTexture);
  glDeleteTextures(1, &sandTexture);
  glDeleteTextures(1, &underwaterSandTexture);
  glDeleteVertexArrays(1, &baseVAO);
  glDeleteBuffers(1, &baseVBO);
  glDeleteBuffers(1, &baseEBO);
  glDeleteVertexArrays(5, baseChunkInstanceVAOs);
  glDeleteBuffers(5, baseChunkInstanceVBOs);
  glDeleteBuffers(5, baseChunkInstanceEBOs);
  glDeleteBuffers(5, baseChunkInstanceModelVBOs);
  hillMapGenerator.deleteGLObjects();
  waterMapGenerator.deleteGLObjects();
  underwaterQuadGenerator.deleteGLObjects();
  scene.cleanUp();
  glfwDestroyWindow(window);
  glfwTerminate();
}

GLFWwindow* initGLFW()
{
  glfwSetErrorCallback([](int, const char* msg){printf("Error while loading GLFW: %s", msg);});
  if (!glfwInit())
    {
      std::cerr << "Error while loading GLFW\n";
      glfwTerminate();
    }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 2);
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Terrain Test", glfwGetPrimaryMonitor(), 0);
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, input.cursorCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return window;
}

void createTiles(std::vector<std::vector<float>>& map, std::vector<TerrainTile>& tiles, bool flat, bool createOnZeroHeight)
{
  for (unsigned int y = 1; y < map.size(); y++)
    {
      for (unsigned int x = 1; x < map[0].size(); x++)
        {
          if (map[y][x] == DENY_TILE_RENDER_VALUE)
            continue;
          bool toCreate;
          if (!flat)
            toCreate = map[y][x] != 0 || map[y-1][x] != 0 || map[y][x-1] != 0 || map[y-1][x-1] != 0;
          else
            toCreate = map[y][x] != 0;
          if (toCreate || createOnZeroHeight)
            {
              float ll;
              float lr;
              float ur;
              float ul;
              if (!flat)
                {
                  ll = map[y][x-1];
                  if (ll == DENY_TILE_RENDER_VALUE)
                    ll = map[y][x];
                  lr = map[y][x];
                  ur = map[y-1][x];
                  if (ur == DENY_TILE_RENDER_VALUE)
                    ur = map[y][x];
                  ul = map[y-1][x-1];
                  if (ul == DENY_TILE_RENDER_VALUE)
                    ul = map[y][x];
                }
              else
                {
                  ll = map[y][x];
                  lr = map[y][x];
                  ur = map[y][x];
                  ul = map[y][x];
                }
              tiles.emplace_back(x, y, ll, lr, ur, ul);
            }
        }
    }
}

void generateBaseTerrainMap(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& waterMap)
{
  std::uniform_real_distribution<float> distribution(0.9f, 1.1f);
  for (unsigned int y = 0; y <= TILES_HEIGHT; y++)
    {
      for (unsigned int x = 0; x <= TILES_WIDTH; x++)
        {
          baseMap[y][x] = waterMap[y][x] * 1.1f * distribution(RANDOMIZER_ENGINE);
        }
    }
}

void smoothBaseTerrainMap(std::vector<std::vector<float>>& baseMap)
{
  //smooth tile below on map
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH; x++)
        {
          if (baseMap[y-1][x] < WATER_LEVEL - WATER_LEVEL / 4)
            baseMap[y][x] += WATER_LEVEL / 1.5f;
        }
    }
  //smooth tile upper on map
  for (unsigned int y = 2; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH; x++)
        {
          if (baseMap[y+1][x] < WATER_LEVEL - WATER_LEVEL / 4)
            baseMap[y][x] += WATER_LEVEL / 1.5f;
        }
    }
  //smooth tile left on map
  for (unsigned int y = 1; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 2; x < TILES_WIDTH; x++)
        {
          if (baseMap[y][x+1] < WATER_LEVEL - WATER_LEVEL / 4)
            baseMap[y][x] += WATER_LEVEL / 1.5f;
        }
    }
  //smooth tile right on map
  for (unsigned int y = 1; y < TILES_HEIGHT; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (baseMap[y][x-1] < WATER_LEVEL - WATER_LEVEL / 4)
            baseMap[y][x] += WATER_LEVEL / 1.5f;
        }
    }
}

void correctBaseTerrainMapAtEdges(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& waterMap)
{
  //correct top and bottom sides of the map
  for (unsigned int x = 0; x < TILES_WIDTH; ++x)
    {
      if (waterMap[0][x] != 0)
        baseMap[0][x] = baseMap[1][x];
      if (waterMap[TILES_HEIGHT][x] != 0)
        baseMap[TILES_HEIGHT][x] = baseMap[TILES_HEIGHT - 1][x];
    }
  //correct left and right sides of the map
  for (unsigned int y = 0; y < TILES_HEIGHT; ++y)
    {
      if (waterMap[y][0] != 0)
        baseMap[y][0] = baseMap[y][1];
      if (waterMap[y][TILES_WIDTH] != 0)
        baseMap[y][TILES_WIDTH] = baseMap[y][TILES_WIDTH - 1];
    }
}

void compressHeightBaseTerrainMap(std::vector<std::vector<float>>& baseMap, float ratio, bool entireRange)
{
  //smooth entire height range
  for (std::vector<float>& row : baseMap)
    {
      for (float& height : row)
        {
          if (height < 0 || entireRange)
            height /= ratio;
        }
    }
}

void denyBaseTerrainMapInvisibleTiles(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap)
{
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (hillMap[y][x] != 0
              && hillMap[y-1][x-1] != 0
              && hillMap[y-1][x] != 0
              && hillMap[y-1][x+1] != 0
              && hillMap[y][x-1] != 0
              && hillMap[y][x+1] != 0
              && hillMap[y+1][x-1] != 0
              && hillMap[y+1][x] != 0
              && hillMap[y+1][x+1] != 0)
            {
              baseMap[y][x] = DENY_TILE_RENDER_VALUE;
            }
        }
    }
}

void splitBaseTerrainToChunks(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& chunkMap, std::vector<TerrainTile>& baseChunks, int chunkSize, bool overlap)
{
  int step = overlap ? chunkSize / 2 : chunkSize - 1;
  for (int y = 0; y < TILES_HEIGHT - step - 1; y += step)
    {
      for (int x = 0; x < TILES_WIDTH - step - 1; x += step)
        {
          bool emptyChunk = true, chunked = true;
          if (overlap)
            {
              for (int y1 = y+1; y1 < y + chunkSize; y1++)
                {
                  for (int x1 = x+1; x1 < x + chunkSize; x1++)
                    {
                      if (chunkMap[y1][x1] != DENY_CHUNK_RENDER_VALUE
                          || chunkMap[y1+1][x1] != DENY_CHUNK_RENDER_VALUE
                          || chunkMap[y1][x1+1] != DENY_CHUNK_RENDER_VALUE
                          || chunkMap[y1+1][x1+1] != DENY_CHUNK_RENDER_VALUE)
                        {
                          chunked = false;
                          break;
                        }
                    }
                }
            }
          for (int y1 = y; y1 < y + chunkSize; y1++)
            {
              for (int x1 = x; x1 < x + chunkSize; x1++)
                {
                  if ((baseMap[y1][x1] != 0 && baseMap[y1][x1] != DENY_TILE_RENDER_VALUE)
                      || (baseMap[y1+1][x1] != 0 && baseMap[y1+1][x1] != DENY_TILE_RENDER_VALUE)
                      || (baseMap[y1+1][x1+1] != 0 && baseMap[y1+1][x1+1] != DENY_TILE_RENDER_VALUE)
                      || (baseMap[y1][x1+1] != 0 && baseMap[y1][x1+1] != DENY_TILE_RENDER_VALUE))
                    {
                      emptyChunk = false;
                      break;
                    }
                }
              if (!emptyChunk)
                break;
            }
          if (emptyChunk && (!chunked || !overlap))
            {
              for (int ydel = y + 1; ydel < y + chunkSize; ydel++)
                {
                  for (int xdel = x + 1; xdel < x + chunkSize; xdel++)
                    {
                      baseMap[ydel][xdel] = DENY_TILE_RENDER_VALUE;
                      chunkMap[ydel][xdel] = DENY_CHUNK_RENDER_VALUE;
                    }
                }
              baseChunks.emplace_back(x, y, 0, 0, 0, 0, false);
            }
        }
    }
}

void removeBaseTerrainUnderwaterTiles(std::vector<std::vector<float>>& baseMap, float thresholdValue)
{
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (baseMap[y-1][x-1] < thresholdValue
              && baseMap[y-1][x] < thresholdValue
              && baseMap[y-1][x+1] < thresholdValue
              && baseMap[y][x-1] < thresholdValue
              && baseMap[y][x] < thresholdValue
              && baseMap[y][x+1] < thresholdValue
              && baseMap[y+1][x-1] < thresholdValue
              && baseMap[y+1][x] < thresholdValue
              && baseMap[y+1][x+1] < thresholdValue)
            baseMap[y][x] = DENY_TILE_RENDER_VALUE;
        }
    }
}

void resetAllGLBuffers()
{
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void setupGLBuffersAttributes()
{
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}
