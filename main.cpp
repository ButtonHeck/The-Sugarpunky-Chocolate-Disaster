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

GLFWwindow* initGLFW();
void generateWaterMap(std::vector<std::vector<float>>& waterMap, unsigned int shoreSizeBase, float waterLevel, unsigned int &numWaterTiles);
void addWaterNearbyBaseTerrain(std::vector<std::vector<float>>& waterMap);
void fillSharpTerrainWithWater(std::vector<std::vector<float>>& waterMap);
void liftWaterLevel(std::vector<std::vector<float>>& waterMap, float liftValue);
void generateBaseTerrainMap(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& waterMap);
void smoothBaseTerrainMap(std::vector<std::vector<float>>& baseMap);
void correctBaseTerrainMapAtEdges(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& waterMap);
void compressHeightBaseTerrainMap(std::vector<std::vector<float>>& baseMap, float ratio, bool entireRange);
void denyBaseTerrainMapInvisibleTiles(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
void splitBaseTerrainToChunks(std::vector<std::vector<float>>& baseMap, std::vector<TerrainTile>& baseChunks, int chunkSize);
void generateHillMap(std::vector<std::vector<float>>& hillMap, std::vector<std::vector<float>>& waterMap, int cycles, float* max_height, HILL_DENSITY density);
void correctHillMapAtPlateaus(std::vector<std::vector<float>>& hillMap, float plateauHeight);
void smoothHillMapHeightChunks(std::vector<std::vector<float>>& hillMap, float baseWeight, float evenWeight, float diagonalWeight);
void removeOrphanHills(std::vector<std::vector<float>>& hillMap);
bool hasWaterNearby(unsigned int x, unsigned int y, std::vector<std::vector<float>>& waterMap, unsigned int radius);
void createTiles(std::vector<std::vector<float>>& map, std::vector<TerrainTile>& tiles, bool flat, bool createOnZeroTiles);
bool isOrphanAt(int x, int y, std::vector<std::vector<float>>& map);
void compressHeight(std::vector<std::vector<float>>& hillMap, float threshold_percent, float* limit, float ratio);
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
TextureLoader textureLoader;
std::default_random_engine RANDOMIZER_ENGINE;

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
  scene.setInt("grassTexture", 0);
  scene.setInt("hillTexture", 1);
  scene.setInt("waterTexture", 2);
  scene.setInt("sandTexture", 3);
  scene.setFloat("waterLevel", WATER_LEVEL);

  //generation water map
  std::vector<std::vector<float>> waterMap;
  std::vector<TerrainTile> waterTiles;
  unsigned int shoreSizeBase = 5;
  initializeMap(waterMap);
  waterTiles.reserve(NUM_TILES);
  unsigned int numWaterTiles = 0;
  generateWaterMap(waterMap, shoreSizeBase, WATER_LEVEL, numWaterTiles);
  while (numWaterTiles < TILES_WIDTH * (shoreSizeBase + 2) * (shoreSizeBase + 2) * 10
         || numWaterTiles > TILES_WIDTH * (shoreSizeBase + 3) * (shoreSizeBase + 3) * 10)
    {
      numWaterTiles = 0;
      initializeMap(waterMap);
      generateWaterMap(waterMap, shoreSizeBase, WATER_LEVEL, numWaterTiles);
    }

  //generating hill height map
  std::vector<std::vector<float>> hillsMap;
  std::vector<TerrainTile> hillTiles;
  hillTiles.reserve(NUM_TILES);
  initializeMap(hillsMap);
  float max_height = 0.0f;
  generateHillMap(hillsMap, waterMap, 8, &max_height, HILL_DENSITY::DENSE);
  generateHillMap(hillsMap, waterMap, 4, &max_height, HILL_DENSITY::THIN);
  compressHeight(hillsMap, 0.15f, &max_height, 1.5f); //slightly compress entire height range
  compressHeight(hillsMap, 0.66f, &max_height, 5.0f); //more heavy compress for top-most peaks
  correctHillMapAtPlateaus(hillsMap, 1.0f);
  smoothHillMapHeightChunks(hillsMap, 0.7f, 0.05f, 0.025f);
  removeOrphanHills(hillsMap);
  createTiles(hillsMap, hillTiles, false, false);
  hillTiles.shrink_to_fit();
  //fill hills buffer
  const size_t HILLS_VERTEX_DATA_LENGTH = hillTiles.size() * 20;
  const size_t HILLS_ELEMENT_DATA_LENGTH = hillTiles.size() * 6;
  GLfloat hillsVertices[HILLS_VERTEX_DATA_LENGTH];
  GLuint hillsIndices[HILLS_ELEMENT_DATA_LENGTH];
  for (unsigned int i = 0; i < hillTiles.size(); i++)
    {
      TerrainTile& tile = hillTiles[i];
      int offset = i * 20;
      int indexArrayOffset = i * 6;
      int index = i * 4;
      //approximation for texture mapping based on height coords of the tile.
      //for now, it works only for tiles which have a slope for either left->right and top->bottom (or vice versa) direction
      //generally speaking it doesn't work for tiles with one of the following principal scheme:
      /*
       * UL UR      0 1       1 0       0 0       0 0
       * LL LR  ->  0 0   or  0 0   or  1 0   or  0 1
       */
      float dyRatio = 1.0, dxRatio = 1.0;
      if ((tile.upperLeft > tile.lowLeft && tile.upperRight > tile.lowRight && tile.upperLeft > tile.lowRight && tile.upperRight > tile.lowLeft)
          || (tile.upperLeft < tile.lowLeft && tile.upperRight < tile.lowRight && tile.upperLeft < tile.lowRight && tile.upperRight < tile.lowLeft))
        {
          dyRatio = std::max(std::abs((tile.upperLeft + tile.upperRight) / 2 - (tile.lowLeft + tile.lowRight) / 2), 1.0f);
        }
      if ((tile.upperLeft > tile.upperRight && tile.upperLeft > tile.lowRight && tile.lowLeft > tile.upperRight && tile.lowLeft > tile.lowRight)
          || (tile.upperLeft < tile.upperRight && tile.upperLeft < tile.lowRight && tile.lowLeft < tile.upperRight && tile.lowLeft < tile.lowRight))
        {
          dxRatio = std::max(std::abs((tile.lowLeft + tile.upperLeft) / 2 - (tile.lowRight + tile.upperRight) / 2), 1.0f);
        }
      //ll
      hillsVertices[offset] = -1- TILES_WIDTH / 2 + tile.mapX;
      hillsVertices[offset+1] = tile.lowLeft;
      hillsVertices[offset+2] = - TILES_HEIGHT / 2 + tile.mapY;
      hillsVertices[offset+3] = 0.0f;
      hillsVertices[offset+4] = 0.0f;
      //lr
      hillsVertices[offset+5] = - TILES_WIDTH / 2 + tile.mapX;
      hillsVertices[offset+6] = tile.lowRight;
      hillsVertices[offset+7] = - TILES_HEIGHT / 2 + tile.mapY;
      hillsVertices[offset+8] = 1.0f * dxRatio;
      hillsVertices[offset+9] = 0.0f;
      //ur
      hillsVertices[offset+10] = - TILES_WIDTH / 2 + tile.mapX;
      hillsVertices[offset+11] = tile.upperRight;
      hillsVertices[offset+12] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      hillsVertices[offset+13] = 1.0f * dxRatio;
      hillsVertices[offset+14] = 1.0f * dyRatio;
      //ul
      hillsVertices[offset+15] = -1 - TILES_WIDTH / 2 + tile.mapX;
      hillsVertices[offset+16] = tile.upperLeft;
      hillsVertices[offset+17] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      hillsVertices[offset+18] = 0.0f;
      hillsVertices[offset+19] = 1.0f * dyRatio;

      bool indicesCrossed = false;
      if (tile.lowRight < tile.upperLeft || tile.upperLeft < tile.lowRight)
        indicesCrossed = true;

      if (!indicesCrossed)
        {
          hillsIndices[indexArrayOffset] = index;
          hillsIndices[indexArrayOffset+1] = index + 1;
          hillsIndices[indexArrayOffset+2] = index + 2;
          hillsIndices[indexArrayOffset+3] = index + 2;
          hillsIndices[indexArrayOffset+4] = index + 3;
          hillsIndices[indexArrayOffset+5] = index;
        }
      else
        {
          hillsIndices[indexArrayOffset] = index + 1;
          hillsIndices[indexArrayOffset+1] = index + 2;
          hillsIndices[indexArrayOffset+2] = index + 3;
          hillsIndices[indexArrayOffset+3] = index + 3;
          hillsIndices[indexArrayOffset+4] = index;
          hillsIndices[indexArrayOffset+5] = index + 1;
        }
    }
  //Hills tiles vertex data
  GLuint hillsVAO, hillsVBO, hillsEBO;
  glGenVertexArrays(1, &hillsVAO);
  glBindVertexArray(hillsVAO);
  glGenBuffers(1, &hillsVBO);
  glGenBuffers(1, &hillsEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hillsEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hillsIndices), hillsIndices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, hillsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(hillsVertices), hillsVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //generating base terrain data
  std::vector<std::vector<float>> baseMap;
  std::vector<TerrainTile> baseTiles, baseTerrainChunks, baseTerrainChunks2, baseTerrainChunks3, baseTerrainChunks4, baseTerrainChunks5;
  baseTiles.reserve(NUM_TILES);
  baseTerrainChunks.reserve(NUM_TILES / (BASE_TERRAIN_CHUNK_SIZE * BASE_TERRAIN_CHUNK_SIZE));
  baseTerrainChunks2.reserve(NUM_TILES / (BASE_TERRAIN_CHUNK_SIZE2 * BASE_TERRAIN_CHUNK_SIZE2));
  baseTerrainChunks3.reserve(NUM_TILES / (BASE_TERRAIN_CHUNK_SIZE3 * BASE_TERRAIN_CHUNK_SIZE3));
  baseTerrainChunks4.reserve(NUM_TILES / (BASE_TERRAIN_CHUNK_SIZE4 * BASE_TERRAIN_CHUNK_SIZE4));
  baseTerrainChunks5.reserve(NUM_TILES / (BASE_TERRAIN_CHUNK_SIZE5 * BASE_TERRAIN_CHUNK_SIZE5));
  initializeMap(baseMap);
  generateBaseTerrainMap(baseMap, waterMap);
  smoothBaseTerrainMap(baseMap);
  compressHeightBaseTerrainMap(baseMap, 2.0f, true);
  correctBaseTerrainMapAtEdges(baseMap, waterMap);
  splitBaseTerrainToChunks(baseMap, baseTerrainChunks, BASE_TERRAIN_CHUNK_SIZE);
  splitBaseTerrainToChunks(baseMap, baseTerrainChunks2, BASE_TERRAIN_CHUNK_SIZE2);
  splitBaseTerrainToChunks(baseMap, baseTerrainChunks3, BASE_TERRAIN_CHUNK_SIZE3);
  splitBaseTerrainToChunks(baseMap, baseTerrainChunks4, BASE_TERRAIN_CHUNK_SIZE4);
  splitBaseTerrainToChunks(baseMap, baseTerrainChunks5, BASE_TERRAIN_CHUNK_SIZE5);
  baseTerrainChunks.shrink_to_fit();
  baseTerrainChunks2.shrink_to_fit();
  baseTerrainChunks3.shrink_to_fit();
  baseTerrainChunks4.shrink_to_fit();
  baseTerrainChunks5.shrink_to_fit();
  denyBaseTerrainMapInvisibleTiles(baseMap, hillsMap);
  createTiles(baseMap, baseTiles, false, true);
  baseTiles.shrink_to_fit();
  const size_t BASE_TILES_SUB_VECTOR_CAPACITY = TILES_HEIGHT * 16;
  unsigned int numBaseSubTiles = baseTiles.size() / BASE_TILES_SUB_VECTOR_CAPACITY + 1;
  std::vector<std::vector<TerrainTile>> baseSubTiles;
  baseSubTiles.reserve(numBaseSubTiles);
  for (unsigned int i = 0; i < numBaseSubTiles; ++i)
    {
      baseSubTiles.push_back(std::vector<TerrainTile>());
    }
  for (auto& emptyTilesSubVector : baseSubTiles)
    emptyTilesSubVector.reserve(BASE_TILES_SUB_VECTOR_CAPACITY);
  unsigned int counter = 0;
  for (auto& tile : baseTiles)
    {
      baseSubTiles[counter/BASE_TILES_SUB_VECTOR_CAPACITY].push_back(tile);
      counter++;
    }
  baseSubTiles.shrink_to_fit();
  const size_t BASE_TERRAIN_DATA_LENGTH = baseSubTiles[0].size() * 20;
  const size_t BASE_TERRAIN_ELEMENT_LENGTH = baseSubTiles[0].size() * 6;
  GLuint baseVAO[numBaseSubTiles], baseVBO[numBaseSubTiles], baseEBO[numBaseSubTiles];
  glGenVertexArrays(numBaseSubTiles, baseVAO);
  glGenBuffers(numBaseSubTiles, baseVBO);
  glGenBuffers(numBaseSubTiles, baseEBO);
  //fill base terrain vertex data
  GLfloat baseVertices[BASE_TERRAIN_DATA_LENGTH];
  GLuint baseIndices[BASE_TERRAIN_ELEMENT_LENGTH];
  for (unsigned int i = 0; i < numBaseSubTiles; i++)
    {
      for (unsigned int c = 0; c < baseSubTiles[i].size(); c++)
        {
          TerrainTile& tile = baseSubTiles[i][c];
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
      glBindVertexArray(baseVAO[i]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, baseEBO[i]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(baseIndices), baseIndices, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, baseVBO[i]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(baseVertices), baseVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
      glBindVertexArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

  //generating base terrain flat tile chunks of BASE_TERRAIN_CHUNK_SIZE
  GLfloat baseChunkInstanceVertices[20] = {
      -1.0f, 0.0f,  1.0f, 0.0f,                         0.0f,
       1.0f, 0.0f,  1.0f, BASE_TERRAIN_CHUNK_SIZE - 4,  0.0f,
       1.0f, 0.0f, -1.0f, BASE_TERRAIN_CHUNK_SIZE - 4,  BASE_TERRAIN_CHUNK_SIZE - 4,
      -1.0f, 0.0f, -1.0f, 0.0f,                         BASE_TERRAIN_CHUNK_SIZE - 4
  };
  GLuint baseChunkInstanceIndices[6] = {0,1,2,2,3,0};
  GLuint baseChunkInstanceVAO, baseChunkInstanceVBO, baseChunkInstanceEBO, baseChunkInstanceModelVBO;
  glGenVertexArrays(1, &baseChunkInstanceVAO);
  glBindVertexArray(baseChunkInstanceVAO);
  glGenBuffers(1, &baseChunkInstanceVBO);
  glGenBuffers(1, &baseChunkInstanceEBO);
  glGenBuffers(1, &baseChunkInstanceModelVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, baseChunkInstanceEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(baseChunkInstanceIndices), baseChunkInstanceIndices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(baseChunkInstanceVertices), baseChunkInstanceVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  const int NUM_INSTANCES = baseTerrainChunks.size();
  glm::mat4* baseInstanceChunkModels = new glm::mat4[NUM_INSTANCES];
  for (unsigned int i = 0; i < baseTerrainChunks.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = baseTerrainChunks[i];
      model = glm::translate(model, glm::vec3(- TILES_WIDTH / 2 + tile.mapX + BASE_TERRAIN_CHUNK_SIZE / 2 - 1, 0.0f, - TILES_HEIGHT / 2 + tile.mapY + BASE_TERRAIN_CHUNK_SIZE / 2 - 1));
      model = glm::scale(model, glm::vec3(BASE_TERRAIN_CHUNK_SIZE / 2 - 1, 0.0f, BASE_TERRAIN_CHUNK_SIZE / 2 - 1));
      baseInstanceChunkModels[i] = model;
    }
  glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceModelVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * baseTerrainChunks.size(), &baseInstanceChunkModels[0], GL_STATIC_DRAW);
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
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //generating base terrain flat tile chunks of BASE_TERRAIN_CHUNK_SIZE2
  GLfloat baseChunkInstanceVertices2[20] = {
      -1.0f, 0.0f,  1.0f, 0.0f,                         0.0f,
       1.0f, 0.0f,  1.0f, BASE_TERRAIN_CHUNK_SIZE2 - 4,  0.0f,
       1.0f, 0.0f, -1.0f, BASE_TERRAIN_CHUNK_SIZE2 - 4,  BASE_TERRAIN_CHUNK_SIZE2 - 4,
      -1.0f, 0.0f, -1.0f, 0.0f,                         BASE_TERRAIN_CHUNK_SIZE2 - 4
  };
  GLuint baseChunkInstanceIndices2[6] = {0,1,2,2,3,0};
  GLuint baseChunkInstanceVAO2, baseChunkInstanceVBO2, baseChunkInstanceEBO2, baseChunkInstanceModelVBO2;
  glGenVertexArrays(1, &baseChunkInstanceVAO2);
  glBindVertexArray(baseChunkInstanceVAO2);
  glGenBuffers(1, &baseChunkInstanceVBO2);
  glGenBuffers(1, &baseChunkInstanceEBO2);
  glGenBuffers(1, &baseChunkInstanceModelVBO2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, baseChunkInstanceEBO2);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(baseChunkInstanceIndices2), baseChunkInstanceIndices2, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceVBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(baseChunkInstanceVertices2), baseChunkInstanceVertices2, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  const int NUM_INSTANCES2 = baseTerrainChunks2.size();
  glm::mat4* baseInstanceChunkModels2 = new glm::mat4[NUM_INSTANCES2];
  for (unsigned int i = 0; i < baseTerrainChunks2.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = baseTerrainChunks2[i];
      model = glm::translate(model, glm::vec3(- TILES_WIDTH / 2 + tile.mapX + BASE_TERRAIN_CHUNK_SIZE2 / 2 - 1, 0.0f, - TILES_HEIGHT / 2 + tile.mapY + BASE_TERRAIN_CHUNK_SIZE2 / 2 - 1));
      model = glm::scale(model, glm::vec3(BASE_TERRAIN_CHUNK_SIZE2 / 2 - 1, 0.0f, BASE_TERRAIN_CHUNK_SIZE2 / 2 - 1));
      baseInstanceChunkModels2[i] = model;
    }
  glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceModelVBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * baseTerrainChunks2.size(), &baseInstanceChunkModels2[0], GL_STATIC_DRAW);
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
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //generating base terrain flat tile chunks of BASE_TERRAIN_CHUNK_SIZE3
  GLfloat baseChunkInstanceVertices3[20] = {
      -1.0f, 0.0f,  1.0f, 0.0f,                         0.0f,
       1.0f, 0.0f,  1.0f, BASE_TERRAIN_CHUNK_SIZE3 - 4,  0.0f,
       1.0f, 0.0f, -1.0f, BASE_TERRAIN_CHUNK_SIZE3 - 4,  BASE_TERRAIN_CHUNK_SIZE3 - 4,
      -1.0f, 0.0f, -1.0f, 0.0f,                         BASE_TERRAIN_CHUNK_SIZE3 - 4
  };
  GLuint baseChunkInstanceIndices3[6] = {0,1,2,2,3,0};
  GLuint baseChunkInstanceVAO3, baseChunkInstanceVBO3, baseChunkInstanceEBO3, baseChunkInstanceModelVBO3;
  glGenVertexArrays(1, &baseChunkInstanceVAO3);
  glBindVertexArray(baseChunkInstanceVAO3);
  glGenBuffers(1, &baseChunkInstanceVBO3);
  glGenBuffers(1, &baseChunkInstanceEBO3);
  glGenBuffers(1, &baseChunkInstanceModelVBO3);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, baseChunkInstanceEBO3);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(baseChunkInstanceIndices3), baseChunkInstanceIndices3, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceVBO3);
  glBufferData(GL_ARRAY_BUFFER, sizeof(baseChunkInstanceVertices3), baseChunkInstanceVertices3, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  const int NUM_INSTANCES3 = baseTerrainChunks3.size();
  glm::mat4* baseInstanceChunkModels3 = new glm::mat4[NUM_INSTANCES3];
  for (unsigned int i = 0; i < baseTerrainChunks3.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = baseTerrainChunks3[i];
      model = glm::translate(model, glm::vec3(- TILES_WIDTH / 2 + tile.mapX + BASE_TERRAIN_CHUNK_SIZE3 / 2 - 1, 0.0f, - TILES_HEIGHT / 2 + tile.mapY + BASE_TERRAIN_CHUNK_SIZE3 / 2 - 1));
      model = glm::scale(model, glm::vec3(BASE_TERRAIN_CHUNK_SIZE3 / 2 - 1, 0.0f, BASE_TERRAIN_CHUNK_SIZE3 / 2 - 1));
      baseInstanceChunkModels3[i] = model;
    }
  glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceModelVBO3);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * baseTerrainChunks3.size(), &baseInstanceChunkModels3[0], GL_STATIC_DRAW);
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
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //generating base terrain flat tile chunks of BASE_TERRAIN_CHUNK_SIZE4
  GLfloat baseChunkInstanceVertices4[20] = {
      -1.0f, 0.0f,  1.0f, 0.0f,                         0.0f,
       1.0f, 0.0f,  1.0f, BASE_TERRAIN_CHUNK_SIZE4 - 4,  0.0f,
       1.0f, 0.0f, -1.0f, BASE_TERRAIN_CHUNK_SIZE4 - 4,  BASE_TERRAIN_CHUNK_SIZE4 - 4,
      -1.0f, 0.0f, -1.0f, 0.0f,                         BASE_TERRAIN_CHUNK_SIZE4 - 4
  };
  GLuint baseChunkInstanceIndices4[6] = {0,1,2,2,3,0};
  GLuint baseChunkInstanceVAO4, baseChunkInstanceVBO4, baseChunkInstanceEBO4, baseChunkInstanceModelVBO4;
  glGenVertexArrays(1, &baseChunkInstanceVAO4);
  glBindVertexArray(baseChunkInstanceVAO4);
  glGenBuffers(1, &baseChunkInstanceVBO4);
  glGenBuffers(1, &baseChunkInstanceEBO4);
  glGenBuffers(1, &baseChunkInstanceModelVBO4);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, baseChunkInstanceEBO4);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(baseChunkInstanceIndices4), baseChunkInstanceIndices4, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceVBO4);
  glBufferData(GL_ARRAY_BUFFER, sizeof(baseChunkInstanceVertices4), baseChunkInstanceVertices4, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  const int NUM_INSTANCES4 = baseTerrainChunks4.size();
  glm::mat4* baseInstanceChunkModels4 = new glm::mat4[NUM_INSTANCES4];
  for (unsigned int i = 0; i < baseTerrainChunks4.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = baseTerrainChunks4[i];
      model = glm::translate(model, glm::vec3(- TILES_WIDTH / 2 + tile.mapX + BASE_TERRAIN_CHUNK_SIZE4 / 2 - 1, 0.0f, - TILES_HEIGHT / 2 + tile.mapY + BASE_TERRAIN_CHUNK_SIZE4 / 2 - 1));
      model = glm::scale(model, glm::vec3(BASE_TERRAIN_CHUNK_SIZE4 / 2 - 1, 0.0f, BASE_TERRAIN_CHUNK_SIZE4 / 2 - 1));
      baseInstanceChunkModels4[i] = model;
    }
  glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceModelVBO4);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * baseTerrainChunks4.size(), &baseInstanceChunkModels4[0], GL_STATIC_DRAW);
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
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //generating base terrain flat tile chunks of BASE_TERRAIN_CHUNK_SIZE5
  GLfloat baseChunkInstanceVertices5[20] = {
      -1.0f, 0.0f,  1.0f, 0.0f,                         0.0f,
       1.0f, 0.0f,  1.0f, BASE_TERRAIN_CHUNK_SIZE5 - 4,  0.0f,
       1.0f, 0.0f, -1.0f, BASE_TERRAIN_CHUNK_SIZE5 - 4,  BASE_TERRAIN_CHUNK_SIZE5 - 4,
      -1.0f, 0.0f, -1.0f, 0.0f,                         BASE_TERRAIN_CHUNK_SIZE5 - 4
  };
  GLuint baseChunkInstanceIndices5[6] = {0,1,2,2,3,0};
  GLuint baseChunkInstanceVAO5, baseChunkInstanceVBO5, baseChunkInstanceEBO5, baseChunkInstanceModelVBO5;
  glGenVertexArrays(1, &baseChunkInstanceVAO5);
  glBindVertexArray(baseChunkInstanceVAO5);
  glGenBuffers(1, &baseChunkInstanceVBO5);
  glGenBuffers(1, &baseChunkInstanceEBO5);
  glGenBuffers(1, &baseChunkInstanceModelVBO5);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, baseChunkInstanceEBO5);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(baseChunkInstanceIndices5), baseChunkInstanceIndices5, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceVBO5);
  glBufferData(GL_ARRAY_BUFFER, sizeof(baseChunkInstanceVertices5), baseChunkInstanceVertices5, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  const int NUM_INSTANCES5 = baseTerrainChunks5.size();
  glm::mat4* baseInstanceChunkModels5 = new glm::mat4[NUM_INSTANCES5];
  for (unsigned int i = 0; i < baseTerrainChunks5.size(); i++)
    {
      glm::mat4 model;
      TerrainTile& tile = baseTerrainChunks5[i];
      model = glm::translate(model, glm::vec3(- TILES_WIDTH / 2 + tile.mapX + BASE_TERRAIN_CHUNK_SIZE5 / 2 - 1, 0.0f, - TILES_HEIGHT / 2 + tile.mapY + BASE_TERRAIN_CHUNK_SIZE5 / 2 - 1));
      model = glm::scale(model, glm::vec3(BASE_TERRAIN_CHUNK_SIZE5 / 2 - 1, 0.0f, BASE_TERRAIN_CHUNK_SIZE5 / 2 - 1));
      baseInstanceChunkModels5[i] = model;
    }
  glBindBuffer(GL_ARRAY_BUFFER, baseChunkInstanceModelVBO5);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * baseTerrainChunks5.size(), &baseInstanceChunkModels5[0], GL_STATIC_DRAW);
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
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //fill water buffer
  addWaterNearbyBaseTerrain(waterMap);
  fillSharpTerrainWithWater(waterMap);
  createTiles(waterMap, waterTiles, true, false);
  waterTiles.shrink_to_fit();
  const size_t WATER_VERTEX_DATA_LENGTH = waterTiles.size() * 20;
  const size_t WATER_ELEMENT_DATA_LENGTH = waterTiles.size() * 6;
  GLfloat waterVertices[WATER_VERTEX_DATA_LENGTH];
  GLuint waterIndices[WATER_ELEMENT_DATA_LENGTH];
  GLfloat waterHeightOffsets[NUM_TILES + TILES_WIDTH * 2]; //a bit overhead, because all we use is the part where we have water...
  //also, we don't have to init waterHeightOffsets, because we update its data every frame
  for (unsigned int i = 0; i < waterTiles.size(); i++)
    {
      TerrainTile& tile = waterTiles[i];
      int offset = i * 20;
      int indexArrayOffset = i * 6;
      int index = i * 4;
      //ll
      waterVertices[offset] = -1- TILES_WIDTH / 2 + tile.mapX;
      waterVertices[offset+1] = tile.lowLeft;
      waterVertices[offset+2] = - TILES_HEIGHT / 2 + tile.mapY;
      waterVertices[offset+3] = 0.0f;
      waterVertices[offset+4] = 0.0f;
      //lr
      waterVertices[offset+5] = - TILES_WIDTH / 2 + tile.mapX;
      waterVertices[offset+6] = tile.lowRight;
      waterVertices[offset+7] = - TILES_HEIGHT / 2 + tile.mapY;
      waterVertices[offset+8] = 1.0f;
      waterVertices[offset+9] = 0.0f;
      //ur
      waterVertices[offset+10] = - TILES_WIDTH / 2 + tile.mapX;
      waterVertices[offset+11] = tile.upperRight;
      waterVertices[offset+12] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      waterVertices[offset+13] = 1.0f;
      waterVertices[offset+14] = 1.0f;
      //ul
      waterVertices[offset+15] = -1 - TILES_WIDTH / 2 + tile.mapX;
      waterVertices[offset+16] = tile.upperLeft;
      waterVertices[offset+17] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      waterVertices[offset+18] = 0.0f;
      waterVertices[offset+19] = 1.0f;

      waterIndices[indexArrayOffset] = index;
      waterIndices[indexArrayOffset+1] = index + 1;
      waterIndices[indexArrayOffset+2] = index + 2;
      waterIndices[indexArrayOffset+3] = index + 2;
      waterIndices[indexArrayOffset+4] = index + 3;
      waterIndices[indexArrayOffset+5] = index;
    }
  //Water tiles vertex data
  GLuint waterVAO, waterVBO, waterEBO;
  glGenVertexArrays(1, &waterVAO);
  glBindVertexArray(waterVAO);
  glGenBuffers(1, &waterVBO);
  glGenBuffers(1, &waterEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(waterIndices), waterIndices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), waterVertices, GL_STREAM_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //print info
  std::cout << "Water on map:     " << numWaterTiles << std::endl;
  std::cout << "Water tiles:      " << waterTiles.size() << std::endl;
  std::cout << "Hills tiles:      " << hillTiles.size() << std::endl;
  std::cout << "x64 Chunks tiles: " << baseTerrainChunks.size() << std::endl;
  std::cout << "x32 Chunks tiles: " << baseTerrainChunks2.size() << std::endl;
  std::cout << "x24 Chunks tiles: " << baseTerrainChunks3.size() << std::endl;
  std::cout << "x16 Chunks tiles: " << baseTerrainChunks4.size() << std::endl;
  std::cout << "x8 Chunks tiles:  " << baseTerrainChunks5.size() << std::endl;
  std::cout << "Base tiles:       " << baseTiles.size() << std::endl;
  std::cout << "#subvecs:         " << numBaseSubTiles << std::endl;
  for (unsigned int i = 0; i < baseSubTiles.size(); ++i)
    std::cout << "Base subvec" << i << ": " << baseSubTiles[i].size() << std::endl;
  std::cout << "Summary: " << (waterTiles.size() + hillTiles.size() + baseTerrainChunks.size() + baseTiles.size()) << std::endl;

  //pre-setup
  glm::mat4 model;
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(cam.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 2.0f, 400.0f);
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
      scene.setInt("surfaceTextureEnum", 2);
      glBindVertexArray(hillsVAO);
      glDrawElements(GL_TRIANGLES, 6 * hillTiles.size(), GL_UNSIGNED_INT, 0);

      //base terrain tiles
      scene.setInt("surfaceTextureEnum", 0);
      scene.setBool("instanceRender", false);
      for (unsigned int i = 0; i < numBaseSubTiles; i++)
        {
          glBindVertexArray(baseVAO[i]);
          glDrawElements(GL_TRIANGLES, 6 * baseSubTiles[i].size(), GL_UNSIGNED_INT, 0);
        }

      //base terrain chunk tiles
      scene.setBool("instanceRender", true);
      glBindVertexArray(baseChunkInstanceVAO);
      glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, NUM_INSTANCES);
      glBindVertexArray(baseChunkInstanceVAO2);
      glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, NUM_INSTANCES2);
      glBindVertexArray(baseChunkInstanceVAO3);
      glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, NUM_INSTANCES3);
      glBindVertexArray(baseChunkInstanceVAO4);
      glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, NUM_INSTANCES4);
      glBindVertexArray(baseChunkInstanceVAO5);
      glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, NUM_INSTANCES5);

      //water tiles
      scene.setInt("surfaceTextureEnum", 1);
      scene.setBool("instanceRender", false);
      glBindVertexArray(waterVAO);
      glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
      for (size_t i = 0; i < sizeof(waterHeightOffsets) / sizeof(GLfloat); i+=2)
        {
            waterHeightOffsets[i] = std::cos(glfwGetTime() * (i % 31 + 1) / 24) / 12 + WATER_LEVEL;
            waterHeightOffsets[i+1] = std::sin(glfwGetTime() * (i % 29 + 1) / 24) / 12 + WATER_LEVEL;
        }
      GLfloat* temp = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
      for (unsigned int i = 0; i < waterTiles.size(); ++i)
        {
          TerrainTile& tile = waterTiles[i];
          *(temp+1+i*20) = waterHeightOffsets[(tile.mapY+1) * TILES_WIDTH + tile.mapX];
          *(temp+6+i*20) = waterHeightOffsets[(tile.mapY+1) * TILES_WIDTH + tile.mapX + 1];
          *(temp+11+i*20) = waterHeightOffsets[tile.mapY * TILES_WIDTH + tile.mapX + 1];
          *(temp+16+i*20) = waterHeightOffsets[tile.mapY * TILES_WIDTH + tile.mapX];
        }
      glUnmapBuffer(GL_ARRAY_BUFFER);
      glEnable(GL_BLEND);
      glDrawElements(GL_TRIANGLES, 6 * waterTiles.size(), GL_UNSIGNED_INT, 0);
      glDisable(GL_BLEND);

      glfwPollEvents();
      glfwSwapBuffers(window);
    }

  //CLEANUP
  glDeleteTextures(1, &flatTexture);
  glDeleteTextures(1, &hillTexture);
  glDeleteTextures(1, &waterTexture);
  glDeleteTextures(1, &sandTexture);
  glDeleteVertexArrays(numBaseSubTiles, baseVAO);
  glDeleteBuffers(numBaseSubTiles, baseVBO);
  glDeleteBuffers(numBaseSubTiles, baseEBO);
  glDeleteVertexArrays(1, &baseChunkInstanceVAO);
  glDeleteBuffers(1, &baseChunkInstanceVBO);
  glDeleteBuffers(1, &baseChunkInstanceEBO);
  glDeleteBuffers(1, &baseChunkInstanceModelVBO);
  glDeleteVertexArrays(1, &hillsVAO);
  glDeleteBuffers(1, &hillsVBO);
  glDeleteBuffers(1, &hillsEBO);
  glDeleteVertexArrays(1, &waterVAO);
  glDeleteBuffers(1, &waterVBO);
  glDeleteBuffers(1, &waterEBO);
  delete[] baseInstanceChunkModels;
  delete[] baseInstanceChunkModels2;
  delete[] baseInstanceChunkModels3;
  delete[] baseInstanceChunkModels4;
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
  glfwWindowHint(GLFW_SAMPLES, 4);
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Terrain Test", glfwGetPrimaryMonitor(), 0);
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, input.cursorCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return window;
}

void generateHillMap(std::vector<std::vector<float>>& hillMap, std::vector<std::vector<float>>& waterMap, int cycles, float* max_height, HILL_DENSITY density)
{
  srand(time(NULL));
  std::uniform_real_distribution<float> heightDistribution(0.3f, 0.7f);
  float density_value = 1.0f * (float)TILES_WIDTH;
  if (density == HILL_DENSITY::THIN)
      density_value = 1.1f * (float)TILES_WIDTH;
  else if (density == HILL_DENSITY::DENSE)
    density_value = 0.9f * (float)TILES_WIDTH;

  //hills kernel generation cycle
  for (int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (rand() % (int)density_value == 0 && !hasWaterNearby(x, y, waterMap, 2))
            {
              hillMap[y][x] += 1.0f;
            }
        }
    }
  *max_height = 1.0f;

  //fattening hills around, based on their kernel positions
  for (int cycle = 1; cycle < cycles; cycle++)
    {
      for (int y = cycle; y < TILES_HEIGHT - cycle; y++)
        {
          for (int x = cycle; x < TILES_WIDTH - cycle; x++)
            {
              if (y >= TILES_HEIGHT)
                break;
              if (rand() % (cycle+1) == cycle && (hillMap[y][x] != 0))
                {
                  int left = x-cycle;
                  if (left <= cycle)
                    left = cycle;
                  int right = x+cycle;
                  if (right >= TILES_WIDTH - cycle - 1)
                    right = TILES_WIDTH - cycle - 1;
                  int top = y-cycle;
                  if (top <= cycle)
                    top = cycle;
                  int bottom = y+cycle;
                  if (bottom >= TILES_HEIGHT - cycle - 1)
                    bottom = TILES_HEIGHT - cycle - 1;
                  for (int y2 = top; y2 <= bottom; y2++)
                    {
                      for (int x2 = left; x2 <= right; x2++)
                        {
                          if (rand() % (cycle + 2) > 1 && !hasWaterNearby(x2, y2, waterMap, 2))
                            {
                              hillMap[y2][x2] += 1.0f - 0.075f * cycle + (heightDistribution(RANDOMIZER_ENGINE) / cycle);
                              if (hillMap[y2][x2] > *max_height)
                                *max_height = hillMap[y2][x2];
                            }
                        }
                    }
                  x += cycle;
                  y += cycle;
                }
            }
        }
        removeOrphanHills(hillMap);
    }
}

void correctHillMapAtPlateaus(std::vector<std::vector<float>>& hillMap, float plateauHeight)
{
  unsigned int yt, yb, xl, xr;
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (hillMap[y][x] == 0)
            continue;
          unsigned int plateauHeightNeighbourTiles = 0, neighboursLimit = 6;
          yt = y - 1;
          yb = y + 1;
          xl = x - 1;
          xr = x + 1;
          for (auto y1 = yt; y1 <= yb; y1++)
            {
              for (auto x1 = xl; x1 <= xr; x1++)
                {
                  if (y1 == y && x1 == x)
                    continue;
                  if (hillMap[y1][x1] <= plateauHeight)
                    ++plateauHeightNeighbourTiles;
                }
            }
          if (plateauHeightNeighbourTiles >= neighboursLimit)
            hillMap[y][x] = 0;
        }
    }
}

void smoothHillMapHeightChunks(std::vector<std::vector<float>>& hillMap, float baseWeight, float evenWeight, float diagonalWeight)
{
  std::vector<std::vector<float>> hillMapSmoothed;
  initializeMap(hillMapSmoothed);
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (hillMap[y][x] == 0)
            continue;
          float smoothedHeight =
                hillMap[y][x] * baseWeight
              + hillMap[y-1][x] * evenWeight
              + hillMap[y+1][x] * evenWeight
              + hillMap[y][x-1] * evenWeight
              + hillMap[y][x+1] * evenWeight
              + hillMap[y-1][x-1] * diagonalWeight
              + hillMap[y-1][x+1] * diagonalWeight
              + hillMap[y+1][x-1] * diagonalWeight
              + hillMap[y+1][x+1] * diagonalWeight;
          hillMapSmoothed[y][x] = smoothedHeight;
        }
    }
  hillMap.assign(hillMapSmoothed.begin(), hillMapSmoothed.end());
}

void removeOrphanHills(std::vector<std::vector<float>>& hillMap)
{
  for (int y = 1; y < TILES_HEIGHT; y++)
    {
      for (int x = 1; x < TILES_WIDTH; x++)
        {
          if (hillMap[y][x] != 0.0f && isOrphanAt(x, y, hillMap))
            hillMap[y][x] = 0.0f;
        }
    }
}

bool hasWaterNearby(unsigned int x, unsigned int y, std::vector<std::vector<float>>& waterMap, unsigned int radius)
{
  int xl = x - radius;
  if (xl <= 0)
    xl = 0;
  int xr = x + radius;
  if (xr >= TILES_WIDTH)
    xr = TILES_WIDTH;
  int yt = y - radius;
  if (yt <= 0)
    yt = 0;
  int yb = y + radius;
  if (yb >= TILES_HEIGHT)
    yb = TILES_HEIGHT;
  for (int x1 = xl; x1 <= xr; x1++)
    {
      for (int y1 = yt; y1 <= yb; y1++)
        {
          if (waterMap[y1][x1] != 0)
            {
              return true;
            }
        }
    }
  return false;
}

void createTiles(std::vector<std::vector<float>>& map, std::vector<TerrainTile>& tiles, bool flat, bool createOnZeroHeight)
{
  for (unsigned int y = 1; y < map.size(); y++)
    {
      for (unsigned int x = 1; x < map[0].size(); x++)
        {
          if (map[y][x] == DENY_VALUE)
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
                  if (ll == DENY_VALUE)
                    ll = map[y][x];
                  lr = map[y][x];
                  ur = map[y-1][x];
                  if (ur == DENY_VALUE)
                    ur = map[y][x];
                  ul = map[y-1][x-1];
                  if (ul == DENY_VALUE)
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

bool isOrphanAt(int x, int y, std::vector<std::vector<float>>& map)
{
  return (map[y-1][x-1] == 0 &&
          map[y-1][x] == 0 &&
          map[y-1][x+1] == 0 &&
          map[y][x-1] == 0 &&
          map[y][x+1] == 0 &&
          map[y+1][x-1] == 0 &&
          map[y+1][x] == 0 &&
          map[y+1][x+1] == 0);
}

void compressHeight(std::vector<std::vector<float>>& map, float threshold_percent, float *limit, float ratio)
{
  float threshold_value = *limit * threshold_percent;
  for (auto& row : map)
    for (auto& height : row)
      {
        if (height < threshold_value)
          continue;
        height = threshold_value + (height - threshold_value) / ratio;
      }
  *limit /= ratio;
}

void generateWaterMap(std::vector<std::vector<float>>& waterMap, unsigned int shoreSizeBase, float waterLevel, unsigned int &numWaterTiles)
{
  srand(time(NULL));
  bool startAxisFromX = rand() % 2 == 0;
  bool riverEnd = false;
  unsigned int curveMaxDistance = rand() % 32 + 32;
  unsigned int curveDistanceStep = 0;
  const unsigned int X_MID_POINT = TILES_WIDTH / 2;
  const unsigned int Y_MID_POINT = TILES_HEIGHT / 2;
  const int MIN_CURVE_CHANGES = 8;
  int numCurveChanges = 0;
  enum DIRECTION : int {
    UP = 0, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT
  };
  DIRECTION dir;
  unsigned int y, x;
  unsigned int startCoord = rand() % TILES_HEIGHT;
  x = startAxisFromX ? startCoord : 0;
  y = startAxisFromX ? 0 : startCoord;
  dir = startAxisFromX ? DOWN : RIGHT;

  while (!riverEnd)
    {
      switch (dir) {
        case UP:
          {
            ++curveDistanceStep;
            --y;
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            if (rand() % 4 == 0)
              x += rand() % 2 == 0 ? 2 : -2;
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            if (x >= TILES_WIDTH)
              {
                x = TILES_WIDTH;
                riverEnd = true;
              }

            waterMap[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 32 + 32;
                dir = (DIRECTION)(rand() % 8);
                while (dir == DOWN || dir == DOWN_LEFT || dir == DOWN_RIGHT || dir == UP
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x < X_MID_POINT && (dir == LEFT || dir == UP_LEFT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y < Y_MID_POINT && (dir == UP_LEFT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x >= X_MID_POINT && (dir == RIGHT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y >= Y_MID_POINT && (dir == DOWN_LEFT || dir == DOWN_RIGHT))))
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case UP_RIGHT:
          {
            ++curveDistanceStep;
            y -= rand() % 2;
            x += rand() % 2;
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            if (x >= TILES_WIDTH)
              {
                x = TILES_WIDTH;
                riverEnd = true;
              }
            waterMap[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 32 + 32;
                dir = (DIRECTION)(rand() % 8);
                while (dir == DOWN_LEFT || dir == DOWN || dir == LEFT || dir == UP_RIGHT
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x < X_MID_POINT && (dir == LEFT || dir == UP_LEFT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y < Y_MID_POINT && (dir == UP_LEFT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x >= X_MID_POINT && (dir == RIGHT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y >= Y_MID_POINT && (dir == DOWN_LEFT || dir == DOWN_RIGHT))))
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case RIGHT:
          {
            ++curveDistanceStep;
            ++x;
            if (x >= TILES_WIDTH)
              {
                x = TILES_WIDTH;
                riverEnd = true;
              }
            if (rand() % 4 == 0)
              y += rand() % 2 == 0 ? 2 : -2;
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            if (y >= TILES_HEIGHT)
              {
                y = TILES_HEIGHT;
                riverEnd = true;
              }

            waterMap[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 32 + 32;
                dir = (DIRECTION)(rand() % 8);
                while (dir == LEFT || dir == UP_LEFT || dir == DOWN_LEFT || dir == RIGHT
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x < X_MID_POINT && (dir == LEFT || dir == UP_LEFT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y < Y_MID_POINT && (dir == UP_LEFT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x >= X_MID_POINT && (dir == RIGHT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y >= Y_MID_POINT && (dir == DOWN_LEFT || dir == DOWN_RIGHT))))
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case DOWN_RIGHT:
          {
            ++curveDistanceStep;
            y += rand() % 2;
            x += rand() % 2;
            if (y >= TILES_HEIGHT)
              {
                y = TILES_HEIGHT;
                riverEnd = true;
              }
            if (x >= TILES_WIDTH)
              {
                x = TILES_WIDTH;
                riverEnd = true;
              }
            waterMap[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 32 + 32;
                dir = (DIRECTION)(rand() % 8);
                while (dir == UP_LEFT || dir == UP || dir == LEFT || dir == DOWN_RIGHT
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x < X_MID_POINT && (dir == LEFT || dir == UP_LEFT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y < Y_MID_POINT && (dir == UP_LEFT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x >= X_MID_POINT && (dir == RIGHT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y >= Y_MID_POINT && (dir == DOWN_LEFT || dir == DOWN_RIGHT))))
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case DOWN:
          {
            ++curveDistanceStep;
            ++y;
            if (y >= TILES_HEIGHT)
              {
                y = TILES_HEIGHT;
                riverEnd = true;
              }
            if (rand() % 4 == 0)
              x += rand() % 2 == 0 ? 2 : -2;
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            if (x >= TILES_WIDTH)
              {
                x = TILES_WIDTH;
                riverEnd = true;
              }

            waterMap[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 32 + 32;
                dir = (DIRECTION)(rand() % 8);
                while (dir == UP || dir == UP_LEFT || dir == UP_RIGHT || dir == DOWN
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x < X_MID_POINT && (dir == LEFT || dir == UP_LEFT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y < Y_MID_POINT && (dir == UP_LEFT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x >= X_MID_POINT && (dir == RIGHT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y >= Y_MID_POINT && (dir == DOWN_LEFT || dir == DOWN_RIGHT))))
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case DOWN_LEFT:
          {
            ++curveDistanceStep;
            y += rand() % 2;
            x -= rand() % 2;
            if (y >= TILES_HEIGHT)
              {
                y = TILES_HEIGHT;
                riverEnd = true;
              }
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            waterMap[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 32 + 32;
                dir = (DIRECTION)(rand() % 8);
                while (dir == UP_RIGHT || dir == UP || dir == RIGHT || dir == DOWN_LEFT
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x < X_MID_POINT && (dir == LEFT || dir == UP_LEFT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y < Y_MID_POINT && (dir == UP_LEFT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x >= X_MID_POINT && (dir == RIGHT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y >= Y_MID_POINT && (dir == DOWN_LEFT || dir == DOWN_RIGHT))))
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case LEFT:
          {
            ++curveDistanceStep;
            --x;
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            if (rand() % 4 == 0)
              y += rand() % 2 == 0 ? 2 : -2;
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            if (y >= TILES_HEIGHT)
              {
                y = TILES_HEIGHT;
                riverEnd = true;
              }

            waterMap[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 32 + 32;
                dir = (DIRECTION)(rand() % 8);
                while (dir == RIGHT || dir == UP_RIGHT || dir == DOWN_RIGHT || dir == LEFT
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x < X_MID_POINT && (dir == LEFT || dir == UP_LEFT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y < Y_MID_POINT && (dir == UP_LEFT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x >= X_MID_POINT && (dir == RIGHT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y >= Y_MID_POINT && (dir == DOWN_LEFT || dir == DOWN_RIGHT))))
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case UP_LEFT:
          {
            ++curveDistanceStep;
            y -= rand() % 2;
            x -= rand() % 2;
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            waterMap[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 32 + 32;
                dir = (DIRECTION)(rand() % 8);
                while (dir == DOWN_RIGHT || dir == DOWN || dir == RIGHT || dir == UP_LEFT
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x < X_MID_POINT && (dir == LEFT || dir == UP_LEFT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y < Y_MID_POINT && (dir == UP_LEFT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (x >= X_MID_POINT && (dir == RIGHT || dir == UP_RIGHT)))
                       || (numCurveChanges < MIN_CURVE_CHANGES && (y >= Y_MID_POINT && (dir == DOWN_LEFT || dir == DOWN_RIGHT))))
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        default:
          break;
        }

      //fattening the kernel
      int shoreSizeYT = rand() % 3 + shoreSizeBase;
      int shoreSizeYB = rand() % 3 + shoreSizeBase;
      int shoreSizeXL = rand() % 3 + shoreSizeBase;
      int shoreSizeXR = rand() % 3 + shoreSizeBase;
      int xl = x - shoreSizeXL;
      if (xl <= 0)
        xl = 0;
      int xr = x + shoreSizeXR;
      if (xr >= TILES_WIDTH)
        xr = TILES_WIDTH;
      int yt = y - shoreSizeYT;
      if (yt <= 0)
        yt = 0;
      int yb = y + shoreSizeYB;
      if (yb >= TILES_HEIGHT)
        yb = TILES_HEIGHT;
      for (int y1 = yt; y1 <= yb; y1++)
        {
          for (int x1 = xl; x1 <= xr; x1++)
            {
              waterMap[y1][x1] = waterLevel;
              ++numWaterTiles;
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

void addWaterNearbyBaseTerrain(std::vector<std::vector<float>>& waterMap)
{
  //add water above the tile
  for (unsigned int y = 0; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if (waterMap[y+1][x] != 0)
            waterMap[y][x] = waterMap[y+1][x];
        }
    }
  //add water below the tile
  for (unsigned int y = TILES_HEIGHT; y > 0; y--)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if (waterMap[y-1][x] != 0)
            waterMap[y][x] = waterMap[y-1][x];
        }
    }
  //add water left to the tile
  for (unsigned int x = 0; x < TILES_WIDTH - 1; x++)
    {
      for (unsigned int y = 0; y < TILES_HEIGHT; y++)
        {
          if (waterMap[y][x+1] != 0)
            waterMap[y][x] = waterMap[y][x+1];
        }
    }
  //add water right to the tile
  for (unsigned int x = TILES_WIDTH; x > 0; x--)
    {
      for (unsigned int y = 0; y < TILES_HEIGHT; y++)
        {
          if (waterMap[y][x-1] != 0)
            waterMap[y][x] = waterMap[y][x-1];
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

void fillSharpTerrainWithWater(std::vector<std::vector<float>>& waterMap)
{
  for (int y2 = 1; y2 < TILES_HEIGHT - 1; y2++)
    {
      for (int x2 = 1; x2 < TILES_WIDTH - 1; x2++)
        {
          if (waterMap[y2][x2] == WATER_LEVEL)
            continue;
          if ((waterMap[y2-1][x2] == WATER_LEVEL && waterMap[y2+1][x2] == WATER_LEVEL) || (waterMap[y2][x2-1] == WATER_LEVEL && waterMap[y2][x2+1] == WATER_LEVEL))
            waterMap[y2][x2] = WATER_LEVEL;
        }
    }
}

void liftWaterLevel(std::vector<std::vector<float>>& waterMap, float liftValue)
{
  for (std::vector<float>& row : waterMap)
    {
      for (float& height : row)
        {
          if (height < 0)
            height += liftValue;
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
              baseMap[y][x] = DENY_VALUE;
            }
        }
    }
}

void splitBaseTerrainToChunks(std::vector<std::vector<float>>& baseMap, std::vector<TerrainTile>& baseChunks, int chunkSize)
{
  for (unsigned int y = 0; y < TILES_HEIGHT - chunkSize / 2; y += chunkSize / 2)
    {
      for (unsigned int x = 0; x < TILES_WIDTH - chunkSize / 2; x += chunkSize / 2)
        {
          bool emptyChunk = true;
          for (unsigned int y1 = y; y1 < y + chunkSize; y1++)
            {
              for (unsigned int x1 = x; x1 < x + chunkSize; x1++)
                {
                  if (baseMap[y1][x1] != 0 || baseMap[y1+1][x1] != 0 || baseMap[y1+1][x+1] != 0 || baseMap[y][x+1] != 0)
                    {
                      emptyChunk = false;
                      break;
                    }
                }
              if (!emptyChunk)
                break;
            }
          if (emptyChunk)
            {
              for (unsigned int ydel = y + 1; ydel < y + chunkSize - 1; ydel++)
                {
                  for (unsigned int xdel = x + 1; xdel < x + chunkSize - 1; xdel++)
                    {
                      baseMap[ydel][xdel] = DENY_VALUE;
                    }
                }
              baseChunks.emplace_back(x, y, 0, 0, 0, 0, false);
            }
        }
    }
}
