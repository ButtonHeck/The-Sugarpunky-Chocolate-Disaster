#include <iostream>
#include <chrono>
#include <omp.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Settings.h"
#include "Timer.h"
#include "Shader.h"
#include "Camera.h"
#include "InputController.h"
#include "TextureLoader.h"
#include "WaterMapGenerator.h"
#include "HillsMapGenerator.h"
#include "UnderwaterQuadMapGenerator.h"
#include "BaseMapGenerator.h"
#include "Skybox.h"
#include "Model.h"
#include "TreeGenerator.h"
#include "FontManager.h"
#include "CoordinateSystemRenderer.h"
#include "SaveLoadManager.h"
#include "BuildableMapGenerator.h"
#include "ModelChunk.h"

GLFWwindow* initGLFW();
void prepareTerrain();
void printMapsInfos();
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar* message, const void* userParam);

int scr_width;
int scr_height;
float aspect_ratio;
GLFWwindow* window;
Timer timer;
Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));
glm::vec3 cursorToViewportDirection;
InputController input;
TextureLoader textureLoader;
WaterMapGenerator* waterMapGenerator = new WaterMapGenerator();
HillsMapGenerator* hillMapGenerator = new HillsMapGenerator(waterMapGenerator->getMap());
UnderwaterQuadMapGenerator underwaterQuadGenerator;
BaseMapGenerator* baseMapGenerator = new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap());
Skybox skybox(PROJ_PATH + "/textures/cubemap/", textureLoader);
CoordinateSystemRenderer csRenderer;
BuildableMapGenerator* buildableMapGenerator = new BuildableMapGenerator(baseMapGenerator->getMap(), hillMapGenerator->getMap());
SaveLoadManager* saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator, buildableMapGenerator);
TreeGenerator* treeGenerator;
std::vector<ModelChunk> treeModelChunks;
std::vector<ModelChunk> hillTreeModelChunks;
unsigned int chunkLoadingDistance = 8;
bool renderShadowOnTrees = true;
bool renderTreeModels = true;
bool animateWater = true;
bool renderDebugText = true;
bool recreateTerrainRequest = false;
bool saveRequest = false;
bool loadRequest = false;
bool showCursor = false;
bool showBuildable = false;
bool modelRenderOptimize = true;
float cursorOnMapX = 0.0f;
float cursorOnMapZ = 0.0f;
float cameraOnMapX = 0.0f;
float cameraOnMapZ = 0.0f;
int cursorOnMapCoordX = 0;
int cursorOnMapCoordZ = 0;
int cameraOnMapCoordX = 0;
int cameraOnMapCoordZ = 0;
std::string cursorTile = "Flat";

int main()
{
  //initializing and presetup routines
  window = initGLFW();
  glewExperimental = GL_TRUE;
  glewInit();
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(glDebugOutput, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  FontManager fontManager("OCTAPOST_1.ttf", glm::ortho(0.0f, (float)scr_width, 0.0f, (float)scr_height));
  fontManager.loadFont();

  //make sure OpenMP is working
  #pragma omp parallel num_threads(2)
  {
    std::cout << ("Hello from thread " + std::to_string(omp_get_thread_num()) + "\n");
  }

  //shaders loading
  Shader hills(PROJ_PATH + "/shaders/hills.vs", PROJ_PATH + "/shaders/hills.fs");
  Shader sand(PROJ_PATH + "/shaders/terrainVertex.vs", PROJ_PATH + "/shaders/sand.fs");
  Shader underwater(PROJ_PATH + "/shaders/terrainVertex.vs", PROJ_PATH + "/shaders/underwater.fs");
  Shader flat(PROJ_PATH + "/shaders/flat.vs", PROJ_PATH + "/shaders/flat.fs");
  Shader water(PROJ_PATH + "/shaders/water.vs", PROJ_PATH + "/shaders/water.fs");
  Shader sky(PROJ_PATH + "/shaders/skybox.vs", PROJ_PATH + "/shaders/skybox.fs");
  Shader modelShader(PROJ_PATH + "/shaders/model.vs", PROJ_PATH + "/shaders/model.fs");
  Shader fontShader(PROJ_PATH + "/shaders/font.vs", PROJ_PATH + "/shaders/font.fs");
  Shader csShader(PROJ_PATH + "/shaders/coordinateSystem.vs",
                          PROJ_PATH + "/shaders/coordinateSystem.gs",
                          PROJ_PATH + "/shaders/coordinateSystem.fs");
  Shader buildableShader(PROJ_PATH + "/shaders/buildableTiles.vs", PROJ_PATH + "/shaders/buildableTiles.fs");
  Shader selectedTileShader(PROJ_PATH + "/shaders/selectedTile.vs", PROJ_PATH + "/shaders/buildableTiles.fs");
  std::vector<Shader*> shaders =
  {&hills, &sand, &underwater, &flat, &water, &sky, &modelShader, &fontShader, &csShader, &buildableShader, &selectedTileShader};

  //models and model-related objects loading
  Model tree1(PROJ_PATH + "/models/tree1/tree1.obj", textureLoader);
  Model tree2(PROJ_PATH + "/models/tree2/tree2.obj", textureLoader);
  Model tree3(PROJ_PATH + "/models/tree3/tree3.obj", textureLoader);
  Model hillTree1(PROJ_PATH + "/models/hillTree1/hillTree1.obj", textureLoader);
  Model hillTree2(PROJ_PATH + "/models/hillTree2/hillTree2.obj", textureLoader);
  Model hillTree3(PROJ_PATH + "/models/hillTree3/hillTree3.obj", textureLoader);
  treeGenerator = new TreeGenerator({tree1, tree2, tree3}, {hillTree1, hillTree2, hillTree3});
  saveLoadManager->setTreeGenerator(*treeGenerator);

  //textures loading
  GLuint baseTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/base.jpg", GL_REPEAT, 0);
  GLuint hillTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/hill.jpg", GL_REPEAT, 1);
  GLuint waterTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/water.png", GL_REPEAT, 2);
  GLuint sandTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/sand.jpg", GL_REPEAT, 3);
  GLuint waterTextureSpec = textureLoader.loadTexture(PROJ_PATH + "/textures/water_specular.png", GL_REPEAT, 4);
  GLuint hillTextureSpec = textureLoader.loadTexture(PROJ_PATH + "/textures/hill_specular.jpg", GL_REPEAT, 6);
  GLuint baseTextureNormal = textureLoader.loadTexture(PROJ_PATH + "/textures/base_normal.jpg", GL_REPEAT, 8);
  GLuint underwaterSandTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/underwater_sand.jpg", GL_REPEAT, 9);
  GLuint sandTexture2 = textureLoader.loadTexture(PROJ_PATH + "/textures/sand2.jpg", GL_REPEAT, 10);
  GLuint baseTexture2 = textureLoader.loadTexture(PROJ_PATH + "/textures/base2.jpg", GL_REPEAT, 11);
  GLuint hillTexture2 = textureLoader.loadTexture(PROJ_PATH + "/textures/hill2.jpg", GL_REPEAT, 12);
  GLuint baseTexture_x2 = textureLoader.loadTexture(PROJ_PATH + "/textures/base_x2.jpg", GL_REPEAT, 14);
  GLuint baseTexture2_x2 = textureLoader.loadTexture(PROJ_PATH + "/textures/base2_x2.jpg", GL_REPEAT, 15);
  std::vector<GLuint*> textures =
  {&baseTexture, &hillTexture, &waterTexture, &sandTexture, &waterTextureSpec, &hillTextureSpec, &baseTextureNormal,
  &underwaterSandTexture, &sandTexture2, &baseTexture2, &hillTexture2, &baseTexture_x2, &baseTexture2_x2};

  //shaders setup
  hills.use();
  hills.setVec3("lightDirTo", LIGHT_DIR_TO);
  hills.setInt("base_diffuse", 14);
  hills.setInt("base_diffuse2", 15);
  hills.setInt("hills_diffuse", 1);
  hills.setInt("hills_diffuse2", 12);
  hills.setInt("hills_specular", 6);
  hills.setInt("base_normal", 8);
  hills.setInt("tilesDimension", TILES_WIDTH);
  sand.use();
  sand.setFloat("waterLevel", WATER_LEVEL);
  sand.setInt("base_diffuse", 0);
  sand.setInt("base_diffuse2", 11);
  sand.setInt("sand_diffuse", 3);
  sand.setInt("sand_diffuse2", 10);
  sand.setInt("base_normal", 8);
  sand.setInt("tilesDimension", TILES_WIDTH);
  sand.setVec3("lightDirTo", LIGHT_DIR_TO);
  sand.setBool("instanceRendering", false);
  underwater.use();
  underwater.setInt("underwater_diffuse", 9);
  underwater.setInt("underwater_normal", 8);
  underwater.setInt("tilesDimension", TILES_WIDTH);
  underwater.setVec3("lightDirTo", LIGHT_DIR_TO);
  flat.use();
  flat.setInt("base_diffuse", 0);
  flat.setInt("base_diffuse2", 11);
  flat.setInt("base_normal", 8);
  flat.setInt("tilesDimension", TILES_WIDTH);
  flat.setVec3("lightDirTo", LIGHT_DIR_TO);
  water.use();
  water.setInt("water_diffuse", 2);
  water.setInt("water_specular", 4);
  water.setInt("tilesDimension", TILES_WIDTH);
  water.setVec3("lightDirTo", LIGHT_DIR_TO);
  modelShader.use();
  modelShader.setVec3("lightDirTo", LIGHT_DIR_TO);
  buildableShader.use();
  buildableShader.setBool("cursorMode", false);
  selectedTileShader.use();
  selectedTileShader.setBool("cursorMode", true);

  //generating the terrain landscape data and filling related vertex/element buffers
  prepareTerrain();
  treeGenerator->setupPlainModels(baseMapGenerator->getMap(), hillMapGenerator->getMap(), treeModelChunks);
  treeGenerator->setupHillModels(hillMapGenerator->getMap(), hillTreeModelChunks);

  //create underwater relief texture manually and bind it to related shader
  GLuint underwaterReliefTextureUnit = 13;
  GLuint underwaterReliefTexture = textureLoader.createUnderwaterReliefTexture(waterMapGenerator, underwaterReliefTextureUnit);
  underwater.use();
  underwater.setInt("bottomRelief", underwaterReliefTextureUnit);
  textures.push_back(&underwaterReliefTexture);

  //etc
  printMapsInfos();
  glm::mat4 model;
  glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)scr_width / (float)scr_height, NEAR_PLANE, FAR_PLANE);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  auto frameTime = std::chrono::high_resolution_clock::now();
  auto currentTime = frameTime;
  unsigned int frames = 0, fps = 0;

  //MAIN LOOP
  while(!glfwWindowShouldClose(window))
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      float delta = timer.tick();
      ++frames;
      currentTime = std::chrono::high_resolution_clock::now();
      if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - frameTime).count() > 1000)
        {
          frameTime = currentTime;
          fps = frames;
          frames = 0;
        }
      input.processKeyboard(delta);
      glm::mat4 view = camera.getViewMatrix();
      glm::vec3 viewPosition = camera.getPosition();
      glm::mat4 projectionView = projection * view;
      glLineWidth(1);

      if (recreateTerrainRequest)
        {
          delete waterMapGenerator;
          delete hillMapGenerator;
          delete baseMapGenerator;
          delete buildableMapGenerator;
          waterMapGenerator = new WaterMapGenerator();
          hillMapGenerator = new HillsMapGenerator(waterMapGenerator->getMap());
          baseMapGenerator = new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap());
          buildableMapGenerator = new BuildableMapGenerator(baseMapGenerator->getMap(), hillMapGenerator->getMap());
          prepareTerrain();
          treeGenerator->setupPlainModels(baseMapGenerator->getMap(), hillMapGenerator->getMap(), treeModelChunks);
          treeGenerator->setupHillModels(hillMapGenerator->getMap(), hillTreeModelChunks);
          delete saveLoadManager;
          saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator, buildableMapGenerator);
          saveLoadManager->setTreeGenerator(*treeGenerator);
          recreateTerrainRequest = false;
          underwaterReliefTexture = textureLoader.createUnderwaterReliefTexture(waterMapGenerator, underwaterReliefTextureUnit);
        }

      //hill tiles
      hills.use();
      hills.setMat4("projectionView", projectionView);
      hills.setMat4("model", model);
      hills.setVec3("viewPosition", viewPosition);
      glBindVertexArray(hillMapGenerator->getVAO());
      glDrawElements(GL_TRIANGLES, 6 * hillMapGenerator->getTiles().size(), GL_UNSIGNED_INT, 0);

      //sand terrain tiles
      sand.use();
      sand.setMat4("projectionView", projectionView);
      sand.setMat4("model", model);
      sand.setVec3("viewPosition", viewPosition);
      glBindVertexArray(baseMapGenerator->getVAO());
      glDrawArrays(GL_TRIANGLES, 0, 6 * baseMapGenerator->getTiles().size());

      //underwater tile
      underwater.use();
      underwater.setMat4("projectionView", projectionView);
      underwater.setMat4("model", model);
      glBindVertexArray(underwaterQuadGenerator.getVAO());
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      //base terrain chunk tiles
      flat.use();
      flat.setMat4("projectionView", projectionView);
      flat.setVec3("viewPosition", viewPosition);
      for (unsigned int vao = 0; vao < NUM_BASE_TERRAIN_CHUNKS; vao++)
        {
          glBindVertexArray(baseMapGenerator->getChunkVAO(vao));
          glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, baseMapGenerator->getNumChunksInstances(vao));
        }
      //base terrain 1x1 tiles
      glBindVertexArray(baseMapGenerator->getCellVAO());
      glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, baseMapGenerator->getNumCellInstances());

      //update cursor-to-map mappings
      if (cursorToViewportDirection.y < 0.0f)
        {
          float ratio = camera.getPosition().y / (-cursorToViewportDirection.y);
          cursorOnMapX = (cursorToViewportDirection.x * ratio) + camera.getPosition().x;
          cursorOnMapZ = (cursorToViewportDirection.z * ratio) + camera.getPosition().z;
          bool cursorOutOfMap = false;
          cursorOnMapX = glm::clamp(cursorOnMapX, -TILES_WIDTH/2.0f, TILES_WIDTH/2.0f);
          cursorOnMapZ = glm::clamp(cursorOnMapZ, -TILES_HEIGHT/2.0f, TILES_HEIGHT/2.0f);
          if (cursorOnMapX == -TILES_WIDTH/2 || cursorOnMapX == TILES_WIDTH/2 ||
              cursorOnMapZ == -TILES_HEIGHT/2 || cursorOnMapZ == TILES_HEIGHT/2)
            cursorOutOfMap = true;
          cursorOnMapCoordX = (int)(TILES_WIDTH + cursorOnMapX) - TILES_WIDTH / 2;
          cursorOnMapCoordX = glm::clamp(cursorOnMapCoordX, 1, TILES_WIDTH - 2);
          cursorOnMapCoordZ = (int)(TILES_HEIGHT + cursorOnMapZ) - TILES_HEIGHT / 2 + 1;
          cursorOnMapCoordZ = glm::clamp(cursorOnMapCoordZ, 1, TILES_HEIGHT - 1);
          if (cursorOutOfMap)
            cursorTile = "out of map";
          else if (buildableMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX] != 0)
            cursorTile = "Flat";
          else if (hillMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX] != 0 ||
                   hillMapGenerator->getMap()[cursorOnMapCoordZ-1][cursorOnMapCoordX] != 0 ||
                   hillMapGenerator->getMap()[cursorOnMapCoordZ-1][cursorOnMapCoordX+1] != 0 ||
                   hillMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX+1] != 0)
            cursorTile = "Hills";
          else
            {
              if (baseMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX] == DENY_TILE_RENDER_VALUE ||
                  baseMapGenerator->getMap()[cursorOnMapCoordZ-1][cursorOnMapCoordX] == DENY_TILE_RENDER_VALUE ||
                  baseMapGenerator->getMap()[cursorOnMapCoordZ-1][cursorOnMapCoordX+1] == DENY_TILE_RENDER_VALUE ||
                  baseMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX+1] == DENY_TILE_RENDER_VALUE)
                cursorTile = "Water";
              else
                cursorTile = "Shore";
            }
        }
      else
        cursorTile = "out of map";

      //update camera map position info
      cameraOnMapX = camera.getPosition().x;
      cameraOnMapZ = camera.getPosition().z;
      cameraOnMapX = glm::clamp(cameraOnMapX, -TILES_WIDTH/2.0f, TILES_WIDTH/2.0f);
      cameraOnMapZ = glm::clamp(cameraOnMapZ, -TILES_HEIGHT/2.0f, TILES_HEIGHT/2.0f);
      cameraOnMapCoordX = (int)(TILES_WIDTH + cameraOnMapX) - TILES_WIDTH / 2;
      cameraOnMapCoordX = glm::clamp(cameraOnMapCoordX, 0, TILES_WIDTH - 1);
      cameraOnMapCoordZ = (int)(TILES_HEIGHT + cameraOnMapZ) - TILES_HEIGHT / 2;
      cameraOnMapCoordZ = glm::clamp(cameraOnMapCoordZ, 0, TILES_HEIGHT - 1);
      ModelChunk cameraChunk = treeModelChunks[0];
      for (unsigned int i = 0; i < treeModelChunks.size(); i++)
        {
          if (treeModelChunks[i].containsPoint(cameraOnMapCoordX, cameraOnMapCoordZ))
            {
              cameraChunk = treeModelChunks[i];
              break;
            }
        }

      //buildable tiles
      if (showBuildable)
        {
          buildableShader.use();
          buildableShader.setMat4("projectionView", projectionView);
          glBindVertexArray(buildableMapGenerator->getVAO());
          glEnable(GL_BLEND);
          glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, buildableMapGenerator->getNumInstances());
          glDisable(GL_BLEND);
        }

      //cursor selected tile
      if (showCursor && buildableMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX] != 0)
        {
          selectedTileShader.use();
          selectedTileShader.setMat4("projectionView", projectionView);
          glm::mat4 selectedModel;
          selectedModel = glm::translate(selectedModel, glm::vec3(-TILES_WIDTH / 2 + cursorOnMapCoordX, 0.0f, -TILES_HEIGHT / 2 + cursorOnMapCoordZ));
          selectedTileShader.setMat4("model", selectedModel);
          glBindVertexArray(buildableMapGenerator->getSelectedTileVAO());
          glEnable(GL_BLEND);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
          glDisable(GL_BLEND);
        }

      //water tiles
      water.use();
      water.setMat4("projectionView", projectionView);
      water.setMat4("model", model);
      water.setVec3("viewPosition", viewPosition);
      std::vector<TerrainTile>& waterTiles = waterMapGenerator->getTiles();
      glBindVertexArray(waterMapGenerator->getVAO());
      if (animateWater)
        {
          glBindBuffer(GL_ARRAY_BUFFER, waterMapGenerator->getVBO());
          GLfloat* waterHeightOffsets = waterMapGenerator->getHeightOffsets();
          double frameTime = glfwGetTime();
          for (size_t i = 0; i < waterMapGenerator->WATER_HEIGHT_OFFSETS_SIZE; i+=2)
            {
                waterHeightOffsets[i] = std::cos(frameTime * (i % 31) / 8) / 16 + WATER_LEVEL;
                waterHeightOffsets[i+1] = std::sin(frameTime * (i% 29) / 8) / 16 + WATER_LEVEL;
            }
          GLfloat* temp = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
          unsigned int numWaterTiles = waterTiles.size();
          glm::vec3 normalLR, normalUL;
          for (unsigned int i = 0; i < numWaterTiles; ++i)
            {
              TerrainTile& tile = waterTiles[i];
              float ll = waterHeightOffsets[(tile.mapY+1) * TILES_WIDTH + tile.mapX];
              float lr = waterHeightOffsets[(tile.mapY+1) * TILES_WIDTH + tile.mapX + 1];
              float ur = waterHeightOffsets[tile.mapY * TILES_WIDTH + tile.mapX + 1];
              float ul = waterHeightOffsets[tile.mapY * TILES_WIDTH + tile.mapX];
              *(temp+1+i*48) = ll;
              *(temp+9+i*48) = lr;
              *(temp+17+i*48) = ur;
              *(temp+25+i*48) = ur;
              *(temp+33+i*48) = ul;
              *(temp+41+i*48) = ll;
              normalLR = glm::cross(
                    glm::vec3(tile.mapX, ur, tile.mapY - 1)
                    -
                    glm::vec3(tile.mapX, lr, tile.mapY)
                    ,
                    glm::vec3(tile.mapX - 1, ll, tile.mapY)
                    -
                    glm::vec3(tile.mapX, lr, tile.mapY));
              normalUL = glm::cross(
                    glm::vec3(tile.mapX - 1, ll, tile.mapY)
                    -
                    glm::vec3(tile.mapX - 1, ul, tile.mapY - 1)
                    ,
                    glm::vec3(tile.mapX, ur, tile.mapY - 1)
                    -
                    glm::vec3(tile.mapX - 1, ul, tile.mapY - 1));
              *(temp+5+i*48) = normalLR.x;
              *(temp+6+i*48) = normalLR.y;
              *(temp+7+i*48) = normalLR.z;
              *(temp+13+i*48) = normalLR.x;
              *(temp+14+i*48) = normalLR.y;
              *(temp+15+i*48) = normalLR.z;
              *(temp+21+i*48) = normalLR.x;
              *(temp+22+i*48) = normalLR.y;
              *(temp+23+i*48) = normalLR.z;

              *(temp+29+i*48) = normalUL.x;
              *(temp+30+i*48) = normalUL.y;
              *(temp+31+i*48) = normalUL.z;
              *(temp+37+i*48) = normalUL.x;
              *(temp+38+i*48) = normalUL.y;
              *(temp+39+i*48) = normalUL.z;
              *(temp+45+i*48) = normalUL.x;
              *(temp+46+i*48) = normalUL.y;
              *(temp+47+i*48) = normalUL.z;
            }
          glUnmapBuffer(GL_ARRAY_BUFFER);
        }
      glEnable(GL_BLEND);
      glDrawArrays(GL_TRIANGLES, 0, 6 * waterTiles.size());
      glDisable(GL_BLEND);

      //Skybox rendering
      sky.use();
      sky.setMat4("view", glm::mat4(glm::mat3(view)));
      sky.setMat4("projection", projection);
      glDepthFunc(GL_LEQUAL);
      glDisable(GL_CULL_FACE);
      glBindVertexArray(skybox.getVAO());
      glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexture());
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glDepthFunc(GL_LESS);
      glEnable(GL_CULL_FACE);

      //trees rendering
      if (renderTreeModels)
        {
          modelShader.use();
          modelShader.setMat4("projectionView", projectionView);
          modelShader.setVec3("viewPosition", viewPosition);
          modelShader.setBool("shadow", renderShadowOnTrees);
          treeGenerator->draw(modelShader, camera, treeModelChunks, hillTreeModelChunks,
                              modelRenderOptimize, chunkLoadingDistance);
        }

      //font rendering
      if (renderDebugText)
        {
          fontManager.renderText(fontShader, "FPS: " + std::to_string(fps), 10.0f, (float)scr_height - 25.0f, 0.35f);
          fontManager.renderText(fontShader,
                                 "camera pos: " + std::to_string(viewPosition.x).substr(0,6) + ": "
                                 + std::to_string(viewPosition.y).substr(0,6) + ": "
                                 + std::to_string(viewPosition.z).substr(0,6), 10.0f, (float)scr_height - 45.0f, 0.35f);
          fontManager.renderText(fontShader,
                                 "camera on map: " + std::to_string(cameraOnMapCoordX) + ": " + std::to_string(cameraOnMapCoordZ),
                                 10.0f, (float)scr_height - 65.0f, 0.35f);
          fontManager.renderText(fontShader,
                                 "View dir: " + std::to_string(camera.getDirection().x).substr(0,6) + ": "
                                 + std::to_string(camera.getDirection().y).substr(0,6) + ": "
                                 + std::to_string(camera.getDirection().z).substr(0,6), 10.0f, (float)scr_height - 85.0f, 0.35f);
          fontManager.renderText(fontShader,
                                 "Cursor at: " + (!showCursor ? "inactive" : (std::to_string(cursorToViewportDirection.x).substr(0,6) + ": "
                                 + std::to_string(cursorToViewportDirection.y).substr(0,6) + ": "
                                 + std::to_string(cursorToViewportDirection.z).substr(0,6))), 10.0f, (float)scr_height - 105.0f, 0.35f);
          fontManager.renderText(fontShader,
                                 "Cursor on map: " + (!showCursor ? "inactive" : (std::to_string(cursorOnMapCoordX) + ": "
                                 + std::to_string(cursorOnMapCoordZ-1) + ", " + cursorTile)),
                                 10.0f, (float)scr_height - 125.0f, 0.35f);
          fontManager.renderText(fontShader,
                                 "camera in chunk: x-" + std::to_string(cameraChunk.getLeft()) + ":" + std::to_string(cameraChunk.getRight())
                                 + ", z-" + std::to_string(cameraChunk.getTop()) + ":" + std::to_string(cameraChunk.getBottom()),
                                 10.0f, (float)scr_height - 145.0f, 0.35f);
          glLineWidth(2);
          csShader.use();
          csShader.setMat4("view", view);
          csShader.setFloat("aspectRatio", aspect_ratio);
          glBindVertexArray(csRenderer.getVAO());
          glDrawArrays(GL_POINTS, 0, 3);
        }

      //reset texture units to terrain textures after we done with models and text
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, baseTexture);

      //save/load routine
      if (saveRequest)
        {
          saveLoadManager->saveToFile(PROJ_PATH + "/saves/testSave.txt", treeModelChunks, hillTreeModelChunks);
          saveRequest = false;
        }
      if (loadRequest)
        {
          saveLoadManager->loadFromFile(PROJ_PATH + "/saves/testSave.txt", treeModelChunks, hillTreeModelChunks);
          loadRequest = false;
          underwaterReliefTexture = textureLoader.createUnderwaterReliefTexture(waterMapGenerator, underwaterReliefTextureUnit);
        }

      glfwPollEvents();
      glfwSwapBuffers(window);
    }

  //cleanup
  for (unsigned int i = 0; i < textures.size(); i++)
    glDeleteTextures(1, textures[i]);
  baseMapGenerator->deleteGLObjects();
  hillMapGenerator->deleteGLObjects();
  waterMapGenerator->deleteGLObjects();
  buildableMapGenerator->deleteGLObjects();
  delete baseMapGenerator;
  delete hillMapGenerator;
  delete waterMapGenerator;
  delete saveLoadManager;
  delete buildableMapGenerator;
  delete treeGenerator;
  underwaterQuadGenerator.deleteGLObjects();
  fontManager.deleteGLObjects();
  csRenderer.deleteGLObjects();
  for (Shader* shader: shaders)
    shader->cleanUp();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void prepareTerrain()
{
  waterMapGenerator->prepareMap(); //prepare water map
  hillMapGenerator->prepareMap(); //generating hill height map
  hillMapGenerator->fillBufferData(false); //fill hills buffer
  baseMapGenerator->prepareMap(true); //generating base terrain data
  baseMapGenerator->fillBufferData(); //fill base terrain vertex data
  baseMapGenerator->fillChunkBufferData(); //generating data for chunk instance rendering
  baseMapGenerator->fillCellBufferData(); //generating data for 1x1 tile instance rendering
  waterMapGenerator->postPrepareMap();
  waterMapGenerator->fillBufferData(); //fill water buffer
  underwaterQuadGenerator.fillBufferData(); //generating underwater flat tile
  skybox.fillBufferData(); //setup skybox
  csRenderer.fillBufferData(); //coordinate system setup
  buildableMapGenerator->prepareMap();
  buildableMapGenerator->fillBufferData();
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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 8);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
  std::cout << glfwGetVersionString() << std::endl;
  scr_width = vidmode->width;
  scr_height = vidmode->height;
  aspect_ratio = (float)scr_width / (float)scr_height;
  GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "Terrain Test", monitor, 0);
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, input.cursorCallback);
  glfwSetMouseButtonCallback(window, input.cursorClickCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return window;
}

void printMapsInfos()
{
  std::cout << "\n-----------------------------------------------------------\n";
  std::cout << "Water tiles:\t" << waterMapGenerator->getTiles().size() << std::endl;
  std::cout << "Hills tiles:\t" << hillMapGenerator->getTiles().size() << std::endl;
  std::cout << "Base tiles:\t" << baseMapGenerator->getTiles().size() << std::endl;
  int instanced = 0;
  for (unsigned int i = 0; i < NUM_BASE_TERRAIN_CHUNKS; i++)
    {
      std::cout << "x" << BASE_TERRAIN_CHUNK_SIZES[i]
                   << "\ttiles:\t"
                   << baseMapGenerator->getChunkTiles(i).size() << "\t(instanced)"
                   << std::endl;
      instanced += baseMapGenerator->getChunkTiles(i).size();
    }
  std::cout << "1x1 \ttiles:\t"
            << baseMapGenerator->getNumCellInstances() << "\t(instanced)"
            << std::endl;
  instanced += baseMapGenerator->getNumCellInstances();
  std::cout << "Summary: \t"
            << (waterMapGenerator->getTiles().size()
                + hillMapGenerator->getTiles().size()
                + baseMapGenerator->getTiles().size()
                + instanced)
            << "\t(" << instanced << " instanced)" << std::endl;
  std::cout << "Trees on flat: ";
  unsigned int numTrees = 0;
  for (unsigned int i = 0; i < treeGenerator->getTreeModels().size(); i++)
    {
      numTrees += treeGenerator->getNumTrees(i);
      std::cout << treeGenerator->getNumTrees(i) << ", ";
    }
  std::cout << "summary: " << numTrees << std::endl;
  std::cout << "Trees on hills: ";
  unsigned int numHillTrees = 0;
  for (unsigned int i = 0; i < treeGenerator->getHillTreeModels().size(); i++)
    {
      numHillTrees += treeGenerator->getNumHillTrees(i);
      std::cout << treeGenerator->getNumHillTrees(i) << ", ";
    }
  std::cout << "summary: " << numHillTrees << std::endl;
  std::cout << "-----------------------------------------------------------\n";
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei,
                            const GLchar* message,
                            const void*)
{
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
  std::cout << "Debug message: (" << id << "): " << message << std::endl;
  switch(source)
    {
    case GL_DEBUG_SOURCE_API: std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other"; break;
    }
  std::cout << std::endl;
  switch(type)
    {
    case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other"; break;
    }
  std::cout << std::endl;
  switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: High"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: Medium"; break;
    case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: Low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: Notification"; break;
    }
  std::cout << "\n\n";
}
